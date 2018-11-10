#ifndef KMUVCL_GRAPHICS_OPERATOR_HPP
#define KMUVCL_GRAPHICS_OPERATOR_HPP

#include "vec.hpp"
#include "mat.hpp"

namespace kmuvcl {
  namespace math {

    /// y_n = s * x_n
    template <unsigned int N, typename T>
    vec<N, T> operator* (const T s, const vec<N, T>& x)
    {
      // TODO: Fill up this function properly
      vec<N, T> y;
      for(int i = 0; i < N; ++i)
      {
	       y(i) = s * x(i);
      }
      return  y;
    }

    /// s = u_n * v_n (dot product)
    template <unsigned int N, typename T>
    T dot(const vec<N, T>& u, const vec<N, T>& v)
    {
      // TODO: Fill up this function properly
      T val = 0;
      for(int i = 0; i < N; ++i)
      {
	       val += u(i) * v(i);
      }
      return  val;
    }

    /// w_3 = u_3 x v_3 (cross product, only for vec3)
    template <typename T>
    vec<3,T> cross(const vec<3, T>& u, const vec<3, T>& v)
    {
      // TODO: Fill up this function properly
      vec<3, T>  w;
      w(0) = u(1) * v(2) - u(2) * v(1);
      w(1) = u(2) * v(0) - u(0) * v(2);
      w(2) = u(0) * v(1) - u(1) * v(0);
      return  w;
    }

    /// y_m = A_{mxn} * x_n
    template <unsigned int M, unsigned int N, typename T>
    vec<M, T> operator* (const mat<M, N, T>& A, const vec<N, T>& x)
    {
      // TODO: Fill up this function properly
      vec<M, T>   y, col;
      for(int i = 0; i < N; ++i)
      {
	      A.get_ith_column(i, col);
        y += x(i) * col;
      }
      return  y;
    };

    /// y_n = x_m * A_{mxn}
    template <unsigned int M, unsigned int N, typename T>
    vec<N, T> operator* (const vec<M, T>& x, const mat<M, N, T>& A)
    {
      // TODO: Fill up this function properly
      vec<N, T>   y;
      vec<M, T>   col;
      for(int i = 0; i < N; ++i)
      {
	      A.get_ith_column(i, col);
        y(i) = dot(col, x);
      }
      return  y;
    }

    /// C_{mxl} = A_{mxn} * B_{nxl}
    template <unsigned int M, unsigned int N, unsigned int L, typename T>
    mat<M, L, T> operator* (const mat<M, N, T>& A, const mat<N, L, T>& B)
    {
      // TODO: Fill up this function properly
      mat<M, L, T>   C;
      vec<N, T> col;
      vec<N, T> row;
      for(int i = 0; i < M; ++i)
      {
	       A.get_ith_row(i, row);
	       for(int j = 0; j < L; ++j)
	       {
           B.get_ith_column(j, col);
	         C(i, j) = dot(row, col);
	       }
      }
      return  C;
    }

    /// C_{mxn} = A_{mxn} / B
    template <unsigned int M, unsigned int N, typename T>
    mat<M, N, T> operator/ (const mat<M, N, T>& A, const T B)
    {
      // TODO: Fill up this function properly
      mat<M, N, T>   C;
      for(int i = 0; i < M; ++i)
      {
         for(int j = 0; j < N; ++j)
         {
           C(i, j) = A(i, j) / B;
         }
      }
      return  C;
    }

