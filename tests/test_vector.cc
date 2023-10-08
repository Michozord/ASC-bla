#include <iostream>

#include <vector.h>

namespace bla = ASC_bla;


int main()
{
  size_t n = 5;
  bla::Vector<double> x(n), y(n);

  for (size_t i = 0; i < x.Size(); i++)
    {
      x(i) = i;
      y(i) = 10;
    }

  bla::Vector<double> z = x+y;
  
  std::cout << "x+y = " << z << std::endl;

  //test for matrix class

  size_t k, l;
  k = 3;
  l = 3;
  bla::Matrix<double> m(k, l);
  for (int i = 0; i < k; i++) {
      for (int j = 0; j < l; j++) {
          m(i, j) = i + j;
      }
  }
  bla::Vector<double> v1(l);
  for (int i = 0; i < l; i++) {
      v1(i) = i;
  }

  bla::Matrix<double> m1(m);
  m1(0, 1) = 9;
  std::cout << m <<'\n' << m1 << '\n';
  std::cout << v1<<'\n'<<'\n';
  std::cout << m1 * v1 << '\n';

  bla::Matrix<double> mat(v1);
  std::cout << mat << '\n';
  std::cout << m1 * mat << '\n';
  std::cout << m1 * m1 << '\n';
  std::cout << (m1 * m1).t() << '\n';
  std::cout << mat.t() << '\n';


  k = 3;
  bla::Matrix<double> m2(k, 1);
  for (int i = 0; i < k; i++) {
      for (int j = 0; j < 1; j++) {
          m2(i, j) = i + j;
      }
  }
  bla::Vector<double> v2(m2);
  bla::Matrix<double> m3(v2);
  //std::cout << m2 << '\n';
  //std::cout << v2 << '\n';
  //std::cout << m3 << '\n';
  
}




