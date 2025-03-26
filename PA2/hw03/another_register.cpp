#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#endif /* __PROGTEST__ */
/*
class Database {
public:
  char ** names;
  char ** surnames;
  char ** streets;
  char ** cities;
  int reference_counter;
};
*/


class Adress {
public:
  char date[12]= "0000-00-00";
  const char * street;
  const char * city;
  Adress(){
    this->street = nullptr;
    this->city = nullptr;
  }
  Adress( const char new_date [], const char * street, const char * city) {
    this->city = strndup(city, strlen(city));
    this->street = strndup(street, strlen(street));
    for (int i = 0; i < 12; i++) {
      this->date[i] = new_date[i];
    }
    return;
  }
  void print () {
    std::cout << this->date << " " << this->street << " " << this->city << " ";
  }
  void print (std::ostream & os) {
    os << this->date << " " << this->street << " " << this->city << "\n";
  }
};

class Person {
  public:
  const char * name;
  const char * surname;
  char id [12] = "000000/0000";
  Adress * adresses;
  int size_adresses;
  int ref_counter;

  Person(const char * name, const char * surname, const char id [],const  char date[],const char * street, const char * city) {
    (*this).name = strndup(name, strlen(name));
    (*this).surname = strndup(surname, strlen(surname));
    for (int i = 0; i < 12; i++) {
      (*this).id[i] = id[i];
    }
    this->adresses = new Adress [1];
    this->size_adresses = 1;
    Adress new_adress(date, street, city);
    (this->adresses)[0] = new_adress;
    ref_counter = 1;
  }
  Person(const Person & old) {
    (*this).name = strndup(old.name, strlen(old.name));
    (*this).surname = strndup(old.surname, strlen(old.surname));
    for (int i = 0; i < 12; i++) {
      (*this).id[i] = old.id[i];
    }
    this->adresses = new Adress [old.size_adresses];
    this->size_adresses = old.size_adresses;
    for (int i = 0; i < this->size_adresses; i++) {
      this->adresses[i].city = strdup(old.adresses[i].city);
      this->adresses[i].street = strdup(old.adresses[i].street);
      for (int j = 0; j < 12; j++) {
        this->adresses[i].date[j] = old.adresses[i].date[j];
      }
    }
    this->ref_counter = 1;
  }
  ~Person(){
    if (this->ref_counter ==1) {
      for (int i = 0; i < size_adresses; i++) {
        free((void*)this->adresses[i].city);
        free((void*)this->adresses[i].street);
      }
      delete[] this->adresses;
      free((void*)this->name);
      free((void*)this->surname);
    } else this->ref_counter -=1;
  }

  void print_person() {
    std::cout << this->id << " " <<
                 this->name << " " <<
                 this->surname << " ";
    for (int i = 0; i < this->size_adresses; i++) {
      this->adresses[i].print();
    }
    std::cout << std::endl;
  }
  void print_person(std::ostream & os) {
    os << this->id << " " << this->name << " " << this->surname << "\n";
    for (int i = 0; i < this->size_adresses; i++) this->adresses[i].print(os);
    return;
  }
  bool append(Adress & new_adress) {
    Adress * new_addresses = new Adress[size_adresses + 1];
    int indent = 0;
    for (int i = 0; i < size_adresses; i++) {
      if (!strcmp(this->adresses[i].date, new_adress.date)) {
        delete [] new_addresses;
        free((void*)new_adress.city);
        free((void*)new_adress.street);
        return false;
      } else if (strcmp(this->adresses[i].date, new_adress.date) > 0) {
        new_addresses[i] = new_adress;
        indent = 1;
      }
      new_addresses[i + indent] = this->adresses[i];
    }
    if (indent == 0) new_addresses[size_adresses] = new_adress;
    this->size_adresses += 1;
    delete [] this->adresses;
    this->adresses = new_addresses;
    return true;

  }
};

