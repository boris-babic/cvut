#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <compare>
#include <functional>
#include <stdexcept>
#include <algorithm>
#endif /* __PROGTEST__ */

class CIterator
{
  public:
    bool                atEnd   () const;
    void                next    ();
    const std::string & name    () const;
    const std::string & addr    () const;
    const std::string & account () const;
  private:
    // todo
};

class Person {
    private:
         std::string name;
         std::string addr;
         std::string account;
         int income;
         int expense;

    public:
        Person( std::string n,
                std::string ad,
                std::string ac): name(n), addr(ad), account(ac), income(0), expense(0) {}
        void print(){
          std::cout << "Osoba: " << this->name <<
                       " byva na " << this->addr <<
                       " s cislom uctu " << this->account <<
                       " a na ucte ma prijem " << this->income <<
                       " a na ucte ma odjem " << this->expense << std::endl;
        }
        bool operator == ( Person & other)  {
          return ((this->get_name() == other.get_name() &&
                  this->get_address() == other.get_address()) ||
                  (this->get_account() == other.get_account()));
        }
        std::string get_name() const {
          return this->name;
        }
        std::string get_account() const {
          return this->account;
        }
        std:: string get_address() const { 
          return this->addr;
        }
        int get_income() const{
          return this->income;
        }
        int get_expense() const{
          return this->expense;
        }
        void change_income(int amount) {
          this->income += amount;
          return;
        }
        void change_expense(int amount) {
          this->expense += amount;
          return;
        }
        void change_name_addr(std::string n, std::string add) {
          this->name = n;
          this->addr = add;
          return;
        }
        void change_account (std::string acc) {
          this->account = acc;
          return;
        }
    
};
bool compare_name(const Person & a, const Person & b) {
  if  (a.get_name() == b.get_name()) {
    return a.get_address() < b.get_address();
  } else {
    return a.get_name() < b.get_name();
  }
}
bool compare_account(const Person & a, const Person & b) {
  return a.get_account() < b.get_account();
}
class CTaxRegister
{
  private:
    std::vector<Person> data_by_name;
    std::vector<Person> data_by_number;
    std::vector<Person>::iterator get_iterator_name(Person guy) {
      return std::lower_bound(data_by_name.begin(), data_by_name.end(), guy, compare_name);
    }
    std::vector<Person>::iterator get_iterator_account(Person guy) {
      return std::lower_bound(data_by_number.begin(), data_by_number.end(), guy, compare_account);
    }
    bool person_in_vector_by_name(Person guy) {
      return std::binary_search(data_by_name.begin(), data_by_name.end(), guy, compare_name);
    }
    bool person_in_vector_by_account(Person guy) {
      return std::binary_search(data_by_number.begin(), data_by_number.end(), guy, compare_account);
    }
    bool person_in_data(Person guy) {
      return (person_in_vector_by_name(guy) || person_in_vector_by_account(guy));
    }
  public:
    CTaxRegister() {}
    bool      birth      ( const std::string    & name,
                           const std::string    & addr,
                           const std::string    & account ) {
    Person newborn(name, addr, account);

    //gets the position where to insert newborn in data_by_name
    auto iterator_name = get_iterator_name(newborn);
    auto iterator_account = get_iterator_account(newborn);
    //checks if the person is already there
    if (!person_in_data(newborn)) {
      data_by_name.insert(iterator_name, newborn);
      data_by_number.insert(iterator_account, newborn);
      return true;
    } else {
      return false;
    }
    }

