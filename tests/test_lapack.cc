#include <iostream>

#include <vector.h>
#include "matrix.h"
#include <lapack_interface.h>


using namespace ASC_bla;
using namespace std;


int main()
{
  Vector<double> x(5);
  Vector<double> y(5);

  for (int i = 0; i < x.Size(); i++)
    {
      x(i) = i;
      y(i) = 2;
    }

  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
  
  AddVectorLapack (2, x, y);  
  cout << "y+2*x = " << y << endl;

  size_t m = 4;
  size_t n = 4; 
  size_t l = 10;
  ASC_bla::Matrix<double,ASC_bla::ColMajor> a(m, n);
  ASC_bla::Matrix<double,ASC_bla::ColMajor> b(n, m);
  ASC_bla::Matrix<double,ASC_bla::ColMajor> c(n, n);

  int k = 0;
  for (size_t i = 0; i < a.Height(); i++){
      for (size_t j = 0; j < a.Width(); j++){
          a(i, j) = 0;
           b(j, i) = k;
           k++;
            
      }
   }

  a(0,0) = 1;
  a(1,2) = 1;
  a(2,1) = 1;
  a(3,3) = 1;
    
  std::cout << "a = \n" << a << std::endl;
  std::cout << "b = \n" << b << std::endl;

  //test matrix multiplication and addition
  MultMatMatLapack(a,b,c);
  std::cout << "a*b = \n" << c << std::endl;
  std::cout << "a+b = \n" << a+b << std::endl;
}

  