bool id_compare(const char * person1, const char * person2) {
  for (int i = 0; i < 12; i++) {
    if (person1[i] != person2[i]) return false;
  }
  return true;
}
class CRegister
{
  public:
  int      size_person;
  Person ** list_person;
  int      index;
  CRegister() {
    size_person = 10;
    list_person = new Person * [size_person];
    index = 0;
  }
  CRegister (const CRegister & old) {
    this->size_person = old.size_person;
    this->list_person = new Person * [this->size_person];
    this->index =  old.index;
    for (int i = 0; i < this->index; i++) {
      this->list_person[i] = old.list_person[i];
      this->list_person[i]->ref_counter += 1;
    }
    return;
  }
  CRegister & operator = (const CRegister & other) {
    if (this != &other){
      CRegister temp(other);
      std::swap(this->index, temp.index);
      std::swap(this->size_person, temp.size_person);
      std::swap(this->list_person, temp.list_person);
    }
    return *this;
  }
  ~CRegister () {
    std::cout << "deleting register---------------------------------------" << std::endl;
    for (int i = 0; i < index; i++) {
      std::cout << "person at index " << i << " with id "<< this->list_person[i]->id <<  " with ref counter" << this->list_person[i]->ref_counter << std::endl;
      if (this->list_person[i]->ref_counter > 1) {
        //std::cout << "left person" << i <<std::endl;
        this->list_person[i]->ref_counter -= 1;
        this->list_person[i] = nullptr;
      } else  if (this->list_person[i]->ref_counter == 1) {
        std::cout << "deleted person" << i << std::endl;
        delete this->list_person[i];
      }
    }
    delete [] this->list_person;
  }

    // default constructor done
    // copy constructor
    // destructor done
    // operator =
    bool add       ( const char     id[],
                     const char     name[],
                     const char     surname[],
                     const char     date[],
                     const char     street[],
                     const char     city[] ) {
      Person * guy = new Person(name, surname, id, date, street, city);
      for (int i = 0; i < index; i++) {
          if (id_compare(this->list_person[i]->id, guy->id)){
            delete guy;
            return false;
          }
      }
      this->append(guy);
      return true;
    }

    bool resettle  ( const char     id[],
                     const char     date[],
                     const char     street[],
                     const char     city[] ) {
      for (int i = 0; i < index; i++) {
          if (id_compare((*this->list_person[i]).id, id)){
            if (this->list_person[i]->ref_counter > 1) {
              this->list_person[i]->ref_counter -=1;
              Person * new_copy = new Person ((*this->list_person[i]));
              this->list_person[i] = new_copy;
            }
            Adress new_adress(date, street, city);
            return (*this->list_person[i]).append(new_adress);
          }
      }
      return false;
    }

    bool print     ( std::ostream & os,
                     const char     id[] ) const {
      for (int i = 0; i < this->index; i++) {
        if (!strcmp(this->list_person[i]->id, id)) {
          //os << "\"###(";
          this->list_person[i]->print_person(os);
          //os << ")###\"";
          return true;
        }
      }
      return false;
    }
    void append(Person * guy) {
      if (index >= size_person){
        size_person = size_person * 2 + 1;
        Person ** new_list = new Person*[size_person];
        for (int i = 0; i < index; i++) {
          new_list[i] = (this->list_person)[i];
        }
        delete [] this->list_person;
        this->list_person = new_list;
      }
      (list_person[index]) = guy;
      this->index +=1;
      return;
    }
};


