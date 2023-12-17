#include <stdio.h>

int main() {
    int t;
    for (t = 0;; t++)
        putchar(t * (t>>11&t>>8&123&t>>3));
    return 0;
}

