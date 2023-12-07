/*************************************************
 *          .,:,,,                                        .::,,,::.
 *        .::::,,;;,                                  .,;;:,,....:i:
 *        :i,.::::,;i:.      ....,,:::::::::,....   .;i:,.  ......;i.
 *        :;..:::;::::i;,,:::;:,,,,,,,,,,..,.,,:::iri:. .,:irsr:,.;i.
 *        ;;..,::::;;;;ri,,,.                    ..,,:;s1s1ssrr;,.;r,
 *        :;. ,::;ii;:,     . ...................     .;iirri;;;,,;i,
 *        ,i. .;ri:.   ... ............................  .,,:;:,,,;i:
 *        :s,.;r:... ....................................... .::;::s;
 *        ,1r::. .............,,,.,,:,,........................,;iir;
 *        ,s;...........     ..::.,;:,,.          ...............,;1s
 *       :i,..,.              .,:,,::,.          .......... .......;1,
 *      ir,....:rrssr;:,       ,,.,::.     .r5S9989398G95hr;. ....,.:s,
 *     ;r,..,s9855513XHAG3i   .,,,,,,,.  ,S931,.,,.;s;s&BHHA8s.,..,..:r:
 *    :r;..rGGh,  :SAG;;G@BS:.,,,,,,,,,.r83:      hHH1sXMBHHHM3..,,,,.ir.
 *   ,si,.1GS,   sBMAAX&MBMB5,,,,,,:,,.:&8       3@HXHBMBHBBH#X,.,,,,,,rr
 *   ;1:,,SH:   .A@&&B#&8H#BS,,,,,,,,,.,5XS,     3@MHABM&59M#As..,,,,:,is,
 *  .rr,,,;9&1   hBHHBB&8AMGr,,,,,,,,,,,:h&&9s;   r9&BMHBHMB9:  . .,,,,;ri.
 *  :1:....:5&XSi;r8BMBHHA9r:,......,,,,:ii19GG88899XHHH&GSr.      ...,:rs.
 *  ;s.     .:sS8G8GG889hi.        ....,,:;:,.:irssrriii:,.        ...,,i1,
 *  ;1,         ..,....,,isssi;,        .,,.                      ....,.i1,
 *  ;h:               i9HHBMBBHAX9:         .                     ...,,,rs,
 *  ,1i..            :A#MBBBBMHB##s                             ....,,,;si.
 *  .r1,..        ,..;3BMBBBHBB#Bh.     ..                    ....,,,,,i1;
 *   :h;..       .,..;,1XBMMMMBXs,.,, .. :: ,.               ....,,,,,,ss.
 *    ih: ..    .;;;, ;;:s58A3i,..    ,. ,.:,,.             ...,,,,,:,s1,
 *    .s1,....   .,;sh,  ,iSAXs;.    ,.  ,,.i85            ...,,,,,,:i1;
 *     .rh: ...     rXG9XBBM#M#MHAX3hss13&&HHXr         .....,,,,,,,ih;
 *      .s5: .....    i598X&&A&AAAAAA&XG851r:       ........,,,,:,,sh;
 *      . ihr, ...  .         ..                    ........,,,,,;11:.
 *         ,s1i. ...  ..,,,..,,,.,,.,,.,..       ........,,.,,.;s5i.
 *          .:s1r,......................       ..............;shs,
 *          . .:shr:.  ....                 ..............,ishs.
 *              .,issr;,... ...........................,is1s;.
 *                 .,is1si;:,....................,:;ir1sr;,
 *                    ..:isssssrrii;::::::;;iirsssssr;:..
 *                         .,::iiirsssssssssrri;;:.

Author:        ALL MASTER
Date:          2023-9-20
Description:   װ��ʶ��
Version:       2.0.1
**************************************************/
#include <opencv2/opencv.hpp>
#include "armor_plate.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

/*************************************************
Function:       ArmorPlate
Description:    ���캯��
Input:          TEAMBLUE
Output:         our_team
*************************************************/
ArmorPlate::ArmorPlate()
{
	our_team_ = TEAMBLUE;
}
/*************************************************
Function:       CameraInit
Description:    ����ͷ��ʼ��
Input:          device
Output:         None
Return:         false or true
Others:         bool
*************************************************/
bool ArmorPlate::CamaraInit(int device)
{	
	capture_armor_.open(device);
	if (!capture_armor_.isOpened())
	{
			cout << "�Ӿ���������ͷ��ʧ��" << endl;
			return false;
	}
	else
		return true;
}
/*************************************************
Function:       setImage
Description:    ROI������趨
Input:          src
Output:         dst
Others:         RGB
*************************************************/



