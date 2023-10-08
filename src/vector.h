#ifndef FILE_VECTOR_H
#define FILE_VECTOR_H

#include <iostream>
#include <cassert>  //not good code, use exception handling (to do)

namespace ASC_bla
{
  //forward declaration of class Matrix
  template <typename T>
  class Matrix;
  
  template <typename T>
  class Vector
  {
    size_t size_;
    T * data_;
    
  public:
    Vector (size_t size) 
      : size_(size), data_(new T[size]) { ; }
    
    Vector (const Vector & v)
      : Vector(v.Size())
    {
      *this = v;
    }

    Vector (Vector && v)
      : size_{0}, data_(nullptr)
    {
      std::swap(size_, v.size_);
      std::swap(data_, v.data_);
    }

    ~Vector () { delete [] data_; }
    
    Vector & operator=(const Vector & v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[i] = v2(i);
      return *this;
    }

    Vector & operator= (Vector && v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[i] = v2(i);
      return *this;
    }
    
    size_t Size() const { return size_; }
    T & operator()(size_t i) { return data_[i]; }
    const T & operator()(size_t i) const { return data_[i]; }

    //cast to Matrix (reason why we need forward declaration of Matrix)
    operator Matrix<T>() const {
        Matrix<T> m(this->Size(), 1);
        for (size_t i = 0; i < this->Size(); ++i) {
            m(i,0) = (*this)(i);
        }
        return m;
    }


  };


  template <typename T>
  Vector<T> operator+ (const Vector<T> & a, const Vector<T> & b)
  {
    Vector<T> sum(a.Size());
    for (size_t i = 0; i < a.Size(); i++)
      sum(i) = a(i)+b(i);
    return sum;
  }
  
  template <typename T>
  std::ostream & operator<< (std::ostream & ost, const Vector<T> & v)
  {
    if (v.Size() > 0)
      ost << v(0);
    for (size_t i = 1; i < v.Size(); i++)
      ost << ", " << v(i);
    return ost;
  }
  


  template <typename T>
  class Matrix
  {
  private:
      size_t m_row;
      size_t m_col;
      T* m_data;
  public:
      //constructor
      Matrix(size_t row, size_t col)
          : m_row(row), m_col(col), m_data(new T[row * col]) {
          ;
      }
      //copy constructor (uses copy assignment)
      Matrix(const Matrix& m)
          : Matrix(m.Row(), m.Col())
      {
          *this = m;
      }

      //copy assignment (uses (.,.) operator)
      //You should decide what to do if dimensions do not match!!!
      Matrix& operator=(const Matrix& rhs)
      {   
          for (size_t i = 0; i < rhs.Row(); i++) {
              for (size_t j = 0; j < rhs.Col(); j++) {
                  (*this)(i, j) = rhs(i, j);
              }
          }
          return *this;
      }

      //destructor
      ~Matrix() { delete[] m_data; }

      //access member functions
      //maybe use verbs instead of nouns for fct (getRows(), not Rows())
      size_t Row() const { return m_row; }
      size_t Col() const { return m_col; }
      //we use return by reference to avoid making copy
      T& operator()(size_t i, size_t j) { return m_data[i * m_col + j]; }
      const T& operator()(size_t i, size_t j) const { return m_data[i * m_col + j]; }

      //cast to Vector<T>
      operator Vector<T>() const { 
          assert((*this).Col() == 1);       //solve with exception handling, not assert!
          Vector<T> v(this->Row());
          for (size_t i = 0; i < this->Row(); ++i) {
              v(i) = (*this)(i, 0);
          }
          return v;
      }
      
      //member fct to print matrix
      void print()
      {
          for (size_t i = 0; i < m_row; i++) {
              for (size_t j = 0; j < m_col; j++) {
                  std::cout << m_data[i * m_col + j] << "   ";
              }
              std::cout << std::endl;
          }
      }

      //transpose matrix
      Matrix<T> t() const {
          Matrix<T> m(this->Col(), this->Row());
          for (size_t i = 0; i < m.Row(); i++) {
              for (size_t j = 0; j < m.Col(); j++) {
                  m(i, j) = (*this)(j, i);
              }
          }
          return m;
      }
  };

  //bad code imo because you have to pay attention as to why you do not get "underflow" error (size_t is unsigned, if I go below 0 I start at very large pos. numb.)
  template <typename T>
  std::ostream& operator<< (std::ostream& ost, const Matrix<T>& m)
  {   
      size_t col = m.Col()-1;
      size_t row = m.Row()-1;
          
      for (size_t i = 0; i < row ; i++) {
          if (col != 0) {
              ost << m(i, 0);
              for (size_t j = 1; j < col; j++) {
                  ost << ", " << m(i, j);
              }
              ost << ", " << m(i, col) << '\n';
          }
          else ost << m(i, 0) << '\n';
      }
      ost << m(row, 0);
      for (size_t j = 1; j <= col; j++) {
          ost << ", " << m(row, j);
      }
      ost << '\n';
          
      return ost;
  }

  //matrix-vector product
  template <typename T>
  Matrix<T> operator* (const Matrix<T>& m, const Vector<T>& v)
  {
      return m * ((Matrix<T>) v); //cast to matrix and use matrix-matrix product


      /*assert(m.Col() == v.Size());
      Vector<T> prod(m.Row());
      T sum{ 0 };
      for (size_t i = 0; i < m.Row(); ++i) {
          for (size_t j = 0; j < m.Col(); ++j) {
              sum += m(i, j) * v(j);
          }
          prod(i) = sum;
          sum = 0;
      }
      return prod;*/
  }

  //matrix-matrix product
  template <typename T>
  Matrix<T> operator* (const Matrix<T>& a, const Matrix<T>& b) {
      assert(a.Col() == b.Row());               //exception handling!
      Matrix<T> c(a.Row(), b.Col());
      for (size_t i = 0; i < a.Row(); ++i) {
          for (size_t j = 0; j < b.Col(); ++j) {
              c(i, j) = 0;
              for (size_t k = 0; k < a.Col(); k++) {
                  c(i, j) += a(i, k) * b(k, j);
              }
          }
      }
      return c;
  }

  

}

#endif
