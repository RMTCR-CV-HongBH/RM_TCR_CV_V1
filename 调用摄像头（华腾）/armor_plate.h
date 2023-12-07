#pragma once

#define img_center_x 320 // ͼ������xֵ
#define img_center_y 240 // ͼ������yֵ

#define T_ANGLE_THRE 5   //�����������ǶȲ�
#define T_ANGLE_THRE180 3   //�����������ǶȲ�
#define T_ANGLE_THREMIN 3   //����������С�ǶȲ�
#define T_ANGLE_THRE180MIN 2   //����������С�ǶȲ�

#define T_HIGH_RAT 0.2   //�ƴ����߲�� �����߶Ȳ�����������1/3
#define T_HIGH_RAT_ANGLE 0.34   //�ƴ��Ƕȷ�����С�Ƕȸ߲�� �����߶Ȳ�����������1/2


#define T_WHIDTH_RAT 0.4  //�ƴ�������   ������Ȳ�����������3/5
#define T_WHIDTH_RAT_ANGLE 0.55  //�ƴ�������

#define L_WH_RAT 0.8 // ������߱�



#define TEAMBLUE 0
constexpr auto TEAMRED = 1;
#include <videoio.hpp>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>

#include "MvCameraControl.h"
#include "opencv2/opencv.hpp"

class ArmorPlate
{
public:
	cv::VideoCapture image;
	cv::Mat camera_image;//�����ȡͼ��
	cv::Mat armor_image_; // �Ӿ�����ԭͼ
	cv::Mat pre_image_; // Ԥ����ͼ��
	cv::VideoCapture capture_armor_;//�Ӿ���������ͷ

	double ZJB_P=0;//װ�װ����
	int our_team_=2; // ����
	int ZJB_class=2; //װ������

	std::string text_p = "Pih:";
	std::string text_y = "Yaw:";
	std::string text_d = "Dis:";
	std::string text_a = "ArmorClass:";
	std::string text_class;
	cv::Point origin1;
	cv::Point origin2;
	cv::Point origin3;
	cv::Point origin4;

	ArmorPlate(); // ���캯��
	bool CamaraInit(int device); // ��������ͷ��ʼ��
	void AutoShoot(); // �ⲿ���ú���

private:
	std::vector<cv::RotatedRect> all_target_;//����Ŀ��
	cv::RotatedRect target_;//Ŀ��

	void ImgPreprocess(const cv::Mat& src, cv::Mat& dst); // ͼ��Ԥ����
	//void setImage(const cv::Mat& src);//ROI������趨
	void FindArmor(cv::Mat& src, cv::Mat& dst, std::vector<cv::RotatedRect>& all, cv::RotatedRect& target); // ��λװ�װ�
	void SolvePnp(std::vector<cv::RotatedRect>& all, cv::Mat& src,cv::RotatedRect& target);

};

class ArmorBuilded // װ�׹���
{
public:
	cv::RotatedRect armorS;
	int build1_No = 0;
	int build2_No = 0;
	int build_features[4]; // �ǶȲ� �߶������߶Ȳ��Ȳ�
	int vot = 0;
};
#pragma once

