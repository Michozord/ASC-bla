#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

#include <iostream>

namespace ASC_bla
{
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
        if (ORD == ORDERING::ColMajor){
            return data_[j * Rows() + i];
        } else {
            return data_[i * Cols() + j];
        }
    }
    const T & operator()(size_t i, size_t j) const {
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
    Matrix<T, ORD1> sum(a.Rows(), a.Cols());
    for (size_t i = 0; i < a.Rows(); i++){
        for(size_t j = 0; j < a.Cols() - 1; j++){
            sum(i, j) = a(i, j) + b(i, j);
        }
    }
    return sum;
  }

}

#endif
