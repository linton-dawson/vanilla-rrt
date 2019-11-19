#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>
#include<random>
using namespace std;

//Linear obstacle coordinates
struct linObstacle {
	double x1,x2;
	double y1,y2;
};

//Distance function
auto getDistance(pair<int,int> p1, pair<int,int> p2)
{
	return sqrt(pow(p2.first - p1.first,2) + pow(p2.second - p1.first,2));
}

//returns nearest vertex present in the graph
pair<int,int> getNearest(pair<double, double> newVert, vector<pair<double,double>> &vertex, int n)
{
	int idx = 0;
	double min = 0;
	for(auto i = 0 ; i < n ; ++i)
	{
		double tmpDist = getDistance(newVert, vertex[i]);
		if(tmpDist < min) {
			min = tmpDist;
			idx = i;
		}
	}
	return vertex[idx];
}


//random configuration using mersenne twister
pair<double,double> randomConfig(double xbound, double ybound, const double xd, const double yd)
{
	random_device rd;
//	random_device rd1;
	mt19937 mtw(rd());
	uniform_real_distribution<double> dist(xbound,xbound + xd);
	auto n1 =dist(mtw);
//	mt19937 mtw1(rd1());
	uniform_real_distribution<> dist1(ybound,ybound + yd);
	auto n2 =dist1(mtw);
	return make_pair(n1,n2);
}


//checking for obstacles(linear)
bool onObstacle(pair<double, double> vert, vector<linObstacle> obs, int nObs)
{		
	double xdiv,ydiv;
	for(auto i = 0 ; i < nObs ; ++i) {
		xdiv = (vert.first - obs[i].x1)/(obs[i].x2 - obs[i].x1);
		ydiv = (vert.second - obs[i].y1)/(obs[i].y2 - obs[i].y1);
		if(xdiv == ydiv)
			return true;
	}
	return false;
}


//double cross( pair<double , double> seg1 , pair<double, double> seg2)
//	return seg1.first * seg2.second - seg2.first * seg1.second;

//gets point direction wrt line segment
double direction(pair<double, double> pi , pair<double,double> pj, pair<double , double> pk)
{
	pair<double,double> seg1;
	seg1.first = pk.first - pi.first;
	seg1.second = pk.second - pi.second;
	pair<double, double> seg2;
	seg2.first = pj.first - pi.first;
	seg2.second = pj.second - pi.second;
	auto tmp1 = seg1.first * seg2.second;
	auto tmp2 = seg2.first * seg1.second;
	return tmp1 - tmp2;
}


//driver function to check intersection of random segment and nearest vertex with obstacle
bool cutsObstacle(pair<int,int> nearVert, pair<int,int> newVert, vector<linObstacle> obs, int nObs)
{
	for(auto i = 0 ; i < nObs ; ++i) {
		auto d1 = direction(make_pair(obs[i].x1,obs[i].y1), make_pair(obs[i].x2, obs[i].y2), nearVert);
		auto d2 = direction(make_pair(obs[i].x1,obs[i].y1), make_pair(obs[i].x2, obs[i].y2), newVert);
		auto d3 = direction(nearVert, newVert, make_pair(obs[i].x1,obs[i].y1));
		auto d4 = direction(nearVert,newVert,make_pair(obs[i].x2, obs[i].y2));
		if(((d1 > 0 && d2 < 0) || (d1 <0 && d2 >0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
			return true;
	}
	return false;
}


int main()
{

	vector<pair<double, double>> vertex;
	double xinit,yinit;
	cout<<"Enter starting coordinates ";
	cin>>xinit>>yinit;
	vertex.push_back(make_pair(xinit,yinit));
	pair<double,double> goal;
	cout<<"Enter goal coordinates ";
	cin>>goal.first>>goal.second;

	int iter;
	cout<<"Enter rrt iterations ";
	cin>>iter;

	int nVert;
	cout<<"Enter number of vertices ";
	cin>>nVert;

	int nObs;
	cout<<"Enter number of obstacles ";
	cin>>nObs;

	vector<linObstacle> obs(nObs);
	for(auto i = 0 ; i < nObs ; ++i)
		cin>>obs[i].x1>>obs[i].y1>>obs[i].x2>>obs[i].y2;

	int count = 0;
	bool vertLimit = false;
	static double xdel = (goal.first - xinit) / nVert;
	static double ydel = (goal.second - yinit) / nVert;
	cout<<xdel<<endl<<ydel<<endl;
	while(count < iter) 
	{
		auto newVert = randomConfig(vertex.back().first, vertex.back().second,xdel + 0.5,ydel + 0.5);
//		xdel = xdel + ((goal.first - vertex.back().first)/nVert);
//		ydel = ydel + ((goal.second - vertex.back().second)/nVert);
		if(onObstacle(newVert,obs,nObs)) {
			cout<<"randomConfig on Obstacle\n";
			continue;
		}
		auto nearVert = getNearest(newVert,vertex,vertex.size());
		if(cutsObstacle(nearVert,newVert,obs,nObs)) {
			cout<<"("<<newVert.first<<","<<newVert.second<<") ("<<nearVert.first<<","<<nearVert.second<<")";
			cout<<" cuts obstacle \n";
			continue;
		}
		cout<<"Adding vertex "<<newVert.first<<" "<<newVert.second<<endl;
		cout<<round(newVert.first)<<" "<<round(newVert.second)<<endl;
		if(round(newVert.first) == goal.first && round(newVert.second) == goal.second) {
			cout<<"GOAL REACHED !\n";
			break;
		}
		if(vertex.size() > nVert){
			vertLimit = true;
			break;
		}
		vertex.push_back(newVert);
				++count;
	}
	if(vertLimit)
		cout<<"Vertex limit reached\n";
	else if(count == iter)
		cout<<"Iteration limit reached, graph reached "<<vertex[nVert].first<<" "<< vertex[nVert].second<<endl;
	cout<<"Vertices are ->\n";
	for(auto i : vertex)
		cout<<i.first<<", "<<i.second<<endl;
	
	return 0;
}
