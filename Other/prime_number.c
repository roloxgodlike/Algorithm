#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <time.h>

/**
 * @brief  判断是否是质数
 * @note
 * @param  n:
 * @retval
 */
char is_prime(uint64_t n)
{
    if (n == 1 || n == 2)
        return 1;
    if ((n % 2) == 0)
        return 0;

    uint64_t half = n / 2;
    for (uint64_t i = 3; i < half; i += 2)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

/**
 * @brief  得到从小到大第n个质数
 * @note
 * @param  n:
 * @retval
 */
uint64_t get_prime(uint32_t n)
{
    if (n == 0)
        return 1;

    if (n == 1)
        return 2;

    int count = 1;
    for (uint64_t i = 3; i < UINT64_MAX; i += 2)
    {
        if (is_prime(i))
        {
            if (++count == n)
            {
                return i;
            }
        }
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 1000; i++)
        printf("p%d-%d\n", i, get_prime(i));
    return 0;
}
