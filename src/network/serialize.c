#include <stdint.h>
#include <arpa/inet.h>

// Serialize a network to a buffer

void pack_i16(char* buf, int16_t val)
{
    buf[0] = val>>8;
    buf[1] = val;
}

void pack_i32(char* buf, int32_t val)
{
    buf[0] = val>>24;
    buf[1] = val>>16;
    buf[2] = val>>8;
    buf[3] = val;
}

void pack_i64(char* buf, int64_t val)
{
    buf[0] = val>>56;
    buf[1] = val>>48;
    buf[2] = val>>40;
    buf[3] = val>>32;
    buf[4] = val>>24;
    buf[5] = val>>16;
    buf[6] = val>>8;
    buf[7] = val;
}

uint16_t unpack_i16(char* buf)
{
    return (buf[0] << 8) | buf[1];
}

uint32_t unpack_i32(char* buf)
{
    return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

int64_t unpack_i64(char* buf)
{
    return ((int64_t)buf[0] << 56) | ((int64_t)buf[1] << 48) | ((int64_t)buf[2] << 40) | ((int64_t)buf[3] << 32) | ((int64_t)buf[4] << 24) | ((int64_t)buf[5] << 16) | ((int64_t)buf[6] << 8) | buf[7];
}


