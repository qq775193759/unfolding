#pragma once
#include<iostream>
#include<vector>
#include<cmath>
using namespace std;


class Unfold_2D
{
public:
	vector<double> x,y;
	vector<double> theta, lenth;
	void xy2theta();
	void theta2xy();
	Unfold_2D(vector<double> px, vector<double> py);
	Unfold_2D(vector<double> ptheta, vector<double> plenth, double x1, double y1);

	double distance(int a, int b);
	double cal_energy();
	double cal_delta_energy(int k);
	vector<double> cal_delta_theta();//calculate delta_theta
};

