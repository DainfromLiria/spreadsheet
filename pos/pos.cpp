#include "pos.h"

//==========================================================================================================
CPos::CPos(const int col, const int row): m_col(col), m_row(row) {}
//==========================================================================================================
void CPos::convert_col(const std::string & str)
{
    for(const char & c: str) m_col = m_col * 26 + (c - 'A' + 1);
    m_col--;
}
//==========================================================================================================
void CPos::convert_row(const std::string & str)
{
    if(str.size() == 0 || (str.size() > 1 && str[0] == '0')) 
        throw std::invalid_argument("Input position is not valid.");
    sscanf(str.c_str(), "%d", &m_row);
}
//==========================================================================================================
CPos::CPos( std::string_view str )
{
    std::string s_col;
    std::string s_row;
    bool after_alpha = false;
    for(size_t i = 0; i < str.size(); i++) 
    {
        if(isalpha(str[i]) && after_alpha == false) s_col += toupper(str[i]);
        else if (isdigit(str[i]) && i > 0) 
        {
          after_alpha = true;
          s_row += str[i];
        }
        else throw std::invalid_argument("Input position is not valid.");
    }
    convert_row(s_row);
    convert_col(s_col);
}
//==========================================================================================================
const int CPos::get_col() const { return m_col; }
//==========================================================================================================
const int CPos::get_row() const { return m_row; }
//==========================================================================================================
std::ostream & operator << (std::ostream & os, const CPos & pos)
{
    os << pos.get_alpha_col() << pos.get_row();
    return os;
}
//==========================================================================================================
bool operator < (const CPos & l_pos,  const CPos & r_pos)
{
    return std::tie(l_pos.m_col, l_pos.m_row) < std::tie(r_pos.m_col, r_pos.m_row);
}
//==========================================================================================================
Pair_Offset operator - (const CPos & l, const CPos & r)
{
    int col_d = l.m_col - r.m_col;
    int row_d = l.m_row - r.m_row;
    return std::make_pair(col_d, row_d);
}
//==========================================================================================================
std::string CPos::get_alpha_col() const
{
    std::string res;
    int tmp = m_col;

    while(tmp >= 0) 
    {
        res = char('A' + tmp % 26) + res;
        tmp = (tmp / 26) - 1;
    }
    return res;
}
//==========================================================================================================