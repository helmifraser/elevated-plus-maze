#include "neural-network.hpp"

int main()
{
  Eigen::MatrixXi a(2,3);
  a << 1, 2, 3, 4, 5, 6;


  std::cout << "--------" << std::endl;
  std::cout << "a: " << std::endl;
  std::cout << a << std::endl;

  std::cout << "--------" << std::endl;

  Eigen::MatrixXi b = a.transpose();
  std::cout << "b: " << std::endl;
  std::cout << b  << std::endl;

  std::cout << "--------" << std::endl;
  std::cout << "a*b: " << std::endl;
  std::cout << a*b << std::endl;

  // Eigen::Vector2d u(-1,1), v(2,0);
  //
  // std::cout << "Here is mat*mat:\n" << mat*mat << std::endl;
  // std::cout << "Here is mat*u:\n" << mat*u << std::endl;
  // std::cout << "Here is u^T*mat:\n" << u.transpose()*mat << std::endl;
  // std::cout << "Here is u^T*v:\n" << u.transpose()*v << std::endl;
  // std::cout << "Here is u*v^T:\n" << u*v.transpose() << std::endl;
  // std::cout << "Let's multiply mat by itself" << std::endl;
  // mat = mat*mat;
  // std::cout << "Now mat is mat:\n" << mat << std::endl;
}
