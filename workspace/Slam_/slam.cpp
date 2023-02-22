#include "slam.h"
#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;
// using namespace std::vector;
void slam_test() {
  std::vector<int> v;
  v.emplace_back(2);
  for (auto& iter : v) {
    cout << iter << endl;
  }

  Matrix3d matrix_33 = Eigen::Matrix3d::Zero();
  matrix_33 = Matrix3d::Random();
  cout << "matrix_33: \n" << matrix_33 << endl;
}
void showss(std::vector<int>& b) {
  // for (int i = 0; i < 10; i++) {
  //   cout << b[i];
  //   /* code */
  // }
  for (auto iter : b) {
    cout << iter;
  }
}

slam_::slam_(/* args */) {}

slam_::~slam_() {}

int slam_::x;
int slam_::y;
