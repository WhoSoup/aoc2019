#include <iostream>
#include <fstream>
#include <string>

std::string data;
int width, height;

char access(int layer, int x, int y) {
    return data[layer*height*width + y*width + x];
}

int main() {
    width = 25;
    height = 6;

    std::ifstream in("input.txt");
    in >> data;

    int totalLayers = data.size() / width / height ;

    int minLayer = width * height, multiple;
    
    for (int l = 0; l < totalLayers; l++) {
        int count[3] = {0,0,0};
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                count[access(l, x, y) - '0']++;
            }
        }
        if (count[0] < minLayer) {
            minLayer = count[0];
            multiple = count[1] * count[2];
        }
    }

    printf("Part One: %d\n", multiple);

    printf("Part Two (%d x %d):\n", width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int firstc = 0;
            for (int l = 0; l < totalLayers; l++) {
                if (access(l,x,y) != '2') {
                    firstc = access(l,x,y);
                    break;
                }
            }
            if (firstc == '0') {
                printf(" ");
            } else if (firstc == '1') {
                printf("X");
            } else {
                printf("."); // shouldn't happen
            }
        }
        printf("\n");
    }
}