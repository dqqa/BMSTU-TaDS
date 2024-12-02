#ifndef HASHER_H__
#define HASHER_H__

#include <assert.h>
#include <stddef.h>

/**
 * @brief Хэширует строку
 *
 * @param str Указатель на строку
 * @return size_t Хэш
 */
inline size_t calc_hash_str(const char *str)
{
    assert(str);

    size_t hash = *str;
    if (hash)
    {
        while (*str != '\0')
        {
            hash = (hash << 5) - hash + (size_t)*str;
            str++;
        }
    }

    return hash;
}

#endif // HASHER_H__
