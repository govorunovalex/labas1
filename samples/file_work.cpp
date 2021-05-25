#include "tset.h"
#include <stdlib.h>

// Доп. Задание 2
int main(int argc, char** argv){
    TSet s(1000);
    if(!s.ReadFromFile(argv[1])){
        std::cout <<"There was no: " << argv[1] << ", so that it will be created!\n";
    }
    s.InsElem(atoi(argv[2]));
    s.WriteToFile(argv[1]);
    return 0;
}