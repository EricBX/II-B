#include "function.h"
#include "prepare.h"
#include "capture.h"
#include "bluetooth.h"

int main()
{
	cout << "hello world" << endl;
	//测试蓝牙
	sendCommand('S');

	//打开外置摄像头的方法：（不同电脑方法不同）
		CvCapture* cam0 = cvCreateCameraCapture(0);
		CvCapture* cam = cvCreateCameraCapture(1);
	//打开内置摄像头的方法：
	//CvCapture* cam = cvCreateCameraCapture(0);

	//测试摄像头
	if (!cam) { cout << "Camera error!" << endl; return 0; }
	//准备活动
	prepare(cam);
	//追踪算法

	capture(cam);
	sendCommand('S');
	cout << "大功告成！";
	return 0;

}




