    template<typename T>
    mat<4, 4, T> inverse(const mat<4, 4, T>& m)
    {
        T m00 = m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3);
        T m01 = m(2, 1) * m(3, 3) - m(3, 1) * m(2, 3);
        T m02 = m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2);
        T m03 = m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3);
        T m04 = m(2, 0) * m(3, 2) - m(3, 0) * m(2, 2);
        T m05 = m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1);
        T m06 = m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3);
        T m07 = m(1, 1) * m(3, 3) - m(3, 1) * m(1, 3);
        T m08 = m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2);
        T m09 = m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3);
        T m10 = m(1, 0) * m(3, 2) - m(3, 0) * m(1, 2);
        T m11 = m(1, 1) * m(3, 3) - m(3, 1) * m(1, 3);
        T m12 = m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1);
        T m13 = m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3);
        T m14 = m(1, 1) * m(2, 3) - m(2, 1) * m(1, 3);
        T m15 = m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2);
        T m16 = m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3);
        T m17 = m(1, 0) * m(2, 2) - m(2, 0) * m(1, 2);
        T m18 = m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1);

        mat<4, 4, T> inverseMat;
        inverseMat(0, 0) = + (m(1, 1) * m00 - m(1, 2) * m01 + m(1, 3) * m02);
        inverseMat(0, 1) = - (m(1, 0) * m00 - m(1, 2) * m03 + m(1, 3) * m04);
        inverseMat(0, 2) = + (m(1, 0) * m01 - m(1, 1) * m03 + m(1, 3) * m05);
        inverseMat(0, 3) = - (m(1, 0) * m02 - m(1, 1) * m04 + m(1, 2) * m05);

        inverseMat(1, 0) = - (m(0, 1) * m00 - m(0, 2) * m01 + m(0, 3) * m02);
        inverseMat(1, 1) = + (m(0, 0) * m00 - m(0, 2) * m03 + m(0, 3) * m04);
        inverseMat(1, 2) = - (m(0, 0) * m01 - m(0, 1) * m03 + m(0, 3) * m05);
        inverseMat(1, 3) = + (m(0, 0) * m02 - m(0, 1) * m04 + m(0, 2) * m05);

        inverseMat(2, 0) = + (m(0, 1) * m06 - m(0, 2) * m07 + m(0, 3) * m08);
        inverseMat(2, 1) = - (m(0, 0) * m06 - m(0, 2) * m09 + m(0, 3) * m10);
        inverseMat(2, 2) = + (m(0, 0) * m11 - m(0, 1) * m09 + m(0, 3) * m12);
        inverseMat(2, 3) = - (m(0, 0) * m08 - m(0, 1) * m10 + m(0, 2) * m12);
  
        inverseMat(3, 0) = - (m(0, 1) * m13 - m(0, 2) * m14 + m(0, 3) * m15);
        inverseMat(3, 1) = + (m(0, 0) * m13 - m(0, 2) * m16 + m(0, 3) * m17);
        inverseMat(3, 2) = - (m(0, 0) * m14 - m(0, 1) * m16 + m(0, 3) * m18);
        inverseMat(3, 3) = + (m(0, 0) * m15 - m(0, 1) * m17 + m(0, 2) * m18);

        T det = m(0, 0) * inverseMat(0, 0) +
                m(0, 1) * inverseMat(0, 1) +
                m(0, 2) * inverseMat(0, 2) +
                m(0, 3) * inverseMat(0, 3);

        inverseMat = inverseMat / det;

        inverseMat = inverseMat.transpose();

        return inverseMat;
    }

    /// ostream for vec class
    template <unsigned int N, typename T>
    std::ostream& operator << (std::ostream& os, const vec<N, T>& v)
    {
      // TODO: Fill up this function properly
      os << "[";
      for(int i=0; i < N - 1; ++i)
      {
	       os << v(i) << ", ";
      }
      os << v(N-1);
      os << "]";

      return  os;
    }

    /// ostream for mat class
    template <unsigned int M, unsigned int N, typename T>
    std::ostream& operator << (std::ostream& os, const mat<M, N, T>& A)
    {
      // TODO: Fill up this function properly
      for(int i = 0; i < M; ++i)
      {
	       os << "[";
	       for(int j = 0; j < N - 1; ++j)
	       {
		         os << A(i, j) << ", ";
	       }
	       os << A(i, N - 1);
	       os << "]";
	       os << std::endl;
      }
      return  os;
    }
  } // math
} // kmuvcl

#endif // KMUVCL_GRAPHICS_OPERATOR_HPP
