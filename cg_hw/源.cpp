#include"Unfold_2D.h"
#include <iostream>
#include <fstream>
#include <sstream>
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

const int INIT_X = 700;
const int INIT_Y = 500;

void read_3d(const char* filename, vector<double> &x, vector<double> &y)
{
	const int dx[6] = {1,-1,0,0,0,0};
	const int dy[6] = {0,0,1,-1,0,0};
	const int dz[6] = {0,0,0,0,-1,1};
	ifstream fin(filename);
	int a, old_a = -1;
	int tmp_x = INIT_X, tmp_y= INIT_Y, tmp_depth1 = 0, tmp_depth2 = 0;
	while(fin>>a)
	{
		if((a != old_a)&&(dy[a]==0))
		{
			//record
			x.push_back(tmp_x);
			y.push_back(tmp_y);
			Unfold_2D::v_depth1.push_back(min(tmp_depth1, tmp_depth2));
			Unfold_2D::v_depth2.push_back(max(tmp_depth1, tmp_depth2));
			Unfold_2D::e_depth.push_back(tmp_depth2);
			tmp_depth1 = tmp_depth2;
			//cout<<x.back()<<" "<<y.back()<<" "<<Unfold_2D::v_depth1.back()<<" "<<Unfold_2D::v_depth2.back()<<endl;
		}
		tmp_x += dx[a]*10;
		tmp_depth2 += dy[a];
		tmp_y += dz[a]*10;
		old_a = a;
	}
}

string getoutname(int n)
{
	stringstream sst;
	sst<<n;
	string res;
	sst>>res;
	res=res+string(".jpg");
	return res;
}

int main()
{
	vector<double> x;
	vector<double> y;
	//input("input.txt", x, y);
	//read_3d("bunny_y.txt", x, y);
	read_3d("eight_x.txt", x, y);
	Unfold_2D test(x,y);
	int n = 0;
	while(1)
	{
		imshow("result", paintxy(test));
		waitKey();
		test = test.transform();
		
		imwrite(getoutname(n), paintxy(test));
		n++;
	}
	return 0;
}