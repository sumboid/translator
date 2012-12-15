#include <stdint.h>

int32_t foo(int32_t);

int main()
{
    int n;
    scanf("%d", &n);
    printf("Yoba: %d\n", (int)foo((int32_t)n));
    return 0;
}
