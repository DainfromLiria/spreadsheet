#pragma once
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "../expression.h"
#include "../pos/pos.h"
#include "../expr/expr.h"
using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;


constexpr unsigned                     SPREADSHEET_CYCLIC_DEPS                 = 0x01;
constexpr unsigned                     SPREADSHEET_FUNCTIONS                   = 0x02;
constexpr unsigned                     SPREADSHEET_FILE_IO                     = 0x04;
constexpr unsigned                     SPREADSHEET_SPEED                       = 0x08;
constexpr unsigned                     SPREADSHEET_PARSER                      = 0x10;

class CExpr;

using ETable = std::map<CPos, CExpr>;
using Visit_Table = std::map<CPos, int>;


class CSpreadsheet
{
  public:
    // PROGTEST methods
    static unsigned  capabilities();
    CSpreadsheet() = default;
    bool load( std::istream & is );
    bool save( std::ostream & os ) const;
    bool setCell( CPos pos, std::string  contents );
    CValue getValue( CPos pos );
    void copyRect( CPos dst, CPos src, int w = 1, int h = 1 );
    
    // helper method to print table
    const void print_table() const; 

    // helper methods for load
    // read count of rows from stream
    int read_count(std::istream & is);
    // read count of chars in pos from stream
    CPos read_pos(std::istream & is);
    // read count of chars in expression from stream
    std::string read_data(std::istream & is);
    
    // helper methods for cycle dependency detection
    // increase visite counter for input pos
    void set_visited(CPos & pos);
    // decrease visite counter for input pos
    void unset_visited(CPos & pos);
  private:
    ETable m_table; // main table
    Visit_Table m_visit_table; // helper table for cycle dependency detection
};