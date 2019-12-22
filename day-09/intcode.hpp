#include <iostream>
#include <string>
#include <sstream>
#include <queue>
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
            n--;
            if (n >= modes.size()) {
                return 0;
            }
            return modes[n];
        }
        int opcode() {
            return code;
        }
    private:
        int code;
        int _next = 0;
        std::vector<int> modes;
};

class IntCode {
    public:
        IntCode(std::string input) {
            Load(input);
        }
        void Load(std::string program) {
            memory.clear();
            std::stringstream ss(program);
            pos = 0;
            relbase = 0;
            for (int64_t i; ss >> i;) {
                memory.push_back(i);    
                if (ss.peek() == ',')
                    ss.ignore();
            }           
        }
        void Set(int64_t p, int64_t val) {
            while (memory.size() <= p) {
                memory.push_back(0);
            }
            memory[p] = val;
        }
        int64_t Get(int64_t i) {
            if (i >= memory.size()) {
                return 0;
            }
            return memory[i];
        }
        void Feed(int64_t i) {
            input.push(i);
        }
        bool HasOutput() {
            return !output.empty();
        }
        int64_t Output() {
            int64_t r = output.front();
            output.pop();
            return r;
        }
        bool Run() {
            
            while (pos != -1) {
                mode = new Mode(Get(pos));

                switch (mode->opcode()) {
                    case 1:
                        pos += OpCodeAdd();
                        break;
                    case 2:
                        pos += OpCodeMul();
                        break;
                    case 3:
                        if (input.empty()) {
                            return false;
                        }
                        pos += OpCodeRead();
                        break;
                    case 4:
                        pos += OpCodeOutput();
                        break;
                    case 5:
                        pos = OpCodeJIT(); // assign
                        break;
                    case 6:
                        pos = OpCodeJIF(); // assign
                        break;
                    case 7:
                        pos += OpCodeLessThan();
                        break;
                    case 8:
                        pos += OpCodeEQ();
                        break;
                    case 9:
                        pos += OpCodeAdjustBase();
                        break;
                    case 99:
                        pos = OpCodeExit(); // assign to pos
                        break;
                    default:
                        std::cout << "Invalid OpCode: " << Get(pos) << "(" << mode->opcode() << ")" << std::endl;
                        return false;
                }

                delete mode;
            }
            return true;
        }
        std::vector<int64_t> memory;
        std::queue<int64_t> input;
        std::queue<int64_t> output;
    private:
        int64_t relbase;
        int64_t pos;
        Mode * mode;
        int64_t addr(int64_t spot) {
            int64_t m = mode->get(spot);
            if (m == 2) {
                return relbase + Get(pos+spot);
            }
            if (m == 1) {
                return pos+spot;
            }
            return Get(pos+spot);
        }
        int64_t access(int64_t spot) {
            int64_t a = addr(spot);
            int64_t r = Get(a);
            return r;
        }
        int64_t OpCodeAdd() {
            int64_t a = access(1);
            int64_t b = access(2);
            int64_t t = addr(3);
            Set(t, a+b);
            return 4;
        }
        int64_t OpCodeMul() {
            int64_t a = access(1);
            int64_t b = access(2);
            int64_t t = addr(3);
            Set(t, a*b);
            return 4;
        }
        int64_t OpCodeRead() {
            int64_t param = addr(1);
            int64_t in = input.front();
            input.pop();
            Set(param, in);
            return 2;
        }
        int64_t OpCodeOutput() {
            int64_t param = access(1);
            output.push(param);
            return 2;
        }
        int64_t OpCodeExit() {
            return -1;
        }

        int64_t OpCodeJIT() {
            int64_t a = access(1);
            int64_t b = access(2);
            if (a != 0) {
                return b;
            }
            return pos + 3;
        }

        int64_t OpCodeJIF() {
            int64_t a = access(1);
            int64_t b = access(2);

            if (a == 0) {
                return b;
            }
            return pos + 3;
        }

        int64_t OpCodeLessThan() {
            int64_t a = access(1);
            int64_t b = access(2);
            int64_t t = addr(3);
            Set(t, a < b ? 1 : 0);
            return 4;
        }

        int64_t OpCodeEQ() {
            int64_t a = access(1);
            int64_t b = access(2);
            int64_t t = addr(3);
            Set(t, a == b ? 1 : 0);
            return 4;
        }
        int64_t OpCodeAdjustBase() {
            int64_t a = access(1);
            relbase += a;
            return 2;
        }
};