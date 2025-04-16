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
class CArray
 {
   public:
    CArray ( int len )
     {
       m_D = new int[ m_L = len ];
       for ( int i = 0; i < m_L; i ++ ) m_D[i] = 0;
     }  
    CArray & operator = ( const CArray src )
     {
       delete [] m_D;  
       m_D = new int[ m_L = src . m_L ];
       for ( int i = 0; i < m_L; i ++ ) m_D[i] = src . m_D[i];
       return *this;
     } 
    ~CArray ( void ) { delete [] m_D; }
    int & operator [] ( int i ) { return m_D[i]; }
   private:   
    int * m_D;
    int   m_L;
 };

int main ( void )
 {
   CArray a(51), b(12);

   b = a;
   a[12] = 71;   
   std::cout << b[12];
   return 0;
 }