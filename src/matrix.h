#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

#include <iostream>
#include <assert.h>
#include "expression.h"
#include "vector.h"
#include <utility>
#include "../ASC-HCP/src/simd.h"
#include "../ASC-HCP/src/simd_avx.h"
#include "../ASC-HCP/src/taskmanager.cc"
#include "../ASC-HCP/src/timer.cc"
#include<functional>



namespace ASC_bla
{   //forward declaration of Vector class
    template <typename T>
    class Vector;
    template <typename T, typename TDIST>
    class VectorView;

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

    MatrixView& operator= (const MatrixView & m2) {
    for (size_t i = 0; i < Height(); i++){
        for (size_t j = 0; j < Width(); j++){
            (*this)(i, j) = m2(i, j);
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

    auto Col (size_t j) const {
        assert(0 <= j && j < Width());
        if constexpr (ORD == ORDERING::ColMajor){
            return VectorView<T, size_t>(Height(), data_ + j*Height());
        }else{
            return VectorView<T, size_t>(Height(), Width(), data_ + j);
        }
    }

    auto Row (size_t i) const {
        assert(0 <= i && i < Height());
        if constexpr (ORD == ORDERING::RowMajor){
            return VectorView<T, size_t>(Width(), data_ + i*Width());
        }else{
            return VectorView<T, size_t>(Width(), Height(), data_ + i); 
        }
        
    }

    auto Diag () const {
        size_t l = (Height() > Width()) ? Width() : Height();
        return VectorView<T, size_t>(l, dist_+1,data_);   
    }

     T* Data() {
      return data_;
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
            return VectorView<T, size_t>(Width(), data_ + i*Width());
        }else{
            return VectorView<T, size_t>(Width(), Height(), data_ + i); 
        }
        
    }

    auto Col (size_t j) const {
        assert(0 <= j && j < Width());
        if constexpr (ORD == ORDERING::ColMajor){
            return VectorView<T, size_t>(Height(), data_ + j*Height());
        }else{
            return VectorView<T, size_t>(Height(), Width(), data_ + j);
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
                //size_t p = i+1;
                size_t p=0;
                while (M(p,i)==0) {++p;}
                I.Row(i)=I.Row(i)+I.Row(p);
                M.Row(i)=M.Row(i)+M.Row(p);
                //while (M(p, i) == 0) {
                //    ++p;
                //}
                //I.swapRows(i, p);
                //M.swapRows(i, p);
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


  template <typename TA, typename TB, ORDERING ORD>
  auto operator* (const Matrix<TA, ORD> & a, const Matrix<TB, ORD> & b)
  {
    assert(a.Width() == b.Height());
    ASC_HPC::StartWorkers(3);
    constexpr size_t SW = 4;
    Matrix<TA, ORD> c(a.Height(), b.Width());
    ASC_HPC::RunParallel(a.Height()/SW, [&a, &b, &c, SW] (int i_step, int size)
    { 
      size_t i = i_step * SW;
      ASC_HPC::RunParallel(b.Width()/SW, [&a, &b, &c, i, SW] (int j_step, int size){
        size_t j = j_step * SW;
        ASC_HPC::SIMD<TA, 4> simd_sum0(0.);
        ASC_HPC::SIMD<TA, 4> simd_sum1(0.);
        ASC_HPC::SIMD<TA, 4> simd_sum2(0.);
        ASC_HPC::SIMD<TA, 4> simd_sum3(0.);
        for(size_t k=0; k<a.Width(); k++){
          ASC_HPC::SIMD<TA, 4> simd_temp0(a(i,k), a(i+1,k), a(i+2,k), a(i+3,k));
          ASC_HPC::SIMD<TA, 4> simd_temp1(a(i,k), a(i+1,k), a(i+2,k), a(i+3,k));
          ASC_HPC::SIMD<TA, 4> simd_temp2(a(i,k), a(i+1,k), a(i+2,k), a(i+3,k));
          ASC_HPC::SIMD<TA, 4> simd_temp3(a(i,k), a(i+1,k), a(i+2,k), a(i+3,k));
          simd_sum0 += b(k, j) * simd_temp0;
          simd_sum1 += b(k, j+1) * simd_temp1;
          simd_sum2 += b(k, j+2) * simd_temp2;
          simd_sum3 += b(k, j+3) * simd_temp3;
        }
        c(i,j) = simd_sum0[0];
        c(i+1,j) = simd_sum0[1];
        c(i+2,j) = simd_sum0[2];
        c(i+3,j) = simd_sum0[3];
        c(i,j+1) = simd_sum1[0];
        c(i+1,j+1) = simd_sum1[1];
        c(i+2,j+1) = simd_sum1[2];
        c(i+3,j+1) = simd_sum1[3];
        c(i,j+2) = simd_sum2[0];
        c(i+1,j+2) = simd_sum2[1];
        c(i+2,j+2) = simd_sum2[2];
        c(i+3,j+2) = simd_sum2[3];
        c(i,j+3) = simd_sum3[0];
        c(i+1,j+3) = simd_sum3[1];
        c(i+2,j+3) = simd_sum3[2];
        c(i+3,j+3) = simd_sum3[3];
      });
      ASC_HPC::RunParallel(b.Width()%SW, [&a, &b, &c, i, SW] (int j_step, int size){
        size_t j = b.Width() - (b.Width()%SW) + j_step;
        ASC_HPC::SIMD<TA, 4> simd_sum0(0.);
        for (size_t k=0; k<a.Width(); k++){
          ASC_HPC::SIMD<TA, 4> simd_temp0(a(i,k), a(i+1,k), a(i+2,k), a(i+3,k));
          simd_sum0 += b(k, j) * simd_temp0;
        }
        c(i,j) = simd_sum0[0];
        c(i+1,j) = simd_sum0[1];
        c(i+2,j) = simd_sum0[2];
        c(i+3,j) = simd_sum0[3];
      });
    });
    ASC_HPC::RunParallel(a.Height()%SW, [&a, &b, &c, SW] (int i_step, int size){
      size_t i = a.Height() - (a.Height()%SW) + i_step;
      size_t j = 0;
      for (j; j+3<b.Width(); j+=SW){
        ASC_HPC::SIMD<TA, 4> simd_sum0(0.);
        for(size_t k=0; k<a.Width(); k++){
          ASC_HPC::SIMD<TA, 4> simd_temp0(b(k, j), b(k, j+1), b(k, j+2), b(k, j+3));
          simd_sum0 += a(i, k) * simd_temp0;
        }
        c(i,j) = simd_sum0[0];
        c(i,j+1) = simd_sum0[1];
        c(i,j+2) = simd_sum0[2];
        c(i,j+3) = simd_sum0[3];
      }
      for (j; j<b.Width(); j++){
        TA sum = 0;
        for (size_t k=0; k<a.Width(); k++){
          sum += a(i,k)*b(k,j);
        }
        c(i,j)=sum;
      }
    });
    ASC_HPC::StopWorkers();
    return c;
  }


  template <typename ...Args>
  std::ostream & operator<< (std::ostream & ost, const MatrixView<Args...> & m)
  {
    for (size_t i = 0; i < m.Height(); i++){
            for(size_t j = 0; j < m.Cols() - 1; j++){
                ost << m(i, j) << ", ";
            }
            ost << m(i, m.Width() - 1) << "\n";
        }
        return ost;
  }

}

#endif
