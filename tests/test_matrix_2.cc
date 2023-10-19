#include <iostream>
#include "vector.h"
#include "matrix.h"

namespace bla = ASC_bla;

int main()
{
    size_t m = 4;
    size_t n = 4; 
    size_t l = 10;
    bla::Matrix<double,bla::ColMajor> a(m, n);
    bla::Matrix<double,bla::ColMajor> b(n, m);
    bla::Matrix<double, bla::ColMajor> d(l, m);
    bla::Vector<double> v(n);

    int k = 0;
    for (size_t i = 0; i < a.Height(); i++){
        for (size_t j = 0; j < a.Width(); j++){
            a(i, j) = 0;
            b(j, i) = k;
            if (i == 0) { v(j) = k; }
            k++;
            
        }
    }
    for (size_t i = 0; i < d.Height(); i++){
        for (size_t j = 0; j < d.Width(); j++){
            d(i, j) = i + j;
        }
    }
    a(0,0) = 1;
    a(1,2) = 1;
    a(2,1) = 1;
    a(3,3) = 1;
    std::cout << "a = \n" << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a*b = " << a*b << std::endl;
    std::cout << "a+b = " << a+b << std::endl;

    std::cout << "v = " << v << std::endl;
    std::cout << "a * v = " << a*v << std::endl;
    std::cout << "b.Row(1) = " << b.Row(1) << std::endl;
    std::cout << "b.Col(3) = " << b.Col(3) << std::endl;
    std::cout << "b.Cols(1, 4) = " << b.Cols(1, 4) << std::endl;
    std::cout << "b.Rows(1, 4) = " << b.Rows(1, 4) << std::endl;
    std::cout << "b.Rows(1, 4) = " << b.Rows(1, 4) << std::endl;

    std::cout << "d.Transpose()" << d.Transpose() << std::endl;
    
}