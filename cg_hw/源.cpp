#include"Unfold_2D.h"
#include "pathIO.h"
#include <iostream>
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



int main()
{
	vector<double> x;
	vector<double> y;
	//input("input.txt", x, y);
	//read_3d("bunny_y.txt", x, y);
	read_3d("bunny_y.txt", x, y);
	Unfold_2D test(x,y);
	int n = 0;
	while(1)
	{
		//imshow("result", paintxy(test));
		//waitKey();
		test = test.transform();
		print_path_3d("bunny_log", test);
		if(n%10 == 0)
			imwrite(getoutname(n), paintxy(test));
		n++;
	}
	return 0;
}