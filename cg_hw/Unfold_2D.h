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
	Unfold_2D(Unfold_2D p2d, int k, double small_delta);
	Unfold_2D(Unfold_2D p2d, vector<double> delta_theta, double step_len);

	double distance(int a, int b);
	double cal_energy();
	double cal_delta_energy(int k, double small_delta, double energy);
	vector<double> cal_delta_theta();//calculate delta_theta
	Unfold_2D transform();
};

