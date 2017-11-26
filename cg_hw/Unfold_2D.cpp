#include "Unfold_2D.h"
#include <thread>


Unfold_2D::Unfold_2D(vector<double> px, vector<double> py):x(px),y(py)
{
	xy2theta();
}

Unfold_2D::Unfold_2D(Unfold_2D p2d, int k, double small_delta):x(p2d.x),y(p2d.y),theta(p2d.theta), lenth(p2d.lenth)
{
	theta[k]+=small_delta;
	theta2xy();
}

Unfold_2D::Unfold_2D(Unfold_2D p2d, vector<double> delta_theta, double step_len):theta(p2d.theta), lenth(p2d.lenth)
{
	x.push_back(p2d.x[0]);
	y.push_back(p2d.y[0]);
	for(int i=0;i<theta.size();i++)
	{
		theta[i] += delta_theta[i]*(-step_len);
	}
	theta2xy();
}

void Unfold_2D::xy2theta()
{
	theta.clear();
	lenth.clear();
	for(int i=1;i<x.size();i++)
	{
		double ttheta,tlenth;
		tlenth = sqrt((x[i]-x[i-1])*(x[i]-x[i-1])+(y[i]-y[i-1])*(y[i]-y[i-1]));
		ttheta = atan2(y[i]-y[i-1], x[i]-x[i-1]);
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
			if(check_depth(i,j) == 0) continue;
			//cout<<i<<" "<<j<<" "<<x[i]<<" "<<y[i]<<" "<<x[j]<<" "<<y[j]<<" "<<x[j+1]<<" "<<y[j+1]<<" "<<v_depth1[i]<<" "<<v_depth2[i]<<" "<<v_depth2[j]<<endl;
			double t_res = distance(i, j)+distance(i, j+1)-distance(j, j+1);
			res += 1/(t_res*t_res);
		}
	return res;
}

double Unfold_2D::cal_delta_energy(int k, double small_delta, double energy)
{
	Unfold_2D tmp_unfold_2d(*this, k, small_delta);
	double res = (tmp_unfold_2d.cal_energy() - energy)/small_delta;
	return res;
}


void thread_fun_cal_delta_energy(int k, double small_delta, double energy, Unfold_2D u2d, double& res)
{
	Unfold_2D tmp_unfold_2d(u2d, k, small_delta);
	res = (tmp_unfold_2d.cal_energy() - energy)/small_delta;
}

vector<double> Unfold_2D::cal_delta_theta()
{
	vector<double> res = theta;
	vector<thread> threads(res.size());
	double energy = cal_energy();
	cout<<energy<<endl;
	double res_sum = 0;
	for(int i=0;i<res.size();i++)
	{
		//res[i] = cal_delta_energy(i, 0.0001, energy);
		threads[i] = thread(thread_fun_cal_delta_energy, i, 0.0001, energy, *this, ref(res[i]));
		//thread_fun_cal_delta_energy(i, 0.0001, energy, *this, res[i]);
		//cout<<res[i]<<endl;
	}
	for(int i=0;i<res.size();i++)
	{
		threads[i].join();
	}
	for(int i=0;i<res.size();i++)
	{
		res_sum += res[i]*res[i];
	}
	res_sum = sqrt(res_sum);
	for(int i=0;i<res.size();i++)
	{
		res[i] = res[i]/res_sum;
		//cout<<res[i]<<" ";
	}
	//cout<<endl;
	return res;
}

Unfold_2D Unfold_2D::transform()
{
	vector<double> delta_theta = cal_delta_theta();
	return Unfold_2D(*this, delta_theta, 0.01);
}

vector<int> Unfold_2D::v_depth1, Unfold_2D::v_depth2, Unfold_2D::e_depth;

int Unfold_2D::check_depth(int i, int j)
{
	if(v_depth1.size() == 0)
		return 1;
	if(v_depth1[i] > e_depth[j])
		return 0;
	if(v_depth2[i] < e_depth[j])
		return 0;
	return 1;
}