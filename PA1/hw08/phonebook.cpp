#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

constexpr int PHONE_DIGITS = 10;

typedef struct TNode
{
  char                               * m_Name;
  struct TNode                       * m_Child[PHONE_DIGITS];
} TNODE;

typedef struct
{
  TNODE                              * m_Root;
  int                                  m_Size;
} TPHONEBOOK;

#endif /* __PROGTEST__ */

TNODE * create_node(char * name = NULL)
{
  TNODE * result = (TNODE *)malloc(sizeof(TNODE));
  
  for(int i = 0; i < 10; i++)
  {
    result->m_Child[i] = NULL;
  }
  if (name == NULL) result->m_Name = nullptr;
  else result->m_Name = name;
  return result;
}
void print_book_recursive(TPHONEBOOK * book, TNODE * current, char * number, int depth)
{
  //printf("zatial\n");
  for(int i = 0; i < depth; i++)
  {
    printf("%c", number[i]);
  }
  //printf("je to good");
  printf(": %s\n", current->m_Name ? current->m_Name : "(null)");
  for (int i = 0; i < 10; i++)
  {
    
    if(current->m_Child[i] != nullptr)
    {
      //printf("vojdem\n");
      number[depth] = '0' + i;
      //printf("tocim sa once again %s(%d)\n", number,i); 
      //printf("zijem?\n");
      print_book_recursive(book, current->m_Child[i], number, depth+1);
    }
  }
}
void print_book(TPHONEBOOK * book)
{
  char number[10];
  //printf("zijem\n");
  if(book->m_Root) print_book_recursive(book, book->m_Root, number, 0);
  else printf("prazdna kniha");
}


bool addPhone_recursive(TPHONEBOOK * book, TNODE * current,  char * phone, char * name)
{
  if(phone[0] == '\0')
  {
    if (current->m_Name == nullptr)
    {
      current->m_Name = strdup(name);
      book->m_Size++;
      return true;
    } else
    {
      free(current->m_Name);
      current->m_Name = strdup(name);
      
      return true;
    }
  }
  int digit = phone[0] - '0';
  if (!current->m_Child[digit])
  {
    current->m_Child[digit] = create_node(nullptr);
  }
  return addPhone_recursive(book, current->m_Child[digit], phone + 1, name);
}

bool         addPhone  ( TPHONEBOOK * book,
                         const char * phone,
                         const char * name )
{
  if (!book->m_Root) book->m_Root = create_node();
  return addPhone_recursive(book, book->m_Root, (char *)phone, (char *)name);
}

void delBook_recursive(TNODE * current)
{
  //printf("vymazal som \n");
  for (int i = 0; i < 10; i++)
  {
    if (current->m_Child[i] != nullptr)
    {
      delBook_recursive(current->m_Child[i]);
    }
  }
  free(current->m_Name);
  free(current);
}
void         delBook   ( TPHONEBOOK * book )
{
  delBook_recursive(book->m_Root);
}



bool delPhone_recursive(TNODE * current, const char * phone, int depth)
{
  int next_digit = phone[depth] - '0';
  if (phone[depth +1] == '\0')
  {
    //idem vymazat dalsi element ak existuje
    if (current->m_Child[next_digit] != nullptr)
    {
      delBook_recursive(current->m_Child[next_digit]);
      current->m_Child[next_digit] = nullptr;
      return true;
    } else return false;
  } else
  {
    if (current->m_Child[next_digit] != nullptr)
    {
      return delPhone_recursive(current->m_Child[next_digit], phone, depth + 1);
    } else
    {
      return false;
    }
  }
}

bool         delPhone  ( TPHONEBOOK * book,
                         const char * phone )
{
  TNODE * current = book->m_Root;
  int depth = 0;
  return delPhone_recursive(current, phone, depth);
}


const char * findPhone ( TPHONEBOOK * book,
                         const char * phone )
{
  return phone;
}

#ifndef __PROGTEST__


void tests()
{
  TPHONEBOOK b = { nullptr, 0 };
  char tmpStr[100];
  const char * name;
  assert ( addPhone ( &b, "420", "Czech Republic" ) );
  assert ( addPhone ( &b, "42022435", "Czech Republic CVUT" ) );
  assert ( addPhone ( &b, "421", "Slovak Republic" ) );
  assert ( addPhone ( &b, "44", "Great Britain" ) );
  strncpy ( tmpStr, "USA", sizeof ( tmpStr ) - 1 );
  assert ( addPhone ( &b, "1", tmpStr ) );
  strncpy ( tmpStr, "Guam", sizeof ( tmpStr ) - 1 );
  assert ( addPhone ( &b, "1671", tmpStr ) );
  assert ( addPhone ( &b, "44", "United Kingdom" ) );
  assert ( b . m_Size == 6 );
  assert ( ! b . m_Root -> m_Name );
  assert ( ! b . m_Root -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Name );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Name );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Name, "Guam" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name, "Czech Republic" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Name, "Slovak Republic" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[4] -> m_Name, "United Kingdom" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[9] );
  name = findPhone ( &b, "420800123456" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  name = findPhone ( &b, "42022435" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  name = findPhone ( &b, "4202243" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "420224343258985224" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "42" );  assert ( !name );
  name = findPhone ( &b, "422" );  assert ( !name );
  name = findPhone ( &b, "4422" );  assert ( !strcmp ( name, "United Kingdom" ) );
  name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "Guam" ) );
  name = findPhone ( &b, "123456789123456789" );  assert ( !strcmp ( name, "USA" ) );
  assert ( delPhone ( &b, "420" ) );
  assert ( delPhone ( &b, "421" ) );
  assert ( delPhone ( &b, "44" ) );
  assert ( delPhone ( &b, "1671" ) );
  assert ( !delPhone ( &b, "1672" ) );
  assert ( !delPhone ( &b, "1671" ) );
  name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "USA" ) );
  name = findPhone ( &b, "4422" );  assert ( !name );
  name = findPhone ( &b, "420800123456" );  assert ( !name );
  name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  assert ( b . m_Size == 2 );
  assert ( ! b . m_Root -> m_Name );
  assert ( ! b . m_Root -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[9] );
  assert ( delPhone ( &b, "1" ) );
  assert ( delPhone ( &b, "42022435" ) );
  assert ( !addPhone ( &b, "12345XYZ", "test" ) );
  assert ( b . m_Size == 0 );
  assert ( ! b . m_Root );

  delBook ( &b );
}

int main ()
{
  /*
  TPHONEBOOK b = { nullptr, 0 };
  bool result;
  addPhone(&b, "421", "SLOVENSKOOOOOO");
  addPhone(&b, "420", "Czech Republic" );
  addPhone(&b, "4203", "Czech Republic CVUT" );
  addPhone(&b, "4202", "je to taaam");
  //printf("hopa\n");
  print_book(&b);
  result = delPhone(&b, "4202");
  printf("vysledok je (%d)\n", result);
  print_book(&b);
  delBook(&b);
  */
  tests();
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
