#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

map<pair<int, int>, int> visited;
map<pair<int, int>, int> visitedSteps;

class Wire {
    public:
        int x = 0;
        int y = 0;
        void Move(string input) {
            string token;
            istringstream sx(input);
            while (getline(sx, token, ',')) {
                move(token);
            }
        }
    private:
        int steps = 0;
        map<pair<int, int>, int> history;
        void move(string token) {
            istringstream sx(token);
            char d;
            int c;
            sx >> d >> c;

            for (int i = 0; i < c; i++) {
                switch (d) {
                    case 'R': x++; break;
                    case 'D': y++; break;
                    case 'L': x--; break;
                    case 'U': y--; break;
                }
                touch();
            }
        }
        void touch() {
            steps++;
            pair<int, int> p {x,y};
            history[p]++;
            if (history[p] == 1) {
                visited[p]++;
                visitedSteps[p] += steps;
            }
        }
};

int main() {
    ifstream in("day3.txt");

    string line;
    while (getline(in, line)) {
        auto w = new Wire();
        w->Move(line);
    }

    int min = 2147483647;
    int minsteps = 2147483647; 
    map<pair<int,int>, int>::iterator itr;
    for (itr=visited.begin(); itr != visited.end(); itr++) {
        if (itr->second > 1) {
            int dist = abs(itr->first.first) + abs(itr->first.second);
            if (dist < min) {
                min = dist;
            }
            // note: this only works if at most 2 wires cross at an intersection.
            // i checked my input and found this to be true
            int steps = visitedSteps[pair<int, int>{itr->first.first, itr->first.second}];
            if (steps < minsteps) {
                minsteps = steps;
            }
        }
    }

    cout << "part one: " << min << endl;
    cout << "part two: " << minsteps << endl;
   
    return 0;
}