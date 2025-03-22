#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <span>
#include <algorithm>
#include <memory>
#include <compare>
#include <complex>
#endif /* __PROGTEST__ */

// keep this dummy version if you do not implement a real manipulator

std::ios_base & ( * poly_var ( const std::string & name ) ) ( std::ios_base & x )
{
  return [] ( std::ios_base & ios ) -> std::ios_base & { return ios; };
}



class CPolynomial
{ 
  public:
    mutable std::vector<double> coefficients;
    std::string variable_name;
    // default constructor
    CPolynomial (std::string new_name = "x") {
      this->coefficients.push_back(0);
      this->variable_name = new_name;
    }
    std::ios_base& operator()(std::ios_base& os) const {
      // If os is an output stream, insert the prefix into it
      if (auto* out = dynamic_cast<std::ostream*>(&os)) {
          *out << variable_name;
      }
      return os;
    }
    CPolynomial operator * (const CPolynomial & other) const {
      CPolynomial result;
      this->remove_zeros();
      other.remove_zeros();
      int total_length = this->coefficients.size() + other.coefficients.size() - 1;
      result.coefficients.resize(total_length);
      for (size_t i = 0; i < this->coefficients.size() ; i++) {
        for (size_t j = 0; j < other.coefficients.size() ; j++) {
          result.coefficients[i+j] += this->coefficients[i] *other.coefficients[j];
        }
      }
      return result;
    }
    CPolynomial operator * (const double multiple) const {
      CPolynomial other;
      other[0] = multiple;
      return ((*this) * other);
    }
    CPolynomial & operator *= (const CPolynomial other)  {
      (*this) = (*this) * other;
      return *this;
    }
    CPolynomial & operator *= (const double other) {
      (*this) = (*this) * other;
      return *this;
    }
    bool operator == (const CPolynomial & other) const {
      this->remove_zeros();
      other.remove_zeros();
      if (this->coefficients.size() != other.coefficients.size()) return false;
      for (size_t i = 0; i < this->coefficients.size() - 1; i++) {
        //return abs(a-b) < 0.0001;
        if (!(abs(this->coefficients[i] - other.coefficients[i]) < 0.0001)) return false;

      }
      return true;
    }
    bool operator != (const CPolynomial & other) const {
      return !((*this) == other);
    }
    friend std::ostream & operator<<(std::ostream &os, const CPolynomial & polynom) {
      return polynom.print(os);
  }

    double & operator [] (size_t index) const {
      if (index >= this->coefficients.size()) this->coefficients.resize(index+1);
      return this->coefficients[index];
    }
    double operator () (const double x) const {
      this->remove_zeros();
      double result = 0;
      for (size_t i = 0; i < this->coefficients.size(); i++){
        result += this->coefficients[i] * pow(x, i);
      }
      return result;
    }
    // operator !
    explicit operator bool() const {
      this->remove_zeros();
      if (this->coefficients.size() > 1) return true;
      else if (this->coefficients[0] != 0) return true;
      else return false;
    }
    unsigned int degree () const{
      this->remove_zeros();
      return this->coefficients.size() -1;
    }
    void remove_zeros() const {
      for (size_t i = this->coefficients.size() - 1; i > 0; i--) {
        if (this->coefficients[i]) break;
        else this->coefficients.pop_back();
      }
      return;
    }
  private:
    


    std::ostream & print(std::ostream & os) const {
      for (size_t i = this->coefficients.size()-1; i > 0; i--) {
        double value = this->coefficients[i];
        if (value > 0) {
          if (i != (this->coefficients.size() - 1)) {
            os << " + ";
          }
          if (value != 1) {
            os << value << "*";
          }
          os << "x^" << i;
        } else if (value < 0) {
          if (i != (this->coefficients.size() - 1)) {
            os << " - ";
          } else {
            os << "- ";
          }
          if (value != -1) {
            os << value * -1.0 << "*";
          }
          os << "x^" << i;
        }
      }
      if (this->coefficients[0] > 0 ) os << " + " << this->coefficients[0];
      else if (this->coefficients[0] < 0) os << " - " << this->coefficients[0] * -1.0;
      else if (this->coefficients.size() == 1) os << "0";
      return os;
    }
    
    
};

#ifndef __PROGTEST__
bool smallDiff ( double a,
                 double b )
                 {
  return abs(a-b) < 0.0001;
}

bool dumpMatch ( const CPolynomial & x,
                 const std::vector<double> & ref )
{
  for (size_t i = 0; i < x.coefficients.size(); i++) {
    if (!smallDiff(x.coefficients[i], ref[i])) return false;
  }
  return true;
}

int main ()
{
  std::cout << "yes its working" << std::endl;

  CPolynomial a, b, c;
  std::ostringstream out, tmp;

  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
  a[4] = 0;
  a[5] = 0;
  a[6] = 0;
  a[7] = 0;
  assert ( smallDiff ( a ( 2 ), 5 ) );
  a.remove_zeros();
  out . str ("");
  out << a;
  std::cout << a << std::endl;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );

  c = a * -2;
  assert ( c . degree () == 3
           && dumpMatch ( c, std::vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
  b[5] = -1;
  b[2] = 3;
  out . str ("");
  out << b;
  assert ( out . str () == "- x^5 + 3*x^2" );
  c = a * b;
  assert ( c . degree () == 8
           && dumpMatch ( c, std::vector<double>{ -0.0, -0.0, -30.0, 10.5, -0.0, 13.0, -3.5, 0.0, -1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- x^8 - 3.5*x^6 + 13*x^5 + 10.5*x^3 - 30*x^2" );
  a *= 5;
  assert ( a . degree () == 3
           && dumpMatch ( a, std::vector<double>{ -50.0, 17.5, 0.0, 5.0 } ) );

  a *= b;
  assert ( a . degree () == 8
           && dumpMatch ( a, std::vector<double>{ 0.0, 0.0, -150.0, 52.5, -0.0, 65.0, -17.5, -0.0, -5.0 } ) );

  assert ( a != b );

  b[5] = 0;
  assert ( static_cast<bool> ( b ) );
  assert ( ! ! b );
  b[2] = 0;
  assert ( !(a == b) );
  a *= 0;
  assert ( a . degree () == 0
           && dumpMatch ( a, std::vector<double>{ 0.0 } ) );

  assert ( a == b );
  assert ( ! static_cast<bool> ( b ) );
  assert ( ! b );

  // bonus - manipulators

  out . str ("");
  out << poly_var ( "y" ) << c;
  assert ( out . str () == "- y^8 - 3.5*y^6 + 13*y^5 + 10.5*y^3 - 30*y^2" );
  out . str ("");
  tmp << poly_var ( "abc" );
  out . copyfmt ( tmp );
  out << c;
  assert ( out . str () == "- abc^8 - 3.5*abc^6 + 13*abc^5 + 10.5*abc^3 - 30*abc^2" );

  return EXIT_SUCCESS;
 }
#endif /* __PROGTEST__ */
 