#include <iostream>
#include <vector>

std::pair<bool,bool> valid(int pw) {
    bool repeat = false;
    bool repeat2 = false;
    int run = 0;
    int prev = 10; // going reverse, so higher than 9
    while (pw > 0) {
        int r = pw % 10;

        if (prev < r) {
            return std::pair<bool,bool>{false,false};
        }

        if (prev == r) {
            repeat = true;
            run++;
        } else {
            if (run == 1) {
                repeat2 = true;
            }
            run = 0;
        }

        pw /= 10;
        prev = r;
    }

    return std::pair<bool,bool>{repeat, repeat2 || run == 1};
}

int main() {
    int start = 353096, end = 843212;

    int v = 0, v2 = 0;
    for (int i = start; i <= end; i++) {
        auto p = valid(i);
        if (p.first) v++;
        if (p.second) v2++;
    }
    std::cout << v << std::endl;
    std::cout << v2 << std::endl;
}