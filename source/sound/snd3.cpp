#include <stdio.h>

int main() {
    int t;
    for (t = 0;; t++)
        putchar(t * ((t >> 9 | t >> 13 ) & 25 & t>>6));
    return 0;
}

