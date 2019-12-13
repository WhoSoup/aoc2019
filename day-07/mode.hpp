#include <vector>

class Mode {
    public:
        Mode(int raw) {
            code = raw % 100;
            raw /= 100;

            while (raw > 0) {
                modes.push_back(raw % 10);
                raw /= 10;
            }
        }
        int get(int n) {
            if (n >= modes.size()) {
                return 0;
            }
            return modes[n];
        }
        int next() {
            return get(_next++);
        }
        int opcode() {
            return code;
        }
    private:
        int code;
        int _next = 0;
        std::vector<int> modes;
};