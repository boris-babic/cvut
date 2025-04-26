#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#endif /* __PROGTEST__ */

class CCell;
class CEmpty;
class CTable;
class CText;
class CImage;

class CCell {
 public:
  CCell() = default;
  virtual ~CCell() = default;

  virtual void testeros() {
    std::cout << "zavolal som ccell test" << std::endl;
  }
  virtual std::vector<std::string> print(int external_width, int external_height) = 0;

  int getwidth() const {
    return this->width;
  }
  int getheight() const {
    return this->height;
  }
 protected:
  int width;
  int height;
};








class CText : public CCell {
 private:
  std::vector<std::string> text;
  
  void setsize() {
    size_t max = 0;
    this->height = this->text.size();
    for (auto i: this->text) {
      if (i.length() > max) {
        max = i.length();
      }
    }
    this->width = max;
    return;
  }

 public:
  enum align { ALIGN_LEFT = 0, ALIGN_RIGHT = 1 };
  align alignment;
  CText(const std::string& text, align al) {
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line, '\n')) {
      this->text.push_back(line);
    }
    this->alignment = al;
    setsize();
    return;
  }
  void setText(const std::string& text) {
    this->text.clear();
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line, '\n')) {
      this->text.push_back(line);
    }
    setsize();
    return;
  }
  void testeros() override {
    std::cout << "zavolal som ctext test" << std::endl;
  }
  virtual std::vector<std::string> print(int external_width, int external_height) override {
    std::vector<std::string> result;
    if (this->alignment == ALIGN_RIGHT) {
      for (auto line : this->text) {
        //os << std::setfill(' ') << std::setw(width) << std::right << line << std::endl;
        std::string padding_left(external_width - line.length(), ' ');
        result.push_back( padding_left + line);
      }
    } else {
      for (auto line : this->text) {
        //os << std::setfill(' ') << std::setw(width) << std::left << line << std::endl;
        std::string padding_right(external_width - line.length(), ' ');
        result.push_back( line + padding_right);
      }
    }
    for (int i = this->text.size(); i < external_height; i++) {
      //os << std::setfill(' ') << std::setw(width) << "*" << std::endl;
      result.push_back(std::string(external_width, ' '));
    }
  return result;
  }
};















class CEmpty : public CCell {
 public:
  CEmpty() {
    this->width = 0;
    this->height = 0;
  }
  void testeros() override {
    std::cout << "zavolal som cempty test" << std::endl;
  }
  virtual std::vector<std::string> print( int external_width, int external_height) override {
    std::vector<std::string> result;
    for (int i = 0; i < external_height; i++) {
      result.push_back(std::string(external_width, ' '));
    }
    return result;
  }
};












class CImage : public CCell {
 public:
  CImage& addRow(const std::string& line) {
    this->image.push_back(line);
    setsize();
    return *this;
  }
  virtual std::vector<std::string> print( int external_width, int external_height) override {
    std::vector<std::string> result;
    int padding_height_up= (external_height - this->height)/2;
    int padding_height_down = external_height - this->height - padding_height_up;

    int padding_width_left = (external_width - this->width)/2;
    int padding_width_right = external_width - this->width - padding_width_left;
    for (int i = 0; i < padding_height_up; i++) {
      //os << std::setfill(' ') << std::setw(external_width)<< "" << std::endl;
      result.push_back(std::string(external_width, ' '));
    }
    for (auto row : image) {
      //os << std::string(padding_width_left, ' ') << row << std::string(padding_width_right, ' ') << std::endl;
      result.push_back(std::string(padding_width_left, ' ') + row + std::string(padding_width_right, ' '));
    }
    for (int i = 0; i < padding_height_down; i++) {
      //os << std::setfill(' ') << std::setw(external_width)<< "" << std::endl;
      result.push_back(std::string(external_width, ' '));
    }
    return result;
  }

 private:
  std::vector<std::string> image;
  void setsize() {
    this->height = this->image.size();
    this->width = this->image[0].length();
    return;
  }
};
















class CTable : public CCell {
 public:
  CTable(int r, int c) {
    this->rows = r;
    this->columns = c;
    for (int i = 0; i < rows; i++) {
      std::vector<std::unique_ptr<CCell>> current_line{};

      for (int j = 0; j < columns; j++) {
        current_line.push_back(std::make_unique<CEmpty>());
      }
      this->table.push_back(std::move(current_line));
    }
    return;
  }

  CCell& getCell(int row, int column) { return *(this->table[row][column]); }

