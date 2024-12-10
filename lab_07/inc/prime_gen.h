#ifndef PRIME_GEN_H__
#define PRIME_GEN_H__

#include <stdbool.h>
#include <stddef.h>

inline size_t calc_next_prime_num(size_t num)
{
    size_t new_num = num + 1;
    while (true)
    {
        bool ok = true;
        for (size_t i = 2; ok && i < new_num / 2 + 1; i++)
        {
            if (new_num % i == 0)
                ok = false;
        }

        if (ok)
            return new_num;

        new_num++;
    }
}

#endif // PRIME_GEN_H__
