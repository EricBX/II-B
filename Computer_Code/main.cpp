#include "function.h"
#include "prepare.h"
#include "capture.h"
#include "bluetooth.h"

int main()
{
	cout << "hello world" << endl;
	//��������
	sendCommand('S');

	//����������ͷ�ķ���������ͬ���Է�����ͬ��
		CvCapture* cam0 = cvCreateCameraCapture(0);
		CvCapture* cam = cvCreateCameraCapture(1);
	//����������ͷ�ķ�����
	//CvCapture* cam = cvCreateCameraCapture(0);

	//��������ͷ
	if (!cam) { cout << "Camera error!" << endl; return 0; }
	//׼���
	prepare(cam);
	//׷���㷨

	capture(cam);
	sendCommand('S');
	cout << "�󹦸�ɣ�";
	return 0;

}




















