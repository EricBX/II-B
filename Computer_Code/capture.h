#ifndef capture_h
#define capture_h

#include "function.h"
#include "bluetooth.h"

//用来追踪小车的函数
void capture(CvCapture* cam)
{
	IplImage *HUE, *SAT, *VAL, *HUE_, *SAT_, *VAL_, *Binary, *Binary_;
	CvRect window, window_;//储存两个矩形边框
	CvTermCriteria criteria = cvTermCriteria(3, 100, 0.1);//迭代终止准则（最大迭代次数20，结果的准确性1）？
	CvConnectedComp comp, comp_;//描述一个连通域的结构体

	cvNamedWindow("H_1"); cvNamedWindow("H_2");//用来显示车头车尾实时追踪位置，在两个窗口中（用来测试追踪的是什么东西）
	cvNamedWindow("选取车头->车尾"); cvSetMouseCallback("选取车头->车尾", mouse_gothsv);//设置鼠标事件后该函数，即便没有经过，也一直处在被响应状态？
	
	//各种铺垫
	Convert = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 3);
	HUE = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); HUE_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	SAT = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); SAT_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	VAL = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); VAL_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	Binary = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); Binary_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	//
	while (gothsv != 2) {
		video = cvQueryFrame(cam);
		cvWarpPerspective(video, transimg3, transmat);//根据变换矩阵计算图像的变换，transimg3储存透视变换的结果	
		cvShowImage("选取车头->车尾", transimg3);
		cvWaitKey(1);
	}

	for (int i = 0; i < num; i++)
	{
		cout << "第" << i+1 << "个拐点为(" << point[i].x << "," << point[i].y << ")" << endl;
	}
	int i = 1;//用来判断是否到达每个点
	cvDestroyWindow("选取车头->车尾");
	//cvNamedWindow("追踪？");
	bool flag = true;
	while (flag) {
		
		video = cvQueryFrame(cam);
		cvWarpPerspective(video, transimg3, transmat);//根据变换矩阵计算图像的变换
		cvCvtColor(transimg3, Convert, CV_BGR2HSV);//颜色空间转换后为：HSV，存储在Convert中
		//车头部分
		cvSplit(Convert, HUE, SAT, VAL, 0);//把Convert中的多通道导出到三个单通道H、S、V中
		cvInRangeS(HUE, cvScalar(hue[0] - 30), cvScalar(hue[0] + 30), HUE);
		cvInRangeS(SAT, cvScalar(sat[0] - 30), cvScalar(sat[0] + 30), SAT);
		cvInRangeS(VAL, cvScalar(val[0] - 30), cvScalar(val[0] + 30), VAL);
		cvMul(HUE, SAT, Binary); cvMul(Binary, VAL, Binary);//矩阵乘法								//这样乘的意义是什么？
		//Binary = HUE;															//什么意义？
		cvShowImage("H_1", Binary);//Binary 储存二值化图像？
		//----
		window = cvRect(head.x - 50, head.y - 50, 100, 100);
		cvMeanShift(Binary, window, criteria, &comp);
		head.x = comp.rect.x + comp.rect.width / 2;
		head.y = comp.rect.y + comp.rect.height / 2;
		cvCircle(transimg3, head, 10, cvScalar(255, 255, 0), 3);//画圆圈
		//车尾部分
		cvSplit(Convert, HUE_, SAT_, VAL_, 0);
		cvInRangeS(HUE_, cvScalar(hue[1] - 30), cvScalar(hue[1] + 30), HUE_);
		cvInRangeS(SAT_, cvScalar(sat[1] - 30), cvScalar(sat[1] + 30), SAT_);
		cvInRangeS(VAL_, cvScalar(val[1] - 30), cvScalar(val[1] + 30), VAL_);
		cvMul(HUE_, SAT_, Binary_); cvMul(Binary_, VAL_, Binary_);
		//Binary_ = HUE_;
		cvShowImage("H_2", Binary_);
		//----
		window_ = cvRect(tail.x - 50, tail.y - 50, 100, 100);
		cvMeanShift(Binary_, window_, criteria, &comp_);
		tail.x = comp_.rect.x + comp_.rect.width / 2;
		tail.y = comp_.rect.y + comp_.rect.height / 2;
		cvCircle(transimg3, tail, 10, cvScalar(255, 0, 255), 3);
		//
		cvShowImage("追踪", transimg3);
		center.x = tail.x / 2 + head.x / 2;
		center.y = tail.y / 2 + head.y / 2;
		cout << "小车现在位置：头部（" << head.x << "，" << head.y << "）\t尾部(" << tail.x << "," << tail.y << ")" << endl;

			if (getDistance(center, point[i]) < 60) {
				cout << "到达第" << i-1 <<"个点"<< endl;
				i++;
			}
			cout << "现在在点" << i-1 << "附近" << endl;
			cout << "距离下一个点：" << getDistance(center, point[i]) << endl;
		//接下来计算夹角正余弦
	//	cout << "'路径'参数：";
		int road_x = point[i].x - tail.x; //cout << "road_x=" << road_x << endl;
		int road_y = point[i].y - tail.y; //cout << "road_y=" << road_y << endl;
		//cout << "小车参数：";
		int car_x = head.x - tail.x;// cout << "car_x=" << car_x << endl;
		int car_y = head.y - tail.y; //cout << "car_y=" << car_y << endl;
		double carLength = sqrt(car_x*car_x + car_y*car_y); //cout << carLength << "carLength" << endl;
		double roadLength = sqrt(road_x*road_x + road_y*road_y); //cout << roadLength << "roadLength" << endl;

		double sinAngle = (car_x*road_y - car_y*road_x) / (carLength*roadLength);
		double cosAngle = (car_x*road_x + car_y*road_y) / (carLength*roadLength);
		
		cout << "sinAngle is "  << sinAngle << endl;
		cout << "cosAngle is "  << cosAngle << endl;
		double c = 0.5;
		if (cosAngle<=0.15||head.x>=350||head.x<=50||head.y<=50||head.y>=350|| tail.x>=350 || tail.x<=50 || tail.y<=50 || tail.y>=350)//后退条件
		{
			sendCommand('B');
			Sleep(700);
			cout << "B" << endl;
		}
		else if (sinAngle >= 0.258*c)//右转条件
		{
			sendCommand('R');
			Sleep(200);
			cout << "R" << endl<<endl;
		}
		else if (sinAngle <= -0.258*c)//左转条件
		{
			sendCommand('L');
			Sleep(200);
			cout << "L" << endl<<endl;
		}
		else
		{
			sendCommand('F');
			//Sleep(250);
			cout << "F" << endl<<endl;
		}

		if (i == num) {
			sendCommand('S');
			cout << "finished!!";
			flag = false;
		}

		if (cvWaitKey(33) >= 0)
		{
			sendCommand('S');
			break;
		}
	}
}

//该函数判定小车是否到某一拐点，可以改成终点判定函数，也可作为行驶时间判定函数或判断是否下新的指令的函数
//目前未利用
#endif