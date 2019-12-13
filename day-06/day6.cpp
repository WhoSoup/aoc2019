#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

class UFO {
    public:
        UFO(std::string name) {
            this->name = name;
        }
        void AddOrbit(UFO * oo) {
            orbiting.push_back(oo);
        }
        void Orbiting(UFO * oo) {
            parent = oo;
        }
        int Orbits(int depth = 0) {
            int o = depth;
            for (UFO * x: orbiting) {
                o += x->Orbits(depth + 1);
            }
            return o;
        }
        bool HasInOrbit(std::string target) {
            if (name.compare(target) == 0) {
                return true;
            }

            for (UFO * x : orbiting) {
                if (x->HasInOrbit(target)) {
                    return true;
                }
            }
            return false;
        }
    bool visited = false;
    std::string name;
    std::vector<UFO*> orbiting;
    UFO * parent = NULL;
};

class Galaxy {
    public:
        Galaxy() {}
        UFO * Lookup(std::string name) {
            std::map<std::string, UFO*>::iterator iter = m.find(name);
            if (iter != m.end()) {
                return iter->second;
            }

            UFO * tmp = new UFO(name);
            m[name]= tmp;
            return tmp;
        }
    std::map<std::string, UFO*> m;
};

int main() {
    std::ifstream in("input.txt");
    std::string first, second;

    Galaxy * galaxy = new Galaxy();

    while (std::getline(in, first, ')') && std::getline(in, second, '\n')) {
        UFO * a = galaxy->Lookup(first);
        UFO * b = galaxy->Lookup(second);
        a->AddOrbit(b);
        b->Orbiting(a);
    }

    std::cout << "Part 1: " << galaxy->Lookup("COM")->Orbits() << std::endl;

    UFO * position = galaxy->Lookup("YOU");
    int moves = 0;
    MOO:
    while (position->name.compare("SAN") != 0) {
        moves++;
        for (UFO * x : position->orbiting) {
            if (x->HasInOrbit("SAN")) {
                position = x;
                goto MOO;
            }
        }
        position = position->parent;
    }

    std::cout << "Part 2: " << (moves-2) << std::endl;

    return 0;
}