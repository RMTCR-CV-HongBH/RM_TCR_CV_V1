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
Description:   装甲识别
Version:       2.0.1
**************************************************/
#include <opencv2/opencv.hpp>
#include "armor_plate.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

/*************************************************
Function:       ArmorPlate
Description:    构造函数
Input:          TEAMBLUE
Output:         our_team
*************************************************/
ArmorPlate::ArmorPlate()
{
	our_team_ = TEAMBLUE;
}
/*************************************************
Function:       CameraInit
Description:    摄像头初始化
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
			cout << "视觉辅助摄像头打开失败" << endl;
			return false;
	}
	else
		return true;
}
/*************************************************
Function:       setImage
Description:    ROI区域的设定
Input:          src
Output:         dst
Others:         RGB
*************************************************/



/*************************************************
Function:       ImgPreprocess
Description:    图像预处理
Input:          src
Output:         dst
Others:         RGB
*************************************************/
void ArmorPlate::ImgPreprocess(const cv::Mat& src, cv::Mat& dst)
{
	/*****************图像预处理，提取特征灯条*********************/
	Mat thres_whole;//用于储存灯条
	Mat color;      //用于存储颜色信息
	Mat color1;
	Mat max_color;  //存放最终的二值图
	Mat element2 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat element =  getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	Mat kernel  =  getStructuringElement(0, Size(5, 5));

	double maxValue_gary; //用于存储提取的最大灰度值
	double maxValue_gary2;
	/*****************区分队伍*********************/
	if (our_team_=1) // 我方红方 对方蓝方
	{
		Mat element;
		vector<Mat> splited;
		cvtColor(src, thres_whole, COLOR_BGR2GRAY);
		minMaxLoc(thres_whole, 0, &maxValue_gary, 0, 0); // 获取最大灰度值
		threshold(thres_whole, thres_whole, maxValue_gary * 0.8, 255, THRESH_BINARY);//0.6 0.8
		dilate(thres_whole, thres_whole, element, Point(-1, -1), 1);
		split(src, splited);
		//imshow("1",thres_whole);
		subtract(splited[0], splited[2], color);
		minMaxLoc(color, 0, &maxValue_gary2, 0, 0); // 获取最大灰度值
		threshold(color, color, maxValue_gary2 * 0.1 , 240, THRESH_BINARY);//0.1 0.2
		/*加入红色灯条的识别，用于省赛*/
		subtract(splited[2], splited[0], color1);
		minMaxLoc(color1, 0, &maxValue_gary2, 0, 0); // 获取最大灰度值
		threshold(color1, color1, maxValue_gary2 * 0.1, 240, THRESH_BINARY);
		
		/*imshow("2", color);*/
		//与或操作
		max_color = (color|color1) & thres_whole ;//clolor与color1取或操作，对所有灯条都识别
		dilate(max_color, dst, element2);
		/*morphologyEx(max_color, max_color, MORPH_OPEN, element);*/
		imshow("RGB预处理", dst);
	}
	else // 我方蓝方 对方红方
	{
		Mat element;
		vector<Mat> splited;
		cvtColor(src, thres_whole, COLOR_BGR2GRAY);
		minMaxLoc(thres_whole, 0, &maxValue_gary, 0, 0); // 获取最大灰度值
		threshold(thres_whole, thres_whole, maxValue_gary * 0.6, 255, THRESH_BINARY);
		dilate(thres_whole, thres_whole, element, Point(-1, -1), 1);
		split(src, splited);
		subtract(splited[2], splited[0], color);
		minMaxLoc(color, 0, &maxValue_gary2, 0, 0); // 获取最大灰度值
		threshold(color, color, maxValue_gary2 * 0.1, 240, THRESH_BINARY);
		max_color = color & thres_whole;
		dilate(max_color, dst, element2);
	    /*morphologyEx(max_color, max_color, MORPH_OPEN, element);*/
		imshow("RGB预处理", dst);
	}
}
/*************************************************
Function:       FindArmor
Description:    辅助射击
Input:          src  dst
Output:         视野目标 击打目标
*************************************************/
void  ArmorPlate::FindArmor(cv::Mat& src, Mat& dst, vector<RotatedRect>& all, RotatedRect& target) // 定位装甲板
{
	all.clear();//清空内容
	target.center.x = 0;
	target.center.y = 0;
	target.size.width = 0;
	target.size.height = 0;//初始化容器
	target.angle = 0;//是否存在目标

	RotatedRect s, s_fitEllipse, s_minAreaRect; // 用于筛选轮廓s_minAreaRect最小矩形
	vector<RotatedRect> ss;//定义一个矩形，存放筛选完的数据
	ss.clear();

	vector<vector<Point>> contours; // 轮廓
	vector<Vec4i> hierarchy; // 层次
	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);//二值图，轮廓，层次
    /*1findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE); */// 寻找轮廓
	Mat drawing = Mat::zeros(dst.size(), CV_8UC3);//初始化一个图
	/*RNG g_rng(12345);*/
	for (int i = 0; i < contours.size(); i++)//筛选轮廓大小
	{
		//Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); // 画出所有轮廓
		//imshow("轮廓", drawing);

		if (contours[i].size() >= 10)
		{
			s_fitEllipse = fitEllipse(contours[i]); // 椭圆拟合
			s_minAreaRect = minAreaRect(contours[i]);//最小矩形

			s.angle = s_fitEllipse.angle; //复制用于调度
			s.center = s_fitEllipse.center;//存储椭圆的中心
			if (s_minAreaRect.size.width > s_minAreaRect.size.height)
			{
				s.size.height = s_minAreaRect.size.width;//高度
				s.size.width = s_minAreaRect.size.height;//宽度
			}//判断矩形的高度和宽度，利用椭圆拟合利用长轴的角度
			else
			{
				s.size.height = s_minAreaRect.size.height;
				s.size.width = s_minAreaRect.size.width;
			}//高于宽比较，一般宽小于高，特定角度会相反
			if ((s.size.width / s.size.height) > L_WH_RAT)//几何筛选 宽/高 >  灯条宽高比
				continue;
			int x = s.center.x - s.size.width; //中心减宽度
			if (x < 0)
				continue;
			int y = s.center.y - s.size.height;//中心减高度
			if (y < 0)
				continue;
			int w = s.size.width + s.size.width;//两边宽度大于中心减宽度
			if (w > dst.cols - x)
				continue;
			int h = s.size.height + s.size.height;
			if (h > dst.rows - y)
				continue;//用于判别，实现基础的筛选

			if ((s.angle < 45 || s.angle > 135) && (s.size.height > 10) && (s.size.height < 150))//角度筛选
				ss.push_back(s);//将疑似目标放入容器
		}
	}

	// 判别装甲
	vector<RotatedRect> armors;//创建容器用于存放装甲
	vector<ArmorBuilded> armor_SECOND;//装甲构成类的容器
	ArmorBuilded armor_FIRST;//选择矩形区域
	static float armor_center_x;
	static float armor_center_y;

	armors.clear();
	armor_SECOND.clear();
	int nL, nW;

	if (ss.size() < 2) // 旋转矩形小于两个，直接返回
	{
		target.center.x = 0;
		target.center.y = 0;
		target.size.width = 0;
		target.size.height = 0;
		target.angle = 0;
		all.push_back(target);//存储所有
		armor_center_x = 0;
		armor_center_y = 0;//将变量全部初始化
	}
	//用于限制旋转矩形过多引起的发疯，可靠性还未进行验证，需要在复杂环境下实际测试
	else if(ss.size()>16)
	{
		target.center.x = 0;
		target.center.y = 0;
		target.size.width = 0;
		target.size.height = 0;
		target.angle = 0;
		all.push_back(target);//存储所有
		armor_center_x = 0;
		armor_center_y = 0;//将变量全部初始化
	}
	//进入筛选环节
	else
	{
		for (int i = 0; i < ss.size() - 1; i++)//角度求解，所有旋转矩形存储在ss
		{
			for (int j = i + 1; j < ss.size(); j++)//进行两两运算12 13 14 23 24 排列运算
			{
				double height_diff = abs(ss[i].size.height - ss[j].size.height); // 高度差
				double height_sum = ss[i].size.height + ss[j].size.height; // 高度和
				double width_diff = abs(ss[i].size.width - ss[j].size.width); // 宽度差
				double width_sum = ss[i].size.width + ss[j].size.width; // 宽度和
				double angle_diff = fabs(ss[i].angle - ss[j].angle); // 角度差
				double Y_diff = abs(ss[i].center.y - ss[j].center.y); // 纵坐标差值
				double X_diff = abs(ss[i].center.x - ss[j].center.x); // 横坐标差值
				double MH_diff = (min(ss[i].size.height, ss[j].size.height)) * 2 / 3; // 高度差限幅
				double height_max = max(ss[i].size.height, ss[j].size.height); // 最大高度

				if (Y_diff < MH_diff && X_diff < height_max * 4 &&
					(angle_diff < T_ANGLE_THRE || 180 - angle_diff < T_ANGLE_THRE180) &&
					height_diff / height_sum < T_HIGH_RAT &&
					width_diff / width_sum < T_WHIDTH_RAT)//几何比较
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2);//中心x值
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2);//中心y值
					armor_FIRST.armorS.angle = ((ss[i].angle + ss[j].angle) / 2);//角度值
					if (180 - angle_diff < T_ANGLE_THRE180)//对另外一种状态判断
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2;//装甲高度
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x)
						+ (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y));//x的坐标差和y的坐标差
					if (nL < nW)//高宽判断
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
					)//不同情况
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //装甲中心的x坐标
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //装甲中心的y坐标
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //装甲所在旋转矩形的旋转角度
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //装甲的高度
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // 装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
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
						armor_SECOND.push_back(armor_FIRST); //将找出的装甲的旋转矩形保存到vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 2 && X_diff < height_max * 4
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //装甲中心的x坐标
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //装甲中心的y坐标
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //装甲所在旋转矩形的旋转角度
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //装甲的高度
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // 装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
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
						armor_SECOND.push_back(armor_FIRST); //将找出的装甲的旋转矩形保存到vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 3 && X_diff < height_max * 5
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //装甲中心的x坐标
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //装甲中心的y坐标
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //装甲所在旋转矩形的旋转角度
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //装甲的高度
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); //装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
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
						armor_SECOND.push_back(armor_FIRST); //将找出的装甲的旋转矩形保存到vector
					}
				}
			}//各类筛选条件
		}
		if (armor_SECOND.size() < 1)
		{
			cout << "装甲补充" << endl;//装甲倾斜角度，造成灯条消失，用于补充另外的装甲，待改善
			int ss_width = 0;
			int ss_ID = 0;
			for (unsigned int SSS = 0; SSS < ss.size(); SSS++) //求任意两个旋转矩形的夹角
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
							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//转化为灰度图像
							equalizeHist(Rio_out, Rio_out);
							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//高斯滤波
							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//二值化
							imshow("RIO_二值化", Rio_out1);
							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//可以省去
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
							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//转化为灰度图像
							equalizeHist(Rio_out, Rio_out);
							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//高斯滤波
							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//二值化
							imshow("RIO_二值化", Rio_out1);
							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//可以省去
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
		else if (armor_SECOND.size() == 1)//找到一个装甲
		{
			target = armor_SECOND[0].armorS;//赋值矩形
			all.push_back(armor_SECOND[0].armorS);//存储所有目标
			armor_center_x = target.center.x;
			armor_center_y = target.center.y;
		}
		else
		{
			cout << "多个装甲" << endl;
			double min_feature = 9999999;
			for (int armor_i = 0; armor_i < armor_SECOND.size(); armor_i++)//对各个灯带进行遍历
			{
				armors.push_back(armor_SECOND[armor_i].armorS);
				//计算加权特征值
				double feature = armor_SECOND[armor_i].build_features[0] * 100 +
					armor_SECOND[armor_i].build_features[1] * 10 +
					armor_SECOND[armor_i].build_features[2] * 100 +
					//armor_SECOND[armor_i].build_features[3] * 0 +
					abs(armor_SECOND[armor_i].armorS.center.x - armor_center_x) * 50 +
					abs(armor_SECOND[armor_i].armorS.center.y - armor_center_y) * 50 -
					armor_SECOND[armor_i].armorS.size.height * 100 -
					armor_SECOND[armor_i].armorS.size.width * 100;
				if (feature < min_feature)//找到最小特征值
				{
					min_feature = feature;
					target = armor_SECOND[armor_i].armorS;
				}

			}
			//存储上一次装甲中心点
			armor_center_x = target.center.x;
			armor_center_y = target.center.y;
			all = armors;
		}
		//长宽比判断装甲板类型
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
Description:    画出视野目标
Input:          all_target_  img
*************************************************/
void DrawAll(vector<RotatedRect> rect, Mat img)
{
	for (int i = 0; i < rect.size(); i++)
	{
		Point2f pp[4];//定义矩形
		rect[i].points(pp); // 计算第i个矩形二维盒子顶点
		line(img, pp[0], pp[1], CV_RGB(255, 255, 255), 1, 8, 0);//绘制线条
		line(img, pp[1], pp[2], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[2], pp[3], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[3], pp[0], CV_RGB(255, 255, 255), 1, 8, 0);
		
	}
}
/*************************************************
Function:       SolvePnp
Description:    Pnp角度解算
Input:          img
Output:         image
*************************************************/
void ArmorPlate::SolvePnp(vector<RotatedRect> & all, cv::Mat& src, RotatedRect& target)
{
	//// 相机内参矩阵
	Mat camera_matrix = (Mat_<double>(3, 3) << 
		2442.49986424421, 0, 0,
		6.79478385375985, 2448.06195518896, 0,
		628.516118142987, 273.017080194222, 1);
	//// 相机畸变系数
	Mat dist_coeffs = (Mat_<double>(5, 1) << 0.00226038124984184 ,- 0.0135214158065301, -0.162138186269883	,1.29814375145539 ,- 4.93593063519896);
	// 旋转向量
	Mat rotation_vector;
	// 平移向量
	Mat translation_vector;
	Mat iamge = src;
	vector<Point2d> image_points;//用于存放图像中的2D点
	cv::Point2f pps[4];//定义矩形

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
		putText(src, text_p + pih, origin1, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//pitch值
		putText(src, text_y + ya, origin2, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//yaw值
	    cout << "------结束---------" << endl << endl << endl;
	}
	else
	{
		double half_x = 13.5;
		double half_y = 8.85;

		for (int i = 0; i < all.size(); i++)
		{
			all[i].points(pps); // 计算第i个矩形二维盒子顶点
		}
		image_points.push_back(Point2d(pps[2]));
		image_points.push_back(Point2d(pps[1]));
		image_points.push_back(Point2d(pps[0]));
		image_points.push_back(Point2d(pps[3]));
		for (int i = 0; i < image_points.size(); i++)
		{
			circle(src, image_points[i], 3, Scalar(0, 0, 255), -1);
		}
		std::vector<Point3d> model_points;//用于存放实际装甲板的大小
		switch (ZJB_class)
		{
		case 1:
			model_points.clear();
			model_points.push_back(Point3f(-half_x, half_y, 0.0)); // 左上角(-42.5mm,-42.5mm)
			model_points.push_back(Point3f(half_x, half_y, 0.0));
			model_points.push_back(Point3f(half_x, -half_y, 0.0));
			model_points.push_back(Point3f(-half_x, -half_y, 0.0));
			solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);
			cout << "小装甲" << endl << ZJB_P << endl << endl;
			text_class = "Small";
			break;
		case 2:
			model_points.push_back(Point3f(-115.0, +28.0, 0)); // 左上角(-42.5mm,-42.5mm)
			model_points.push_back(Point3f(+115.0, +28.0, 0));//右上角
			model_points.push_back(Point3f(+115.0, -28.0, 0));//
			model_points.push_back(Point3f(-115.0, -28.0, 0));
			solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);
			cout << "大装甲" << endl << ZJB_P << endl << endl;
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

		//显示
		int font_face = cv::FONT_HERSHEY_COMPLEX;
		string pih = to_string(d_pitch);
		string ya = to_string(d_yaw);
		string dis = to_string(distance);

		origin1.x = 10; origin1.y = 20;
		origin2.x = 10; origin2.y = 40;
		origin3.x = 10; origin3.y = 60;
		origin4.x = 10; origin4.y = 80;

		putText(src, text_p + pih, origin1, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//pitch值
		putText(src, text_y + ya, origin2, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//yaw值
		putText(src, text_d + dis, origin3, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//距离
		putText(src, text_a + text_class, origin4, font_face, 0.5, cv::Scalar(0, 255, 255), 1, 8, 0);//装甲板类型
	}
}
/*************************************************
Function:       DrawTarget
Description:    画出击打目标
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
	pts[3].y = box.center.y;//绘制装甲板

	pts[4].x = img_center_x;
	pts[4].y = img_center_y - 10;
	pts[5].x = img_center_x;
	pts[5].y = img_center_y + 10;
	pts[6].x = img_center_x - 10;
	pts[6].y = img_center_y;
	pts[7].x = img_center_x + 10;
	pts[7].y = img_center_y;//绘制图像中心十字
	line(img, pts[0], pts[1], CV_RGB(0, 255, 0), 2, 8, 0);
	line(img, pts[2], pts[3], CV_RGB(0, 255, 0), 2, 8, 0);
	line(img, pts[4], pts[5], CV_RGB(255, 255, 255), 2, 8, 0);
	line(img, pts[6], pts[7], CV_RGB(255, 255, 255), 2, 8, 0);
	
}
/*************************************************
Function:       AutoShoot
Description:    辅助射击
Others:         主函数调用
*************************************************/
void ArmorPlate::AutoShoot()
{
	ImgPreprocess(armor_image_, pre_image_);//图像预处理
	FindArmor(armor_image_, pre_image_, all_target_, target_); // 定位装甲板
	SolvePnp(all_target_, armor_image_, target_);
	DrawAll(all_target_, armor_image_);//画出所有目标
	DrawTarget(target_, armor_image_);//画出击打目标
	imshow("原图", armor_image_);
	waitKey(1);
}