  template <typename celltype>
  void setCell(int row, int column, const celltype& newcontent) {
    this->table[row][column].reset();
    this->table[row][column] = std::move(std::make_unique<celltype>(newcontent));
    return;
  }
  virtual std::vector<std::string> print(int external_width, int external_height) override {
    std::vector<std::string> result{};
    return result;
  }
  /*
bool operator ==
bool operator!=(const CTable& other) const {}
*/
friend std::ostream& operator<<(std::ostream& os, const CTable& table) {
  std::vector<std::string> result = table.get_resulting_table();
  for (auto row: result) {
    os << row << std::endl;
  }
  return os;
}
std::vector<std::string> get_resulting_table() const {
  std::vector<std::string> result;

  std::vector<int> max_column_width;
  max_column_width.resize(this->columns, 0);
  std::vector<int> max_row_height;
  max_row_height.resize(this->rows, 0);

  for (int row = 0; row < this->rows; row++) {
    for (int column = 0; column < this->columns; column++) {
      if ((*this->table[row][column].get()).getheight() > max_row_height[row]) {
        max_row_height[row] = this->table[row][column].get()->getheight();
      }
      if((*this->table[row][column].get()).getwidth() > max_column_width[column]) {
        max_column_width[column] = this->table[row][column].get()->getwidth();
      }
    }
  }
  int total_rows = 0;
  for (auto i : max_row_height) total_rows += i;
  total_rows += rows + 1;
  result.resize(total_rows);
  std::string line_divider {'+'};
  for (int i = 0; i < this->columns; i++) {
    line_divider += std::string(max_column_width[i], '-');
    line_divider += '+';
  }
  
  int cell_index = 0;
  result[cell_index++] = line_divider;
  for (int row = 0; row < this->rows; row++) {
    for (int column = 0; column < this->columns; column++) {
      std::vector<std::string> current_cell = this->table[row][column].get()->print(max_column_width[column], max_row_height[row]);
      for (int current_line_index = 0; current_line_index < max_row_height[row]; current_line_index++ ) {
        if (column == 0) result[cell_index + current_line_index] += '|';
        result[cell_index + current_line_index] += current_cell[current_line_index];
        result[cell_index + current_line_index] += '|';
      }
    }
    cell_index += max_row_height[row];
    result[cell_index++] = line_divider;
  }
  return result;
}

 private:
  std::vector<std::vector<std::unique_ptr<CCell>>> table;

  int rows;
  int columns;
};
















