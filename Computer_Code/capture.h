#ifndef capture_h
#define capture_h

#include "function.h"
#include "bluetooth.h"

//����׷��С���ĺ���
void capture(CvCapture* cam)
{
	IplImage *HUE, *SAT, *VAL, *HUE_, *SAT_, *VAL_, *Binary, *Binary_;
	CvRect window, window_;//�����������α߿�
	CvTermCriteria criteria = cvTermCriteria(3, 100, 0.1);//������ֹ׼������������20�������׼ȷ��1����
	CvConnectedComp comp, comp_;//����һ����ͨ��Ľṹ��

	cvNamedWindow("H_1"); cvNamedWindow("H_2");//������ʾ��ͷ��βʵʱ׷��λ�ã������������У���������׷�ٵ���ʲô������
	cvNamedWindow("ѡȡ��ͷ->��β"); cvSetMouseCallback("ѡȡ��ͷ->��β", mouse_gothsv);//��������¼���ú���������û�о�����Ҳһֱ���ڱ���Ӧ״̬��
	
	//�����̵�
	Convert = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 3);
	HUE = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); HUE_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	SAT = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); SAT_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	VAL = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); VAL_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	Binary = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1); Binary_ = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
	//
	while (gothsv != 2) {
		video = cvQueryFrame(cam);
		cvWarpPerspective(video, transimg3, transmat);//���ݱ任�������ͼ��ı任��transimg3����͸�ӱ任�Ľ��	
		cvShowImage("ѡȡ��ͷ->��β", transimg3);
		cvWaitKey(1);
	}

	for (int i = 0; i < num; i++)
	{
		cout << "��" << i+1 << "���յ�Ϊ(" << point[i].x << "," << point[i].y << ")" << endl;
	}
	int i = 1;//�����ж��Ƿ񵽴�ÿ����
	cvDestroyWindow("ѡȡ��ͷ->��β");
	//cvNamedWindow("׷�٣�");
	bool flag = true;
	while (flag) {
		
		video = cvQueryFrame(cam);
		cvWarpPerspective(video, transimg3, transmat);//���ݱ任�������ͼ��ı任
		cvCvtColor(transimg3, Convert, CV_BGR2HSV);//��ɫ�ռ�ת����Ϊ��HSV���洢��Convert��
		//��ͷ����
		cvSplit(Convert, HUE, SAT, VAL, 0);//��Convert�еĶ�ͨ��������������ͨ��H��S��V��
		cvInRangeS(HUE, cvScalar(hue[0] - 30), cvScalar(hue[0] + 30), HUE);
		cvInRangeS(SAT, cvScalar(sat[0] - 30), cvScalar(sat[0] + 30), SAT);
		cvInRangeS(VAL, cvScalar(val[0] - 30), cvScalar(val[0] + 30), VAL);
		cvMul(HUE, SAT, Binary); cvMul(Binary, VAL, Binary);//����˷�								//�����˵�������ʲô��
		//Binary = HUE;															//ʲô���壿
		cvShowImage("H_1", Binary);//Binary �����ֵ��ͼ��
		//----
		window = cvRect(head.x - 50, head.y - 50, 100, 100);
		cvMeanShift(Binary, window, criteria, &comp);
		head.x = comp.rect.x + comp.rect.width / 2;
		head.y = comp.rect.y + comp.rect.height / 2;
		cvCircle(transimg3, head, 10, cvScalar(255, 255, 0), 3);//��ԲȦ
		//��β����
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
		cvShowImage("׷��", transimg3);
		center.x = tail.x / 2 + head.x / 2;
		center.y = tail.y / 2 + head.y / 2;
		cout << "С������λ�ã�ͷ����" << head.x << "��" << head.y << "��\tβ��(" << tail.x << "," << tail.y << ")" << endl;

			if (getDistance(center, point[i]) < 60) {
				cout << "�����" << i-1 <<"����"<< endl;
				i++;
			}
			cout << "�����ڵ�" << i-1 << "����" << endl;
			cout << "������һ���㣺" << getDistance(center, point[i]) << endl;
		//����������н�������
	//	cout << "'·��'������";
		int road_x = point[i].x - tail.x; //cout << "road_x=" << road_x << endl;
		int road_y = point[i].y - tail.y; //cout << "road_y=" << road_y << endl;
		//cout << "С��������";
		int car_x = head.x - tail.x;// cout << "car_x=" << car_x << endl;
		int car_y = head.y - tail.y; //cout << "car_y=" << car_y << endl;
		double carLength = sqrt(car_x*car_x + car_y*car_y); //cout << carLength << "carLength" << endl;
		double roadLength = sqrt(road_x*road_x + road_y*road_y); //cout << roadLength << "roadLength" << endl;

		double sinAngle = (car_x*road_y - car_y*road_x) / (carLength*roadLength);
		double cosAngle = (car_x*road_x + car_y*road_y) / (carLength*roadLength);
		
		cout << "sinAngle is "  << sinAngle << endl;
		cout << "cosAngle is "  << cosAngle << endl;
		double c = 0.5;
		if (cosAngle<=0.15||head.x>=350||head.x<=50||head.y<=50||head.y>=350|| tail.x>=350 || tail.x<=50 || tail.y<=50 || tail.y>=350)//��������
		{
			sendCommand('B');
			Sleep(700);
			cout << "B" << endl;
		}
		else if (sinAngle >= 0.258*c)//��ת����
		{
			sendCommand('R');
			Sleep(200);
			cout << "R" << endl<<endl;
		}
		else if (sinAngle <= -0.258*c)//��ת����
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

//�ú����ж�С���Ƿ�ĳһ�յ㣬���Ըĳ��յ��ж�������Ҳ����Ϊ��ʻʱ���ж��������ж��Ƿ����µ�ָ��ĺ���
//Ŀǰδ����
#endif