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

  //test LU-class
  ASC_bla::Matrix<double,ASC_bla::ColMajor> M(3, 3);
  M(0,0)=1; M(0,1)=2; M(0,2)=4;
  M(1,0)=2; M(1,1)=3; M(1,2)=8;
  M(2,0)=-1; M(2,1)=-3; M(2,2)=-1;
  std::cout <<"Our Matrix M is: \n"<< M << std::endl;

  ASC_bla::Matrix<double,ASC_bla::ColMajor> L = LapackLU(M).LFactor();  
  ASC_bla::Matrix<double,ASC_bla::ColMajor> U = LapackLU(M).UFactor(); 
  ASC_bla::Matrix<double,ASC_bla::ColMajor> P = LapackLU(M).PFactor(); 
  std::cout <<"The L-Faktor is: \n"<< L << std::endl;
  std::cout <<"The U-Faktor is: \n"<< U << std::endl;
  std::cout <<"The P-Faktor is: \n"<< P << std::endl;
  std::cout <<"L*U=\n"<<(L*U)<<std::endl;
  std::cout <<"P*L*U=\n"<<P*(L*U)<<std::endl;
}

  
