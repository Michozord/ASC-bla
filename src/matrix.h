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
    using BASE::dist_;
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
        if(ORD == ORDERING::RowMajor){
            return VectorView<T>(Width(), data_ + i*Width());
        }else{
            //return VectorView<T>(Width(), Height(), data_ + i);   // this is correct command, but raises error 
            return VectorView<T>(Width(), data_ + i*Width());       // this is only placeholder to return something!
        }
        
    }

    VectorView<T> Col (size_t j) const {
        assert(0 <= j && j < Width());
        if(ORD == ORDERING::ColMajor){
            return VectorView<T>(Height(), data_ + j*Height());
        }else{
            //return VectorView<T>(Height(), Width(), data_ + j);   // this is correct command, but raises error 
            return VectorView<T>(Height(), data_ + j*Height());     // this is only placeholder to return something!
        }
    }

    MatrixView<T, ORD> Rows (size_t first, size_t next) const {
        assert(0 <= first && first < Height());
        assert(0 < next && next <= Height());
        assert(first < next);
        if(ORD == ORDERING::RowMajor){
            return MatrixView<T, ORD>(next-first, Width(), dist_, data_+first*Width());
        } else {
            return MatrixView<T, ORD>(next-first, Width(), Height(), data_+first);
        }
    }

    MatrixView<T, ORD> Cols (size_t first, size_t next) const {
        assert(0 <= first && first < Width());
        assert(0 < next && next <= Width());
        assert(first < next);
        if(ORD == ORDERING::ColMajor){
          return MatrixView<T, ORD>(Height(), next-first, dist_, data_+first*Height());
        } else {
          return MatrixView<T, ORD>(Height(), next-first, Width(), data_+first);
        }
    }

    auto Transpose () const{
      if constexpr(ORD==ORDERING::ColMajor){
        return MatrixView<T, ASC_bla::RowMajor>(Width(), Height(), data_);
      }else{
        return MatrixView<T, ASC_bla::ColMajor>(Width(), Height(), data_);
      }
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
