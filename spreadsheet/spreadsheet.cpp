#include "spreadsheet.h"

//==========================================================================================================
unsigned  CSpreadsheet::capabilities() { return SPREADSHEET_CYCLIC_DEPS; }
//==========================================================================================================
int CSpreadsheet::read_count(std::istream & is)
{
    char s;
    std::string str_sum;
    int c_sum = 0;
    // read check_sum
    while(is >> std::noskipws >> s)
    {
        if(isdigit(s)) str_sum += s;
        else if(s == '\n') break;
        else throw std::invalid_argument("Corrupted file");
    }
    sscanf(str_sum.c_str(), "%d", &c_sum);
    return c_sum;
}
//==========================================================================================================
CPos CSpreadsheet::read_pos( std::istream & is)
{
    char s;
    std::string str_sum;
    int c_sum = 0;
    // read check_sum
    while(is >> std::noskipws >> s)
    {
        if(isdigit(s)) str_sum += s;
        else if(s == '"') break;
        else throw std::invalid_argument("Corrupted file");
    }
    sscanf(str_sum.c_str(), "%d", &c_sum);
    c_sum--; // already read first "
    // read pos
    std::string s_pos;
    while(is >> std::noskipws >> s)
    {
        if(isalnum(s)) 
        {
            s_pos += s;
            c_sum--;
        }
        else if(s == '"') 
        {
            c_sum--;
            break;
        }
        else throw std::invalid_argument("Corrupted file");
    }
    if(c_sum != 0) throw std::invalid_argument("Corrupted file");
    // try to create CPos object
    return CPos(s_pos);
}
//==========================================================================================================
std::string CSpreadsheet::read_data(std::istream & is)
{
    char s;
    std::string str_sum;
    int c_sum = 0;
    // read check_sum
    while(is >> std::noskipws >> s)
    {
        if(isdigit(s)) str_sum += s;
        else if(s == '"') break;
        else throw std::invalid_argument("Corrupted file");
    }
    sscanf(str_sum.c_str(), "%d", &c_sum);
    c_sum--; // already read first "
    // read data
    std::string data;
    while(is >> std::noskipws >> s)
    {
        if(s == '"' && c_sum == 1) 
        {
            c_sum--;
            break;
        }
        else
        {
            data += s;
            c_sum--;
        }
    }
    if(c_sum != 0) throw std::invalid_argument("Corrupted file");
    return data;
}
//==========================================================================================================
bool CSpreadsheet::load( std::istream & is ) 
{
    if(!is.good()) return false;
    // create tmp table. If data was corrupted => copy data back to m_table
    ETable tmp = m_table;
    m_table.clear();
    // read cells count
    int cnt = 0;
    try { cnt = read_count(is); }
    catch(const std::invalid_argument & e) { m_table = tmp; return false; }
    

    for(int i = 0; i < cnt; i++)
    {
        // read pos
        CPos new_pos;
        try { new_pos = read_pos(is); }
        catch(const std::invalid_argument & e) { m_table = tmp; return false; }
    
        // read delimiter
        char s;
        s = is.get();
        if(s != ',') 
        {
            m_table = tmp; 
            return false;
        }

        // read data
        std::string data;
        try 
        { 
            data = read_data(is); 
            setCell(new_pos, data);
        }
        catch(const std::invalid_argument & e) { m_table = tmp; return false; }
    }

    // check if file has data after reading
    char has_data;
    while(is >> std::noskipws >> has_data)
    {
        if(!is.eof()) 
        {
            m_table = tmp;
            return false;
        }
    }
    
    return true;
}
//==========================================================================================================
bool CSpreadsheet::save( std::ostream & os ) const 
{
    int c_cnt = 0; // count of cells 
    std::ostringstream tmp_os;
    for(const auto & [key, val]: m_table)
    {
        // calculating control sum for pos
        std::ostringstream f_oss;
        f_oss << "\"" << key  << "\"";
        std::string f_part = f_oss.str();

        // calculating control sum for val
        std::ostringstream s_oss;
        s_oss << "\"" << val << "\"";
        std::string s_part = s_oss.str();

        // send data to output stream (dont send cells with undef values)
        if(s_part.size() > 2) 
        {
            tmp_os << f_part.size() << f_part << "," << s_part.size() << s_part;
            c_cnt++;
        }
    }
    os << c_cnt << "\n" << tmp_os.str();
    return true;
}
//==========================================================================================================
// src - from left top, dst - to left top
void CSpreadsheet::copyRect( CPos dst, CPos src, int w, int h) 
{
    if(w < 0 || h < 0) return; // size must be positive
    
    ETable tmp;
    // col,row of pos from
    int b_col = src.get_col();
    int b_row = src.get_row();
    // col, row of pos to
    int e_col = dst.get_col();
    int e_row = dst.get_row();
    
    // calculate offset
    Pair_Offset ofs(dst - src);
    
    // make copy of src rect
    for(int i = b_col; i < b_col+w; i++)
    {
        for(int j = b_row; j < b_row+h; j++)
        {
            tmp[CPos(i, j)] = m_table[CPos(i, j)];
        }
    }

    // copy cells from old rect  to new rect
    for(int ei = e_col, bi = b_col; ei < e_col+w; ei++, bi++)
    {
        for(int ej = e_row, bj = b_row; ej < e_row+h; ej++, bj++)
        {
            tmp[CPos(bi, bj)].update_all_pos(ofs); // update refs
            m_table[CPos(ei, ej)] = tmp[CPos(bi, bj)];
        }
    }
}
//==========================================================================================================
bool CSpreadsheet::setCell( CPos pos, std::string  contents ) 
{
    try
    {
        CExpr tmp(contents);
        m_table[pos] = tmp;
    }
    catch(const std::invalid_argument & e) { return false; }
    return true;
}
//==========================================================================================================
CValue CSpreadsheet::getValue( CPos pos ) 
{ 
    if(m_table.contains(pos)) 
    {
        if(m_visit_table.contains(pos)) 
        {
            if(m_visit_table[pos] >= 2) return CValue();
        }
        return m_table[pos].eval_seq(*this);
    }
    return CValue();
}
//==========================================================================================================
const void CSpreadsheet::print_table() const
{
    for(const auto & [key, val]: m_table)
    {
        std::cout << "\"" << key  << "\",\"" << val << "\"" << std::endl;
    }
}
//==========================================================================================================
void CSpreadsheet::set_visited(CPos & pos) { m_visit_table[pos] += 1; }
//==========================================================================================================
void CSpreadsheet::unset_visited(CPos & pos) { m_visit_table[pos] -= 1; }
//==========================================================================================================