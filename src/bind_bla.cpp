#include <sstream>
#include <pybind11/pybind11.h>

#include "vector.h"
#include "matrix.h"

using namespace ASC_bla;
namespace py = pybind11;




PYBIND11_MODULE(bla, m) {
    m.doc() = "Basic linear algebra module"; // optional module docstring
    
     py::class_<Vector<double>> (m, "Vector", py::buffer_protocol())
      .def(py::init<size_t>(),
           py::arg("size"), "create vector of given size")

      .def_buffer([](Vector<double> &m) -> py::buffer_info {
        return py::buffer_info(
            m.Data(),                               /* Pointer to buffer */
            sizeof(double),                          /* Size of one scalar */
            py::format_descriptor<double>::format(), /* Python struct-style format descriptor */
            1,                                      /* Number of dimensions */
            { m.Size() },                 /* Buffer dimensions */
            { sizeof(double) }            /* Strides (in bytes) for each index */ 
        );
      })
      .def("__len__", &Vector<double>::Size,
           "return size of vector")
      
      .def("__setitem__", [](Vector<double> & self, int i, double v) {
        if (i < 0) i += self.Size();
        if (i < 0 || i >= self.Size()) throw py::index_error("vector index out of range");
        self(i) = v;
      })
      .def("__getitem__", [](Vector<double> & self, int i) { return self(i); })
      
      .def("__setitem__", [](Vector<double> & self, py::slice inds, double val)
      {
        size_t start, stop, step, n;
        if (!inds.compute(self.Size(), &start, &stop, &step, &n))
          throw py::error_already_set();
        self.Range(start, stop).Slice(0,step) = val;
      })
      
      .def("__add__", [](Vector<double> & self, Vector<double> & other)
      { return Vector<double> (self+other); })

      .def("__rmul__", [](Vector<double> & self, double scal)
      { return Vector<double> (scal*self); })
      
      .def("__str__", [](const Vector<double> & self)
      {
        std::stringstream str;
        str << self;
        return str.str();
      })

     .def(py::pickle(
        [](Vector<double> & self) { // __getstate__
            /* return a tuple that fully encodes the state of the object */
          return py::make_tuple(self.Size(),
                                py::bytes((char*)(void*)&self(0), self.Size()*sizeof(double)));
        },
        [](py::tuple t) { // __setstate__
          if (t.size() != 2)
            throw std::runtime_error("should be a 2-tuple!");

          Vector<double> v(t[0].cast<size_t>());
          py::bytes mem = t[1].cast<py::bytes>();
          std::memcpy(&v(0), PYBIND11_BYTES_AS_STRING(mem.ptr()), v.Size()*sizeof(double));
          return v;
        }))
    ;

    //Matrix binding
    py::class_<Matrix<double,ASC_bla::RowMajor>> (m, "Matrix", py::buffer_protocol())
      .def(py::init<size_t,size_t>(),
           py::arg("height"), py::arg("width"))
      
      .def_buffer([](Matrix<double,ASC_bla::RowMajor> &m) -> py::buffer_info {
        return py::buffer_info(
            m.Data(),                               /* Pointer to buffer */
            sizeof(double),                          /* Size of one scalar */
            py::format_descriptor<double>::format(), /* Python struct-style format descriptor */
            2,                                      /* Number of dimensions */
            { m.Height(), m.Width() },                 /* Buffer dimensions */
            { sizeof(double) * m.Width(),             /* Strides (in bytes) for each index */
              sizeof(double) }
        );
      })
      
      //basic properties
      .def_property_readonly("shape",[](const Matrix<double, RowMajor>& self) {
           return std::tuple(self.Height(), self.Width());
      })
      .def_property_readonly("height",[](const Matrix<double, RowMajor>& self) {
           return self.Height();
      })
      .def_property_readonly("width",[](const Matrix<double, RowMajor>& self) {
           return self.Width();
      })

      //setters and getters
      .def("__setitem__", [](Matrix<double,RowMajor> & self, std::tuple<int, int> ind , double v) {
        if (std::get<0>(ind) < 0 || std::get<1>(ind) < 0) throw py::index_error("matrix index out of range");
        self(std::get<0>(ind),std::get<1>(ind)) = v;
      })
      .def("__getitem__", [](Matrix<double,RowMajor> & self, std::tuple<int, int> ind) { return self(std::get<0>(ind),std::get<1>(ind)); })
      
      //printing
      .def("__str__", [](const Matrix<double,RowMajor> & self)
      {
        std::stringstream str; 
        str << self;
        return str.str();
      })

      //basic operators
      .def("__add__", [](Matrix<double, RowMajor> & self, Matrix<double, RowMajor> & other)
      { 
        if (self.Height()!=other.Height() || self.Width()!=other.Width()) throw py::index_error("Matrix dimension not the same, addition impossible");
        return Matrix<double,RowMajor> (self+other); 
      })

      .def("__sub__", [](Matrix<double, RowMajor> & self, Matrix<double, RowMajor> & other)
      { 
        if (self.Height()!=other.Height() || self.Width()!=other.Width()) throw py::index_error("Matrix dimension not the same, subtraction impossible");
        return Matrix<double,RowMajor> (self+(-1)*other); 
      })

      //scalar multiplication
      .def("__rmul__", [](Matrix<double, RowMajor> & self, double scal)
      { return Matrix<double, RowMajor> (scal*self); })

      //matrix multiplication
      .def("__mul__", [](Matrix<double, RowMajor> & self,Matrix<double, RowMajor> & other )
      {
        if (self.Width()!=other.Height()) throw py::index_error("dimension do not fit matrix-multiplication");
        return Matrix<double,RowMajor>(self*other);
        })  
      
      //matrix-vector multiplication
      .def("__mul__", [](Matrix<double, RowMajor> & self,Vector<double> & other )
      {
        if (self.Width()!=other.Size()) throw py::index_error("dimension do not fit mat-vec-multiplication");
        return Vector<double>(self*other);
        })  


      //pickling support
      .def(py::pickle(
        [](Matrix<double,RowMajor> & self) { // __getstate__
            /* return a tuple that fully encodes the state of the object */
          return py::make_tuple(self.Height(),
                                self.Width(),
                                py::bytes((char*)(void*)&self(0,0), self.Height()*self.Width()*sizeof(double)));
        },
        [](py::tuple t) { // __setstate__
          if (t.size() != 3)
            throw std::runtime_error("should be a 3-tuple!");

          Matrix<double,RowMajor> m(t[0].cast<size_t>(),t[1].cast<size_t>());
          py::bytes mem = t[2].cast<py::bytes>();
          std::memcpy(&m(0,0), PYBIND11_BYTES_AS_STRING(mem.ptr()), m.Height()*m.Width()*sizeof(double));
          return m;
        }))

      

    ;
    
}
