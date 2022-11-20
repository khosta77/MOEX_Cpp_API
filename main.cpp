#include <iostream>
#include "Date.h"
#include "Candle.h"
#include "HMOEX.h"
using namespace std;


int main() {
#if TEST_METHODS
    moex_history mh;
    for (size_t i = 0; i < TEST_SIZE; ++i) { // std::vector<Candle>, 
       std::vector<Candle> df = mh.parser("GAZP", Date("2022-11-14"), Date("2022-11-18"));
       for (auto it : df) 
           cout << it.df_candle["OPEN"] << " " << it.date.date() << endl;
    }
#endif
/*    [[maybe_unused]] auto df = md.engines();
    cout << df.size() << endl;
    for(auto it : df) {
        cout << it.first << " " << it.second << endl;
    }
    cout << endl;
    cout << "Запрос был по: " << df[0].first << endl;
    [[maybe_unused]] auto df2 = md.markets(df[0].first);
    for(auto it : df2) {
        cout << it.first << " | " << it.second << endl;
*/

    return 0;
}
