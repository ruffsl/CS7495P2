#include  <time.h>
#include <iostream>
#include <cfloat>
#include <gtsam/nonlinear/ExtendedKalmanFilter.h>
#include <gtsam/nonlinear/Symbol.h>
#include <gtsam/slam/PriorFactor.h>
#include <gtsam/slam/BetweenFactor.h>
#include <gtsam/geometry/Point2.h>
#include <sstream>
#include "predictor_table.h"
#include "gt_table.h"
#include "clustering.hpp"
#include "list_everything_odd.h"
#include <armadillo>

using namespace std;
using namespace arma;
using namespace gtsam;
typedef Point2 LinearMeasurement;

// initialize at file boundaries
int reinit_at[3] = {
  528,
  1125,
  2083
};

double locationxy [60][2]= {
  {33.774 ,-84.3961},
  {33.774 ,-84.3966},
  {33.774 ,-84.3977},
  {33.777 ,-84.3907},
  {33.777 ,-84.3914},
  {33.777 ,-84.3949},
  {33.7715 ,-84.3954},
  {33.7723 ,-84.3945},
  {33.7723 ,-84.3951},
  {33.7723 ,-84.3958},
  {33.7725 ,-84.3966},
  {33.7728 ,-84.3947},
  {33.7729 ,-84.3954},
  {33.7732 ,-84.397},
  {33.7737 ,-84.3954},
  {33.7737 ,-84.3972},
  {33.7741 ,-84.3984},
  {33.7746 ,-84.3988},
  {33.7747 ,-84.3969},
  {33.7747 ,-84.3993},
  {33.7752 ,-84.4},
  {33.7753 ,-84.3967},
  {33.7753 ,-84.4006},
  {33.7758 ,-84.3967},
  {33.7759 ,-84.4008},
  {33.7759 ,-84.4013},
  {33.7761 ,-84.4019},
  {33.7761 ,-84.4024},
  {33.7764 ,-84.3961},
  {33.7764 ,-84.3967},
  {33.7766 ,-84.3956},
  {33.7767 ,-84.4023},
  {33.7769 ,-84.3898},
  {33.7769 ,-84.3967},
  {33.7771 ,-84.3921},
  {33.7771 ,-84.3936},
  {33.7771 ,-84.394},
  {33.7771 ,-84.3976},
  {33.7773 ,-84.4022},
  {33.7778 ,-84.3936},
  {33.7778 ,-84.3978},
  {33.7779 ,-84.3921},
  {33.7782 ,-84.3925},
  {33.7782 ,-84.3931},
  {33.7783 ,-84.3979},
  {33.7787 ,-84.3936},
  {33.7788 ,-84.3978},
  {33.7793 ,-84.3935},
  {33.7799 ,-84.3937},
  {33.7799 ,-84.3977},
};


auto ssplit(const std::string& value, char separator) 
  -> std::vector<std::string> { 
  std::vector<std::string> result; 
  std::string::size_type p = 0; 
  std::string::size_type q; 
  while ((q = value.find(separator, p)) != std::string::npos) { 
    result.emplace_back(value, p, q - p); p = q + 1; \
  }
  result.emplace_back(value, p); 
  return result; 
}

vector<int> closed;

Point2 executeSearch(double x, double y, int file) {
  double min = DBL_MAX;
  int argmin = 0;
  for(int i = 0 ; i < 50; i++) {
    mat a, b;
    a << x << y << endr;
    b << locationxy[i][0] << locationxy[i][1] << endr;
    double d = dist(a, b);
    if(d < min) {
      min = d;
      argmin = i;
    }
      
  }
  
  stringstream ssfiles;
  ssfiles << "echo \"" << videos[file] << "\" > query";
  FILE *in;
  if(!(in = popen(ssfiles.str().c_str(), "r"))) {
    cout << "OUCH" << endl;
  }      
  
  char buff[512];
  stringstream ss;
  ss << "./VocabMatch ../data/forrest/tree_db_" << argmin << ".out ../data/list_everything_even.in query 1 result";
  
  ifstream infile("result");
  if(!(in = popen(ss.str().c_str(), "r"))) {
    cout << "OUCH" << endl;
  }    

  while(fgets(buff, sizeof(buff), in)!=NULL){
    //  cout << buff;
  }
  pclose(in);
  
  cout << argmin << endl;
  if (infile.good()) {
    string sLine;
    getline(infile, sLine);
    vector<string> cmp = ssplit(sLine, ' ');
    int num = atoi(cmp[1].c_str());
    Point2 point(pred[num][0], pred[num][1]);

    if(!(in = popen("rm result", "r"))) {
      cout << "OUCH" << endl;
    }

    return point;
  }

}

int main(int argc, char** argv) {

  
  // Record time
  time_t start_time = clock(); 

  Point2 x_initial(gt[0][0], gt[0][1]);
  SharedDiagonal P_initial = noiseModel::Diagonal::Sigmas(Vector_(2, 
								  0.1, 0.1));
  // Create an ExtendedKalmanFilter object
  ExtendedKalmanFilter<Point2> ekf(x_initial, P_initial);
	
  // R: obs
  SharedDiagonal Q = noiseModel::Diagonal::Sigmas(Vector_(2, .1,
							  .1), true);
  // Q: trans
  SharedDiagonal R = noiseModel::Diagonal::Sigmas(Vector_(2, .099, .099), true);
  
  
  Point2 lastSeen = x_initial;
  Point2 difference(.1, .1);

  int init = -1;
  int file = 1;
  for(int x = 0; x < 2475; x+= 1) {
    Symbol xp('x',  file);	  
    Symbol xpp('x', file + 1);	  

    if(file == 1) {
      Point2 _x_initial(gt[x][0], gt[x][1]);
      SharedDiagonal _P_initial = 
	noiseModel::Diagonal::Sigmas(Vector_(2, .1, .1));
      
      ExtendedKalmanFilter<Point2> _ekf(_x_initial, _P_initial);		
      ekf = _ekf;
    }
            
    BetweenFactor<Point2> factor2(xp, xpp, difference, Q);
    Point2 x_predict = ekf.predict(factor2);
    mat pred, obs;

    Point2 z = executeSearch(x_predict.x() + rand(),  x_predict.y() + rand(), file);    
    PriorFactor<Point2> factor(xp, z, R);
    Point2 x_update = ekf.update(factor);	      
    
    difference = (z - lastSeen) / 2;
    lastSeen = z;

    pred << z.x() << z.y() << endr;    
    obs << gt[x][0] << gt[x][1] << endr;
    
    //    if(file > 100) {
      cout << dist(pred, obs) << endl;
      //}
    file++;
    if(file == reinit_at[init]) {
      file = 1;
      init++;
    }

    file++;
  }

  return 0;
};

