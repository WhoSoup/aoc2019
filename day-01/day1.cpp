#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int fuel(int mass) {
    return mass / 3 - 2;
}

int fuelfuel(int mass) {
    int f = fuel(mass);
    return f <= 0 ? 0 : f + fuelfuel(f);
}

int main() {
    ifstream in("input.txt");
    int f;
    long total1 = 0;
    long total2 = 0;
    while (in >> f) {
        total1 += fuel(f);
        total2 += fuelfuel(f);
    }
    cout << total1 << endl;
    cout << total2 << endl;
    return 0;
}