#include "expr.h"

//==========================================================================================================
// CExpr
//==========================================================================================================
CExpr::CExpr(const std::string & str)
{
    try { parseExpression(str, *this); }
    catch(const std::invalid_argument & e)
    {
        throw std::invalid_argument("None valid argument for cell value.");
    }
}
//==========================================================================================================
Pair_Literal CExpr::get_lr()
{
    m_is_expr = true;
    Shared_Literal r = m_seq.top();
    m_seq.pop();
    Shared_Literal l = m_seq.top();
    m_seq.pop();
    return std::make_pair(l, r);
}
//----------------------------------------------------------------------------------------------------------
// +
void CExpr::opAdd() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CAdd>(CAdd(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------   
// -
void CExpr::opSub() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CSub>(CSub(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// *
void CExpr::opMul() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CMul>(CMul(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// /
void CExpr::opDiv() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CDiv>(CDiv(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// ^
void CExpr::opPow() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CPow>(CPow(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// unar -
void CExpr::opNeg() 
{
    m_is_expr = true;
    std::shared_ptr<CLiteral> val = m_seq.top();
    m_seq.pop();
    m_seq.push(std::make_shared<CNeg>(CNeg(val)));
}
//----------------------------------------------------------------------------------------------------------
// =
void CExpr::opEq() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CEq>(CEq(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// <>
void CExpr::opNe() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CNe>(CNe(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// <
void CExpr::opLt() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CLt>(CLt(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// <=
void CExpr::opLe() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CLe>(CLe(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// >
void CExpr::opGt() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CGt>(CGt(p.first, p.second)));
}
//----------------------------------------------------------------------------------------------------------
// >=
void CExpr::opGe() 
{
    Pair_Literal p = get_lr();
    m_seq.push(std::make_shared<CGe>(CGe(p.first, p.second)));    
}
//----------------------------------------------------------------------------------------------------------
void CExpr::valNumber(double val) { m_seq.push(std::make_shared<CVal>(CVal(val))); }
//----------------------------------------------------------------------------------------------------------
void CExpr::valString(std::string val) { m_seq.push(std::make_shared<CVal>(CVal(val))); }
//----------------------------------------------------------------------------------------------------------
void CExpr::valReference(std::string val) 
{
    m_is_expr = true;
    m_seq.push(std::make_shared<CRef>(CRef(val)));
}
//----------------------------------------------------------------------------------------------------------
CValue CExpr::eval_seq(CSpreadsheet & table) 
{ 
    if(m_seq.empty()) return CValue(); // cell has none define value
    return m_seq.top()->apply(table);
}
//----------------------------------------------------------------------------------------------------------
void CExpr::update_all_pos(const Pair_Offset & ofs) { if(!m_seq.empty()) m_seq.top()->update_ref(ofs); }
//----------------------------------------------------------------------------------------------------------
CExpr::CExpr(const CExpr & old): m_is_expr(old.m_is_expr)
{
    if(old.m_seq.empty()) m_seq.push(std::make_shared<CVal>(CVal(CValue())));
    else m_seq.push(old.m_seq.top()->clone());
}
//----------------------------------------------------------------------------------------------------------
CExpr & CExpr::operator = (const CExpr & old)
{
    if(this == &old) return *this;

    // pop old expr
    if(!m_seq.empty()) m_seq.pop();

    // push new from old
    if(old.m_seq.empty()) m_seq.push(std::make_shared<CVal>(CVal(CValue())));
    else m_seq.push(old.m_seq.top()->clone());

    m_is_expr = old.m_is_expr;
    return *this;
}
//----------------------------------------------------------------------------------------------------------
std::ostream & operator << (std::ostream & os, const CExpr & expr)
{
    if(!expr.m_seq.empty()) 
    {
        if(expr.m_is_expr) os << "=";
        expr.m_seq.top()->print(os);
    }
    return os;
}
//==========================================================================================================



//==========================================================================================================
// BONUS!!!!
//==========================================================================================================
void CExpr::valRange(std::string val) {}
//----------------------------------------------------------------------------------------------------------
void CExpr::funcCall(std::string fnName, int paramCount ) {}
//==========================================================================================================
