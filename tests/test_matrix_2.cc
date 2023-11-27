#include <iostream>
#include "vector.h"
#include "matrix.h"
#include <utility>  //for std::move
namespace bla = ASC_bla;

void test_mat_mult(){
    size_t m = 13;
    size_t n = 14;
    bla::Matrix<double,bla::ColMajor> x(m, n);
    bla::Matrix<double,bla::ColMajor> y(n, m);
    size_t k = 0;
    for (size_t i = 0; i < m; i++){
        for (size_t j = 0; j < n; j++){
            if (i==j){ x(i, j) = 1;}else{x(i,j)=0;}
            y(j,i) = k;
            k++; 
        }
    }
    std::cout << "x = \n" << x<<std::endl<<"y = \n"<<y<<std::endl;
    std::cout << "x*y = \n" << x*y << std::endl;

}


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
    std::cout << "b = \n" << b << std::endl;

    //test matrix multiplication and addition
    std::cout << "a*b = \n" << a*b << std::endl;
    std::cout << "a+b = \n" << a+b << std::endl;

    //test matrix-vector product
    std::cout << "v = " << v << std::endl;
    std::cout << "a * v = " << a*v << std::endl;

    //test Row,Col methods
    std::cout << "b.Row(1) = " << b.Row(1) << std::endl;
    std::cout << "b.Col(3) = " << b.Col(3) << std::endl;
    std::cout << "b.Cols(1, 4) = \n" << b.Cols(1, 4) << std::endl;
    std::cout << "b.Rows(1, 4) = \n" << b.Rows(1, 4) << std::endl;
    std::cout << "b.Rows(1, 4) = \n" << b.Rows(1, 4) << std::endl;

    //test transpose
    std::cout << "d.Transpose()=\n" << d.Transpose() << std::endl;
    
    //test move construcion and assignment
    bla::Matrix<double,bla::ColMajor> e(std::move(a));
    std::cout << "move-constructed a from e = \n"<< e << std::endl;
    e=std::move(b);
    std::cout <<"we move-assigned b to e, so e = \n" << e << std::endl;

    //test matrix inversion
    bla::Matrix<double, bla::ColMajor> M2(2, 2);
    M2(0, 0) = 0; M2(0, 1) = 1; M2(1, 0) = 2; M2(1, 1) = 1;
    std::cout << "M2 = \n" << M2 << '\n';
    std::cout << "M2^(-1) = \n" << M2.invert() << '\n';
    bla::Matrix<double, bla::ColMajor> M3(3,3);
    M3(0, 0) = 1; M3(0, 1) = 0; M3(0, 2) = 0;
    M3(1, 0) = 0; M3(1, 1) = 0; M3(1, 2) = 2; 
    M3(2, 0) = 0; M3(2, 1) = 1; M3(2, 2) = 0;
    std::cout << "M3 = \n"<< M3 << '\n';
    std::cout << "M3^(-1) = \n" << M3.invert() << '\n';
    test_mat_mult();
}