#ifndef __PROGTEST__
int main() {
  std::ostringstream oss;
  CTable t0 ( 3, 2 );
  t0 . setCell ( 0, 0, CText ( "Hello,\n"
  "Hello Kitty", CText::ALIGN_LEFT ) );
  t0 . setCell ( 1, 0, CText ( "Lorem ipsum dolor sit amet", CText::ALIGN_LEFT ) );
  t0 . setCell ( 2, 0, CText ( "Bye,\n"
  "Hello Kitty", CText::ALIGN_RIGHT ) );
  t0 . setCell ( 1, 1, CImage ()
        . addRow ( "###                   " )
        . addRow ( "#  #                  " )
        . addRow ( "#  # # ##   ###    ###" )
        . addRow ( "###  ##    #   #  #  #" )
        . addRow ( "#    #     #   #  #  #" )
        . addRow ( "#    #     #   #  #  #" )
        . addRow ( "#    #      ###    ###" )
        . addRow ( "                     #" )
        . addRow ( "                   ## " )
        . addRow ( "                      " )
        . addRow ( " #    ###   ###   #   " )
        . addRow ( "###  #   # #     ###  " )
        . addRow ( " #   #####  ###   #   " )
        . addRow ( " #   #         #  #   " )
        . addRow ( "  ##  ###   ###    ## " ) );
    std::vector<std::string> bruh = t0.get_resulting_table();
    for (auto row : bruh) {
      std::cout << row << std::endl;
    }
    
      /*
        t0 . setCell ( 2, 1, CEmpty () );
        oss . str ("");
        oss . clear ();
        oss << t0;
        assert ( oss . str () ==
        "+--------------------------+----------------------+\n"
        "|Hello,                    |                      |\n"
        "|Hello Kitty               |                      |\n"
        "+--------------------------+----------------------+\n"
        "|Lorem ipsum dolor sit amet|###                   |\n"
        "|                          |#  #                  |\n"
        "|                          |#  # # ##   ###    ###|\n"
        "|                          |###  ##    #   #  #  #|\n"
        "|                          |#    #     #   #  #  #|\n"
        "|                          |#    #     #   #  #  #|\n"
        "|                          |#    #      ###    ###|\n"
        "|                          |                     #|\n"
        "|                          |                   ## |\n"
        "|                          |                      |\n"
        "|                          | #    ###   ###   #   |\n"
        "|                          |###  #   # #     ###  |\n"
        "|                          | #   #####  ###   #   |\n"
        "|                          | #   #         #  #   |\n"
        "|                          |  ##  ###   ###    ## |\n"
        "+--------------------------+----------------------+\n"
        "|                      Bye,|                      |\n"
        "|               Hello Kitty|                      |\n"
        "+--------------------------+----------------------+\n" );
        t0 . setCell ( 0, 1, t0 . getCell ( 1, 1 ) );
        t0 . setCell ( 2, 1, CImage ()
        . addRow ( "*****   *      *  *      ******* ******  *" )
        . addRow ( "*    *  *      *  *      *            *  *" )
        . addRow ( "*    *  *      *  *      *           *   *" )
        . addRow ( "*    *  *      *  *      *****      *    *" )
        . addRow ( "****    *      *  *      *         *     *" )
        . addRow ( "*  *    *      *  *      *        *       " )
        . addRow ( "*   *   *      *  *      *       *       *" )
        . addRow ( "*    *    *****   ****** ******* ******  *" ) );
        dynamic_cast<CText &> ( t0 . getCell ( 1, 0 ) ) . setText ( "Lorem ipsum dolor sit amet,\n"
        "consectetur adipiscing\n"
        "elit. Curabitur scelerisque\n"
        "lorem vitae lectus cursus,\n"
        "vitae porta ante placerat. Class aptent taciti\n"
        "sociosqu ad litora\n"
        "torquent per\n"
        "conubia nostra,\n"
        "per inceptos himenaeos.\n"
        "\n"
        "Donec tincidunt augue\n"
        "sit amet metus\n"
        "pretium volutpat.\n"
        "Donec faucibus,\n"
        "ante sit amet\n"
        "luctus posuere,\n"
        "mauris tellus" );
        oss . str ("");
        oss . clear ();
        oss << t0;
        assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|Hello,                                        |          ###                             |\n"
        "|Hello Kitty                                   |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |          ###                             |\n"
        "|elit. Curabitur scelerisque                   |          #  #                            |\n"
        "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
        "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
        "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
        "|torquent per                                  |          #    #     #   #  #  #          |\n"
        "|conubia nostra,                               |          #    #      ###    ###          |\n"
        "|per inceptos himenaeos.                       |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|Donec tincidunt augue                         |                                          |\n"
        "|sit amet metus                                |           #    ###   ###   #             |\n"
        "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
        "|Donec faucibus,                               |           #   #####  ###   #             |\n"
        "|ante sit amet                                 |           #   #         #  #             |\n"
        "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );
        CTable t1 ( t0 );
        t1 . setCell ( 1, 0, CEmpty () );
        t1 . setCell ( 1, 1, CEmpty () );
        oss . str ("");
        oss . clear ();
        oss << t0;
        assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|Hello,                                        |          ###                             |\n"
        "|Hello Kitty                                   |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |          ###                             |\n"
        "|elit. Curabitur scelerisque                   |          #  #                            |\n"
        "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
        "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
        "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
        "|torquent per                                  |          #    #     #   #  #  #          |\n"
        "|conubia nostra,                               |          #    #      ###    ###          |\n"
        "|per inceptos himenaeos.                       |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|Donec tincidunt augue                         |                                          |\n"
        "|sit amet metus                                |           #    ###   ###   #             |\n"
        "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
        "|Donec faucibus,                               |           #   #####  ###   #             |\n"
        "|ante sit amet                                 |           #   #         #  #             |\n"
        "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );
        oss . str ("");
        oss . clear ();
        oss << t1;
        assert ( oss . str () ==
        "+-----------+------------------------------------------+\n"
        "|Hello,     |          ###                             |\n"
        "|Hello Kitty|          #  #                            |\n"
        "|           |          #  # # ##   ###    ###          |\n"
        "|           |          ###  ##    #   #  #  #          |\n"
        "|           |          #    #     #   #  #  #          |\n"
        "|           |          #    #     #   #  #  #          |\n"
        "|           |          #    #      ###    ###          |\n"
        "|           |                               #          |\n"
        "|           |                             ##           |\n"
        "|           |                                          |\n"
        "|           |           #    ###   ###   #             |\n"
        "|           |          ###  #   # #     ###            |\n"
        "|           |           #   #####  ###   #             |\n"
        "|           |           #   #         #  #             |\n"
        "|           |            ##  ###   ###    ##           |\n"
        "+-----------+------------------------------------------+\n"
        "+-----------+------------------------------------------+\n"
        "|       Bye,|*****   *      *  *      ******* ******  *|\n"
        "|Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|           |*    *  *      *  *      *           *   *|\n"
        "|           |*    *  *      *  *      *****      *    *|\n"
        "|           |****    *      *  *      *         *     *|\n"
        "|           |*  *    *      *  *      *        *       |\n"
        "|           |*   *   *      *  *      *       *       *|\n"
        "|           |*    *    *****   ****** ******* ******  *|\n"
        "+-----------+------------------------------------------+\n" );
        t1 = t0;
        t1 . setCell ( 0, 0, CEmpty () );
        t1 . setCell ( 1, 1, CImage ()
        . addRow ( "  ********                    " )
        . addRow ( " **********                   " )
        . addRow ( "**        **                  " )
        . addRow ( "**             **        **   " )
        . addRow ( "**             **        **   " )
        . addRow ( "***         ********  ********" )
        . addRow ( "****        ********  ********" )
        . addRow ( "****           **        **   " )
        . addRow ( "****           **        **   " )
        . addRow ( "****      **                  " )
        . addRow ( " **********                   " )
        . addRow ( "  ********                    " ) );
        oss . str ("");
        oss . clear ();
        oss << t0;
        assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|Hello,                                        |          ###                             |\n"
        "|Hello Kitty                                   |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |          ###                             |\n"
        "|elit. Curabitur scelerisque                   |          #  #                            |\n"
        "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
        "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
        "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
        "|torquent per                                  |          #    #     #   #  #  #          |\n"
        "|conubia nostra,                               |          #    #      ###    ###          |\n"
        "|per inceptos himenaeos.                       |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|Donec tincidunt augue                         |                                          |\n"
        "|sit amet metus                                |           #    ###   ###   #             |\n"
        "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
        "|Donec faucibus,                               |           #   #####  ###   #             |\n"
        "|ante sit amet                                 |           #   #         #  #             |\n"
        "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );
        oss . str ("");
        oss . clear ();
        oss << t1;
        assert ( oss . str () ==
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                              |          ###                             |\n"
        "|                                              |          #  #                            |\n"
        "|                                              |          #  # # ##   ###    ###          |\n"
        "|                                              |          ###  ##    #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #     #   #  #  #          |\n"
        "|                                              |          #    #      ###    ###          |\n"
        "|                                              |                               #          |\n"
        "|                                              |                             ##           |\n"
        "|                                              |                                          |\n"
        "|                                              |           #    ###   ###   #             |\n"
        "|                                              |          ###  #   # #     ###            |\n"
        "|                                              |           #   #####  ###   #             |\n"
        "|                                              |           #   #         #  #             |\n"
        "|                                              |            ##  ###   ###    ##           |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|Lorem ipsum dolor sit amet,                   |                                          |\n"
        "|consectetur adipiscing                        |                                          |\n"
        "|elit. Curabitur scelerisque                   |        ********                          |\n"
        "|lorem vitae lectus cursus,                    |       **********                         |\n"
        "|vitae porta ante placerat. Class aptent taciti|      **        **                        |\n"
        "|sociosqu ad litora                            |      **             **        **         |\n"
        "|torquent per                                  |      **             **        **         |\n"
        "|conubia nostra,                               |      ***         ********  ********      |\n"
        "|per inceptos himenaeos.                       |      ****        ********  ********      |\n"
        "|                                              |      ****           **        **         |\n"
        "|Donec tincidunt augue                         |      ****           **        **         |\n"
        "|sit amet metus                                |      ****      **                        |\n"
        "|pretium volutpat.                             |       **********                         |\n"
        "|Donec faucibus,                               |        ********                          |\n"
        "|ante sit amet                                 |                                          |\n"
        "|luctus posuere,                               |                                          |\n"
        "|mauris tellus                                 |                                          |\n"
        "+----------------------------------------------+------------------------------------------+\n"
        "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
        "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
        "|                                              |*    *  *      *  *      *           *   *|\n"
        "|                                              |*    *  *      *  *      *****      *    *|\n"
        "|                                              |****    *      *  *      *         *     *|\n"
        "|                                              |*  *    *      *  *      *        *       |\n"
        "|                                              |*   *   *      *  *      *       *       *|\n"
        "|                                              |*    *    *****   ****** ******* ******  *|\n"
        "+----------------------------------------------+------------------------------------------+\n" );
        assert ( t0 != t1 );
        assert ( !( t0 == t1 ) );
        assert ( t0 . getCell ( 1, 1 ) == t0 . getCell ( 0, 1 ) );
        assert ( ! ( t0 . getCell ( 1, 1 ) != t0 . getCell ( 0, 1 ) ) );
        assert ( t0 . getCell ( 0, 0 ) != t0 . getCell ( 0, 1 ) );
        assert ( ! ( t0 . getCell ( 0, 0 ) == t0 . getCell ( 0, 1 ) ) );
        
        return EXIT_SUCCESS;
    */
 }
#endif /* __PROGTEST__ */
