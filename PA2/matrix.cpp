#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
// ####################################################################
// Forward declaration
template <typename T_>
class CMatrixView;
// ####################################################################
template <typename T_>
class CMatrix {
 public:
  CMatrix(const size_t h, const size_t w) : m_H(h), m_W(w) {
    m_Data =
        std::make_shared<std::vector<std::vector<T_>>>(h, std::vector<T_>(w));
  }

  CMatrix(const CMatrix & source) {
    m_H = source.m_H;
    m_W = source.m_W;
    this->m_Data = source.m_Data;
  }

  // operator () to access elements
  T_ &operator()(size_t row, size_t col) {
    if (row >= m_H || col >= m_W) throw std::out_of_range("out of range");
    return (*this->m_Data)[row][col];
  }

  CMatrixView<T_> createView(ssize_t rowOffset, ssize_t colOffset) {
    CMatrixView<T_> result(*this, rowOffset, colOffset);
    return result;
  }

  size_t m_H;
  size_t m_W;
  std::shared_ptr<std::vector<std::vector<T_>>> m_Data;
};
// ####################################################################

template <typename T_>
class CMatrixView {
 public:
  CMatrixView(CMatrix<T_> &m, ssize_t roww, ssize_t coll)
      : row(roww), col(coll) {
    this->matrix = std::make_shared<CMatrix<T_>>(m);
  }
  CMatrixView(CMatrixView<T_> & source) : row(source.row), col(source.col) {
    this->matrix = source.matrix;
  }
  // operator () to access elements
  T_ &operator()(ssize_t rowOffset, ssize_t colOffset) {
    return (*this->matrix)(row + rowOffset, col + colOffset);
  }

 private:
  ssize_t row;
  ssize_t col;
  std::shared_ptr<CMatrix<T_>> matrix;
};

// ####################################################################
int main() {
  bool exceptionThrown = false;

  CMatrix<int> m1(3, 3);

  std::cout << m1(0, 0) << std::endl;
  m1(0, 0) = 100;
  std::cout << m1(0, 0) << std::endl;

  std::cout << m1(0, 1) << std::endl;
  m1(0, 1) = 200;
  std::cout << m1(0, 1) << std::endl;

  m1(2, 2) = 400;
  m1(1, 1) = 0;

  assert(m1(0, 0) == 100);
  assert(m1(1, 1) == 0);
  assert(m1(2, 2) == 400);

  CMatrixView<int> v1 = m1.createView(1, 1);

  assert(v1(-1, -1) == 100);
  v1(-1, -1) = 50;
  assert(v1(-1, -1) == 50);
  /*
  exceptionThrown = false;
  try {
    v1(2, 2) = 0;
  } catch (const std::out_of_range &) {
    exceptionThrown = true;
  }
  assert(exceptionThrown);
  */
  
  CMatrix<int> m2(4, 5);
  
  m2(1, 3) = 100;
  m2(0, 2) = 200;
  m2(3, 4) = 400;
  m2(0, 1) = 600;
  
  assert(m2(1, 3) == 100);
  assert(m2(0, 1) == 600);
  assert(m2(3, 4) == 400);
  
  CMatrixView<int> v2 = m2.createView(-2, 5);
  
  assert(v2(2, -3) == 200);
  assert(v2(5, -1) == 400);
  
  CMatrixView<int> v3(v2);
  
  assert(v3(2, -3) == 200);
  assert(v3(5, -1) == 400);
  
  v3 = v1;
  
  assert(v3(-1, -1) == 50);
  
  
  m2 = m1;
  
  exceptionThrown = false;
  try {
    v2(2, 2) = 0;
    } catch (const std::logic_error &) {
exceptionThrown = true;
}
assert(exceptionThrown);

std::cout << "WE DID ITTTTT" << std::endl;
  return EXIT_SUCCESS;
}