#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

#include <iostream>
#include <assert.h>

namespace ASC_bla
{   //forward declaration of Vector class
    template <typename T>
    class Vector;

  enum ORDERING { ColMajor, RowMajor };
  template <typename T, ORDERING ORD>
  class Matrix
  {

    size_t rows_;
    size_t cols_;
    T * data_;
    
  public:
    Matrix (size_t rows, size_t cols) 
      : rows_(rows), cols_(cols), data_(new T[rows_ * cols_]) { ; }
    
    Matrix (const Matrix & m)
      : Matrix(m.Rows(), m.Cols())
    {
      *this = m;
    }

    Matrix (Matrix && m)
      : rows_{0}, cols_{0}, data_(nullptr)
    {
      std::swap(rows_, m.rows_);
      std::swap(cols_, m.cols_);
      std::swap(data_, m.data_);
    }

    ~Matrix () { delete [] data_; }
    
    Matrix & operator=(const Matrix & m2){
        rows_ = m2.Rows();
        cols_ = m2.Cols();
        delete data_;
        data_= new T[rows_ * cols_ ];
        if (ORD == ORDERING::ColMajor){
            for(size_t i = 0; i < m2.Rows(); i++){
                for(size_t j = 0; j< m2.Cols(); j++){
                    data_[j * m2.Rows() + i] = m2(i, j);
                }
            }
        }else{
            for(size_t i = 0; i < m2.Rows(); i++){
                for(size_t j = 0; j< m2.Cols(); j++){
                    data_[i * m2.Cols() + j] = m2(i, j);
                }
            }
        }
        return *this;
    }
    //why can we not just switch like in move constructor?
    Matrix & operator= (Matrix && m2){
        rows_ = m2.Rows();
        cols_ = m2.Cols();
        delete data_;
        data_= new T[rows_ * cols_ ];
        if (ORD == ORDERING::ColMajor){
            for(size_t i = 0; i < m2.Rows(); i++){
                for(size_t j = 0; j< m2.Cols(); j++){
                    data_[j * m2.Rows() + i] = m2(i, j);
                }
            }
        }else{
            for(size_t i = 0; i < m2.Rows(); i++){
                for(size_t j = 0; j< m2.Cols(); j++){
                    data_[i * m2.Cols() + j] = m2(i, j);
                }
            }
        }
        return *this;
    }
    
    size_t Rows() const { return rows_; }
    size_t Cols() const { return cols_; }

    T & operator()(size_t i, size_t j) { 
        assert(0 <= i && i < Rows() && 0 <= j && j < Cols());
        if (ORD == ORDERING::ColMajor){
            return data_[j * Rows() + i];
        } else {
            return data_[i * Cols() + j];
        }
    }
    const T & operator()(size_t i, size_t j) const {
        assert(0 <= i && i < Rows() && 0 <= j && j < Cols());
        if (ORD == ORDERING::ColMajor){
            return data_[j * Rows() + i];
        } else { 
            return data_[i * Cols() + j];
        } 
    }

    Matrix<T, ORD> transpose(){
        Matrix<T, ORD> res(Cols(), Rows());
        for (size_t i = 0; i < Rows(); i++){
            for(size_t j = 0; j < Cols(); j++){
                res(j, i) = (*this)(i, j);
            }
        }
        return res;
    }
  };


  template <typename T, ORDERING ORD>
  std::ostream & operator<< (std::ostream & ost, const Matrix<T, ORD> & m)
  {
    for (size_t i = 0; i < m.Rows(); i++){
        for(size_t j = 0; j < m.Cols() - 1; j++){
            ost << m(i, j) << ", ";
        }
        ost << m(i, m.Cols() - 1) << "\n";
    }
    return ost;
  }

  template <typename T, ORDERING ORD1, ORDERING ORD2>
  Matrix<T, ORD1> operator+ (const Matrix<T, ORD1> & a, const Matrix<T, ORD2> & b)
  {
    assert(a.Cols() == b.Cols() && a.Rows() == b.Rows());
    Matrix<T, ORD1> sum(a.Rows(), a.Cols());
    for (size_t i = 0; i < a.Rows(); i++){
        for(size_t j = 0; j < a.Cols(); j++){   //fixed error
            sum(i, j) = a(i, j) + b(i, j);
        }
    }
    return sum;
  }


  //Added Matrix-Matrix Product and Matrix-Vector Product
  template <typename T, ORDERING ORD1, ORDERING ORD2>
  Matrix<T, ORD1> operator* (const Matrix<T, ORD1>& a, const Matrix<T, ORD2>& b)
  {
      assert(a.Cols() == b.Rows());
      Matrix<T, ORD1> c(a.Rows(), b.Cols());
      for (size_t i = 0; i < a.Rows(); ++i) {
          for (size_t j = 0; j < b.Cols(); ++j) {
              c(i, j) = 0;
              for (size_t k = 0; k < a.Cols(); k++) {
                  c(i, j) += a(i, k) * b(k, j);
              }
          }
      }
      return c;
  }

  //decided to return matrix to avoid loss of "ordering" information/better printing
  template <typename T, ORDERING ORD1>
  Matrix<T, ORD1> operator* (const Matrix<T, ORD1>& a, const Vector<T>& b)
  {
      assert(a.Cols() == b.Size());
      Matrix<T, ORD1> c(a.Rows(), 1);
      for (size_t i = 0; i < a.Rows(); ++i) {
          c(i, 0) = 0;
          for (size_t k = 0; k < a.Cols(); k++) {
                  c(i, 0) += a(i, k) * b(k);
          }
      }
      return c;
  }


}

#endif
