#include <iostream>
#include<vector.h>
#include <matrix.h>

namespace bla = ASC_bla;

int main()
{
    size_t m = 3;
    size_t n = 5; 
    bla::Matrix<double,bla::ColMajor> a(m, n), c(1, n);
    bla::Matrix<double,bla::ColMajor> b(m, n);
    bla::Matrix<double, bla::ColMajor> d(n, m);
    bla::Vector<double> v(n);

    int k = 0;
    for (size_t i = 0; i < a.Rows(); i++){
        for (size_t j = 0; j < a.Cols(); j++){
            a(i, j) = k;
            b(i, j) = -k;
            if (i == 0) { v(j) = k; }
            k++;
            
        }
    }
    for (size_t i = 0; i < d.Rows(); i++) {
        for (size_t j = 0; j < d.Cols(); j++) {
            d(i, j) = i+j;
        }
    }

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    c = b;
    c(1, 2) = 17;
    std::cout << "c = " << c << std::endl;
    c = a + b;
    std::cout << "a + b = " << c << std::endl;
    c = b.transpose();
    std::cout << "b^T = " << c << std::endl;

    //test matrix-matrix-product
    std::cout << "d = " << d << std::endl;
    std::cout << "a*d =" << '\n' << a * d << '\n';

    //test matrix-vector-product
    std::cout << "v = " << v << '\n' << std::endl;
    std::cout << "a*v = " << '\n' << a * v << std::endl;

    //Invalid matrix operations to test assertion tests:

    //std::cout << "c(1, 10) = " << c(1, 10) << std::endl;
    //std::cout << "b + b^T = " << b + c << std::endl;
}