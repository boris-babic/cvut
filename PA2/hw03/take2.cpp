#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#endif /* __PROGTEST__ */

bool date_compare(char date1 [], char date2 []){
  for (int i = 0; i < 12; i++) {
    if (date1[i] != date2[i]) return false;
  }
  return true;
}
class Adress {
public:
  char date[12]= "0000-00-00";
  const char * street;
  const char * city;
  Adress(){
    this->street = nullptr;
    this->city = nullptr;
  }
  ~Adress() {
    free((void*)this->city);
    free((void*)this->street);
  }
  Adress(const Adress& other) {
    this->city = strdup(other.city);
    this->street = strdup(other.street);
    strncpy(this->date, other.date, 12);
    this->date[11] = '\0';
  }
  Adress& operator=(const Adress& other) {
      if (this != &other) {
          free((void*)city);
          free((void*)street);
          this->city = strdup(other.city);
          this->street = strdup(other.street);
          strncpy(this->date, other.date, 12);
          this->date[11] = '\0';
      }
      return *this;
  }
  Adress( const char new_date [], const char * street, const char * city) {
    this->city = strndup(city, strlen(city)+1);
    this->street = strndup(street, strlen(street)+1);
    for (int i = 0; i < 11; i++) {
      this->date[i] = new_date[i];
    }
    this->date[11] = '\0';
    return;
  }
  void print () {
    std::cout << this->date << " " << this->street << " " << this->city << std::endl;
  }
  void print (std::ostream & os) {
    os << this->date << " " << this->street << " " << this->city << std::endl;
  }
};

