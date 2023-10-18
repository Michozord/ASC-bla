#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

#include <iostream>
#include <assert.h>
#include "expression.h"
#include "vector.h"

namespace ASC_bla
{   //forward declaration of Vector class
    template <typename T>
    class Vector;

  enum ORDERING { ColMajor, RowMajor };

  template <typename T, ORDERING ORD> 
  class MatrixView : public MatExpr<MatrixView<T, ORD>>
  {
  protected:
    size_t height_, width_, dist_;
    T * data_;
  public:
    MatrixView (size_t height, size_t width, size_t dist, T * data)
      : data_(data), height_(height), width_(width), dist_(dist) { }
    
    MatrixView (size_t height, size_t width, T * data)
      : data_(data), height_(height), width_(width) { 
            if (ORD == ORDERING::ColMajor){
                dist_ = height;
            }else{
                dist_ = width;
            }
      }
    
    template <typename TB>
    MatrixView & operator= (const MatExpr<TB> & m2)
    {
      for (size_t i = 0; i < Height(); i++){
        for (size_t j = 0; j < Width(); j++){
            (*this)(i, j) = m2(i, j);
        }
      } 
      return *this;
    }

    MatrixView & operator= (T scal)
    {
      for (size_t i = 0; i < Height(); i++){
        for (size_t j = 0; j < Width(); j++){
            (*this)(i, j) = scal;
        }
      } 
      return *this;
    }
    
    auto View() const { return MatrixView(height_, width_, dist_, data_); }
    size_t Height() const { return height_; }
    size_t Width() const { return width_; }
    T & operator()(size_t i, size_t j) { 
        assert(0 <= i && i < Height() && 0 <= j && j < Width());
        if (ORD == ORDERING::ColMajor){
            return data_[j * dist_ + i];
        } else {
            return data_[i * dist_ + j];
        } 
     }
    const T & operator()(size_t i, size_t j) const { 
        assert(0 <= i && i < Height() && 0 <= j && j < Width());
        if (ORD == ORDERING::ColMajor){
            return data_[j * dist_ + i];
        } else {
            return data_[i * dist_ + j];
        } 
     }
      
  };

  
  template <typename T, ORDERING ORD> 
  class Matrix : public MatrixView<T, ORD>
  {
    typedef MatrixView<T, ORD> BASE;
    using BASE::height_;
    using BASE::width_;
    using BASE::data_;
  public:
    Matrix (size_t height, size_t width) 
      : MatrixView<T, ORD> (height, width, new T[height * width]) { ; }
    
    Matrix (const Matrix & m)
      : Matrix(m.Height(), m.Width())
    {
      *this = m;
    }

    Matrix (Matrix && m)
      : MatrixView<T, ORD> (0, nullptr)
    {
      std::swap(height_, m.height_);
      std::swap(width_, m.width_);
      std::swap(data_, m.data_);
    }

    template <typename TB>
    Matrix (const MatExpr<TB> & m)
      : Matrix(m.Height(), m.Width())
    {
      *this = m;
    }
    
    
    ~Matrix () { delete [] data_; }

    size_t Height() const { return height_; }
    size_t Width() const { return width_; }

    using BASE::operator=;
    Matrix & operator=(const Matrix & m2)
    {
        for (size_t i = 0; i < m2.Height(); i++){
            for (size_t j = 0; j < m2.Width(); j++){
                (*this)(i, j) = m2(i, j);
            }
        } 
        return *this;
    }

    Matrix & operator= (Matrix && m2)
    {
        for (size_t i = 0; i < m2.Height(); i++){
            for (size_t j = 0; j < m2.Width(); j++){
                (*this)(i, j) = m2(i, j);
            }
        } 
        return *this;
    }
    
    VectorView<T> Row (size_t i) const {
        assert(0 <= i && i < Height());
        T * data = new T[Width()];
        for(size_t j = 0; j < Width(); j++){
            data[j] = (*this)(i, j);
        }
        return VectorView<T>(Width(), data);
    }

    VectorView<T> Col (size_t j) const {
        assert(0 <= j && j < Width());
        T * data = new T[Height()];
        for(size_t i = 0; i < Height(); i++){
            data[i] = (*this)(i, j);
        }
        return VectorView<T>(Height(), data);
    }

  };


  template <typename ...Args>
  std::ostream & operator<< (std::ostream & ost, const MatrixView<Args...> & m)
  {
    for (size_t i = 0; i < m.Rows(); i++){
            for(size_t j = 0; j < m.Cols() - 1; j++){
                ost << m(i, j) << ", ";
            }
            ost << m(i, m.Cols() - 1) << "\n";
        }
        return ost;
  }

}

#endif
