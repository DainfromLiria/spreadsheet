#include "literals.h"

//==========================================================================================================
// CLiteral
//==========================================================================================================
std::ostream & operator << (std::ostream & os, const CLiteral & lit)
{
    lit.print(os);
    return os;
}
//==========================================================================================================


//==========================================================================================================
// CVal
//==========================================================================================================
CVal::CVal(CValue val): m_val(std::move(val)) {}
//----------------------------------------------------------------------------------------------------------
CValue CVal::apply(CSpreadsheet & table) { return m_val; }
//----------------------------------------------------------------------------------------------------------
void CVal::update_ref(const Pair_Offset & ofs) { return; } // update only reference, not value
//----------------------------------------------------------------------------------------------------------
Shared_Literal CVal::clone() const { return std::make_shared<CVal>(*this); }
//----------------------------------------------------------------------------------------------------------
void CVal::print(std::ostream & os) const 
{
    if(std::holds_alternative<std::string>(m_val)) os << std::get<std::string>(m_val);
    else if(std::holds_alternative<double>(m_val)) os << std::get<double>(m_val);
}
//==========================================================================================================


//==========================================================================================================
// CRef
//==========================================================================================================
CRef::CRef(const std::string & str)
{
    std::string tmp;
    for(size_t i = 0; i < str.size(); i++)
    {
        if(str[i] == '$')
        {
            if(i == 0) m_is_col_abs = true;
            else m_is_row_abs = true;
        }
        else tmp += str[i];
    }
    m_pos = CPos(tmp);
}
//----------------------------------------------------------------------------------------------------------
CValue CRef::apply(CSpreadsheet & table) 
{
    table.set_visited(m_pos);
    CValue res = table.getValue(m_pos); 
    table.unset_visited(m_pos);
    return res;
}
//----------------------------------------------------------------------------------------------------------
void CRef::update_ref(const Pair_Offset & ofs) 
{
    int o_col = m_pos.get_col();
    int o_row = m_pos.get_row();
    if(m_is_col_abs == false) o_col += ofs.first;
    if(m_is_row_abs == false) o_row += ofs.second;
    m_pos = CPos(o_col, o_row);
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CRef::clone() const { return std::make_shared<CRef>(*this); }
//----------------------------------------------------------------------------------------------------------
void CRef::print(std::ostream & os) const 
{
    if(m_is_col_abs) os << "$";
    os << m_pos.get_alpha_col();
    if(m_is_row_abs) os << "$";
    os << m_pos.get_row();
}
//==========================================================================================================


//==========================================================================================================
// COpBin
//==========================================================================================================
COpBin::COpBin(const Shared_Literal & l, const Shared_Literal & r): _l(l), _r(r) {}
//----------------------------------------------------------------------------------------------------------
void COpBin::update_ref(const Pair_Offset & ofs) 
{
    _l->update_ref(ofs);
    _r->update_ref(ofs);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CAdd
CAdd::CAdd(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CAdd::apply(CSpreadsheet & table)
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l))
    {
        if(std::holds_alternative<double>(r)) res = std::get<double>(l) + std::get<double>(r);
        else if(std::holds_alternative<std::string>(r)) res = std::to_string(std::get<double>(l)) + std::get<std::string>(r);
    }
    else if(std::holds_alternative<std::string>(l))
    {
        if(std::holds_alternative<double>(r)) res = std::get<std::string>(l) + std::to_string(std::get<double>(r));
        else if(std::holds_alternative<std::string>(r)) res = std::get<std::string>(l) + std::get<std::string>(r);
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CAdd::clone() const { return std::make_shared<CAdd>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CAdd::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "+";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CSub
CSub::CSub(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CSub::apply(CSpreadsheet & table)
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        res = std::get<double>(l) - std::get<double>(r);
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CSub::clone() const { return std::make_shared<CSub>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CSub::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "-";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CMul
CMul::CMul(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CMul::apply(CSpreadsheet & table)
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        res = std::get<double>(l) * std::get<double>(r);
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CMul::clone() const { return std::make_shared<CMul>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CMul::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "*";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CDiv
CDiv::CDiv(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CDiv::apply(CSpreadsheet & table)
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        if(std::get<double>(r) == 0) return res; // return std::monostate
        res = std::get<double>(l) / std::get<double>(r);
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CDiv::clone() const { return std::make_shared<CDiv>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CDiv::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "/";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CPow
CPow::CPow(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CPow::apply(CSpreadsheet & table)
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        res = pow(std::get<double>(l), std::get<double>(r));
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CPow::clone() const { return std::make_shared<CPow>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CPow::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "^";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CEq
CEq::CEq(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CEq::apply(CSpreadsheet & table)
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        if(std::get<double>(l) == std::get<double>(r)) res = 1.0;
        else res = 0.0;
    }
    else if(std::holds_alternative<std::string>(l) && std::holds_alternative<std::string>(r))
    {
        if(std::get<std::string>(l) == std::get<std::string>(r)) res = 1.0;
        else res = 0.0;
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CEq::clone() const { return std::make_shared<CEq>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CEq::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "=";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CNe
CNe::CNe(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CNe::apply(CSpreadsheet & table)
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        if(std::get<double>(l) != std::get<double>(r)) res = 1.0;
        else res = 0.0;
    }
    else if(std::holds_alternative<std::string>(l) && std::holds_alternative<std::string>(r))
    {
        if(std::get<std::string>(l) != std::get<std::string>(r)) res = 1.0;
        else res = 0.0;
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CNe::clone() const { return std::make_shared<CNe>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CNe::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "<>";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLt
CLt::CLt(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CLt::apply(CSpreadsheet & table) 
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        if(std::get<double>(l) < std::get<double>(r)) res = 1.0;
        else res = 0.0;
    }
    else if(std::holds_alternative<std::string>(l) && std::holds_alternative<std::string>(r))
    {
        if(std::get<std::string>(l) < std::get<std::string>(r)) res = 1.0;
        else res = 0.0;
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CLt::clone() const { return std::make_shared<CLt>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CLt::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "<";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLe
CLe::CLe(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CLe::apply(CSpreadsheet & table) 
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        if(std::get<double>(l) <= std::get<double>(r)) res = 1.0;
        else res = 0.0;
    }
    else if(std::holds_alternative<std::string>(l) && std::holds_alternative<std::string>(r))
    {
        if(std::get<std::string>(l) <= std::get<std::string>(r)) res = 1.0;
        else res = 0.0;
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CLe::clone() const { return std::make_shared<CLe>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CLe::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << "<=";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CGt
CGt::CGt(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CGt::apply(CSpreadsheet & table) 
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        if(std::get<double>(l) > std::get<double>(r)) res = 1.0;
        else res = 0.0;
    }
    else if(std::holds_alternative<std::string>(l) && std::holds_alternative<std::string>(r))
    {
        if(std::get<std::string>(l) > std::get<std::string>(r)) res = 1.0;
        else res = 0.0;
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CGt::clone() const { return std::make_shared<CGt>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CGt::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << ">";
    _r->print(os);
    os << ")";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CGe
CGe::CGe(const Shared_Literal & l, const Shared_Literal & r): COpBin(l, r) {}
//----------------------------------------------------------------------------------------------------------
CValue CGe::apply(CSpreadsheet & table) 
{
    CValue l = _l->apply(table);
    CValue r = _r->apply(table);
    CValue res;
    if(std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
    {
        if(std::get<double>(l) >= std::get<double>(r)) res = 1.0;
        else res = 0.0;
    }
    else if(std::holds_alternative<std::string>(l) && std::holds_alternative<std::string>(r))
    {
        if(std::get<std::string>(l) >= std::get<std::string>(r)) res = 1.0;
        else res = 0.0;
    }
    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CGe::clone() const { return std::make_shared<CGe>(_l->clone(), _r->clone()); }
//----------------------------------------------------------------------------------------------------------
void CGe::print(std::ostream & os) const 
{
    os << "(";
    _l->print(os);
    os << ">=";
    _r->print(os);
    os << ")";
}
//==========================================================================================================


//==========================================================================================================
// COpUnar
//==========================================================================================================
COpUnar::COpUnar(const Shared_Literal & val): _val(val) {}
//----------------------------------------------------------------------------------------------------------
void COpUnar::update_ref(const Pair_Offset & ofs) { _val->update_ref(ofs); }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CNeg
CNeg::CNeg(const Shared_Literal & val): COpUnar(val) {}
//----------------------------------------------------------------------------------------------------------
CValue CNeg::apply(CSpreadsheet & table) 
{
    CValue val = _val->apply(table);
    CValue res;
    if(std::holds_alternative<double>(val)) res = -std::get<double>(val);

    return res;
}
//----------------------------------------------------------------------------------------------------------
Shared_Literal CNeg::clone() const { return std::make_shared<CNeg>(_val->clone()); }
//----------------------------------------------------------------------------------------------------------
void CNeg::print(std::ostream & os) const 
{
    os << "(";
    os << "-";
    _val->print(os);
    os << ")";
}
//==========================================================================================================