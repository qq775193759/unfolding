#include "Unfold_2D.h"


Unfold_2D::Unfold_2D(vector<double> ptheta, vector<double> plenth, double x1, double y1):theta(ptheta), lenth(plenth)
{
	x.push_back(x1);
	y.push_back(y1);
	theta2xy();
}

Unfold_2D::Unfold_2D(vector<double> px, vector<double> py):x(px),y(py)
{
	xy2theta();
}

void Unfold_2D::xy2theta()
{
	theta.clear();
	lenth.clear();
	for(int i=1;i<x.size();i++)
	{
		double ttheta,tlenth;
		tlenth = sqrt((x[i]-x[i-1])*(x[i]-x[i-1])+(y[i]-y[i-1])*(y[i]-y[i-1]));
		ttheta = atan2(x[i]-x[i-1], y[i]-y[i-1]);
		theta.push_back(ttheta);
		lenth.push_back(tlenth);
	}
}

void Unfold_2D::theta2xy()
{
	double tx=x[0], ty=y[0];
	x.clear();y.clear();
	x.push_back(tx);
	y.push_back(ty);
	for(int i=0;i<lenth.size();i++)
	{
		tx+=lenth[i]*cos(theta[i]);
		ty+=lenth[i]*sin(theta[i]);
		x.push_back(tx);
		y.push_back(ty);
	}
}

double Unfold_2D::distance(int a, int b)
{
	return sqrt((x[a]-x[b])*(x[a]-x[b])+(y[a]-y[b])*(y[a]-y[b]));
}

double Unfold_2D::cal_energy()
{
	double res = 0;
	for(int i=0;i<x.size();i++)//point
		for(int j=0;j<theta.size();j++)//edge (j, j+1)
		{
			if(i==j) continue;
			if(i==(j+1)) continue;
			double t_res = distance(i, j)+distance(i, j+1)-distance(j, j+1);
			res += 1/(t_res*t_res);
		}
}

double Unfold_2D::cal_delta_energy(int k)
{
	double res;
	return res;
}

vector<double> Unfold_2D::cal_delta_theta()
{
	vector<double> res = theta;
	for(int i=0;i<res.size();i++)
		res[i] = cal_delta_energy(i);
	return res;
}