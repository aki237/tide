#include "Text.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef Node<int> Train;

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}


int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

void Print(Train* t) {
    Train* temp = t;
    while (temp->next != nullptr) {
	std::cout << temp->data << std::endl;
	temp = temp->next;
    }
}

int main(int argc, char *argv[]) {
    Train* t = new Train();
    Train* cursor = t;
    for (int i = 1 ; i <= 100 ; i++) {
	cursor->data = i * 2;
	cursor = cursor->NewAfter();
    }
    Print(t);
    return 0;
}
