#include <chrono>
#include <iostream>
#include <random>
#include "matrix.h"
#include <cstdlib>
#include "lapack_interface.h"
#include "clapack.h"

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

int main(){
    int n=1000;
    bla::Matrix<double,bla::RowMajor> x1(n,n);
    bla::Matrix<double,bla::RowMajor> x2(n,n);
    bla::Matrix<double,bla::RowMajor> Mx(n,n);

    fill(x1); fill(x2); 
    size_t flops = n*n*n;
    size_t runs = size_t (1e9 / flops) + 1;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < runs; i++){
        MultMatMatLapack(x1,x2,Mx);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double>(end-start).count();

    std::cout<<"Results Lapack:" << std::endl; 
    std::cout << "n = " << n << ", time = " << time << " s, GFlops = " 
        << (flops*runs)/time*1e-9 << std::endl;

    std::cout <<"Results with template expressions: "<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < runs; i++){
        Mx=x1*x2;
    }
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double>(end-start).count();
            
    std::cout << "n = " << n << ", time = " << time << " s, GFlops = " 
        << (flops*runs)/time*1e-9 << std::endl;

       



}