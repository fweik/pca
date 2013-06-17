/*
  Copyright (C) 2013 Florian Weik <florianweik@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/


#include <iostream>
#include <fstream>
#include <vector>

#include <Eigen/Eigenvalues>

using namespace Eigen;

void read_data( int particles_per_sample, char *filename, std::vector<VectorXd> &X) {
  VectorXd x(3*particles_per_sample);
  int i;
  std::ifstream f;

  std::cout << "Reading data from " << filename << std::endl;

  f.open(filename);

  if(!f.good())
    std::cout << "Error opening file" << filename << std::endl;

  while(f.good()) {
    x = VectorXd::Zero(3*particles_per_sample);
    for(i = 0; (i < 3*particles_per_sample) && (f.good()); ++i) {
      f >> x(i);
    }
    /* Use only complete samples. */
    if(i == (3*particles_per_sample)) {
      X.push_back(x);
    }
  }
}

void calc_mean(std::vector<VectorXd> &X, VectorXd &avg) {
  for (std::vector<VectorXd>::iterator it = X.begin() ; it != X.end(); ++it) {
    avg += *it;
  }

  avg /= X.size();
}

void remove_com(std::vector<VectorXd> &X) {
    Vector3d com;
  for (std::vector<VectorXd>::iterator it = X.begin() ; it != X.end(); ++it) {
    com = Vector3d::Zero();

    for( int i = 0; i < (*it).size(); i += 3) {
      com[0] += (*it)[i + 0];
      com[1] += (*it)[i + 1];
      com[2] += (*it)[i + 2];
    }
    for( int i = 0; i < (*it).size(); i += 3) {
      (*it)[i + 0] -= com[0] / (*it).size();
      (*it)[i + 1] -= com[1] / (*it).size();
      (*it)[i + 2] -= com[2] / (*it).size();
    }
  }
}

void calc_cov(std::vector<VectorXd> &X, MatrixXd &covm, VectorXd &avg) {
  VectorXd y(X[0].size());

  if( ( covm.rows() != y.size() ) || ( covm.cols() != y.size())) {
    std::cout << "Error, dimensions do not match." << std::endl;
    return;
  }

  covm = MatrixXd::Zero( y.size(), y.size());

  for (std::vector<VectorXd>::iterator it = X.begin() ; it != X.end(); ++it) {
    y = *it - avg;

    for(int i = 0; i < covm.rows(); ++i)
      for(int j = 0; j < covm.cols(); ++j) {
	covm(i,j) += y[i]*y[j];
      }
  }
  covm /= y.size();
}

int main(int argc, char **argv) {
  std::ofstream evec, eval;

  std::string evec_filename, eval_filename;
  
  switch(argc) {
  case 3:
    evec_filename = "eigenvectors.dat";
    eval_filename = "eigenvalues.dat";
    break;
  case 5:
    evec_filename = argv[3];
    eval_filename = argv[4];
  default:
    std::cout << "usage: " << argv[0] << " <particles_per_sample> <sample_filename> [ <eigenvector_filename> ] [ <eigenvalue_filename> ]" << std::endl;
    return -1;
    break;
  }
 
  evec.open(evec_filename.c_str());

  if(evec.bad())
    std::cout << "Error opening file" << evec_filename << std::endl;

  eval.open(eval_filename.c_str());

  if(eval.bad())
    std::cout << "Error opening file" << eval_filename << std::endl;

  int particles_per_sample = atoi(argv[1]);
  std::vector<VectorXd> samples;
  VectorXd mean(3*particles_per_sample);
  MatrixXd cov(3*particles_per_sample, 3*particles_per_sample);
  read_data( particles_per_sample, argv[2], samples);

  remove_com( samples);
  calc_mean( samples, mean);
  calc_cov( samples, cov, mean);

  SelfAdjointEigenSolver<MatrixXd> es;

  es.compute(cov);

  evec << es.eigenvectors();
  eval << es.eigenvalues();

  return (es.info() == Success);
}
