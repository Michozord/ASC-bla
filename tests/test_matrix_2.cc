#include <iostream>
#include "vector.h"
#include "matrix.h"

namespace bla = ASC_bla;

int main()
{
    size_t m = 4;
    size_t n = 4; 
    bla::Matrix<double,bla::ColMajor> a(m, n);
    bla::Matrix<double,bla::ColMajor> b(n, m);
    bla::Matrix<double, bla::ColMajor> d(n, m);
    bla::Vector<double> v(n);

    int k = 0;
    for (size_t i = 0; i < a.Height(); i++){
        for (size_t j = 0; j < a.Width(); j++){
            a(i, j) = 0;
            b(i, j) = k;
            if (i == 0) { v(j) = k; }
            k++;
            
        }
    }
    a(0,0) = 1;
    a(1,2) = 1;
    a(2,1) = 1;
    a(3,3) = 1;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a*b = " << a*b << std::endl;
    std::cout << "a+b = " << a+b << std::endl;
    d = a;
    d = 0.5 * d;
    std::cout << "d = " << d << std::endl;
    std::cout << "v = " << v << std::endl;
    std::cout << "a * v = " << a*v << std::endl;
    std::cout << "a.Row(1) = " << a.Row(1) << std::endl;
    std::cout << "a.Col(3) = " << a.Col(3) << std::endl;

    
}