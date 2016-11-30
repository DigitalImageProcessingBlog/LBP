#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <time.h>
#include <Windows.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;
uchar *lbpData;
uchar *lbpStr;

void CheckImage(string name,Mat img,int time=0)
{
	imshow(name, img);
	uchar key = waitKey(time);
	switch (key)
	{
	case 's':
		cout << "please print save name" << endl;
		cin >> name;
		imwrite("..\\image\\" + name + ".png", img);
		break;
	case 'S':
		cout << "please print save name" << endl;
		cin >> name;
		imwrite("..\\image\\" + name + ".png", img);
		break;
	}

}
//求三个数的中位数  
int mid(int a, int b, int c)
{
	int max = a;
	int min = b;
	if ((a <= b&&b <= c) || (c <= b&&b <= a))
	{
		return b;
	}
	else if ((b <= a&&a <= c) || (c <= a&&a <= b))
	{
		return a;
	}
	else
	{
		return c;
	}
}
// 计算跳变次数  
int getHopCount(int i)
{
	int a[8] = { 0 };
	int cnt = 0;
	int k = 7;
	while (i)
	{
		a[k] = i & 1;
		i = i >> 1;
		--k;
	}
	for (k = 0; k < 7; k++)
	{
		if (a[k] != a[k + 1])
		{
			++cnt;
		}
	}
	if (a[0] != a[7])
	{
		++cnt;
	}
	return cnt;
}

// 降维数组 由256->59  
void lbp59table(uchar *table)
{
	memset(table, 0, 256);
	uchar temp = 1;
	for (int i = 0; i < 256; i++)
	{
		if (getHopCount(i) <= 2)    // 跳变次数<=2 的为非0值  
		{
			table[i] = temp;
			temp++; 
		}
	}
}


void convert59(Mat &image, Mat &result, uchar *table)
{
	int height = image.rows;
	int width = image.cols;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int temp = image.at<uchar>(y, x);
			//cout << temp << " ";
			temp = table[image.at<uchar>(y, x)];
			result.at<uchar>(y, x) = table[image.at<uchar>(y, x)];   //  降为59维空间  
		}
	}
}
void uniformLBP(Mat &image, Mat &result)
{
	int height = image.rows;
	int width = image.cols;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			uchar neighbor[8] = { 0 };
			neighbor[0] = image.at<uchar>(mid(y - 1,0,height-1), mid(x - 1, 0, width - 1));
			neighbor[1] = image.at<uchar>(mid(y - 1, 0, height - 1), mid(x, 0, width - 1));
			neighbor[2] = image.at<uchar>(mid(y - 1, 0, height - 1), mid(x + 1, 0, width - 1));
			neighbor[3] = image.at<uchar>(mid(y, 0, height - 1), mid(x + 1, 0, width - 1));
			neighbor[4] = image.at<uchar>(mid(y + 1, 0, height - 1), mid(x + 1, 0, width - 1));
			neighbor[5] = image.at<uchar>(mid(y + 1, 0, height - 1), mid(x, 0, width - 1));
			neighbor[6] = image.at<uchar>(mid(y + 1, 0, height - 1), mid(x - 1, 0, width - 1));
			neighbor[7] = image.at<uchar>(mid(y, 0, height - 1), mid(x - 1, 0, width - 1));
			uchar center = image.at<uchar>(mid(y, 0, height - 1), mid(x, 0, width - 1));
			uchar temp = 0;
			for (int k = 0; k < 8; k++)
			{
				temp += (neighbor[k] >= center)* (1 << k);  // 计算LBP的值  
			}
			result.at<uchar>(y, x) = temp;   
			
		}
	}
}


int main()
{
	uchar table[256];
	lbp59table(table);
	stringstream tempPath;
	lbpData = new uchar[5900];
	Mat image = imread("E:\\taonvlang\\img\\face\\10010\\3.png", 0);
	if (!image.data)
	{
		cout << "Fail to load image" << endl;
		return 0;
	}
	Mat result,result59;
	result.create(Size(image.cols, image.rows), image.type());
	result59.create(Size(image.cols, image.rows), image.type());
	uniformLBP(image, result);
	CheckImage("LBP", result);
	convert59(result, result59,table);
	CheckImage("result59", result59);
	return 0;
}
