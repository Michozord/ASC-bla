#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

#include <iostream>
#include <assert.h>
#include "expression.h"
#include "vector.h"
#include <utility>

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
      : MatrixView<T, ORD> (0, 0, 0, nullptr)  //fixed error
    {
      std::swap(height_, m.height_);
      std::swap(width_, m.width_);
      std::swap(dist_, m.dist_);              //added this, otherwise error    
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
    {   assert(width_==m2.width_ && height_==m2.height_);   //solve this later in a better manner!
        for (size_t i = 0; i < m2.Height(); i++){
            for (size_t j = 0; j < m2.Width(); j++){
                (*this)(i, j) = m2(i, j);
            }
        } 
        return *this;
    }


    // move assignment rewritten
    Matrix & operator= (Matrix && m2)
    {
      //check self-assignment
      if(&m2 == this) return *this;     

      //get resources from m2
      std::swap(height_, m2.height_);
      std::swap(width_, m2.width_);
      std::swap(dist_, m2.dist_); 

      //delete data_ and leave m2 in destructable state
      delete[] data_;
      data_=m2.data_;
      m2.data_=nullptr;
      
      return *this;
    }
    
    auto Row (size_t i) const {
        assert(0 <= i && i < Height());
        if constexpr (ORD == ORDERING::RowMajor){
            return VectorView<T>(Width(), data_ + i*Width());
        }else{
            return VectorView<T, size_t>(Width(), Height(), data_ + i); 
        }
        
    }

    auto Col (size_t j) const {
        assert(0 <= j && j < Width());
        if constexpr (ORD == ORDERING::ColMajor){
            return VectorView<T>(Height(), data_ + j*Height());
        }else{
            return VectorView<T>(Height(), Width(), data_ + j);
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

    void swapRows(size_t n, size_t m) {
        for (size_t i = 0; i < width_; ++i) {
            std::swap((*this)(m, i), (*this)(n, i));
        }
    }

    Matrix<T, ORD> invert() 
    {
      assert(height_ == width_);
      size_t n = height_;
      Matrix<T, ORD> I(height_, width_);
      for (size_t i = 0; i < n; i++) {
          for (size_t j = 0; j < n; j++) {   
              I(i,j)=(i==j);
          }
      }
      Matrix<T, ORD> M(*this);
      for (size_t i = 0; i < n; ++i) 
        {
            if (M(i, i) == 0) {
                size_t p = i+1;
                while (M(p, i) == 0) {
                    ++p;
                }
                I.swapRows(i, p);
                M.swapRows(i, p);
            }
            I.Row(i)=(1.0/M(i,i))*I.Row(i);
            M.Row(i)=(1.0/M(i,i))*M.Row(i);
            for (size_t k = 0; k < n; ++k) 
            { 
                if (k != i) {
                  I.Row(k)=(-M(k,i))*I.Row(i)+I.Row(k);
                  M.Row(k)=(-M(k,i))*M.Row(i)+M.Row(k);
                } 
            }
        }
        return I;
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
