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
    ifstream myfile;
    myfile.open("input.txt");
    string line;
    long total1 = 0;
    long total2 = 0;
    while (getline(myfile, line)) {
        int i = stoi(line);
        total1 += fuel(i);
        total2 += fuelfuel(i);
    }
    myfile.close();
    cout << total1 << endl;
    cout << total2 << endl;
    return 0;
}