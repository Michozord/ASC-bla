.. ASC-bla documentation master file, created by
   sphinx-quickstart on Tue Aug 29 06:39:02 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to ASC-bla's documentation!
===================================

ASC-bla is a C++ library for basic linear algebra operations.
The library provides template classes **Vector** and **Matrix**.

Installation is via git-clone:

..  code-block::
    
    git clone https://github.com/TUWien-ASC/ASC-bla.git


To configure and build some tests do

..  code-block::

    cd ASC-bla
    mkdir build
    cd build
    cmake ..
    make
    

To use ASC-bla in your code, set the compiler include path properly, and include the header files

..  code-block::

    #include <vector.h>
    #include <matrix.h>

All objects are implemented in the namespace ASC_bla. To use them with less typing, you can set

..  code-block::
    
    namespace bla = ASC_bla;

or even

..  code-block::
    
    using namespace ASC_bla;

    

You can create vectors and compute with vectors like:

..  code-block:: cpp
                 
   Vector<double> x(5), y(5), z(5);
   for (int i = 0; i < x.Size(); i++)
      x(i) = i;
   y = 5.0
   z = x+3*y;
   cout << "z = " << z << endl;


For matrices you can choose between row-major (`RowMajor`) or column-major (`ColMajor`) storage,
default is row-major. Supported are following basic operations: scalar-matrix multiplication, 
matrix addition, matrix-vector multiplication and matrix-matrix multiplication.

.. code-block:: cpp
   
   Matrix<double,RowMajor> m1(5,3), m2(5,3), m3(3,3);
   Vector<double> v(5);
   for (int i = 0; i < m1.Height(); i++){
      for (int j = 0; j < m1.Width(); j++){
         m1(i,j) = i+j;
         m2(i,j) = i-j;
      }
   }
   m3 = 3.7;

   for (int i = 0; i < v.Size(); i++)
      v(i) = i;

   Matrix sum = 2 * m1 + m2;
   Vector mat_vec_product = m1 * v;
   Matrix mat_mat_product = m1 * m3;


You can extract a single row or a single column from a matrix:

..  code-block:: cpp

   Vector col1 = mat_mat_product.Col(1);
   Vector row2 = mat_mat_product.Row(2);


or more rows or columns:

.. code-block:: cpp

   Matrix cols = mat_mat_product.Cols(1,3);     // columns 1 and 2, column 3 is excluded
   Matrix rows = mat_mat_product.Rows(0,3);     // rows 0, 1 and 2, row 3 is excluded

You can also get transpose matrix and for square matrices you can compute its inverse: 

.. code-block:: cpp
   
      Matrix transposed = m1.Transpose();
      Matrix inverse = m3.invert();
   
.. toctree::
   :maxdepth: 2
   :caption: Contents:



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
