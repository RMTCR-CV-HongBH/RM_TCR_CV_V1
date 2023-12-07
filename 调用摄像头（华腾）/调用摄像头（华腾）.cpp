#include<opencv2/core/core.hpp>
#include"armor_plate.h"
#include <highgui.hpp>
#include <imgproc.hpp>
//#include"HKcamera.h"
using namespace cv;
using namespace std;

#define ARMOR_DEVICE 1

/*************************************************
						   _ooOoo_
						  o8888888o
						  88" . "88
						  (| -_- |)
						  O\  =  /O
					   ____/`---'\____
					 .'  \\|     |//  `.
					/  \\|||  :  |||//  \
				   /  _||||| -:- |||||-  \
				   |   | \\\  -  /// |   |
				   | \_|  ''\---/''  |   |
				   \  .-\__  `-`  ___/-. /
				 ___`. .'  /--.--\  `. . __
			  ."" '<  `.___\_<|>_/___.'  >'"".
			 | | :  `- \`.;`\ _ /`;.`/ - ` : | |
			 \  \ `-.   \_ __\ /__ _/   .-` /  /
		======`-.____`-.___\_____/___.-`____.-'======
						   `=---='
		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

				 佛祖保佑       永无BUG
                 ROBOMASTER     TKKC TCR 
*************************************************/
bool CamerRead(ArmorPlate& armor_param);

/*******************用于相机启动以及进入图像处理***********************/
int main()
{
	ArmorPlate armor;//创建实例化对象
//
//	int nRet = MV_OK;
//	void* handle = NULL;
//	MV_CC_DEVICE_INFO_LIST stDeviceList;
//	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
//	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList); //枚举设备
//	unsigned int nIndex = 0;//相机在系统中的设备号为0
//
//	nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
//	nRet = MV_CC_OpenDevice(handle);
//	nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
//	nRet = MV_CC_StartGrabbing(handle);
//
//	MVCC_INTVALUE stParam; // 获取数据包大小
//	memset(&stParam, 0, sizeof(MVCC_INTVALUE));
//	nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
//	MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
//	memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
//	unsigned char* pData = (unsigned char*)malloc(sizeof(unsigned char) * stParam.nCurValue);
//	unsigned int nDataSize = stParam.nCurValue;
//	while (1)
//	{
//		nRet = MV_CC_GetOneFrameTimeout(handle, pData, nDataSize, &stImageInfo, 1000);
//		//取流结果存在nRet里，但是此时的数据类型是byte类型，需要装换成opencv的Mat类型
//		if (nRet == MV_OK)
//		{
//			armor.armor_image_ = Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC3, pData);//转换为Mat类型
//			//高斯滤波，实际效果等待测试
//			/*cv::GaussianBlur(armor.armor_image_, armor.armor_image_, cv::Size(3,3), 3, 3);*/
//			//在这里进入图像处理函数
//			armor.AutoShoot();
//		}
//		else
//		{
//			cout << "相机未启动！！！！！" << endl << endl;
//		}
//	}
//}

	/*******************用于视频流的读入以及进入图像处理***********************/
	armor.CamaraInit(ARMOR_DEVICE); // 视觉辅助摄像头初始化
	/*VideoCapture capture(ARMOR_DEVICE);*/
	VideoCapture capture("D://robo/2	.mp4");
	armor.capture_armor_ = capture;
	/*armor.AutoShoot();*/
	while (1)
	{
		if (!CamerRead(armor))
			continue;
		armor.AutoShoot();
	}
	return 0;

}
		bool CamerRead(ArmorPlate& armor_param)
	{
		armor_param.capture_armor_.read(armor_param.armor_image_);
		if (!armor_param.armor_image_.data)
		{
			armor_param.CamaraInit(ARMOR_DEVICE); // 视觉辅助摄像头初始化
			return false;
		}
		else
			return true;
	}
		
		
