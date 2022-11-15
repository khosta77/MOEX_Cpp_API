#include <iostream>
#include "GuideMOEX.h"

using namespace std;


int main() {
    guideMOEX md;
    [[maybe_unused]] auto df = md.engines();
    cout << df.size() << endl;
    for(auto it : df) {
        cout << it.first << " " << it.second << endl;
    }
    cout << endl;
    cout << "Запрос был по: " << df[0].first << endl;
    [[maybe_unused]] auto df2 = md.markets(df[0].first);
    for(auto it : df2) {
        cout << it.first << " | " << it.second << endl;
    }
    return 0;
}
