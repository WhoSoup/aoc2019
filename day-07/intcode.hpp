#include <iostream>
#include <string>
#include <sstream>
#include "mode.hpp"



class IntCode {
    public:
        IntCode(std::string input) {
            Load(input);
        }
        void Load(std::string program) {
            inputter = 0;
            memory.clear();
            input.clear();
            output.clear();
            std::stringstream ss(program);

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
        std::vector<int> output;
    private:
        int pos;
        int inputter;
        Mode * mode;
        int access(int spot) {
            int m = mode->next();
            if (m == 1) {
                return memory[pos+spot];
            }
            return memory[memory[pos+spot]];
        }
        int OpCodeAdd() {
            int a = access(1);
            int b = access(2);
            int t = memory[pos+3];
            memory[t] = a+b;
            return 4;
        }
        int OpCodeMul() {
            int a = access(1);
            int b = access(2);
            int t = memory[pos+3];
            memory[t] = a*b;
            return 4;
        }
        int OpCodeRead() {
            int param = memory[pos+1];
            int input = Read();
            memory[param] = input;
            return 2;
        }
        int OpCodePrint() {
            int param = access(1);
            output.push_back(param);
            return 2;
        }
        int OpCodeExit() {
            return -1;
        }

        int OpCodeJIT() {
            int a = access(1);
            int b = access(2);
            if (a != 0) {
                return b;
            }
            return pos + 3;
        }

        int OpCodeJIF() {
            int a = access(1);
            int b = access(2);
            if (a == 0) {
                return b;
            }
            return pos + 3;
        }

        int OpCodeLessThan() {
            int a = access(1);
            int b = access(2);
            int t = memory[pos+3];
            memory[t] = (a < b ? 1 : 0);
            return 4;
        }

        int OpCodeEQ() {
            int a = access(1);
            int b = access(2);
            int t = memory[pos+3];
            memory[t] = (a == b ? 1 : 0);
            return 4;
        }

        void p() {
            for (int i: memory) {
                printf("%d ", i);
            }
            printf("\n");
        }
};