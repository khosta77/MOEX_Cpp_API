#include <iostream>
#include "DMOEX.h"
#include "Time.h"

using namespace std;

#include "Date.h"

int main() {
    moex_day md;
    // Date d1("2022-10-31"), d2("2022-11-3");
    [[maybe_unused]] auto df = md.parser("MTSS");
    Time t("22:12:3");
    cout << t.time() << endl;
    return 0;
}
