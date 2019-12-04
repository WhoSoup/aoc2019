#include <iostream>
#include <vector>

bool valid(int pw) {
    bool repeat = false;
    int prev = 10; // going reverse, so higher than 9
    while (pw > 0) {
        int r = pw % 10;

        if (prev == r) {
            repeat = true;
        }
        if (prev < r) {
            return false;
        }

        pw /= 10;
        prev = r;
    }

    return repeat;
}

bool valid2(int pw) {

    int prev = 10;
    int run = 0;
    bool repeat = false;
    while (pw > 0) {
        int r = pw % 10;
        
        if (r == prev) {
            run++;
        } else {
            if (run == 1) {
                repeat = true;
            }
            run = 0;
        }
        if (prev < r) {
            return false;
        }

        pw /= 10;
        prev = r;
    }

    return repeat || run == 1;
}


int main() {
    int start = 353096, end = 843212;

    int v = 0, v2 = 0;
    for (int i = start; i <= end; i++) {
        if (valid(i)) {
            v++;
        }
        if (valid2(i)) {
            v2++;
        } 
    }
    std::cout << v << std::endl;
    std::cout << v2 << std::endl;
}