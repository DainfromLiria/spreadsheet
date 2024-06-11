#pragma once
#include <iostream>
#include <string>
#include <algorithm>

using Pair_Offset = std::pair<int, int>; // first is column, second is row

// Class convert input alphanumeric string_view into two ints
class CPos
{
  public:
    // num constructor
    CPos(const int col = 0, const int row = 0);

    // convert alphabetic col representation to number
    void convert_col(const std::string & str);
    void convert_row(const std::string & str);
    // string constructor
    CPos( std::string_view str );

    // getters
    const int get_col() const;
    const int get_row() const;
    // convert number column representation to alphabetic
    std::string get_alpha_col() const;

    // operators
    // print pos in format colrow, where col is alphabetic in uppercase. Exemple: A1, BF12
    friend std::ostream & operator << (std::ostream & os, const CPos & pos);
    // used for storing data in map (primary sorted by col, secondary by row)
    friend bool operator < (const CPos & l_pos,  const CPos & r_pos);
    // return offset that must be added to src pos to get dst pos
    friend Pair_Offset operator - (const CPos & l, const CPos & r);

  private:
    int m_col = 0;
    int m_row = 0;
};
