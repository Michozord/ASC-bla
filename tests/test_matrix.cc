#include <iostream>

#include <matrix.h>

namespace bla = ASC_bla;

int main()
{
    size_t m = 3;
    size_t n = 5; 
    bla::Matrix<double,bla::ColMajor> a(m, n), c(m, n);
    bla::Matrix<double,bla::ColMajor> b(m, n);

    int k = 0;
    for (size_t i = 0; i < a.Rows(); i++){
        for (size_t j = 0; j < a.Cols(); j++){
            a(i, j) = k;
            b(i, j) = -k;
            k++;
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
}