#pragma once
#include "../literals/literals.h"

// forward declaration because this classes used in eval_seq
class CLiteral;
class CSpreadsheet;

using CValue = std::variant<std::monostate, double, std::string>;
using Shared_Literal = std::shared_ptr<CLiteral>;
using Pair_Literal = std::pair<Shared_Literal, Shared_Literal>;

// class for expression and values (store, build, evaluate)
class CExpr: public CExprBuilder
{
  public:
    // constructors
    CExpr() = default;
    CExpr(const std::string & str);

    // copy constructor and op =
    CExpr(const CExpr & old);
    CExpr & operator = (const CExpr & old);
    
    // operators
    void opAdd() override;
    void opSub() override;
    void opMul() override;
    void opDiv() override;
    void opPow() override;
    void opNeg() override;
    void opEq() override;
    void opNe() override;
    void opLt() override;
    void opLe() override;
    void opGt() override;
    void opGe() override;

    // values
    void valNumber(double val) override;
    void valString(std::string val) override;
    void valReference(std::string val) override;

    // evaluate created m_seq
    CValue eval_seq(CSpreadsheet & table);

    // helper methods
    // return left and right operand as pair
    Pair_Literal get_lr(); 
    // update all links
    void update_all_pos(const Pair_Offset & ofs);
    // print expression
    friend std::ostream & operator << (std::ostream & os, const CExpr & expr);

    // !!! BONUS
    void valRange(std::string val) override;
    void funcCall(std::string fnName, int paramCount ) override;
  private:
    std::stack<Shared_Literal> m_seq;
    bool m_is_expr = false; // false, if is only double or string. If expr has = on begin, true
};
