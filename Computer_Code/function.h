#ifndef function_h
#define function_h

#include <highgui.h>
//#include <cxcore.h>
#include <cv.h>
#include "stdio.h"
#include <queue>
#include <atlbase.h>
#include <cmath>
#include "templeteclass.h"

using namespace std;


CvPoint2D32f originpoints[4]; //保存四个点的原始坐标
CvPoint2D32f newpoints[4]; //保存这四个点的新坐标


int cnt = 0;//透视变换边界点计数
bool flag = true;//标记：是否取过点
int num = 0;//路径点个数
int number = 0;
CvPoint beginpoint, head, tail, center;//路径起点、小车头尾和中心
int headX, headY, tailX, tailY;



IplImage* video;
IplImage* transimg1 = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 3);//创建一个400*400的24位彩色图像，保存变换结果//感觉400*400太小？
IplImage* transimg2 = cvCreateImage(cvSize(400, 400), 8, 1);


IplImage* transimg3 = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 3);//保存追踪图像？
//IplImage* temp = cvCreateImage(cvSize(400, 400), 8, 1);//transimg2的副本，没用到
IplImage* Convert = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 3);//追踪的时候用它~



IplImage* themap = cvCreateImage(cvSize(400, 400), 8, 3);//1.画出轨道路径点
CvMat* transmat = cvCreateMat(3, 3, CV_32FC1); //创建一个3*3的单通道32位浮点矩阵保存变换数据(透视变换时作为变换矩阵)

int gothsv = 0;//选取车头车尾时用来计数
double hue[2], sat[2], val[2];//车头和车尾的hsv



//手动确定透视变换边界点
void mouse0(int mouseevent, int x, int y, int flags, void*param)//发现该函数会不停的执行，不知道是否必要？
{
	if ((mouseevent == CV_EVENT_LBUTTONDOWN) && (cnt < 4))
	{
		cout << "选取的透视变换的第"<< cnt+1 <<"个点为(" << x << "," << y <<")"<< endl;
		originpoints[cnt].x = x;
		originpoints[cnt].y = y;
		++cnt;
	}

}


CvPoint* point;//路径点
double distance2(int x1, int y1, int x2, int y2) {
	double tmp = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	std::cout << tmp << std::endl;
	return tmp;
}

//细化处理
void cvThin(IplImage* src, IplImage* dst, int iterations)
{
	cvCopyImage(src, dst);
	BwImage dstdat(dst);
	IplImage* t_image = cvCloneImage(src);
	BwImage t_dat(t_image);
	for (int n = 0; n < iterations; n++)
	for (int s = 0; s <= 1; s++) {
		cvCopyImage(dst, t_image);
		for (int i = 0; i < src->height; i++)
		for (int j = 0; j < src->width; j++)
		if (t_dat[i][j]) {
			int a = 0, b = 0;
			int d[8][2] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };
			int p[8];
			p[0] = (i == 0) ? 0 : t_dat[i - 1][j];
			for (int k = 1; k <= 8; k++) {
				if (i + d[k % 8][0] < 0 || i + d[k % 8][0] >= src->height
					|| j + d[k % 8][1] < 0 || j + d[k % 8][1] >= src->width)
					p[k % 8] = 0;
				else p[k % 8] = t_dat[i + d[k % 8][0]][j + d[k % 8][1]];
				if (p[k % 8]) {
					b++;
					if (!p[k - 1]) a++;
				}
			}
			if (b >= 2 && b <= 6 && a == 1)
			if (!s && !(p[2] && p[4] && (p[0] || p[6])))
				dstdat[i][j] = 0;
			else if (s && !(p[0] && p[6] && (p[2] || p[4])))
				dstdat[i][j] = 0;
		}
	}
	cvReleaseImage(&t_image);
}
//霍夫变换//实际上没有用到
/*
void Hough(IplImage* src, IplImage *dst)
{
CvMemStorage* storage = cvCreateMemStorage();
CvSeq* lines = cvHoughLines2(src, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 30, 40, 40);  //Hough变换找直线
cvSetZero(dst);
for (int i = 0; i < lines->total; i++)
{
CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);

cvLine(dst, line[0], line[1], cvScalar(255));
}

}
*/
//获取两点距离的函数
int getDistance(CvPoint x, CvPoint y)
{
	return sqrt(pow(x.x - y.x, 2) + pow(x.y - y.y, 2));
}

/*
//计算向量夹角的函数
double calculateSinAngle(CvPoint2D32f roadPointd, CvPoint2D32f roadPointf, int headX, int headY, int tailX, int tailY)
{
int road_x = roadPointd.x - roadPointf.x;
int road_y = roadPointd.y - roadPointf.y;
int car_x = headX - tailX;
int car_y = headY - tailY;
double carLength = sqrt(car_x*car_x + car_y*car_y);
double roadLength = sqrt(road_x*road_x + road_y*road_y);
double sinAngle = (car_x*road_y - car_y*road_x) / (carLength*roadLength);
return(sinAngle);

}

double calculateCosAngle(CvPoint2D32f roadPointd, CvPoint2D32f roadPointf, int headX, int headY, int tailX, int tailY)
{
	int road_x = roadPointd.x - roadPointf.x;
	int road_y = roadPointd.y - roadPointf.y;
	int car_x = headX - tailX;
	int car_y = headY - tailY;
	double carLength = sqrt(car_x*car_x + car_y*car_y);
	double roadLength = sqrt(road_x*road_x + road_y*road_y);
	double cosAngle = (car_x*road_x + car_y*road_y) / (carLength*roadLength);
	return(cosAngle);
}
*/






//手动确定起点的函数
void mouse1(int mouseevent, int x, int y, int flags, void* param)//
{

	if ((mouseevent == CV_EVENT_LBUTTONDOWN) && (number == 0)){
		beginpoint = cvPoint(x, y);
		cvCircle(transimg2, beginpoint, 10, cvScalar(255), 3);
		cvShowImage("win2", transimg2);
		cout << "begin point is  (" << beginpoint.x << "," << beginpoint.y << ")" << endl;
		number++;
	}
}


//选取车头车尾的函数
void mouse_gothsv(int mouseevent, int x, int y, int flags, void* param)
{
	cvCvtColor(transimg3, Convert, CV_BGR2HSV);//把transimg3保存到Convert中，并从BGR空间转换到HSV空间，其实相当于凭空创建一个HSV空间的Convert
	RGBImage imgcol(Convert);// bgr 分别对应 hsv

	if (mouseevent == CV_EVENT_LBUTTONUP&& gothsv == 0) { //车头识别点选择
		head.x = x; head.y = y;
		hue[0] = imgcol[head.y][head.x].b;
		sat[0] = imgcol[head.y][head.x].g;
		val[0] = imgcol[head.y][head.x].r;
		++gothsv;
	}
	if (mouseevent == CV_EVENT_LBUTTONDOWN && gothsv == 1) { //车尾识别点选择
		tail.x = x; tail.y = y;
		hue[1] = imgcol[tail.y][tail.x].b;
		sat[1] = imgcol[tail.y][tail.x].g;
		val[1] = imgcol[tail.y][tail.x].r;
		++gothsv;
	}
}





#endif