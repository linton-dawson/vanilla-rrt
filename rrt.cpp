#include<iostream>
#include<vector>
#include<algorithm>
#include<time>
#include<random>
using namespace std;

struct linObstacle {
	int x1,x2;
	int y1,y2;
};

pair<int,int> getNearest(pair<int, int> newVert, vector<pair<int,int>> &vertex, int n)
{
	for(auto i = 0 ; i < n ; ++i)
	{
		int idx;
		int tmpDist = getDistance(newVert, vertex[i]);
		if(tmpDist < min) {
			min = tmpDist;
			idx = i;
		}
	}
}

//random configuration using mersenne twister
pair<int,int> randomConfig()
{
	random_device random_dev;
	mt19937 random_eng(random_dev());
	normal_distribution<int> dist(0,1000);
	return make_pair(dist(random_eng),dist(random_eng));
}

//checking for obstacles(linear)
bool onObstacle(pair<int, int> vert, vector<linObstacle> obs, int nObs)
{		
	const int xdiv,ydiv;
	for(auto i = 0 ; i < nObs ; ++i) {
		xdiv = (vert.first - obs[i].x1)/(obs[i].x2 - obs[i].x1);
		ydiv = (vert.second - obs[i].y1)/(obs[i].y2 - obs[i].y1);
		if(xdiv == ydiv)
			return true;
	}
	return false;
}

bool cutsObstacle(pair<int,int> nearVert, pair<int,int> newVert, vector<linObstacle> obs, int nObs)
{
	double a1 = newVert.second - nearVert.second; 
    double b1 = nearVert.first - newVert.first; 
    double c1 = a1*(nearVert.first) + b1*(nearVert.second); 
 	for(auto i = 0 ; i < nObs ; ++i) {
	    double a2 = obs[i].y2 - obs[i].y1; 
		double b2 = obs[i].x1 - obs[i].x2;  
		double c2 = a2*(obs[i].x1)+ b2*(obs[i].y1); 
  
	    double determinant = a1*b2 - a2*b1; 
  		if (determinant == 0) 
        	return false;
	}
	

auto getDistance(pair<int,int> p1, pair<int,int> p2)
{
	return sqrt(pow(p2.first - p1.first,2) + pow(p2.second - p1.first,2));
}


int main()
{

	vector<pair<int , int>> vertex;
	vertex.reserve(1000);
	cout<<"Enter starting coordinates ";
	cin>>vertex[0].first>>vertex[0].second;

	pair<const int, const int> goal;
	cout<<"Enter goal coordinates ";
	cin>>goal.first>>goal.second;

	int iter;
	cout<<"Enter rrt iterations "
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
	while(count < nVert)
	{
		auto randVert = randomConfig();
		if(onObstacle(newVert,obs,nObs)) {
			continue;
		}
		auto nearVert = getNearest(randVert,vertex,vertex.size());
		++count;
	}
	if(count == iter)
		cout<<"Iteration limit reached, graph reached "+ vertex[nVert].first + " "+ vertex[nVert].second<<endl;
	else
		cout<<"Goal reached. Final coordinates "+ newVert.first + " "+ newVert.second<<endl;
	return 0;
}