#ifndef __PROGTEST__
int main ()
{
  char   lID[12], lDate[12], lName[50], lSurname[50], lStreet[50], lCity[50];
  std::ostringstream oss;
  CRegister  a;
  assert ( a . add ( "123456/7890", "John", "Smith", "2000-01-01", "Main street", "Seattle" ) == true );
  assert ( a . add ( "987654/3210", "Freddy", "Kruger", "2001-02-03", "Elm street", "Sacramento" ) == true );
  assert ( a . resettle ( "123456/7890", "2003-05-12", "Elm street", "Atlanta" ) == true );
  assert ( a . resettle ( "123456/7890", "2002-12-05", "Sunset boulevard", "Los Angeles" ) == true );
  oss . str ( "" );
  assert ( a . print ( oss, "123456/7890" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(123456/7890 John Smith
2000-01-01 Main street Seattle
2002-12-05 Sunset boulevard Los Angeles
2003-05-12 Elm street Atlanta
)###" ) );
  oss . str ( "" );
  assert ( a . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
)###" ) );
  CRegister b ( a );
  assert ( b . resettle ( "987654/3210", "2008-04-12", "Elm street", "Cinccinati" ) == true );
  assert ( a . resettle ( "987654/3210", "2007-02-11", "Elm street", "Indianapolis" ) == true );
  oss . str ( "" );
  assert ( a . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2007-02-11 Elm street Indianapolis
)###" ) );
  oss . str ( "" );
  assert ( b . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
)###" ) );
a = b;
std::cout << "asi som zmazal a" << std::endl;
assert ( a . resettle ( "987654/3210", "2011-05-05", "Elm street", "Salt Lake City" ) == true );
oss . str ( "" );
assert ( a . print ( oss, "987654/3210" ) == true );
assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
2011-05-05 Elm street Salt Lake City
)###" ) );
oss . str ( "" );
assert ( b . print ( oss, "987654/3210" ) == true );
assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
)###" ) );
assert ( b . add ( "987654/3210", "Joe", "Lee", "2010-03-17", "Abbey road", "London" ) == false );

assert ( a . resettle ( "987654/3210", "2001-02-03", "Second street", "Milwaukee" ) == false );

oss . str ( "" );
assert ( a . print ( oss, "666666/6666" ) == false );
CRegister  c;
  strncpy ( lID, "123456/7890", sizeof ( lID ) );
  strncpy ( lName, "John", sizeof ( lName ) );
  strncpy ( lSurname, "Smith", sizeof ( lSurname ) );
  strncpy ( lDate, "2000-01-01", sizeof ( lDate) );
  strncpy ( lStreet, "Main street", sizeof ( lStreet ) );
  strncpy ( lCity, "Seattle", sizeof ( lCity ) );
  assert (  c . add ( lID, lName, lSurname, lDate, lStreet, lCity ) == true );
  strncpy ( lID, "987654/3210", sizeof ( lID ) );
  strncpy ( lName, "Freddy", sizeof ( lName ) );
  strncpy ( lSurname, "Kruger", sizeof ( lSurname ) );
  strncpy ( lDate, "2001-02-03", sizeof ( lDate) );
  strncpy ( lStreet, "Elm street", sizeof ( lStreet ) );
  strncpy ( lCity, "Sacramento", sizeof ( lCity ) );
  assert (  c . add ( lID, lName, lSurname, lDate, lStreet, lCity ) == true );
  strncpy ( lID, "123456/7890", sizeof ( lID ) );
  strncpy ( lDate, "2003-05-12", sizeof ( lDate) );
  strncpy ( lStreet, "Elm street", sizeof ( lStreet ) );
  strncpy ( lCity, "Atlanta", sizeof ( lCity ) );
  assert ( c . resettle ( lID, lDate, lStreet, lCity ) == true );
  strncpy ( lID, "123456/7890", sizeof ( lID ) );
  strncpy ( lDate, "2002-12-05", sizeof ( lDate) );
  strncpy ( lStreet, "Sunset boulevard", sizeof ( lStreet ) );
  strncpy ( lCity, "Los Angeles", sizeof ( lCity ) );
  assert ( c . resettle ( lID, lDate, lStreet, lCity ) == true );
  oss . str ( "" );
  assert ( c . print ( oss, "123456/7890" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(123456/7890 John Smith
2000-01-01 Main street Seattle
2002-12-05 Sunset boulevard Los Angeles
2003-05-12 Elm street Atlanta
)###" ) );

  std::cout << "konec" << std::endl;
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