    bool      death      ( const std::string    & name,
                           const std::string    & addr ) {
      Person deadguy (name, addr, "null");
      auto iterator_name = std::lower_bound(data_by_name.begin(),
                                     data_by_name.end(),
                                     deadguy,
                                     compare_name);
      //same but in data_by_account
      auto iterator_account = std::lower_bound(data_by_number.begin(),
                                              data_by_number.end(),
                                              deadguy,
                                              compare_account);
    if ((iterator_name == data_by_name.end())||  
        ((*iterator_name).get_name() != deadguy.get_name() ||
        (*iterator_name).get_address() != deadguy.get_address())) {
      return false;
    } else if (*iterator_name == deadguy) {
      data_by_name.erase(iterator_name);
      data_by_number.erase(iterator_account);
      return true;
    } else {
      return false;
    }
    }
    bool      income     ( const std::string    & account,
                           int                    amount ) {
      Person guy ("Jozko Mrkvicka", "nowhere", account);
      auto iterator_account = std::lower_bound( data_by_number.begin(),
                                                data_by_number.end(),
                                                guy,
                                                compare_account);
      if ((iterator_account == data_by_number.end()) ||
          ((*iterator_account).get_account() != guy.get_account())) {
        return false;
      } else {
        guy.change_account((*iterator_account).get_account());
        guy.change_name_addr((*iterator_account).get_name(), (*iterator_account).get_address());
        auto iterator_name = std::lower_bound(data_by_name.begin(),
                                     data_by_name.end(),
                                     guy,
                                     compare_name);
        (*iterator_account).change_income(amount);
        (*iterator_name).change_income(amount);
        return true;
      }
    }
    
    bool      income     ( const std::string    & name,
                           const std::string    & addr,
                           int                    amount ) {
      Person guy (name, addr, "null");
      
      auto iterator_name = std::lower_bound(data_by_name.begin(),
                                                data_by_name.end(),
                                                guy,
                                                compare_name);
      if ((iterator_name == data_by_number.end()) || 
          ((*iterator_name).get_name() != guy.get_name() ||
           (*iterator_name).get_address() != guy.get_address())) {
        return false;
      } else {
        guy.change_account((*iterator_name).get_account());
        guy.change_name_addr((*iterator_name).get_name(), (*iterator_name).get_address());
        auto iterator_account = std::lower_bound( data_by_number.begin(),
                                                data_by_number.end(),
                                                guy,
                                                compare_account);
        (*iterator_account).change_income(amount);
        (*iterator_name).change_income(amount);
        return true;
      }
    }
    bool      expense    ( const std::string    & account,
                           int                    amount ){
      Person guy ("Jozko Mrkvicka", "nowhere", account);
      auto iterator_account = std::lower_bound( data_by_number.begin(),
                                                data_by_number.end(),
                                                guy,
                                                compare_account);
      if ((iterator_account == data_by_number.end()) ||
          ((*iterator_account).get_account() != guy.get_account())) {
        return false;
      } else {
        guy.change_account((*iterator_account).get_account());
        guy.change_name_addr((*iterator_account).get_name(), (*iterator_account).get_address());
        auto iterator_name = std::lower_bound(data_by_name.begin(),
                                      data_by_name.end(),
                                      guy,
                                      compare_name);
        (*iterator_account).change_expense(amount);
        (*iterator_name).change_expense(amount);
        return true;
      }
    }
    bool      expense    ( const std::string    & name,
                           const std::string    & addr,
                           int                    amount ) {
      Person guy (name, addr, "null");
      
      auto iterator_name = std::lower_bound(data_by_name.begin(),
                                                data_by_name.end(),
                                                guy,
                                                compare_name);
      if ((iterator_name == data_by_number.end()) || 
          ((*iterator_name).get_name() != guy.get_name() ||
           (*iterator_name).get_address() != guy.get_address())) {
        return false;
      } else {
        guy.change_account((*iterator_name).get_account());
        guy.change_name_addr((*iterator_name).get_name(), (*iterator_name).get_address());
        auto iterator_account = std::lower_bound( data_by_number.begin(),
                                                data_by_number.end(),
                                                guy,
                                                compare_account);
        (*iterator_account).change_expense(amount);
        (*iterator_name).change_expense(amount);
        return true;
      }
    }
    bool      audit      ( const std::string    & name,
                           const std::string    & addr,
                           std::string          & account,
                           int                  & sumIncome,
                           int                  & sumExpense ) const {
      Person guy (name, addr, "null");
      auto iterator_name = std::lower_bound(data_by_name.begin(),
                                            data_by_name.end(),
                                            guy,
                                            compare_name);
      if ((iterator_name == data_by_number.end()) || 
          ((*iterator_name).get_name() != guy.get_name() ||
          (*iterator_name).get_address() != guy.get_address())) {
        return false;
      } else {
        sumIncome = (*iterator_name).get_income();
        sumExpense = (*iterator_name).get_expense();
        account = (*iterator_name) .get_account();
        return true;
      }
    }
    void      print_by_name() {
      for (auto person :this->data_by_name) {
        person.print();
      }
    }
    void      print_by_number() {
      for (auto person :this->data_by_number) {
        person.print();
      }
    }
    CIterator listByName () const;
};

