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


using namespace std;
class A
{
  public:
    A ( int x = 88 ) : m_X ( new int ( x ) ) { }
    A ( const A & src ) : m_X ( new int ( *src . m_X ) ) { }
    virtual ~A ( void ) { delete m_X; }
    virtual void print ( void ) const { cout << *m_X; }
  private: 
    int *m_X;
};

class B : public A
{
  public:
    B ( int x, int y ) : A ( x ), m_Y ( new int ( y ) ) { }
    B ( const B & src ) : m_Y ( new int ( *src.m_Y ) ) { }   
    virtual ~B ( void ) { delete m_Y; }
    virtual void print ( void ) const { A::print (); cout << *m_Y; }
  private: 
    int *m_Y;
};

void foo ( const B val )
{
  val . print ( );
}

int main ( void )
{
  B test ( 34, 87 );

  foo ( test );
  return 0;
}