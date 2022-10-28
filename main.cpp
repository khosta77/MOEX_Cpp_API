#include <iostream>
//#include "SL_MOEX.h"

using namespace std;


//// http://iss.moex.com/iss/history/engines/stock/markets/shares/boa
////rds/tqbr/securities.xml?date=2013-12-20
//
///** // https://iss.moex.com/iss/history/engines/stock/markets/shares/boards/TQBR/securities/RUAL.xml?from=2022-09-29&till=2022-09-29
// * Вот эта ссылка позволит получить к открытии закритию рус акции в определенную дату
// * \param history
// *
// * \param
// * */

#include "Date.h"

int main() {
    Date d(2022, 10, 10);
    cout << d.date() << endl;
//    MOEX_parser moex;
//    auto df = moex.parser("GAZP");
//    cout << df["DATE"] << " " << df["OPEN"] << endl;
    return 0;
}