#ifndef __PROGTEST__
int main ()
{
  std::string acct;
  int    sumIncome, sumExpense;
  CTaxRegister b0;
  std::cout << "yespls" << std::endl;
  assert ( b0 . birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
  assert ( b0 . birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
  assert ( b0 . birth ( "Peter Hacker", "Main Street 17", "634oddT" ) );
  assert ( b0 . birth ( "John Smith", "Main Street 17", "Z343rwZ" ) );
  assert ( !b0 . birth ( "John Smith", "Main Street 17", "nah4Z" ) );
  assert ( !b0 . birth ( "Joasda Smith", "Main Street 17", "Z343rwZ" ) );

  b0.print_by_name();
  std::cout << "\n\n" << std::endl;
  b0.print_by_number();
  std::cout << "\ntestujeme income" << std::endl;




  

  assert ( b0 . income ( "Xuj5#94", 1000 ) );
  assert ( b0 . income ( "634oddT", 2000 ) );
  assert ( b0 . income ( "123/456/789", 3000 ) );
  assert ( b0 . income ( "634oddT", 4000 ) );
  assert ( b0 . income ( "Peter Hacker", "Main Street 17", 2000 ) );

  b0.print_by_name();
  std::cout << "\n\n" << std::endl;
  b0.print_by_number();

  assert ( b0 . expense ( "Jane Hacker", "Main Street 17", 2000 ) );
  assert ( b0 . expense ( "John Smith", "Main Street 17", 500 ) );
  assert ( b0 . expense ( "Jane Hacker", "Main Street 17", 1000 ) );
  assert ( b0 . expense ( "Xuj5#94", 1300 ) );

  assert ( b0 . audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
  assert ( acct == "123/456/789" );
  assert ( sumIncome == 3000 );
  assert ( sumExpense == 0 );
  assert ( b0 . audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "Xuj5#94" );
  assert ( sumIncome == 1000 );
  assert ( sumExpense == 4300 );
  assert ( b0 . audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "634oddT" );
  assert ( sumIncome == 8000 );
  assert ( sumExpense == 0 );
  assert ( b0 . audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "Z343rwZ" );
  assert ( sumIncome == 0 );
  assert ( sumExpense == 500 );
  /*
  CIterator it = b0 . listByName ();
  assert ( ! it . atEnd ()
           && it . name () == "Jane Hacker"
           && it . addr () == "Main Street 17"
           && it . account () == "Xuj5#94" );
  it . next ();
  assert ( ! it . atEnd ()
           && it . name () == "John Smith"
           && it . addr () == "Main Street 17"
           && it . account () == "Z343rwZ" );
  it . next ();
  assert ( ! it . atEnd ()
           && it . name () == "John Smith"
           && it . addr () == "Oak Road 23"
           && it . account () == "123/456/789" );
  it . next ();
  assert ( ! it . atEnd ()
           && it . name () == "Peter Hacker"
           && it . addr () == "Main Street 17"
           && it . account () == "634oddT" );
  it . next ();
  assert ( it . atEnd () );

  assert ( b0 . death ( "John Smith", "Main Street 17" ) );
  */
  CTaxRegister b1;
  assert ( b1 . birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
  assert ( b1 . birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
  assert ( !b1 . income ( "634oddT", 4000 ) );
  assert ( !b1 . expense ( "John Smith", "Main Street 18", 500 ) );
  assert ( !b1 . audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
  assert ( !b1 . death ( "Peter Nowak", "5-th Avenue" ) );
  assert ( !b1 . birth ( "Jane Hacker", "Main Street 17", "4et689A" ) );
  assert ( !b1 . birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
  assert ( b1 . death ( "Jane Hacker", "Main Street 17" ) );
  assert ( b1 . birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
  assert ( b1 . audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
  assert ( acct == "Xuj5#94" );
  assert ( sumIncome == 0 );
  assert ( sumExpense == 0 );
  assert ( !b1 . birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );
  std::cout << "im done" <<std::endl;
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
