#include <stdio.h>

void test(int a, int b) {
    printf("%d %d\n", a, b);
}
  /* overloaded function */
void test(char *a, int b) {
    printf("%s %d\n", a, b);
}
void main() {
    test(1,2);
    test("abc", 1);
}
