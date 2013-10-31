#include "clustering.hpp"
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include <cmath>
#include <cstdio>

using namespace std;
using namespace arma;

double dist(arma::mat x, arma::mat y) {
  double R = 6371000; // km

  mat d = (x - y) * M_PI / 180;  

  x = x * M_PI / 180;
  y = y * M_PI / 180;

  double a = sin(d(0,0)/2) * sin(d(0,0)) +
    sin(d(0,1)/2) * sin(d(0,1)/2) * cos(x(0, 0)) * cos(y(0 ,0)); 
  double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
  return R * c;
}

vector<mat> locations(double radius, vector<mat> points) {
  
  srand(time(0));
  vector<mat> locations;
  while(points.size() > 0) {
    int center    = rand() % points.size();
    mat location  = points[center];
    mat _location = zeros(1, 2);
    while(dist(location, _location) > 0) {
      mat l = location; //zeros(1, 2);            
      int num = 1;
      for(int i = 0; i < points.size(); i++) {
	double x = dist(location, points[i]);
	if(x < radius) { 
	  l += points[i];
	  num++;
	}
      }      
      l /= num;
      // printf("           %2.20f %2.20f %d\n", l(0, 0), l(0,1), num);
      _location = location;
      location = l;
    }
    printf("L: %2.20f %2.20f\n", location(0, 0), location(0,1));
    vector<mat> _points;
    for(int i = 0; i < points.size(); i++) {
      if(dist(location, points[i]) >= radius) { 
	  _points.push_back(points[i]);
      } else {
	printf(" - %2.20f %2.20f\n", points[i](0, 0), points[i](0,1));
      }
    }
    points = _points;
    locations.push_back(location);
    
  }
  
  return locations;
}
