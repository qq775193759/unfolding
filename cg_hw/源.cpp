#include"Unfold_2D.h"

#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 600;



Mat paintxy(Unfold_2D unfold_2d)
{
	Mat img(Size(WINDOW_SIZE_X,WINDOW_SIZE_Y),CV_8UC3,Scalar(0,0,0));
	for(int i=0;i<unfold_2d.lenth.size();i++)
		line(img, Point(unfold_2d.x[i], unfold_2d.y[i]), Point(unfold_2d.x[i+1], unfold_2d.y[i+1]), Scalar(255,255,255));
	return img;
}

void input(const char* filename, vector<double> &x, vector<double> &y)
{
	ifstream fin(filename);
	int a,b;
	while(fin>>a)
	{
		fin>>b;
		x.push_back(a);
		y.push_back(b);
	}
}

int main()
{
	vector<double> x;
	vector<double> y;
	input("input.txt", x, y);
	cout<<"aaa"<<endl;
	Unfold_2D test(x,y);
	cout<<"aaa"<<endl;
	imshow("result", paintxy(test));

	waitKey();
	return 0;
}