#include <iostream>
#include <vector>

bool valid(int pw) {

    std::vector<int> digits;
    bool repeat = false;
    while (pw > 0) {
        int r = pw % 10;

        if (!digits.empty()) {
            if (digits.back() == r) {
                repeat = true;
            }
            if (digits.back() < r) {
                return false;
            }
        }

        digits.push_back(r);
        pw /= 10;
    }

    return repeat;
}

int main() {
    int start = 353096, end = 843212;

    int v = 0;
    for (int i = start; i <= end; i++) {
        if (valid(i)) {
            v++;
        }
    }
    std::cout << v;
}