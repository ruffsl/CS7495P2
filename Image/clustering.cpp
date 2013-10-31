#include "clustering.hpp"
#include <cstdlib> /* srand, rand */
#include <ctime> /* time */
#include <cmath>
#include <cstdio>
#include <sstream>

using namespace cs7495;
using namespace std;
using namespace arma;

double dist(arma::mat x, arma::mat y) {
  double R = 6371 * 1000; // km

  mat d = (x - y) *  M_PI / 180;

  x = x * M_PI / 180;
  y = y * M_PI / 180;

  double a = sin(d(0,0) / 2) * sin(d(0,0) / 2) +
    sin(d(0,1)/2) * sin(d(0,1)/2) * cos(x(0, 0)) * cos(y(0 ,0));
  double c = 2 * atan2(sqrt(a), sqrt(1-a));
  return R * c;
}

vector<mat> locations(double radius, vector<ImgInfo> points) {
  
  //srand(time(0));
  vector<mat> locations;
  while(points.size() > 10) {
    int center = rand() % points.size();
    mat location;
    location << points[center].GPScoord[0] << points[center].GPScoord[1] << endr;
    
    printf("L: %2.20f %2.20f\n", location(0, 0), location(0,1));
    vector<ImgInfo> _points;
    ofstream myfile;


    stringstream ss;
    ss << points[center].GPScoord[0] << "_" << points[center].GPScoord[1]  << "cluster.txt";
    ss.precision(20);
    string s = ss.str();   
    myfile.open(s);

    for(int i = 0; i < points.size(); i++) {
      mat point;
      point << points[i].GPScoord[0] << points[i].GPScoord[1] << endr;
      if(dist(location, point) >= radius) {
         _points.push_back(points[i]);
      } else {	
	myfile << points[i].pathToSIFTfile <<"\n";
      }
    }
    myfile.close();
    points = _points;
    locations.push_back(location);
  }
  cout << locations.size() << endl;
  return locations;
}