class Person {
  public:
  const char * name;
  const char * surname;
  char id [12] = "000000/0000";
  Adress * adresses;
  int size_adresses;
  Person () {
    this->surname = nullptr;
    this->name = nullptr;
    this->adresses = nullptr;
  }
  Person(const char * name, const char * surname, const char id [],const  char date[],const char * street, const char * city) {
    (*this).name = strndup(name, strlen(name)+1);
    (*this).surname = strndup(surname, strlen(surname)+1);
    strncpy(this->id, id, 12);
    this->adresses = new Adress [1];
    this->size_adresses = 1;
    Adress new_adress(date, street, city);
    (this->adresses)[0] = new_adress;
  }
  Person(const Person & old) {
    (*this).name = strndup(old.name, strlen(old.name)+1);
    (*this).surname = strndup(old.surname, strlen(old.surname)+1);
    strncpy(this->id, old.id, 12);
    this->adresses = new Adress [old.size_adresses];
    this->size_adresses = old.size_adresses;

    for (int i = 0; i < this->size_adresses; i++) {
      this->adresses[i].city = strdup(old.adresses[i].city);
      this->adresses[i].street = strdup(old.adresses[i].street);
      for (int j = 0; j < 12; j++) {
        this->adresses[i].date[j] = old.adresses[i].date[j];
      }
    }
  }
  Person& operator=(const Person& old) {
    if (this != &old) {
        free((void*)name);
        free((void*)surname);
        delete[] adresses;

        this->name = strdup(old.name);
        this->surname = strdup(old.surname);
        strncpy(this->id, old.id, 12);

        this->size_adresses = old.size_adresses;
        this->adresses = new Adress[size_adresses];
        for (int i = 0; i < size_adresses; i++) {
            this->adresses[i] = old.adresses[i];
        }
    }
    return *this;
  }
  ~Person(){
    delete[] this->adresses;
    free((void*)this->name);
    free((void*)this->surname);
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
    os << this->id << " " << this->name << " " << this->surname << std::endl;
    for (int i = 0; i < this->size_adresses; i++) this->adresses[i].print(os);
    return;
  }
  bool append(Adress & new_adress) {
    Adress * new_addresses = new Adress[size_adresses + 1];
    int indent = 0;
    for (int i = 0; i < size_adresses; i++) {
      if (date_compare(this->adresses[i].date, new_adress.date)) {
        delete [] new_addresses;
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
class Data_Person {
public:
  Person * data;
  int index;
  int size_person;
  int ref_counter;
  Data_Person() {
    this->index = 0;
    this->size_person = 10;
    this->ref_counter = 1;
    this->data = new Person [size_person];
  }
  Data_Person(Data_Person & old) {
    this->index = old.index;
    this->size_person = old.size_person;
    this->ref_counter = 1;
    this->data = new Person [size_person];
    for (int i = 0; i < index; i++) {
      this->data[i] = old.data[i];
    }
    return;
  }
  ~Data_Person(){
    if (this->ref_counter <= 1) {
      delete [] this->data;
    }
    else this->ref_counter -=1;
    return;
  }
  void append(Person guy) {
    if(this->index >= size_person) {
      size_person = size_person * 2 + 1;
      Person * new_list = new Person[size_person];
      for (int i = 0; i < this->index; i++) {
        new_list[i ] = this->data[i];
      delete [] this->data;
      this->data = new_list;
    }
    this->data[index] = guy;
    this->index +=1;
    return;
  }
};
class CRegister
{
  public:
  Data_Person * list_person;
  CRegister() {
    this->list_person = new Data_Person;
  }
  CRegister (const CRegister & old) {
    this->list_person = old.list_person;
    this->list_person->ref_counter +=1;
  }
  CRegister & operator = (const CRegister & other) {
    if (this != &other) {
        Data_Person* old = list_person;     
        other.list_person->ref_counter++;     
        list_person = other.list_person;        
        if (old->ref_counter <= 1) {            
            delete old;                          
        } else {
            old->ref_counter--;                  
        }
    }
    return *this;
  }
  ~CRegister () {
    if (this->list_person->ref_counter <= 1) delete this->list_person;
    else this->list_person->ref_counter -=1;
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
    if (id == nullptr || name == nullptr || surname == nullptr || date == nullptr || street == nullptr || city == nullptr) return false;
    Person guy(name, surname, id, date, street, city);
    for (int i = 0; i < this->list_person->index; i++) {
      if (id_compare(this->list_person->data[i].id, guy.id)) return false;
    }
    if (this->list_person->ref_counter > 1) make_deep_copy();
    this->list_person->append(guy);
    return true;
  }

    bool resettle  ( const char     id[],
                     const char     date[],
                     const char     street[],
                     const char     city[] ) {
    if (id == nullptr || date == nullptr || street == nullptr || city == nullptr) return false;
    for (int i = 0; i < this->list_person->index; i++) {
      if (id_compare((this->list_person->data[i]).id, id)) {
        if (this->list_person->ref_counter > 1) make_deep_copy();
        Adress new_address(date,street, city);
        return this->list_person->data[i].append(new_address);
      }
    }
    return false;
    }

    bool print     ( std::ostream & os,
                     const char     id[] ) const {
      if (id == nullptr) return false;
      for (int i = 0; i < this->list_person->index; i++) {
        if (!strcmp(this->list_person->data[i].id, id)) {
          this->list_person->data[i].print_person(os);
          return true;
        }
      }
      return false;
    }
    void make_deep_copy () {
      if (this->list_person->ref_counter > 1) {
        Data_Person * new_list = new Data_Person((*this->list_person));
        this->list_person->ref_counter -=1;
        this->list_person = new_list;
      }
      return;
    }
};


#ifndef __PROGTEST__

int main ()
{
  char   lID[12], lDate[12], lName[50], lSurname[50], lStreet[50], lCity[50];
  std::ostringstream oss;
  CRegister  a;
  std::cout << "zijem" <<std::endl;
  assert ( a . add ( "123456/7890", "John", "Smith", "2000-01-01", "Main street", "Seattle" ) == true );
  assert ( a . add ( "987654/3210", "Freddy", "Kruger", "2001-02-03", "Elm street", "Sacramento" ) == true );
  assert ( a . resettle ( "123456/7890", "2003-05-12", "Elm street", "Atlanta" ) == true );
  assert ( a . resettle ( "123456/7890", "2002-12-05", "Sunset boulevard", "Los Angeles" ) == true );
  a.list_person->data[0].print_person();
  
  oss . str ( "" );
  assert ( a . print ( oss, "123456/7890" ) == true );
  std::cout << oss.str().c_str() ;
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
