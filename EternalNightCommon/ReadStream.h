#pragma once

#include <stdint.h>


typedef struct ReadStreamS read_stream;

read_stream* read_stream_create(const char* byteRef, uint32_t size);
void read_stream_destroy(read_stream* stream);
uint8_t read_stream_read_byte(read_stream* stream);
uint32_t read_stream_read_bytes(read_stream* stream, uint8_t* bytes, uint32_t count);
