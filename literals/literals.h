#pragma once
#include "../spreadsheet/spreadsheet.h"

// forward declaration because this class used in eval_seq
class CSpreadsheet;
class CLiteral;

using CValue = std::variant<std::monostate, double, std::string>;
using Shared_Literal = std::shared_ptr<CLiteral>;
using Pair_Offset = std::pair<int, int>; // first is column, second is row

//==========================================================================================================
// main abatract class
//==========================================================================================================
class CLiteral
{
    public:
        virtual ~CLiteral() = default;
        // clone current object
        virtual Shared_Literal clone() const = 0;
        // apply operation on constructed inherit object.
        // in operators return std::monostate if operation for input operands types not defined
        virtual CValue apply(CSpreadsheet & table) = 0;
        // update reference if rect was copyed. Calculate new col,row adding input offset. Used for copyRect.
        virtual void update_ref(const Pair_Offset & ofs) = 0;

        // print literal
        virtual void print(std::ostream & os) const = 0;
        friend std::ostream & operator << (std::ostream & os, const CLiteral & lit);

};
//==========================================================================================================

//==========================================================================================================
// binary operators
//==========================================================================================================
class COpBin: public CLiteral
{
    public:
        COpBin(const Shared_Literal & l, const Shared_Literal & r);
        virtual ~COpBin() = default;
        void update_ref(const Pair_Offset & ofs) override;

    protected:
        Shared_Literal _l; // left operand
        Shared_Literal _r; // right operand
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op +
class CAdd: public COpBin
{
    public:
        CAdd(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op -
class CSub: public COpBin
{
    public:
        CSub(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op *
class CMul: public COpBin
{
    public:
        CMul(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op /
class CDiv: public COpBin
{
    public:
        CDiv(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op ^
class CPow: public COpBin
{
    public:
        CPow(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op ==
class CEq: public COpBin
{
    public:
        CEq(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op <>
class CNe: public COpBin
{
    public:
        CNe(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op <
class CLt: public COpBin
{
    public:
        CLt(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op <=
class CLe: public COpBin
{
    public:
        CLe(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op >
class CGt: public COpBin
{
    public:
        CGt(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op >=
class CGe: public COpBin
{
    public:
        CGe(const Shared_Literal & l, const Shared_Literal & r);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//==========================================================================================================


//==========================================================================================================
// unary operators
//==========================================================================================================
class COpUnar: public CLiteral
{
    public:
        COpUnar(const Shared_Literal & val);
        virtual ~COpUnar() = default;
        void update_ref(const Pair_Offset & ofs) override;

    protected:
        Shared_Literal _val; // value before or after operator
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// op unar -
class CNeg: public COpUnar
{
    public:
        CNeg(const Shared_Literal & val);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
};
//==========================================================================================================


//==========================================================================================================
// real value => std::string or double or std::monostate
//==========================================================================================================
class CVal: public CLiteral
{
    public:
        CVal(CValue val);
        Shared_Literal clone() const override;
        CValue apply(CSpreadsheet & table) override; 
        void update_ref(const Pair_Offset & ofs) override;
        void print(std::ostream & os) const override;

    private:
        CValue m_val; // default std::monostate
};


//==========================================================================================================
// ref on exists cell
//==========================================================================================================
class CRef: public CLiteral
{
    public:
        CRef(const std::string & str);
        CValue apply(CSpreadsheet & table) override;
        Shared_Literal clone() const override;
        void print(std::ostream & os) const override;
        void update_ref(const Pair_Offset & ofs) override;

    private:
        CPos m_pos;
        // default is pos WITHOUT $ => relatively reference
        bool m_is_col_abs = false;
        bool m_is_row_abs = false;
};
//==========================================================================================================