/*************************************************
Function:       ImgPreprocess
Description:    ͼ��Ԥ����
Input:          src
Output:         dst
Others:         RGB
*************************************************/
void ArmorPlate::ImgPreprocess(const cv::Mat& src, cv::Mat& dst)
{
	/*****************ͼ��Ԥ������ȡ��������*********************/
	Mat thres_whole;//���ڴ������
	Mat color;      //���ڴ洢��ɫ��Ϣ
	Mat color1;
	Mat max_color;  //������յĶ�ֵͼ
	Mat element2 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat element =  getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	Mat kernel  =  getStructuringElement(0, Size(5, 5));

	double maxValue_gary; //���ڴ洢��ȡ�����Ҷ�ֵ
	double maxValue_gary2;
	/*****************���ֶ���*********************/
	if (our_team_=1) // �ҷ��췽 �Է�����
	{
		Mat element;
		vector<Mat> splited;
		cvtColor(src, thres_whole, COLOR_BGR2GRAY);
		minMaxLoc(thres_whole, 0, &maxValue_gary, 0, 0); // ��ȡ���Ҷ�ֵ
		threshold(thres_whole, thres_whole, maxValue_gary * 0.8, 255, THRESH_BINARY);//0.6 0.8
		dilate(thres_whole, thres_whole, element, Point(-1, -1), 1);
		split(src, splited);
		//imshow("1",thres_whole);
		subtract(splited[0], splited[2], color);
		minMaxLoc(color, 0, &maxValue_gary2, 0, 0); // ��ȡ���Ҷ�ֵ
		threshold(color, color, maxValue_gary2 * 0.1 , 240, THRESH_BINARY);//0.1 0.2
		/*�����ɫ������ʶ������ʡ��*/
		subtract(splited[2], splited[0], color1);
		minMaxLoc(color1, 0, &maxValue_gary2, 0, 0); // ��ȡ���Ҷ�ֵ
		threshold(color1, color1, maxValue_gary2 * 0.1, 240, THRESH_BINARY);
		
		/*imshow("2", color);*/
		//������
		max_color = (color|color1) & thres_whole ;//clolor��color1ȡ������������е�����ʶ��
		dilate(max_color, dst, element2);
		/*morphologyEx(max_color, max_color, MORPH_OPEN, element);*/
		imshow("RGBԤ����", dst);
	}
	else // �ҷ����� �Է��췽
	{
		Mat element;
		vector<Mat> splited;
		cvtColor(src, thres_whole, COLOR_BGR2GRAY);
		minMaxLoc(thres_whole, 0, &maxValue_gary, 0, 0); // ��ȡ���Ҷ�ֵ
		threshold(thres_whole, thres_whole, maxValue_gary * 0.6, 255, THRESH_BINARY);
		dilate(thres_whole, thres_whole, element, Point(-1, -1), 1);
		split(src, splited);
		subtract(splited[2], splited[0], color);
		minMaxLoc(color, 0, &maxValue_gary2, 0, 0); // ��ȡ���Ҷ�ֵ
		threshold(color, color, maxValue_gary2 * 0.1, 240, THRESH_BINARY);
		max_color = color & thres_whole;
		dilate(max_color, dst, element2);
	    /*morphologyEx(max_color, max_color, MORPH_OPEN, element);*/
		imshow("RGBԤ����", dst);
	}
}
/*************************************************
Function:       FindArmor
Description:    �������
Input:          src  dst
Output:         ��ҰĿ�� ����Ŀ��
*************************************************/
void  ArmorPlate::FindArmor(cv::Mat& src, Mat& dst, vector<RotatedRect>& all, RotatedRect& target) // ��λװ�װ�
{
	all.clear();//�������
	target.center.x = 0;
	target.center.y = 0;
	target.size.width = 0;
	target.size.height = 0;//��ʼ������
	target.angle = 0;//�Ƿ����Ŀ��

	RotatedRect s, s_fitEllipse, s_minAreaRect; // ����ɸѡ����s_minAreaRect��С����
	vector<RotatedRect> ss;//����һ�����Σ����ɸѡ�������
	ss.clear();

	vector<vector<Point>> contours; // ����
	vector<Vec4i> hierarchy; // ���
	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);//��ֵͼ�����������
    /*1findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE); */// Ѱ������
	Mat drawing = Mat::zeros(dst.size(), CV_8UC3);//��ʼ��һ��ͼ
	/*RNG g_rng(12345);*/
	for (int i = 0; i < contours.size(); i++)//ɸѡ������С
	{
		//Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); // ������������
		//imshow("����", drawing);

		if (contours[i].size() >= 10)
		{
			s_fitEllipse = fitEllipse(contours[i]); // ��Բ���
			s_minAreaRect = minAreaRect(contours[i]);//��С����

			s.angle = s_fitEllipse.angle; //�������ڵ���
			s.center = s_fitEllipse.center;//�洢��Բ������
			if (s_minAreaRect.size.width > s_minAreaRect.size.height)
			{
				s.size.height = s_minAreaRect.size.width;//�߶�
				s.size.width = s_minAreaRect.size.height;//���
			}//�жϾ��εĸ߶ȺͿ�ȣ�������Բ������ó���ĽǶ�
			else
			{
				s.size.height = s_minAreaRect.size.height;
				s.size.width = s_minAreaRect.size.width;
			}//���ڿ�Ƚϣ�һ���С�ڸߣ��ض��ǶȻ��෴
			if ((s.size.width / s.size.height) > L_WH_RAT)//����ɸѡ ��/�� >  ������߱�
				continue;
			int x = s.center.x - s.size.width; //���ļ����
			if (x < 0)
				continue;
			int y = s.center.y - s.size.height;//���ļ��߶�
			if (y < 0)
				continue;
			int w = s.size.width + s.size.width;//���߿�ȴ������ļ����
			if (w > dst.cols - x)
				continue;
			int h = s.size.height + s.size.height;
			if (h > dst.rows - y)
				continue;//�����б�ʵ�ֻ�����ɸѡ

			if ((s.angle < 45 || s.angle > 135) && (s.size.height > 10) && (s.size.height < 150))//�Ƕ�ɸѡ
				ss.push_back(s);//������Ŀ���������
		}
	}

	// �б�װ��
	vector<RotatedRect> armors;//�����������ڴ��װ��
	vector<ArmorBuilded> armor_SECOND;//װ�׹����������
	ArmorBuilded armor_FIRST;//ѡ���������
	static float armor_center_x;
	static float armor_center_y;

	armors.clear();
	armor_SECOND.clear();
	int nL, nW;

	if (ss.size() < 2) // ��ת����С��������ֱ�ӷ���
	{
		target.center.x = 0;
		target.center.y = 0;
		target.size.width = 0;
		target.size.height = 0;
		target.angle = 0;
		all.push_back(target);//�洢����
		armor_center_x = 0;
		armor_center_y = 0;//������ȫ����ʼ��
	}
	//����������ת���ι�������ķ��裬�ɿ��Ի�δ������֤����Ҫ�ڸ��ӻ�����ʵ�ʲ���
	else if(ss.size()>16)
	{
		target.center.x = 0;
		target.center.y = 0;
		target.size.width = 0;
		target.size.height = 0;
		target.angle = 0;
		all.push_back(target);//�洢����
		armor_center_x = 0;
		armor_center_y = 0;//������ȫ����ʼ��
	}
	//����ɸѡ����
	else
	{
		for (int i = 0; i < ss.size() - 1; i++)//�Ƕ���⣬������ת���δ洢��ss
		{
			for (int j = i + 1; j < ss.size(); j++)//������������12 13 14 23 24 ��������
			{
				double height_diff = abs(ss[i].size.height - ss[j].size.height); // �߶Ȳ�
				double height_sum = ss[i].size.height + ss[j].size.height; // �߶Ⱥ�
				double width_diff = abs(ss[i].size.width - ss[j].size.width); // ��Ȳ�
				double width_sum = ss[i].size.width + ss[j].size.width; // ��Ⱥ�
				double angle_diff = fabs(ss[i].angle - ss[j].angle); // �ǶȲ�
				double Y_diff = abs(ss[i].center.y - ss[j].center.y); // �������ֵ
				double X_diff = abs(ss[i].center.x - ss[j].center.x); // �������ֵ
				double MH_diff = (min(ss[i].size.height, ss[j].size.height)) * 2 / 3; // �߶Ȳ��޷�
				double height_max = max(ss[i].size.height, ss[j].size.height); // ���߶�

				if (Y_diff < MH_diff && X_diff < height_max * 4 &&
					(angle_diff < T_ANGLE_THRE || 180 - angle_diff < T_ANGLE_THRE180) &&
					height_diff / height_sum < T_HIGH_RAT &&
					width_diff / width_sum < T_WHIDTH_RAT)//���αȽ�
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2);//����xֵ
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2);//����yֵ
					armor_FIRST.armorS.angle = ((ss[i].angle + ss[j].angle) / 2);//�Ƕ�ֵ
					if (180 - angle_diff < T_ANGLE_THRE180)//������һ��״̬�ж�
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2;//װ�׸߶�
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x)
						+ (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y));//x��������y�������
					if (nL < nW)//�߿��ж�
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if (Y_diff < nW / 3)
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST);
					}
				}
				else if ((angle_diff < T_ANGLE_THREMIN || 180 - angle_diff < T_ANGLE_THRE180MIN) &&
					Y_diff < MH_diff * 3 / 2 && X_diff < height_max * 4 &&
					height_diff / height_sum < T_HIGH_RAT_ANGLE &&
					width_diff / width_sum < T_WHIDTH_RAT_ANGLE
					)//��ͬ���
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //װ�����ĵ�x����
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //װ�����ĵ�y����
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //װ��������ת���ε���ת�Ƕ�
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //װ�׵ĸ߶�
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // װ�׵Ŀ�ȵ�������LED������ת������������ľ���
					if (nL < nW)
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if (Y_diff < nW / 2)
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST); //���ҳ���װ�׵���ת���α��浽vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 2 && X_diff < height_max * 4
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //װ�����ĵ�x����
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //װ�����ĵ�y����
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //װ��������ת���ε���ת�Ƕ�
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //װ�׵ĸ߶�
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // װ�׵Ŀ�ȵ�������LED������ת������������ľ���
					if (nL < nW)
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if ((abs(ss[i].center.y - ss[j].center.y) < nW / 2))
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST); //���ҳ���װ�׵���ת���α��浽vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 3 && X_diff < height_max * 5
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //װ�����ĵ�x����
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //װ�����ĵ�y����
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //װ��������ת���ε���ת�Ƕ�
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //װ�׵ĸ߶�
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); //װ�׵Ŀ�ȵ�������LED������ת������������ľ���
					if (nL < nW)
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if (Y_diff < nW / 2)
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST); //���ҳ���װ�׵���ת���α��浽vector
					}
				}
			}//����ɸѡ����
		}
		if (armor_SECOND.size() < 1)
		{
			cout << "װ�ײ���" << endl;//װ����б�Ƕȣ���ɵ�����ʧ�����ڲ��������װ�ף�������
			int ss_width = 0;
			int ss_ID = 0;
			for (unsigned int SSS = 0; SSS < ss.size(); SSS++) //������������ת���εļн�
			{
				if (ss[SSS].size.width > ss_width && (ss[SSS].size.width / ss[SSS].size.height) < 0.4 && (ss[SSS].size.width / ss[SSS].size.height) > 0.15)
				{
					ss_width = ss[SSS].size.width;
					ss_ID = SSS;
				}

			}
			int WIDTH = 3 * ss[ss_ID].size.height;
			int HEIGHT = 3 * ss[ss_ID].size.height;
			int XX_RIGHT = ss[ss_ID].center.x;
			int XX_LEFT = ss[ss_ID].center.x - WIDTH;
			int YY = ss[ss_ID].center.y - ss[ss_ID].size.height * 3 / 2;
			//
			if (XX_RIGHT + WIDTH > 1024)
			{
				WIDTH = 1024 - XX_RIGHT;
			}
			if (XX_RIGHT < 0)
			{
				XX_RIGHT = 0;
			}
			if (XX_LEFT < 0)
			{
				XX_LEFT = 0;
			}
			if (XX_LEFT + WIDTH > 1024)
			{
				WIDTH = 1024 - XX_LEFT;
			}
			if (YY + HEIGHT > 768)
			{
				HEIGHT = 768 - YY;
			}
			if (YY < 0)
			{
				YY = 0;
			}
			/*
					
							{	if (ss[ss_ID].angle > 45)
						{
							Mat LEFT_rio = src(Rect(XX_RIGHT, YY, WIDTH, HEIGHT));
							Mat  Rio_out, Rio_out1;
							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ��
							equalizeHist(Rio_out, Rio_out);
							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//��˹�˲�
							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//��ֵ��
							imshow("RIO_��ֵ��", Rio_out1);
							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//����ʡȥ
							int	centerX_num = 0;
							for (size_t ii = 0; ii < last_contours.size(); ii++)
								if (last_contours[ii].size() > 10)
								{
									s_center = minAreaRect(last_contours[ii]);
									ss_center.push_back(s_center);
								}
							}
							for (size_t iii = 0; iii < ss_center.size() - 1; iii++)
							{
								if (ss_center[iii].size.height > ss[ss_ID].size.height / 2 && ss_center[iii].size.width / ss_center[iii].size.width > 0.5)
								{
									for (size_t jjj = iii + 1; jjj < ss_center.size(); jjj++)
									{
										int centerX_sum = abs(ss_center[iii].center.x - ss_center[jjj].center.x);
										int centerY_sum = abs(ss_center[iii].center.y - ss_center[jjj].center.y);
										if (centerX_sum < 5 && centerY_sum < 5)
											centerX_num++;
									}
								}
							}
							if (centerX_num > 3)
							{
								ss[ss_ID].center.x = ss[ss_ID].center.x + cos((double)(180.0 - ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;
								ss[ss_ID].center.y = ss[ss_ID].center.y - sin((double)(180.0 - ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;
								target.center = ss[ss_ID].center;
								target.size.width = ss[ss_ID].size.height * 2;
								target.size.height = ss[ss_ID].size.height;
								target.angle = ss[ss_ID].angle;
								all.push_back(ss[ss_ID]);
							}
						}
						else
						{
							Mat LEFT_rio = src(Rect(XX_LEFT, YY, WIDTH, HEIGHT));
							Mat  Rio_out, Rio_out1;
							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ��
							equalizeHist(Rio_out, Rio_out);
							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//��˹�˲�
							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//��ֵ��
							imshow("RIO_��ֵ��", Rio_out1);
							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//����ʡȥ
							int	centerX_num = 0;
							for (size_t ii = 0; ii < last_contours.size(); ii++)
							{
								if (last_contours[ii].size() > 10)
								{
									s_center = minAreaRect(last_contours[ii]);
									ss_center.push_back(s_center);
								}
							}
							for (size_t iii = 0; iii < ss_center.size() - 1; iii++)
							{
								if (ss_center[iii].size.height > ss[ss_ID].size.height / 2 && ss_center[iii].size.width / ss_center[iii].size.width > 0.5)
								{
									for (size_t jjj = iii + 1; jjj < ss_center.size(); jjj++)
									{
										int centerX_sum = abs(ss_center[iii].center.x - ss_center[jjj].center.x);
										int centerY_sum = abs(ss_center[iii].center.y - ss_center[jjj].center.y);
										if (centerX_sum < 5 && centerY_sum < 5)
										{
											centerX_num++;
										}
									}
								}
							}
							if (centerX_num > 3)
							{
								ss[ss_ID].center.x = ss[ss_ID].center.x - cos((double)(ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;
								ss[ss_ID].center.y = ss[ss_ID].center.y - sin((double)(ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;
								target.center = ss[ss_ID].center;
								target.size.width = ss[ss_ID].size.height * 2;
								target.size.height = ss[ss_ID].size.height;
								target.angle = ss[ss_ID].angle;
								all.push_back(ss[ss_ID]);
							}
						}*/
		}
		else if (armor_SECOND.size() == 1)//�ҵ�һ��װ��
		{
			target = armor_SECOND[0].armorS;//��ֵ����
			all.push_back(armor_SECOND[0].armorS);//�洢����Ŀ��
			armor_center_x = target.center.x;
			armor_center_y = target.center.y;
		}
		else
		{
			cout << "���װ��" << endl;
			double min_feature = 9999999;
			for (int armor_i = 0; armor_i < armor_SECOND.size(); armor_i++)//�Ը����ƴ����б���
			{
				armors.push_back(armor_SECOND[armor_i].armorS);
				//�����Ȩ����ֵ
				double feature = armor_SECOND[armor_i].build_features[0] * 100 +
					armor_SECOND[armor_i].build_features[1] * 10 +
					armor_SECOND[armor_i].build_features[2] * 100 +
					//armor_SECOND[armor_i].build_features[3] * 0 +
					abs(armor_SECOND[armor_i].armorS.center.x - armor_center_x) * 50 +
					abs(armor_SECOND[armor_i].armorS.center.y - armor_center_y) * 50 -
					armor_SECOND[armor_i].armorS.size.height * 100 -
					armor_SECOND[armor_i].armorS.size.width * 100;
				if (feature < min_feature)//�ҵ���С����ֵ
				{
					min_feature = feature;
					target = armor_SECOND[armor_i].armorS;
				}

			}
			//�洢��һ��װ�����ĵ�
			armor_center_x = target.center.x;
			armor_center_y = target.center.y;
			all = armors;
		}
		//������ж�װ�װ�����
		ZJB_P = target.size.width / target.size.height;
		if (ZJB_P > 1 && ZJB_P <=3)
		{
			ZJB_class = 1;
		}
		else if (ZJB_P > 3 && ZJB_P <=8)
		{
		    ZJB_class = 2;
		}
	}
}
/*************************************************
Function:       DrawAll
Description:    ������ҰĿ��
Input:          all_target_  img
*************************************************/
void DrawAll(vector<RotatedRect> rect, Mat img)
{
	for (int i = 0; i < rect.size(); i++)
	{
		Point2f pp[4];//�������
		rect[i].points(pp); // �����i�����ζ�ά���Ӷ���
		line(img, pp[0], pp[1], CV_RGB(255, 255, 255), 1, 8, 0);//��������
		line(img, pp[1], pp[2], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[2], pp[3], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[3], pp[0], CV_RGB(255, 255, 255), 1, 8, 0);
		
	}
}
/*************************************************
Function:       SolvePnp
Description:    Pnp�ǶȽ���
Input:          img
Output:         image
*************************************************/
void ArmorPlate::SolvePnp(vector<RotatedRect> & all, cv::Mat& src, RotatedRect& target)
{
	//// ����ڲξ���
	Mat camera_matrix = (Mat_<double>(3, 3) << 
		2442.49986424421, 0, 0,
		6.79478385375985, 2448.06195518896, 0,
		628.516118142987, 273.017080194222, 1);
	//// �������ϵ��
	Mat dist_coeffs = (Mat_<double>(5, 1) << 0.00226038124984184 ,- 0.0135214158065301, -0.162138186269883	,1.29814375145539 ,- 4.93593063519896);
	// ��ת����
	Mat rotation_vector;
	// ƽ������
	Mat translation_vector;
	Mat iamge = src;
	vector<Point2d> image_points;//���ڴ��ͼ���е�2D��
	cv::Point2f pps[4];//�������

	double pitch =0;
	double yaw = 0;
	double d_pitch =0;
	double d_yaw = 0;

	if (target.angle==0)
	{
		pitch = 0;
		yaw = 0;
	    d_pitch = 0;
		d_yaw = 0;

		int font_face = cv::FONT_HERSHEY_COMPLEX;
		string pih = to_string(d_pitch);
		string ya = to_string(d_yaw);
		origin1.x = 10; origin1.y = 20;
		origin2.x = 10; origin2.y = 40;
		putText(src, text_p + pih, origin1, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//pitchֵ
		putText(src, text_y + ya, origin2, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//yawֵ
	    cout << "------����---------" << endl << endl << endl;
	}
	else
	{
		double half_x = 13.5;
		double half_y = 8.85;

		for (int i = 0; i < all.size(); i++)
		{
			all[i].points(pps); // �����i�����ζ�ά���Ӷ���
		}
		image_points.push_back(Point2d(pps[2]));
		image_points.push_back(Point2d(pps[1]));
		image_points.push_back(Point2d(pps[0]));
		image_points.push_back(Point2d(pps[3]));
		for (int i = 0; i < image_points.size(); i++)
		{
			circle(src, image_points[i], 3, Scalar(0, 0, 255), -1);
		}
		std::vector<Point3d> model_points;//���ڴ��ʵ��װ�װ�Ĵ�С
		switch (ZJB_class)
		{
		case 1:
			model_points.clear();
			model_points.push_back(Point3f(-half_x, half_y, 0.0)); // ���Ͻ�(-42.5mm,-42.5mm)
			model_points.push_back(Point3f(half_x, half_y, 0.0));
			model_points.push_back(Point3f(half_x, -half_y, 0.0));
			model_points.push_back(Point3f(-half_x, -half_y, 0.0));
			solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);
			cout << "Сװ��" << endl << ZJB_P << endl << endl;
			text_class = "Small";
			break;
		case 2:
			model_points.push_back(Point3f(-115.0, +28.0, 0)); // ���Ͻ�(-42.5mm,-42.5mm)
			model_points.push_back(Point3f(+115.0, +28.0, 0));//���Ͻ�
			model_points.push_back(Point3f(+115.0, -28.0, 0));//
			model_points.push_back(Point3f(-115.0, -28.0, 0));
			solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);
			cout << "��װ��" << endl << ZJB_P << endl << endl;
			text_class = "Big";
			break;
		}

		//model_points.clear();
		image_points.clear();

		double x = translation_vector.at<double>(0, 0);
		double y = translation_vector.at<double>(1, 0);
		double z = translation_vector.at<double>(2, 0);
        double distance = sqrt(x * x + y * y + z * z);
		
		pitch = y / sqrt(x * x + z * z);
		yaw = x / z;
		
		d_pitch = (-atan(pitch) * 180 / CV_PI)+5;
		d_yaw = (-atan(yaw) * 180 / CV_PI)+5;

		//double pitch_end = d_pitch;
		//double yaw_end = d_yaw;
		cout << "pitch" << endl << d_pitch << endl << endl;
		cout << "yaw" << endl << d_yaw << endl << endl;
		cout << "diatance" << endl << distance << endl << endl;

		//��ʾ
		int font_face = cv::FONT_HERSHEY_COMPLEX;
		string pih = to_string(d_pitch);
		string ya = to_string(d_yaw);
		string dis = to_string(distance);

		origin1.x = 10; origin1.y = 20;
		origin2.x = 10; origin2.y = 40;
		origin3.x = 10; origin3.y = 60;
		origin4.x = 10; origin4.y = 80;

		putText(src, text_p + pih, origin1, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//pitchֵ
		putText(src, text_y + ya, origin2, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//yawֵ
		putText(src, text_d + dis, origin3, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//����
		putText(src, text_a + text_class, origin4, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//װ�װ�����
	}
}
/*************************************************
Function:       DrawTarget
Description:    ��������Ŀ��
Input:          target_  img
*************************************************/
void DrawTarget(RotatedRect box, Mat img)
{
	Point2f pts[8];//
	pts[0].x = box.center.x;
	pts[0].y = box.center.y - 10;
	pts[1].x = box.center.x;
	pts[1].y = box.center.y + 10;
	pts[2].x = box.center.x - 10;
	pts[2].y = box.center.y;
	pts[3].x = box.center.x + 10;
	pts[3].y = box.center.y;//����װ�װ�

	pts[4].x = img_center_x;
	pts[4].y = img_center_y - 10;
	pts[5].x = img_center_x;
	pts[5].y = img_center_y + 10;
	pts[6].x = img_center_x - 10;
	pts[6].y = img_center_y;
	pts[7].x = img_center_x + 10;
	pts[7].y = img_center_y;//����ͼ������ʮ��
	line(img, pts[0], pts[1], CV_RGB(0, 255, 0), 2, 8, 0);
	line(img, pts[2], pts[3], CV_RGB(0, 255, 0), 2, 8, 0);
	line(img, pts[4], pts[5], CV_RGB(255, 255, 255), 2, 8, 0);
	line(img, pts[6], pts[7], CV_RGB(255, 255, 255), 2, 8, 0);
	
}
/*************************************************
Function:       AutoShoot
Description:    �������
Others:         ����������
*************************************************/
void ArmorPlate::AutoShoot()
{
	ImgPreprocess(armor_image_, pre_image_);//ͼ��Ԥ����
	FindArmor(armor_image_, pre_image_, all_target_, target_); // ��λװ�װ�
	SolvePnp(all_target_, armor_image_, target_);
	DrawAll(all_target_, armor_image_);//��������Ŀ��
	DrawTarget(target_, armor_image_);//��������Ŀ��
	imshow("ԭͼ", armor_image_);
	waitKey(1);
}
