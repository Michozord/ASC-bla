//DISCLAIMER: You have to run this in release mode, otherwise your measurements will be absolute garbage (much too slow!)

#include <chrono>
#include <iostream>
#include <random>
#include <matrix.h>
#include <cstdlib>

namespace bla = ASC_bla;

void fill(bla::Matrix<double,bla::RowMajor>& m){
    std::uniform_real_distribution<double> unif(0,10);
    std::default_random_engine re;
    for (size_t i = 0; i < m.Height(); i++)
    {
        for (size_t j = 0; j < m.Width(); j++)
        {
            m(i,j)= unif(re);
        }
        
    }
    
}

int main() {

    bla::Matrix<double,bla::RowMajor> x1(10,10);
    bla::Matrix<double,bla::RowMajor> x2(10,10);
    bla::Matrix<double,bla::RowMajor> y1(100,100);
    bla::Matrix<double,bla::RowMajor> y2(100,100);
    bla::Matrix<double,bla::RowMajor> z1(1000,1000);
    bla::Matrix<double,bla::RowMajor> z2(1000,1000);
    bla::Matrix<double,bla::RowMajor> Mz(1000,1000);
    bla::Matrix<double,bla::RowMajor> My(100,100);
    bla::Matrix<double,bla::RowMajor> Mx(10,10);

    fill(x1); fill(x2); fill(y1); fill(y2); fill(z1); fill(z2);

       
  
    auto start = std::chrono::steady_clock::now(); 
    for (size_t i = 0; i < 10; i++)
    {
        Mz=z1*z2;
    }
    auto end = std::chrono::steady_clock::now(); 
    
    std::cout << "time for n=1000: "<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()/10.0<< std::endl;

    start = std::chrono::high_resolution_clock::now(); 
    for (size_t i = 0; i < 10; i++)
    {
        My=y1*y2;
    }
    end = std::chrono::high_resolution_clock::now();    

    std::cout << "time for n=100: "<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()/10.0<< std::endl;

    start = std::chrono::high_resolution_clock::now(); 
    for (size_t i = 0; i < 10; i++)
    {
        Mx=x1*x2;
    }
    end = std::chrono::high_resolution_clock::now();    

    std::cout << "time for n=10: "<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()/10.0<< std::endl;
    

    return 0;
}
