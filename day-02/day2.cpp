#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <stdio.h>

class IntCode {
    public:
        IntCode(std::string input) {
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
        void Set(int pos, int val) {
            memory[pos] = val;
        }
        int Run() {
            int pos = 0;
            while (pos != -1) {
                switch (memory[pos]) {
                    case 1:
                        pos = OpCodeAdd(pos);
                        break;
                    case 2:
                        pos = OpCodeMul(pos);
                        break;
                    case 99:
                        pos = OpCodeExit(pos);
                        break;
                    default:
                        std::cout << "Invalid OpCode" << memory[pos] << std::endl;
                        return -999;
                }
            }
            return memory[0];
        }
    private:
        std::vector<int> memory;
        int OpCodeAdd(int p) {
            int a = memory[p+1];
            int b = memory[p+2];
            int t = memory[p+3];
            memory[t] = memory[a] + memory[b];
            return p + 4;
        }
        int OpCodeMul(int p) {
            int a = memory[p+1];
            int b = memory[p+2];
            int t = memory[p+3];
            memory[t] = memory[a] * memory[b];
            return p + 4;
        }
        int OpCodeExit(int p) {
            return -1;
        }
};

int main()
{
    std::string str = "1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,10,1,19,1,19,9,23,1,23,6,27,2,27,13,31,1,10,31,35,1,10,35,39,2,39,6,43,1,43,5,47,2,10,47,51,1,5,51,55,1,55,13,59,1,59,9,63,2,9,63,67,1,6,67,71,1,71,13,75,1,75,10,79,1,5,79,83,1,10,83,87,1,5,87,91,1,91,9,95,2,13,95,99,1,5,99,103,2,103,9,107,1,5,107,111,2,111,9,115,1,115,6,119,2,13,119,123,1,123,5,127,1,127,9,131,1,131,10,135,1,13,135,139,2,9,139,143,1,5,143,147,1,13,147,151,1,151,2,155,1,10,155,0,99,2,14,0,0";

    auto machine = IntCode(str);

    machine.Set(1,12);
    machine.Set(2,2);
    std::cout << machine.Run() << std::endl;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            machine.Load(str);
            machine.Set(1, i);
            machine.Set(2, j);

            int n = machine.Run();
            if (n == 19690720) {
                std::cout << (100 * i + j) << std::endl;
                goto end;
            }
        }
    }
    std::cout << "Nothing found" << std::endl;
    end:
    return 0;
}