#include"HKcamera.h"
#include"armor_plate.h"
using namespace std;
Camera camera;
int OpenCamera()
{
    //1.获取SDK版本信息
    camera.sdk_V = MV_CC_GetSDKVersion();
    printf("SDK version is [0x%x]\n", camera.sdk_V);

    //2.查找目标设备
    camera.ret = MV_OK;
    camera.handle = NULL;

    MV_CC_DEVICE_INFO_LIST hk_devices;
    memset(&hk_devices, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    camera.ret = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &hk_devices);
    if (camera.ret != MV_OK)
    {
        std::cout << "enum devices faild!" << std::endl;
        return -1;
    }
    std::cout << "共检测到相机的个数为：" << hk_devices.nDeviceNum << std::endl;

    //3.判断设备是否可访问
    if (hk_devices.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i < hk_devices.nDeviceNum; i++)
        {
            printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* pDeviceInfo = hk_devices.pDeviceInfo[i];

            bool access = MV_CC_IsDeviceAccessible(pDeviceInfo, 1);

            if (!access) {
                printf("[device %d]:could not access... nRet:[0x%x]\n", i, camera.ret);
            }
            else {
                printf("[device %d]:is OK... nRet:[0x%x]\n", i, camera.ret);
            }
        }
    }
    else
    {
        printf("Find No Devices!\n");
        return -1;
    }

    if (hk_devices.nDeviceNum > 0) {
        MV_CC_DEVICE_INFO* hk_camera = hk_devices.pDeviceInfo[0];
        if (printDeviceInfo(hk_camera) == false) {
            return -1;
        }
    }
    else {
        std::cout << "no device found" << std::endl;
        return -1;
    }

    //4.创建相机句柄
    camera.ret = MV_CC_CreateHandle(&camera.handle, hk_devices.pDeviceInfo[0]);
    if (camera.ret != MV_OK) {
        return -1;
    }

   /* 5.打开设备*/
    camera.ret = MV_CC_OpenDevice(camera.handle);
    if (camera.ret != MV_OK) {
        cout << "设备未打开" << endl;
        return -1;
    }
    else {
        std::cout << "设备已经打开" << std::endl;
    }

    //6.设置触发模式为off
    camera.ret = MV_CC_SetEnumValue(camera.handle, "TriggerMode", 0);
    if (camera.ret != MV_OK) {
        return -1;
    }
    //7.获取数据包的大小
    MVCC_INTVALUE hk_param;
    memset(&hk_param, 0, sizeof(MVCC_INTVALUE));
    camera.ret = MV_CC_GetIntValue(camera.handle, "PayloadSize", &hk_param);
    if (camera.ret != MV_OK) 
    {
        return -1;
    }
    camera.payload_size = hk_param.nCurValue;
}

cv::Mat readCamera(cv::Mat& src_img)
{
        camera.ret = MV_CC_StartGrabbing(camera.handle);
        if (camera.ret != MV_OK) {
            cout << "grab image failed!" << endl;
        }
        MV_FRAME_OUT_INFO_EX hk_imginfo;
        memset(&hk_imginfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * (camera.payload_size));
        /*cv::Mat src_img;*/
        while (!(_kbhit() && _getch() == 0x1b)) {
            camera.ret = MV_CC_GetOneFrameTimeout(camera.handle, data, camera.payload_size, &hk_imginfo, 1000);
            if (camera.ret != MV_OK) {
                free(data);
                data = NULL;
            }
            if (hk2cv(&hk_imginfo, data, src_img) == false)
            {
                continue;
            }
            imshow("t1", src_img);
            cv::waitKey(1);
            return src_img;
        }
}

bool printDeviceInfo(MV_CC_DEVICE_INFO* hk_device) {
    if (NULL == hk_device)
    {
        printf("The Pointer of hk_device is NULL!\n");
        return false;
    }
    if (hk_device->nTLayerType == MV_GIGE_DEVICE)
    {
        int nIp1 = ((hk_device->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((hk_device->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((hk_device->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (hk_device->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // print current ip and user defined name
        printf("CurrentIp: %d.%d.%d.%d\n", nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n\n", hk_device->SpecialInfo.stGigEInfo.chUserDefinedName);
    }
    else if (hk_device->nTLayerType == MV_USB_DEVICE)
    {
        printf("UserDefinedName: %s\n", hk_device->SpecialInfo.stUsb3VInfo.chUserDefinedName);
        printf("Serial Number: %s\n", hk_device->SpecialInfo.stUsb3VInfo.chSerialNumber);
        printf("Device Number: %d\n\n", hk_device->SpecialInfo.stUsb3VInfo.nDeviceNumber);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}

bool hk2cv(MV_FRAME_OUT_INFO_EX* hk_imginfo, unsigned char* data, cv::Mat& src_img) {
    cv::Mat cv_img;
    if (hk_imginfo->enPixelType == PixelType_Gvsp_Mono8) {
        cv_img = cv::Mat(hk_imginfo->nHeight, hk_imginfo->nWidth, CV_8UC1, data);
    }
    else if (hk_imginfo->enPixelType == PixelType_Gvsp_RGB8_Packed) {
        for (unsigned int j = 0; j < hk_imginfo->nHeight; j++) {
            for (unsigned int i = 0; i < hk_imginfo->nWidth; i++) {
                unsigned char red = data[j * (hk_imginfo->nWidth * 3) + i * 3];
                data[j * (hk_imginfo->nWidth * 3) + i * 3] = data[j * (hk_imginfo->nWidth * 3) + i * 3 + 2];
                data[j * (hk_imginfo->nWidth * 3) + i * 3 + 2] = red;
            }
        }
        cv_img = cv::Mat(hk_imginfo->nHeight, hk_imginfo->nWidth, CV_8UC3, data);
    }
    else {
        printf("unsupported pixel format\n");
        return false;
    }

    if (cv_img.data == NULL) {
        return false;
    }
    cv_img.copyTo(src_img);
    return true;
}