#pragma once

#include "ReadStream.h"

#include <stdint.h>

void crypto_initialize(void);
void crypto_decrypt(const uint32_t * data, size_t size, uint32_t seed);
uint32_t crypto_hash_string(const wchar_t * filename);
uint64_t crypto_hash_filename(const char* key);
void crypto_decrypt_bytes(uint8_t* data, size_t size, uint32_t seed);
int crypto_decrypt_table(read_stream* r, uint32_t size, const char* name, uint32_t** table);

