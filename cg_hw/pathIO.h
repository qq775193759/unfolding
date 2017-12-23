#pragma once

#include <fstream>
#include <sstream>
#include"Unfold_2D.h"

using namespace std;

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

const int INIT_X = 300;
const int INIT_Y = 500;
const int DELTA_X = 40;
const int DELTA_Y = -40;

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
		tmp_x += dx[a]*DELTA_X;
		tmp_depth2 += dy[a];
		tmp_y += dz[a]*DELTA_Y;
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

void read_2d_config_from_3d(const char* filename, vector<double> &x, vector<double> &y)
{
	const int dx[6] = {1,1,0,0,1,1};
	const int dy[6] = {0,0,1,-1,0,0};
	ifstream fin(filename);
	int a, old_a = -1;
	int tmp_x = INIT_X, tmp_y= INIT_Y;
	while(fin>>a)
	{
		if(dx[a] != dx[old_a])
		{
			//record
			x.push_back(tmp_x);
			y.push_back(tmp_y);
		}
		tmp_x += dx[a]*3;
		tmp_y += dy[a]*3;
		old_a = a;
	}
	cout<<x.size()<<endl;
}

void read_path_3d(const char* filename, vector<double> &x, vector<double> &y)
{
	ifstream fin(filename);
	double a,b;
	int depth, old_depth;
	int n;
	//first one
	fin>>n;
	fin>>a>>b>>old_depth;
	x.push_back(a);
	y.push_back(b);
	//middle
	for(int i=1;i<n;i++)
	{
		fin>>a>>b>>depth;
		x.push_back(a);
		y.push_back(b);
		Unfold_2D::v_depth1.push_back(min(old_depth, depth));
		Unfold_2D::v_depth2.push_back(max(old_depth, depth));
		Unfold_2D::e_depth.push_back(depth);
		old_depth = depth;
	}
	//last one
	fin>>depth;
	Unfold_2D::v_depth1.push_back(min(old_depth, depth));
	Unfold_2D::v_depth2.push_back(max(old_depth, depth));
	Unfold_2D::e_depth.push_back(depth);
}


void print_config_3d(const char* filename, Unfold_2D u2d)
{
	ofstream fout(filename);
	int depth = 0;
	cout<<u2d.v_depth1.size()<<endl;
	cout<<u2d.v_depth2.size()<<endl;
	cout<<u2d.e_depth.size()<<endl;
	cout<<u2d.x.size()<<endl;
	fout<<u2d.x.size()<<" ";
	for(int i=0;i<u2d.x.size();i++)
	{
		fout<<depth<<" ";
		depth = Unfold_2D::e_depth[i];
		fout<<depth<<" ";
	}
	fout<<endl;
	fout.close();
}

void print_path_2d(const char* filename, Unfold_2D u2d)
{
	ofstream fout(filename, ios::app);
	for(int i=0;i<u2d.x.size();i++)
	{
		fout<<u2d.x[i]<<" "<<u2d.y[i]<<" ";
	}
	fout<<endl;
	fout.close();
}