#ifndef prepare_h
#define prepare_h

#include "function.h"

//preparation(透视变换，细化处理，路径选点)

void RoadPoint(IplImage* src, CvPoint begin, int &num)
{
	seqQueue<CvPoint> que;
	bool find = true;
	CvPoint center = cvPoint(begin.x, begin.y);
	CvPoint tmp = cvPoint(0, 0);
	int i, j;
	BwImage deal(src);

	que.enQueue(center); ++num;

	while (find == true)
	{

		find = false;
		std::cout << 1 << std::endl;//右边
		for (i = center.x + 30, j = center.y - 30; !find && j<center.y + 30; ++j) {
			if (deal[j][i] == 255 && (distance2(tmp.x, tmp.y, i, j) >= 900)) {

				tmp = cvPoint(center.x, center.y);
				center.x = i; center.y = j;

				que.enQueue(center);
				++num; find = true;
			}
		}

		std::cout << 2 << std::endl;//底边
		for (i = center.x + 30, j = center.y + 30; !find && i>center.x - 30; --i) {
			if (deal[j][i] == 255 && (distance2(tmp.x, tmp.y, i, j) >= 900)) {
				tmp = cvPoint(center.x, center.y);
				center.x = i; center.y = j;
				que.enQueue(center);
				++num; find = true;
			}
		}

		std::cout << 3 << std::endl;//左边
		for (i = center.x - 30, j = center.y + 30; !find && j>center.y - 30; --j) {
			if (deal[j][i] == 255 && (distance2(tmp.x, tmp.y, i, j) >= 900)) {
				tmp = cvPoint(center.x, center.y);
				center.x = i; center.y = j;
				que.enQueue(center);
				++num; find = true;
			}
		}
		std::cout << 4 << std::endl;//上边
		for (i = center.x - 30, j = center.y - 30; !find && i<center.x + 30; ++i) {
			if (deal[j][i] == 255 && (distance2(tmp.x, tmp.y, i, j) >= 900)) {
				tmp = cvPoint(center.x, center.y);
				center.x = i; center.y = j;
				que.enQueue(center);
				++num; find = true;
			}
		}
		std::cout << num << std::endl;

	}
	//
	//std::cout << "????+" << num << std::endl;
	point = new CvPoint[num];
	i = 0;
	while (!que.isEmpty()) {
		point[i] = que.deQueue();
		++i;
	}
}


void prepare(CvCapture* cam)
{
	RGBImage imgclass(transimg1);
	BwImage trans12(transimg2);

	cvNamedWindow("win0");
	while (1)
	{
		video = cvQueryFrame(cam);//从图像中抓取一帧
		cvSetMouseCallback("win0", mouse0);//进行透视变换选点
		cvShowImage("win0", video);
		if (cvWaitKey(33) >= 0)  break;//按任意键完成透视变换
	}
	cout << "透视变换完成，按任意键开始细化处理……" << endl;

	newpoints[0] = cvPoint2D32f(0, 0);
	newpoints[1] = cvPoint2D32f(400, 0);
	newpoints[2] = cvPoint2D32f(0, 400);
	newpoints[3] = cvPoint2D32f(400, 400);
	int nn = 4;
	int *judge = &nn;
	while (1)
	{
		video = cvQueryFrame(cam);
		cvGetPerspectiveTransform(originpoints, newpoints, transmat);//取得透视变换矩阵
		cvWarpPerspective(video, transimg1, transmat); //根据变换矩阵计算图像的变换——透视变换
		cvDestroyWindow("win0");//关闭win0

		cvNamedWindow("win1");
		cvShowImage("win1", transimg1); //win1显示透视变换后的图像


		//cvWaitKey(0);
		//cout << "hi" << endl;

		cvCreateTrackbar("tracker", "win1", judge, 255, NULL);//创建横滑块用来调整细化程度，每一次拖动滑块后要按回车键

		for (int i = 0; i < 400; i++)
		{
			for (int j = 0; j < 400; j++)
			{
				if ((imgclass[i][j].r < *judge) && (imgclass[i][j].b < *judge) && (imgclass[i][j].g < *judge))
				{
					trans12[i][j] = 255;
				}

				else trans12[i][j] = 0;
			}
		}

		cvNamedWindow("win2");
		cvThin(transimg2, transimg2, 10);
		cvShowImage("win2", transimg2);//win2显示细化处理后的图像
		if (cvWaitKey(33) >= 0) break;//按任意键完成细化处理
	}
	cvDestroyWindow("win1");
	cout << "细化处理完成！" << endl << "接下来请在beginpoint上选择路径起点" << endl;

	cvNamedWindow("beginpoint");
	cvSetMouseCallback("beginpoint", mouse1);
	cvShowImage("beginpoint", transimg2);
	//cvCopyImage(transimg2, temp);//transimg2拷贝一下，不知道为啥，反正没用
	cvWaitKey(0);
	cvDestroyWindow("win2");
	cvDestroyWindow("beginpoint");
	//int num = 0;
	RoadPoint(transimg2, beginpoint, num);
	cout << "总共" << num << "个点";
	//接下来画路径点
	themap = cvCloneImage(transimg1);
	cout << "路径点如图所示：" << endl;
	cvNamedWindow("路径点");
	for (int k = 0; k<num; ++k) {
		cvCircle(themap, point[k], 5, cvScalar(255, 255, 255));
		cvShowImage("路径点", themap);
		cvWaitKey(1);
	}
}

#endif