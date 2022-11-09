#include <iostream>
#include "HMOEX.h"

using namespace std;

#include "Date.h"

int main() {
    moex_history mh;
    Date d1("2022-10-31"), d2("2022-11-3");
    [[maybe_unused]] auto df = mh.parser("MTSS", d1, d2);
    return 0;
}
