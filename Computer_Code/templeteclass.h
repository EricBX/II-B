#ifndef seqQueue_h
#define seqQueue_h
//借鉴的各种类模板
//queue.h
template <class T>
class Queue {
public:
	virtual bool isEmpty() = 0;
	virtual void enQueue(const T &x) = 0;
	virtual T deQueue() = 0;
	virtual T getHead() = 0;
	virtual ~Queue() {}
};

//seqQueue.h
template <class T>
class seqQueue :public Queue<T> {
protected:
	T *elem;
	int maxSize;
	int front, rear;
	void doubleSpace();

public:
	seqQueue(int size = 10) {
		elem = new T[size]; maxSize = size; front = rear = 0;
	}
	~seqQueue() { delete[] elem; }
	bool isEmpty() { return front == rear; }
	void enQueue(const T &x);
	T deQueue();
	T getHead() { return elem[(front + 1) % maxSize]; }
};

template <class T>
void seqQueue<T>::doubleSpace()
{
	T *center = elem;
	elem = new T[2 * maxSize];
	for (int i = 1; i<maxSize; ++i) {
		elem[i] = center[(front + i) % maxSize];
	}
	front = 0; rear = maxSize - 1;
	maxSize *= 2;
	delete center;
	return;
}

template <class T>
void seqQueue<T>::enQueue(const T &x)
{
	if ((rear + 1) % maxSize == front) doubleSpace();
	rear = (rear + 1) % maxSize;
	elem[rear] = x;
	return;
}

template <class T>
T seqQueue<T>::deQueue()
{
	front = (front + 1) % maxSize;
	return elem[front];
}


//创建类模板用来储存不同类型图像
template<class T>
class Image {
public:
	IplImage* imgp;
	Image(IplImage* img = 0) :imgp(img) {}
	~Image()
	{
		imgp = 0;
	}

	inline T* operator[] (const int rowIndx)
	{
		return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));
	}
};
typedef struct {
	unsigned char b, g, r;
} RgbPixel;
typedef Image<RgbPixel> RGBImage;//彩色图像类，用来便于访问RGB值
typedef Image<unsigned char> BwImage;//灰度图像类，可用来读取灰度值
#endif
