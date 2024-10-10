#include "ReadStream.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct ReadStreamS
{
    const char* data;
    size_t index;
    size_t size;
} read_stream;


read_stream* read_stream_create(const char* byteRef, const uint32_t size)
{
    read_stream* stream = malloc(sizeof(read_stream));
    assert(stream != NULL);
    stream->data = byteRef;
    stream->index = 0;
    stream->size = size;

    return stream;
}

void read_stream_destroy(read_stream* stream)
{
    assert(stream != NULL);
    free(stream);
}

uint8_t read_stream_read_byte(read_stream* stream)
{
    assert(stream != NULL);
    assert(stream->index < stream->size);

    return stream->data[stream->index++];
}

uint32_t read_stream_read_bytes(read_stream* stream, uint8_t* bytes, uint32_t count)
{
    assert(stream != NULL);
    assert(stream->index + count <= stream->size);

    memcpy(bytes, stream->data + stream->index, count);
    stream->index += count;

    return count;
}
