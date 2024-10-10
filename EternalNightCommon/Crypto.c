#include "Crypto.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

uint32_t crypto_buffer[0x500];

void crypto_initialize(void)
{
    uint32_t seed = 0x0010001;

    for (int index1 = 0; index1 < 0x100; index1++)
    {
        int index2 = index1;
        for (int i = 0; i < 5; i++)
        {
            seed = (seed * 125 + 3) % 0x2AAAAB;
            const uint32_t temp1 = (seed & 0xFFFF) << 0x10;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            const uint32_t temp2 = seed & 0xFFFF;
            crypto_buffer[index2] = temp1 | temp2;
            index2 += 0x100;
        }
    }
}

void crypto_decrypt(uint32_t* data, const size_t size, uint32_t seed)
{
    uint32_t seed2 = 0xeeeeeeee;
    for (size_t i = 0; i < size; i++)
    {
        seed2 += crypto_buffer[0x400 + (seed & 0xFF)];
        uint32_t result = data[i];
        result ^= seed + seed2;
        seed = (~seed << 21) + 0x11111111 | seed >> 11;
        seed2 = result + seed2 + (seed2 << 5) + 3;
        data[i] = result;
    }
}

uint32_t crypto_hash_string(const char* key, const uint32_t hashType)
{
    uint32_t seed1 = 0x7FED7FED;
    uint32_t seed2 = 0xEEEEEEEE;

    /* prepare seeds. */
    for (size_t i = 0; i < strlen(key); i++)
    {
        const char upper_char = toupper(key[i]);
        seed1 = crypto_buffer[hashType * 0x100 + (uint32_t)upper_char] ^ seed1 + seed2;
        seed2 = (uint32_t)upper_char + seed1 + seed2 + (seed2 << 5) + 3;
    }

    return seed1;
}

uint64_t crypto_hash_filename(const char* key)
{
    const uint32_t a = crypto_hash_string(key, 1);
    const uint32_t b = crypto_hash_string(key, 2);
    return (uint64_t)a << 32 | (uint64_t)b;
}

void crypto_decrypt_bytes(uint8_t* data, const size_t size, uint32_t seed)
{
    uint32_t seed2 = 0xEEEEEEEE;

    for (size_t i = 0; i < size - 3; i += 4)
    {
        seed2 += crypto_buffer[0x400 + (seed & 0xFF)];
        uint32_t result = data[i + 0] | data[i + 1] << 8 | data[i + 2] << 16 | data[i + 3] << 24;
        result ^= seed + seed2;
        seed = (~seed << 21) + 0x11111111 | seed >> 11;
        seed2 = result + seed2 + (seed2 << 5) + 3;

        data[i + 0] = (uint8_t)(result & 0xFF);
        data[i + 1] = (uint8_t)(result >> 8 & 0xFF);
        data[i + 2] = (uint8_t)(result >> 16 & 0xFF);
        data[i + 3] = (uint8_t)(result >> 24 & 0xFF);
    }
}

int crypto_decrypt_table(read_stream* r, uint32_t size, const char* name, uint32_t** table) {
    uint32_t seed = crypto_hash_string(name, 3);
    uint32_t seed2 = 0xEEEEEEEE;
    size *= 4;

    *table = (uint32_t*)malloc(size * sizeof(uint32_t));
    if (*table == NULL) {
        return -1; // Allocation error
    }

    for (uint32_t i = 0; i < size; i++)
    {
        uint8_t buf[4];
        seed2 += crypto_buffer[0x400 + (seed & 0xFF)];

        if (read_stream_read_bytes(r, buf, 4) != 4) {
            free(*table);
            return -2;
        }

        uint32_t result = buf[0] | buf[1] << 8 | buf[2] << 16 | buf[3] << 24;
        result ^= seed + seed2;

        seed = (~seed << 21) + 0x11111111 | seed >> 11;
        seed2 = result + seed2 + (seed2 << 5) + 3;
        (*table)[i] = result;
    }

    return 0;
}
