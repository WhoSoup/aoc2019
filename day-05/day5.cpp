#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <list>
#include <cassert>

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
        void p() {
            std::cout << "pos: " << pos << std::endl;
            std::cout << "opcode: " << code << std::endl;
            for (int i = 0; i < modes.size(); i++) {
                std::cout << i << ": " << modes[i] << std::endl;
            }
        }
    private:
        int pos;
        int code;
        int _next = 0;
        std::vector<int> modes;
};

class IntCode {
    public:
        IntCode(std::string input) {
            inputter = 0;
            Load(input);
        }
        void Load(std::string input) {
            memory.clear();
            std::stringstream ss(input);

            for (int i; ss >> i;) {
                memory.push_back(i);    
                if (ss.peek() == ',')
                    ss.ignore();
            }           
        }
        void Set(int p, int val) {
            memory[p] = val;
        }
        int Read() {
            assert(inputter < input.size());
            return input[inputter++];
        }
        int Run() {
            pos = 0;
            while (pos != -1) {
                mode = new Mode(memory[pos]);

                switch (mode->opcode()) {
                    case 1:
                        pos += OpCodeAdd();
                        break;
                    case 2:
                        pos += OpCodeMul();
                        break;
                    case 3:
                        pos += OpCodeRead();
                        break;
                    case 4:
                        pos += OpCodePrint();
                        break;
                    case 99:
                        pos = OpCodeExit(); // assign to pos
                        break;
                    default:
                        std::cout << "Invalid OpCode: " << memory[pos] << "(" << mode->opcode() << ")" << std::endl;
                        return -999;
                }

                delete mode;
            }
            return memory[0];
        }
        std::vector<int> memory;
        std::vector<int> input;
    private:
        int pos;
        int inputter;
        Mode * mode;
        int access(int spot, int mode) {
            if (mode == 1) {
                return memory[pos+spot];
            }
            return memory[memory[pos+spot]];
        }
        int OpCodeAdd() {
            int a = access(1, mode->next());
            int b = access(2, mode->next());
            int t = memory[pos+3];
            memory[t] = a+b;
//            printf("opcode 1: %d %d %d\n", a, b, t);
            return 4;
        }
        int OpCodeMul() {
            int a = access(1, mode->next());
            int b = access(2, mode->next());
            int t = memory[pos+3];
            memory[t] = a*b;
            //printf("opcode 2: %d %d %d\n", a, b, t);
            return 4;
        }
        int OpCodeRead() {
            int param = memory[pos+1];
            int input = Read();
            memory[param] = input;
            //std::cout << "opcode 3: param = " << param << ", input = " << input << std::endl;
            return 2;
        }
        int OpCodePrint() {
            int param = access(1, mode->next());
            //std::cout << "opcode 4: param = " << param << std::endl;
            printf("instruction %d: %d\n", pos, param);
            return 2;
        }
        int OpCodeExit() {
            return -1;
        }

        void p() {
            for (int i: memory) {
                printf("%d ", i);
            }
            printf("\n");
        }
};

int main()
{
    std::string str = "3,225,1,225,6,6,1100,1,238,225,104,0,1,192,154,224,101,-161,224,224,4,224,102,8,223,223,101,5,224,224,1,223,224,223,1001,157,48,224,1001,224,-61,224,4,224,102,8,223,223,101,2,224,224,1,223,224,223,1102,15,28,225,1002,162,75,224,1001,224,-600,224,4,224,1002,223,8,223,1001,224,1,224,1,224,223,223,102,32,57,224,1001,224,-480,224,4,224,102,8,223,223,101,1,224,224,1,224,223,223,1101,6,23,225,1102,15,70,224,1001,224,-1050,224,4,224,1002,223,8,223,101,5,224,224,1,224,223,223,101,53,196,224,1001,224,-63,224,4,224,102,8,223,223,1001,224,3,224,1,224,223,223,1101,64,94,225,1102,13,23,225,1101,41,8,225,2,105,187,224,1001,224,-60,224,4,224,1002,223,8,223,101,6,224,224,1,224,223,223,1101,10,23,225,1101,16,67,225,1101,58,10,225,1101,25,34,224,1001,224,-59,224,4,224,1002,223,8,223,1001,224,3,224,1,223,224,223,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,1108,226,226,224,102,2,223,223,1005,224,329,101,1,223,223,107,226,226,224,1002,223,2,223,1005,224,344,1001,223,1,223,107,677,226,224,102,2,223,223,1005,224,359,101,1,223,223,7,677,226,224,102,2,223,223,1005,224,374,101,1,223,223,108,226,226,224,102,2,223,223,1006,224,389,101,1,223,223,1007,677,677,224,102,2,223,223,1005,224,404,101,1,223,223,7,226,677,224,102,2,223,223,1006,224,419,101,1,223,223,1107,226,677,224,1002,223,2,223,1005,224,434,1001,223,1,223,1108,226,677,224,102,2,223,223,1005,224,449,101,1,223,223,108,226,677,224,102,2,223,223,1005,224,464,1001,223,1,223,8,226,677,224,1002,223,2,223,1005,224,479,1001,223,1,223,1007,226,226,224,102,2,223,223,1006,224,494,101,1,223,223,1008,226,677,224,102,2,223,223,1006,224,509,101,1,223,223,1107,677,226,224,1002,223,2,223,1006,224,524,1001,223,1,223,108,677,677,224,1002,223,2,223,1005,224,539,1001,223,1,223,1107,226,226,224,1002,223,2,223,1006,224,554,1001,223,1,223,7,226,226,224,1002,223,2,223,1006,224,569,1001,223,1,223,8,677,226,224,102,2,223,223,1006,224,584,101,1,223,223,1008,677,677,224,102,2,223,223,1005,224,599,101,1,223,223,1007,226,677,224,1002,223,2,223,1006,224,614,1001,223,1,223,8,677,677,224,1002,223,2,223,1005,224,629,101,1,223,223,107,677,677,224,102,2,223,223,1005,224,644,101,1,223,223,1108,677,226,224,102,2,223,223,1005,224,659,101,1,223,223,1008,226,226,224,102,2,223,223,1006,224,674,1001,223,1,223,4,223,99,226";

    auto machine = new IntCode(str);
    machine->input.push_back(1);
    machine->Run();

    return 0;
}