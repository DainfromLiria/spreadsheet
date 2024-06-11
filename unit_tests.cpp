#include "spreadsheet/spreadsheet.h"


//==========================================================================================================
// function from Progtest file
//==========================================================================================================
bool valueMatch( const CValue & r, const CValue & s )
{
  if ( r . index () != s . index () )
    return false;
  if ( r . index () == 0 )
    return true;
  if ( r . index () == 2 )
    return std::get<std::string> ( r ) == std::get<std::string> ( s );
  if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
    return true;
  if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
    return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
           || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
  return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}
//==========================================================================================================
// helper function for comparation CPos objects
bool equal(const CPos & x, const CPos & y )
{
  return (x.get_col() == y.get_col()) && (x.get_row() == y.get_row());
}
//==========================================================================================================
// TESTS
//==========================================================================================================
void test_Progtest()
{
  CSpreadsheet x0, x1;
  std::ostringstream oss;
  std::istringstream iss;
  std::string data;
  assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "A2" ), "20.5" ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "3e1" ) );
  assert ( x0 . setCell ( CPos ( "A4" ), "=40" ) );
  assert ( x0 . setCell ( CPos ( "A5" ), "=5e+1" ) );
  assert ( x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
  assert ( x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ) );
  assert ( x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ) );
  assert ( x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
  assert ( x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ) );
  assert ( x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ) );
  assert ( x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ) );
  assert ( x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -110.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A1" ), "12" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ) );
  x1 = x0;
  assert ( x0 . setCell ( CPos ( "A2" ), "100" ) );
  assert ( x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ) );
  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  iss . clear ();
  iss . str ( data );
  assert ( x1 . load ( iss ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "4e1" ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
    data[i] ^=0x5a;
  iss . clear ();
  iss . str ( data );
  assert ( ! x1 . load ( iss ) );
  assert ( x0 . setCell ( CPos ( "D0" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "D1" ), "20" ) );
  assert ( x0 . setCell ( CPos ( "D2" ), "30" ) );
  assert ( x0 . setCell ( CPos ( "D3" ), "40" ) );
  assert ( x0 . setCell ( CPos ( "D4" ), "50" ) );
  assert ( x0 . setCell ( CPos ( "E0" ), "60" ) );
  assert ( x0 . setCell ( CPos ( "E1" ), "70" ) );
  assert ( x0 . setCell ( CPos ( "E2" ), "80" ) );
  assert ( x0 . setCell ( CPos ( "E3" ), "90" ) );
  assert ( x0 . setCell ( CPos ( "E4" ), "100" ) );
  assert ( x0 . setCell ( CPos ( "F10" ), "=D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F11" ), "=$D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F12" ), "=D$0+5" ) );
  assert ( x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ) );
  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 1, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 2, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ) );
  assert ( x0 . setCell ( CPos ( "F0" ), "-27" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
  x0 . copyRect ( CPos ( "H12" ), CPos ( "H13" ), 1, 2 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
}
//==========================================================================================================
void test_CPos()
{
  assert(equal(CPos("AA12"), CPos(26, 12)));
  assert(equal(CPos("A0"), CPos(0, 0)));
  assert(equal(CPos("A10"), CPos(0, 10)));
  assert(equal(CPos("AB0"), CPos(27, 0)));
  assert(equal(CPos("Z0"), CPos(25, 0)));
  assert(equal(CPos("C1"), CPos(2, 1)));
  assert(equal(CPos("aZ0"), CPos(51, 0)));
  
  std::vector<std::string_view> error_cases = {"Z3A2", "Z011", "011", "abc", "A11B", "0A", "a-10", "-A10"};
  for(std::string_view pos: error_cases)
  {
    try
    {
      CPos a(pos);
      std::cout << "Test with input argument: " << pos << " faild. Constructor must throw an error!" << std::endl; 
      return;
    }
    catch(const std::invalid_argument & e) { }
  }

  assert(CPos("AA12").get_alpha_col() == "AA");
  assert(CPos("AB12").get_alpha_col() == "AB");
  assert(CPos("Az12").get_alpha_col() == "AZ");
  assert(CPos("C12").get_alpha_col() == "C");
  assert(CPos("CBDf12").get_alpha_col() == "CBDF");
  assert(CPos("CBDfgty12").get_alpha_col() == "CBDFGTY");
}
//==========================================================================================================
void test_CExpr()
{
  CSpreadsheet table;
  // test empty cell
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A0"))) == true);

  // operator +
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= 2 + 2 + A0 ");
  assert(std::get<std::string>(table.getValue(CPos("A1"))) == "4.000000ok");
  table.setCell(CPos("A1"), "= 2 + A0 ");
  CValue tmp = table.getValue(CPos("A1"));
  assert(std::get<std::string>(table.getValue(CPos("A1"))) == "2.000000ok");
  table.setCell(CPos("A1"), "= A0 + 2 ");
  tmp = table.getValue(CPos("A1"));
  assert(std::get<std::string>(table.getValue(CPos("A1"))) == "ok2.000000");
  table.setCell(CPos("A1"), "= A0 + A0 ");
  tmp = table.getValue(CPos("A1"));
  assert(std::get<std::string>(table.getValue(CPos("A1"))) == "okok");
  table.setCell(CPos("A0"), "NEW_OK");
  tmp = table.getValue(CPos("A1"));
  assert(std::get<std::string>(table.getValue(CPos("A1"))) == "NEW_OKNEW_OK");

  // operator -
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= 2 - 2 + 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 2);
  table.setCell(CPos("A1"), "= 2 - 2 - A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 - A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 - A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 - 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A2 - 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2- 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == -2.5);

  // operator -
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= 2 * 2 + 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 6);
  table.setCell(CPos("A1"), "= 2 * 2 - A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 * A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 * A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 * 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A2 * 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A3"), "1");
  table.setCell(CPos("A1"), "= A3 + 2 * 4.5 - 1 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 9);

  // operator /
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= 2 / 2 + 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 3);
  table.setCell(CPos("A1"), "= 2 / 2 - A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 / A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 / A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 / 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A2 / 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 / 0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A2 / 0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 0 / 2 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "1");
  table.setCell(CPos("A1"), "= A3 / 2 + 4.5 - 1 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 4);


  // operator ^
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= 2 ^ 2 - 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 2);
  table.setCell(CPos("A1"), "= 2 ^ 2 - A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 ^ A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 ^ A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 ^ 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A2 ^ 2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 ^ 0");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= A2 ^ 0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 0 ^ 2 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "1");
  table.setCell(CPos("A1"), "= A3 ^ 2 + 4.5 - 1 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 4.5);

  // operator unar -
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= (-2) ^ 2 - 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 2);
  table.setCell(CPos("A1"), "= -2 ^ 2 - A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= -A0 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= -A2 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= -2 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == -2);
  table.setCell(CPos("A1"), "= A2 ^ (-0)");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= -0 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "-1");
  table.setCell(CPos("A1"), "= A3 ^ 2 + 4.5 - 1 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 4.5);


  // operator =
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= (-2) = (-2)");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= 2 = A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 = A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 = A0 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= A2 = A0 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A4"), "neok");
  table.setCell(CPos("A1"), "= A0 = A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "-1");
  table.setCell(CPos("A1"), "= A3 = 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);


  // operator <>
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= (-2) <> (-2)");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= (-2) <> 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= 2 <> A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 <> A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 <> A0 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= A2 <> A0 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A4"), "neok");
  table.setCell(CPos("A1"), "= A0 <> A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A3"), "-1");
  table.setCell(CPos("A1"), "= A3 <> 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);


  // operator <
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= (-2) < 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= (-2) < (-2)");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= 2 < A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 < A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 < A0 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= A2 < A0 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A4"), "teok");
  table.setCell(CPos("A1"), "= A0 < A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A3"), "1");
  table.setCell(CPos("A1"), "= A3 < 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);

  // operator <=
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= (-2) <= 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= (-2) <= (-2)");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= 2 <= A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 <= A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 <= A0 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= A2 <= A0 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A4"), "teok");
  table.setCell(CPos("A1"), "= A0 <= A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A4"), "neok");
  table.setCell(CPos("A1"), "= A0 <= A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "1");
  table.setCell(CPos("A1"), "= A3 <= 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A3"), "4.5");
  table.setCell(CPos("A1"), "= A3 <= 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);

  // operator >
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= (-2) > 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= (-2) > (-2)");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= 2 > A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 > A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 > A0 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= A2 > A0 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A4"), "teok");
  table.setCell(CPos("A1"), "= A0 > A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A4"), "neok");
  table.setCell(CPos("A1"), "= A0 > A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A3"), "1");
  table.setCell(CPos("A1"), "= A3 > 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "4.5");
  table.setCell(CPos("A1"), "= A3 > 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "5.5");
  table.setCell(CPos("A1"), "= A3 > 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);


  // operator >=
  table.setCell(CPos("A0"), "ok");
  table.setCell(CPos("A1"), "= (-2) >= 2");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A1"), "= (-2) >= (-2)");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= 2 >= A2");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= 2 >= A0");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A1"), "= A0 >= A0 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A1"), "= A2 >= A0 ");
  assert(std::holds_alternative<std::monostate>(table.getValue(CPos("A1"))) == true);
  table.setCell(CPos("A4"), "teok");
  table.setCell(CPos("A1"), "= A0 >= A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A4"), "neok");
  table.setCell(CPos("A1"), "= A0 >= A4 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A3"), "1");
  table.setCell(CPos("A1"), "= A3 >= 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 0);
  table.setCell(CPos("A3"), "4.5");
  table.setCell(CPos("A1"), "= A3 >= 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
  table.setCell(CPos("A3"), "5.5");
  table.setCell(CPos("A1"), "= A3 >= 4.5 ");
  assert(std::get<double>(table.getValue(CPos("A1"))) == 1);
}
//==========================================================================================================
void test_CopyTable()
{
  // copy constructor
  CSpreadsheet a, b;
  a.setCell(CPos("A0"), "ok");
  a.setCell(CPos("A1"), "= 2 + 2 + A0 ");
  b = a;
  b.setCell(CPos("A1"), "= A0 + A0 ");
  assert(std::get<std::string>(a.getValue(CPos("A1"))) == "4.000000ok");
  assert(std::get<std::string>(b.getValue(CPos("A1"))) == "okok");
  

  // op =
  a.setCell(CPos("A0"), "ok");
  a.setCell(CPos("A1"), "= 2 + 2 + A0 ");
  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "ok");
  assert(std::get<std::string>(a.getValue(CPos("A1"))) == "4.000000ok");
  b.setCell(CPos("A0"), "bok");
  b.setCell(CPos("A1"), "= A0 + A0 ");
  assert(std::get<std::string>(b.getValue(CPos("A0"))) == "bok");
  assert(std::get<std::string>(b.getValue(CPos("A1"))) == "bokbok");
  a = b;
  a.setCell(CPos("A0"), "boka");
  a.setCell(CPos("A1"), "=2 + A0");
  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "boka");
  assert(std::get<std::string>(a.getValue(CPos("A1"))) == "2.000000boka");
  assert(std::get<std::string>(b.getValue(CPos("A0"))) == "bok");
  assert(std::get<std::string>(b.getValue(CPos("A1"))) == "bokbok");
}
//==========================================================================================================
void test_copyRect()
{
  CSpreadsheet a;
  a.setCell(CPos("A0"), "ok");
  a.setCell(CPos("A1"), "3");
  a.setCell(CPos("B0"), "= $A1 + 1");
  a.setCell(CPos("B1"), "= A1 / 1");
  a.setCell(CPos("C0"), "= A$1 ^ 1");
  a.setCell(CPos("C1"), "= $A$1 * (-1)");

  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "ok");
  assert(std::get<double>(a.getValue(CPos("A1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("B0"))) == 4);
  assert(std::get<double>(a.getValue(CPos("B1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C0"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C1"))) == -3);

  a.setCell(CPos("D0"), "5");
  a.setCell(CPos("A4"), "correct");
  a.setCell(CPos("E3"), "test");
  assert(std::get<double>(a.getValue(CPos("D0"))) == 5);
  assert(std::get<std::string>(a.getValue(CPos("E3"))) == "test");
  assert(std::get<std::string>(a.getValue(CPos("A4"))) == "correct");

  // make copy
  a.copyRect(CPos("D3"), CPos("A0"), 3, 2);

  // new Rect
  assert(std::get<std::string>(a.getValue(CPos("D3"))) == "ok");
  assert(std::get<double>(a.getValue(CPos("D4"))) == 3);
  assert(std::get<std::string>(a.getValue(CPos("E3"))) == "correct1.000000"); // A4
  assert(std::get<double>(a.getValue(CPos("E4"))) == 3); // D4
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("F3"))) == true); // D1
  assert(std::get<double>(a.getValue(CPos("F4"))) == -3); // A1

  // check origin Rect
  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "ok");
  assert(std::get<double>(a.getValue(CPos("A1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("B0"))) == 4);
  assert(std::get<double>(a.getValue(CPos("B1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C0"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C1"))) == -3);


  // make copy
  a.copyRect(CPos("E3"), CPos("D3"), 2, 2);

  // new Rect
  assert(std::get<std::string>(a.getValue(CPos("E3"))) == "ok");
  assert(std::get<double>(a.getValue(CPos("E4"))) == 3);
  assert(std::get<std::string>(a.getValue(CPos("F3"))) == "correct1.000000"); 
  assert(std::get<double>(a.getValue(CPos("F4"))) == 3); 

  // old rects
  assert(std::get<std::string>(a.getValue(CPos("D3"))) == "ok");
  assert(std::get<double>(a.getValue(CPos("D4"))) == 3);
  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "ok");
  assert(std::get<double>(a.getValue(CPos("A1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("B0"))) == 4);
  assert(std::get<double>(a.getValue(CPos("B1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C0"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C1"))) == -3);
}
//==========================================================================================================
void test_save_load()
{
  CSpreadsheet a;
  a.setCell(CPos("A0"), "\to\"\nk\n");
  a.setCell(CPos("A1"), "3");
  a.setCell(CPos("B0"), "= $A1 + 1");
  a.setCell(CPos("B1"), "= A1 / 1");
  a.setCell(CPos("C0"), "= A$1 ^ 1");
  a.setCell(CPos("C1"), "= $A$1 * (-1)");
  a.setCell(CPos("C2"), "= A0 * 1" );
  a.setCell(CPos("C3"), "= (2 + 3) ^ 2 " );
  a.setCell(CPos("D3"), "= A1 * 10 " );

  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "\to\"\nk\n");
  assert(std::get<double>(a.getValue(CPos("A1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("B0"))) == 4);
  assert(std::get<double>(a.getValue(CPos("B1"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C0"))) == 3);
  assert(std::get<double>(a.getValue(CPos("C1"))) == -3);
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("C2"))) == true);
  assert(std::get<double>(a.getValue(CPos("C3"))) == 25);
  assert(std::get<double>(a.getValue(CPos("D3"))) == 30);
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("E7"))) == true);

  a.copyRect(CPos("E7"), CPos("D3"), 2, 2);
  
  std::ostringstream oss;
  std::istringstream iss;
  std::string data;

  oss.clear();
  oss.str("");
  assert (a.save(oss));
  data = oss.str();

  iss.clear();
  iss.str(data);
  CSpreadsheet b;
  assert (b.load(iss));

  assert(std::get<std::string>(b.getValue(CPos("A0"))) == "\to\"\nk\n");
  assert(std::get<double>(b.getValue(CPos("A1"))) == 3);
  assert(std::get<double>(b.getValue(CPos("B0"))) == 4);
  assert(std::get<double>(b.getValue(CPos("B1"))) == 3);
  assert(std::get<double>(b.getValue(CPos("C0"))) == 3);
  assert(std::get<double>(b.getValue(CPos("C1"))) == -3);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("C2"))) == true);
  assert(std::get<double>(b.getValue(CPos("C3"))) == 25);
  assert(std::get<double>(b.getValue(CPos("D3"))) == 30);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("E7"))) == true);


  data = "\"A0\",\"sdfghjk \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  assert(std::get<std::string>(b.getValue(CPos("A0"))) == "\to\"\nk\n");
  assert(std::get<double>(b.getValue(CPos("A1"))) == 3);
  assert(std::get<double>(b.getValue(CPos("B0"))) == 4);
  assert(std::get<double>(b.getValue(CPos("B1"))) == 3);
  assert(std::get<double>(b.getValue(CPos("C0"))) == 3);
  assert(std::get<double>(b.getValue(CPos("C1"))) == -3);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("C2"))) == true);
  assert(std::get<double>(b.getValue(CPos("C3"))) == 25);
  assert(std::get<double>(b.getValue(CPos("D3"))) == 30);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("E7"))) == true);

  data = "1\n4\"A0\",10\"sdfghjk \"";
  iss.clear();
  iss.str(data);
  assert (b.load(iss));

  assert(std::get<std::string>(b.getValue(CPos("A0"))) == "sdfghjk ");
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("A1"))) == true);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("E7"))) == true);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("B0"))) == true);

  oss.clear();
  oss.str("");
  assert (a.save(oss));
  data = oss.str();
  data += "\"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  assert(std::get<std::string>(b.getValue(CPos("A0"))) == "sdfghjk ");
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("A1"))) == true);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("E7"))) == true);
  assert(std::holds_alternative<std::monostate>(b.getValue(CPos("B0"))) == true);

  oss.clear();
  oss.str("");
  assert (b.save(oss));
  data = oss.str();
  iss.clear();
  iss.str(data);
  assert (a.load(iss));

  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "sdfghjk ");
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("A1"))) == true);
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("E7"))) == true);
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("B0"))) == true);

  a.setCell(CPos("A0"), "\to\"\nk\n");
  assert(std::get<std::string>(a.getValue(CPos("A0"))) == "\to\"\nk\n");
  assert(std::get<std::string>(b.getValue(CPos("A0"))) == "sdfghjk ");


  data = "1\n4\"A0\",,10\"sdfghjk \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  data = "1\n4\"A0\"10\"sdfghjk \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  data = "-1\n4\"A0\",10\"sdfghjk \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  data = "1\n4\"A0\",12\"\"sdfghjk\n \"";
  iss.clear();
  iss.str(data);
  assert (b.load(iss));

  data = "1\n4\"A0\",\"\"sdfghjk\n \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  data = "1\n0\"A0\",12\"\"sdfghjk\n \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  data = "1\n-1\"A0\",12\"\"sdfghjk\n \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  data = "2\n4\"A0\",-1\"\"sdfghjk\n \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));

  data = "2\n4\"A0\",12\"\"sdfghjk\n \"4\"A1\",12\"\"sdfghjk\n \"";
  iss.clear();
  iss.str(data);
  assert (b.load(iss));

  data = "1\n4\"A0\",12\"\"sdfghjk\n \"4\"A1\",12\"\"sdfghjk\n \"";
  iss.clear();
  iss.str(data);
  assert (!b.load(iss));
}
//==========================================================================================================
void test_save_load2()
{
  CSpreadsheet a;
  a.setCell(CPos("B1"), "phghctjryihstcvqbkhflpq");
  a.setCell(CPos("P15"), "=B1");
  assert(std::get<std::string>(a.getValue(CPos("B1"))) == "phghctjryihstcvqbkhflpq");
  assert(std::get<std::string>(a.getValue(CPos("P15"))) == "phghctjryihstcvqbkhflpq");
  
  std::ostringstream oss;
  std::istringstream iss;
  std::string data;

  oss.clear();
  oss.str("");
  assert (a.save(oss));
  data = oss.str();

  iss.clear();
  iss.str(data);
  CSpreadsheet b;
  assert (b.load(iss));

  assert(std::get<std::string>(b.getValue(CPos("B1"))) == "phghctjryihstcvqbkhflpq");
  assert(std::get<std::string>(b.getValue(CPos("P15"))) == "phghctjryihstcvqbkhflpq");
}
//==========================================================================================================
void test_cycle_dependency()
{
  CSpreadsheet a;
  a.setCell(CPos("A0"), "=A1");
  a.setCell(CPos("A1"), "=A0");
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("A1"))) == true);


  a.setCell(CPos("B0"), "=2 + $A1");
  a.setCell(CPos("C1"), "=B0");
  assert(std::holds_alternative<std::monostate>(a.getValue(CPos("B0"))) == true);
}
//==========================================================================================================


int main()
{
  test_CPos();
  test_CExpr();
  test_CopyTable();
  test_copyRect();
  test_save_load();
  test_Progtest();
  test_save_load2();
  test_cycle_dependency();
  
  return 0;
}