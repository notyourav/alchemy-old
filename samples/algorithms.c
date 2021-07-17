//
// Created by Theo on 4/18/21.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

long calc_fib(long x) {
    if (x <= 1)
        return x;
    return calc_fib(x - 1) + calc_fib(x - 2);
}

void do_sqrt(long x) {
    printf("%f\n", sqrt(x));
}

void fib(long x) {
    printf("%ld\n", calc_fib(x));
}

void lcg(long x) {
    printf("%ld\n", (1103515245 * x + 12345) & 0x7FFFFFFF);
}

int main() {
    char input1[200];
    char input2[200];
    long num = 0;

    printf("Enter integer: ");
    scanf("%s", input1);
    num = atol(input1);

    printf("Choose algorithm:\n"
           "f - fibonacci\n"
           "l - LCG seed\n"
           "s - stl sqrt()\n"
           "Choice: ");
    scanf("%s", input2);

    switch (input2[0]) {
        case 'f':
            fib(num);
            break;
        case 'l':
            lcg(num);
            break;
        case 's':
            do_sqrt(num);
            break;
    }
    return 0;
}
