#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned int *buffer;
unsigned int *bufMap;
unsigned int buffersize;
unsigned int insertMemory(unsigned int addr) {
    realloc(buffer,(++buffersize) * sizeof(unsigned int));
    realloc(bufMap,buffersize * sizeof(unsigned int));
    buffer[buffersize - 1] = 0xFFFFFFFF;
    bufMap[buffersize - 1] = 0xFFFFFFFF;
    unsigned int z,a;
    for (z=1;z<buffersize;z++) {
        if (bufMap[z] > addr && bufMap[z-1] < addr) {
            for (a=buffersize-1;a>z;a++) {
                bufMap[a] = bufMap[a-1];
                buffer[a] = bufMap[a-1];
            }
            buffer[z] = 0;
            bufMap[z] = addr;
            return addr;
        }
    }
}
unsigned int getLocation(unsigned int x) {
    unsigned int y;
    for (y=0;y<buffersize;y++) {
        if (bufMap[y] == x) return y;
    }
    return -1;
}
unsigned int getMemoryAddrFromCursor(unsigned int c) {
    unsigned int l = getLocation(c);
    if (l == -1) {
        return insertMemory(c);
    } else return l;
}
int main(int argc, char *kwargs[]) {
    char *code;
    buffer = calloc(1,sizeof(int));
    bufMap = calloc(1,sizeof(int));
    buffersize=1;
    unsigned int cursor=0;
    if (argc > 1) code = kwargs[1];
    else code = "+++++[->++++++++++++++<]>.";
    int length = strlen(code);
    int x,y;
    int *stack = calloc(length / 2,sizeof(int));
    int *pairs = calloc(length,sizeof(int));
    int sOff = 0;
    int pOff = 0;
    int inc = 1;
    int temp;
    bufMap[0] = 0;
    for (x=0;x<length;x++) {
        if (code[x] == '[') stack[sOff++] = x;
        if (code[x] == ']') {
            pairs[pOff++] = stack[--sOff];
            pairs[pOff++] = x;
        }
    }
    free(stack);
    for (x=0;x<length;x+=inc) {
        inc = 1;
        switch(code[x]) {
            case '-':
                buffer[getMemoryAddrFromCursor(cursor)]--;
            break;

            case '+':
                buffer[getMemoryAddrFromCursor(cursor)]++;
            break;

            case '>':
                cursor++;
            break;
            case '<':
                cursor--;
            break;
            case '.':
                putchar(buffer[getMemoryAddrFromCursor(cursor)]);
            break;
            case ',':
                buffer[getMemoryAddrFromCursor(cursor)] = getchar();
            break;
            case '[':
                if (!buffer[getMemoryAddrFromCursor(cursor)]) {
                    for (y=0;y<length;y+=2) {
                        if (x == pairs[y]) { temp = y + 1; break; }
                    }
                    x = pairs[temp];
                }
            break;
            case ']':
                if (buffer[getMemoryAddrFromCursor(cursor)]) {
                    for (y=1;y<length;y+=2) {
                        if (x == pairs[y]) { temp = y - 1; break; }
                    }
                    x = pairs[temp];
                }
            break;
            case '*':
                cursor = buffer[getMemoryAddrFromCursor(cursor)];
            break;
            case '&':
                buffer[getMemoryAddrFromCursor(cursor)] = x;
            break;
            case '$':
                x = buffer[getMemoryAddrFromCursor(cursor)];
                inc = 0;
            break;
        }
    }
    return 0;
}
