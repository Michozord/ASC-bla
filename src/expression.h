#ifndef FILE_EXPRESSION_H
#define FILE_EXPRESSION_H

#include <assert.h>

namespace ASC_bla
{

  template <typename T>
  class VecExpr
  {
  public:
    auto Upcast() const { return static_cast<const T&> (*this); }
    size_t Size() const { return Upcast().Size(); }
    auto operator() (size_t i) const { return Upcast()(i); }
  };
  
 
  template <typename TA, typename TB>
  class SumVecExpr : public VecExpr<SumVecExpr<TA,TB>>
  {
    TA a_;
    TB b_;
  public:
    SumVecExpr (TA a, TB b) : a_(a), b_(b) { }

    auto operator() (size_t i) const { return a_(i)+b_(i); }
    size_t Size() const { return a_.Size(); }      
  };
  
  template <typename TA, typename TB>
  auto operator+ (const VecExpr<TA> & a, const VecExpr<TB> & b)
  {
    return SumVecExpr(a.Upcast(), b.Upcast());
  }



  
  template <typename TSCAL, typename TV>
  class ScaleVecExpr : public VecExpr<ScaleVecExpr<TSCAL,TV>>
  {
    TSCAL scal_;
    TV vec_;
  public:
    ScaleVecExpr (TSCAL scal, TV vec) : scal_(scal), vec_(vec) { }

    auto operator() (size_t i) const { return scal_*vec_(i); }
    size_t Size() const { return vec_.Size(); }      
  };
  
  template <typename T>
  auto operator* (double scal, const VecExpr<T> & v)
  {
    return ScaleVecExpr(scal, v.Upcast());
  }



  template <typename T>
  std::ostream & operator<< (std::ostream & ost, const VecExpr<T> & v)
  {
    if (v.Size() > 0)
      ost << v(0);
    for (size_t i = 1; i < v.Size(); i++)
      ost << ", " << v(i);
    return ost;
  }


  template <typename T>
  class MatExpr
  {
  public:
    auto Upcast() const { return static_cast<const T&> (*this); }
    size_t Height() const { return Upcast().Height(); }
    size_t Width() const { return Upcast().Width(); }
    auto operator() (size_t i, size_t j) const { return Upcast()(i, j); }
  };
  
 
  template <typename TA, typename TB>
  class SumMatExpr : public MatExpr<SumMatExpr<TA,TB>>
  {
    TA a_;
    TB b_;
  public:
    SumMatExpr (TA a, TB b) : a_(a), b_(b) { }

    auto operator() (size_t i, size_t j) const { return a_(i, j)+b_(i, j); }
    size_t Height() const { return a_.Height(); }      
    size_t Width() const { return a_.Width(); }      

  };
  
  template <typename TA, typename TB>
  auto operator+ (const MatExpr<TA> & a, const MatExpr<TB> & b)
  {
    return SumMatExpr(a.Upcast(), b.Upcast());
  }

  template <typename TA, typename TB>
  class MatMatExpr : public MatExpr<MatMatExpr<TA,TB>>
  {
    TA a_;
    TB b_;
  public:
    MatMatExpr (TA a, TB b) : a_(a), b_(b) { }

    auto operator() (size_t i, size_t j) const {
      assert(a_.Width() == b_.Height());
      auto res = a_(0, 0);
      /*maybe we should rewrite this as follows:
      decltype (a_(0,0)*b_(0,0)) res = 0;
      this way we can also multiply e.g. a complex with a double matrix*/
      res = 0;
      for (size_t k = 0; k < a_.Width(); k++) {
          res += a_(i, k) * b_(k, j);
      }
      return res;
     }
    size_t Height() const { return a_.Height(); }      
    size_t Width() const { return b_.Width(); }      

  };
  
  template <typename TA, typename TB>
  auto operator* (const MatExpr<TA> & a, const MatExpr<TB> & b)
  {
    return MatMatExpr(a.Upcast(), b.Upcast());
  }

  template <typename TMAT, typename TVEC>
  class MatVecExpr : public VecExpr<MatVecExpr<TMAT,TVEC>>
  {
    TVEC vec_;
    TMAT mat_;
  public:
    MatVecExpr (TMAT mat, TVEC vec) : vec_(vec), mat_(mat) { }

    auto operator() (size_t i) const { 
      assert(vec_.Size() == mat_.Width());
      auto res = vec_(0);
      res = 0;
      for(size_t k = 0; k<vec_.Size(); k++){
        res += mat_(i, k) * vec_(k);
      }
      return res;
    }
    size_t Size() const { return mat_.Height(); } 
  };
  
  template <typename TA, typename TB> 
  auto operator* (const MatExpr<TA> & m, const VecExpr<TB> & v)
  {
    return MatVecExpr(m.Upcast(), v.Upcast());
  }

  
  template <typename TSCAL, typename TM>
  class ScaleMatExpr : public MatExpr<ScaleMatExpr<TSCAL,TM>>
  {
    TSCAL scal_;
    TM mat_;
  public:
    ScaleMatExpr (TSCAL scal, TM mat) : scal_(scal), mat_(mat) { }

    auto operator() (size_t i, size_t j) const { return scal_*mat_(i, j); }
    size_t Width() const { return mat_.Width(); }
    size_t Height() const { return mat_.Height(); }     
  };
  
  template <typename T> 
  auto operator* (double scal, const MatExpr<T> & m)
  {
    return ScaleMatExpr(scal, m.Upcast());
  }

  



  template <typename T>
  std::ostream & operator<< (std::ostream & ost, const MatExpr<T> & m)
  {
    for (size_t i = 0; i < m.Height(); i++){
        for(size_t j = 0; j < m.Width() - 1; j++){
            ost << m(i, j) << ", ";
        }
        ost << m(i, m.Width() - 1) << "\n";
    }
    return ost;
  }
  
}
 
#endif
