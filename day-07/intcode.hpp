#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include "mode.hpp"


class IntCode {
    public:
        IntCode(std::string input) {
            Load(input);
        }
        void Load(std::string program) {
            memory.clear();
            std::stringstream ss(program);
            pos = 0;
            for (int i; ss >> i;) {
                memory.push_back(i);    
                if (ss.peek() == ',')
                    ss.ignore();
            }           
        }
        void Set(int p, int val) {
            memory[p] = val;
        }
        void Feed(int i) {
            input.push(i);
        }
        bool HasOutput() {
            return !output.empty();
        }
        int Output() {
            int r = output.front();
            output.pop();
            return r;
        }
        bool Run() {
            
            while (pos != -1) {
                mode = new Mode(memory[pos]);

                /*for (int i = 0; i < memory.size(); i++) {
                    if (i > 0) {
                        printf(",");
                    }
                    if (i == pos) {
                        printf(">%d<", memory[i]);
                    } else {
                        printf("%d", memory[i]);
                    }
                }
                printf("\n");*/

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
                    case 99:
                        pos = OpCodeExit(); // assign to pos
                        break;
                    default:
                        std::cout << "Invalid OpCode: " << memory[pos] << "(" << mode->opcode() << ")" << std::endl;
                        return false;
                }

                /*for (int i = 0; i < memory.size(); i++) {
                    if (i > 0) {
                        printf(",");
                    }
                    if (i == pos) {
                        printf(">%d<", memory[i]);
                    } else {
                        printf("%d", memory[i]);
                    }
                }
                printf("\n======================================\n");*/


                delete mode;
            }
            return true;
        }
        std::vector<int> memory;
        std::queue<int> input;
        std::queue<int> output;
    private:
        int pos;
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
//            printf("Add(%d, %d) => %d\n", a,b,t);
            return 4;
        }
        int OpCodeMul() {
            int a = access(1);
            int b = access(2);
            int t = memory[pos+3];
            memory[t] = a*b;
//            printf("Mul(%d, %d) => %d\n", a,b,t);
            return 4;
        }
        int OpCodeRead() {
            int param = memory[pos+1];
            int in = input.front();
            input.pop();
            memory[param] = in;
//            printf("Read[%d] = %d\n", param, in);
            return 2;
        }
        int OpCodeOutput() {
            int param = access(1);
            output.push(param);
//            printf("Output = %d\n", param);
            return 2;
        }
        int OpCodeExit() {
//            printf("Exit\n");
            return -1;
        }

        int OpCodeJIT() {
            int a = access(1);
            int b = access(2);
//            printf("Jump to %d? %d\n", b, a!=0);
            if (a != 0) {
                return b;
            }
            return pos + 3;
        }

        int OpCodeJIF() {
            int a = access(1);
            int b = access(2);
//            printf("Jump to %d? %d\n", b, a==0);
            if (a == 0) {
                return b;
            }
            return pos + 3;
        }

        int OpCodeLessThan() {
            int a = access(1);
            int b = access(2);
            int t = memory[pos+3];
//            printf("LessThan[%d] = %d < %d\n", t, a, b);
            memory[t] = (a < b ? 1 : 0);
            return 4;
        }

        int OpCodeEQ() {
            int a = access(1);
            int b = access(2);
            int t = memory[pos+3];
//            printf("Equal[%d] = %d < %d\n", t, a, b);
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