#include <iostream>
#include "HMOEX.h"

using namespace std;



#include "Date.h"

int main() {
    moex_history mh;
    [[maybe_unused]] auto df = mh.parser("MTSS");
//    Date d(2022, 10, 10);
//    cout << d.date() << endl;
//    MOEX_parser moex;
//    auto df = moex.parser("GAZP");
//    cout << df["DATE"] << " " << df["OPEN"] << endl;
    return 0;
}