#ifndef prepare_h
#define prepare_h

#include "function.h"

//preparation(͸�ӱ任��ϸ������·��ѡ��)

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
		std::cout << 1 << std::endl;//�ұ�
		for (i = center.x + 30, j = center.y - 30; !find && j<center.y + 30; ++j) {
			if (deal[j][i] == 255 && (distance2(tmp.x, tmp.y, i, j) >= 900)) {

				tmp = cvPoint(center.x, center.y);
				center.x = i; center.y = j;

				que.enQueue(center);
				++num; find = true;
			}
		}

		std::cout << 2 << std::endl;//�ױ�
		for (i = center.x + 30, j = center.y + 30; !find && i>center.x - 30; --i) {
			if (deal[j][i] == 255 && (distance2(tmp.x, tmp.y, i, j) >= 900)) {
				tmp = cvPoint(center.x, center.y);
				center.x = i; center.y = j;
				que.enQueue(center);
				++num; find = true;
			}
		}

		std::cout << 3 << std::endl;//���
		for (i = center.x - 30, j = center.y + 30; !find && j>center.y - 30; --j) {
			if (deal[j][i] == 255 && (distance2(tmp.x, tmp.y, i, j) >= 900)) {
				tmp = cvPoint(center.x, center.y);
				center.x = i; center.y = j;
				que.enQueue(center);
				++num; find = true;
			}
		}
		std::cout << 4 << std::endl;//�ϱ�
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
		video = cvQueryFrame(cam);//��ͼ����ץȡһ֡
		cvSetMouseCallback("win0", mouse0);//����͸�ӱ任ѡ��
		cvShowImage("win0", video);
		if (cvWaitKey(33) >= 0)  break;//����������͸�ӱ任
	}
	cout << "͸�ӱ任��ɣ����������ʼϸ��������" << endl;

	newpoints[0] = cvPoint2D32f(0, 0);
	newpoints[1] = cvPoint2D32f(400, 0);
	newpoints[2] = cvPoint2D32f(0, 400);
	newpoints[3] = cvPoint2D32f(400, 400);
	int nn = 4;
	int *judge = &nn;
	while (1)
	{
		video = cvQueryFrame(cam);
		cvGetPerspectiveTransform(originpoints, newpoints, transmat);//ȡ��͸�ӱ任����
		cvWarpPerspective(video, transimg1, transmat); //���ݱ任�������ͼ��ı任����͸�ӱ任
		cvDestroyWindow("win0");//�ر�win0

		cvNamedWindow("win1");
		cvShowImage("win1", transimg1); //win1��ʾ͸�ӱ任���ͼ��


		//cvWaitKey(0);
		//cout << "hi" << endl;

		cvCreateTrackbar("tracker", "win1", judge, 255, NULL);//�����Ử����������ϸ���̶ȣ�ÿһ���϶������Ҫ���س���

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
		cvShowImage("win2", transimg2);//win2��ʾϸ��������ͼ��
		if (cvWaitKey(33) >= 0) break;//����������ϸ������
	}
	cvDestroyWindow("win1");
	cout << "ϸ��������ɣ�" << endl << "����������beginpoint��ѡ��·�����" << endl;

	cvNamedWindow("beginpoint");
	cvSetMouseCallback("beginpoint", mouse1);
	cvShowImage("beginpoint", transimg2);
	//cvCopyImage(transimg2, temp);//transimg2����һ�£���֪��Ϊɶ������û��
	cvWaitKey(0);
	cvDestroyWindow("win2");
	cvDestroyWindow("beginpoint");
	//int num = 0;
	RoadPoint(transimg2, beginpoint, num);
	cout << "�ܹ�" << num << "����";
	//��������·����
	themap = cvCloneImage(transimg1);
	cout << "·������ͼ��ʾ��" << endl;
	cvNamedWindow("·����");
	for (int k = 0; k<num; ++k) {
		cvCircle(themap, point[k], 5, cvScalar(255, 255, 255));
		cvShowImage("·����", themap);
		cvWaitKey(1);
	}
}

#endif