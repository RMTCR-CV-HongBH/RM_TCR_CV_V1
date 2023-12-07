#pragma once
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>

#include "MvCameraControl.h"
#include "opencv2/opencv.hpp"

class Camera
{
public:
	cv::Mat img_camera;
	unsigned int payload_size;
	int ret;
	void* handle;
	unsigned int sdk_V;

};

bool printDeviceInfo(MV_CC_DEVICE_INFO* hk_device);
bool hk2cv(MV_FRAME_OUT_INFO_EX* hk_imginfo, unsigned char* data, cv::Mat& src_img);
int OpenCamera();
cv::Mat readCamera(cv::Mat& src_img);
int open();
#pragma once
