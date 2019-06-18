#include <assert.h>
#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//show 함수를 사용할때 내부적으로 사용할 버퍼의 길이입니다.
#define __BITS_SHOWING_BUFFER_LEN__ 65536

inline bool __is_big_endian()
{
    const uint32_t n = 1;
    return ((const uint8_t*)&n)[3] == 1;
}
inline bool __is_little_endian()
{
    const uint32_t n = 1;
    return ((const uint8_t*)&n)[0] == 1;
}

bool __bitfield_is_big()
{
    union bit_and_byte_union
    {
        unsigned char byte;
        struct 
        {
            unsigned char _1: 1;
            unsigned char _2: 1;
            unsigned char _3: 1;
            unsigned char _4: 1;
            unsigned char _5: 1;
            unsigned char _6: 1;
            unsigned char _7: 1;
            unsigned char _8: 1;
        };
    } hmm;
    hmm.byte=1;
    return hmm._1==1;
}

union __bit_and_byte
{
    uint8_t byte;
    struct 
    {
        unsigned char _1: 1;
        unsigned char _2: 1;
        unsigned char _3: 1;
        unsigned char _4: 1;
        unsigned char _5: 1;
        unsigned char _6: 1;
        unsigned char _7: 1;
        unsigned char _8: 1;
    };
};
/* T: 바꿀 타입
/* value: 바꿀 값
/* dest: 문자열 저장할 목적지(char*)
/*	*/
#define as_bits(T, _value, /*char**/_dest) \
{ \
	T value = _value; \
	char* dest = _dest; \
	enum {byte_size=sizeof(T)}; \
	union __bit_and_byte bytes[byte_size]; \
	\
	/*엔디안체크*/ \ 
	if(__is_big_endian()) \
        for(int i=0; i<byte_size; ++i) \
            bytes[i].byte = ((const uint8_t*)&value)[i]; \
    else if(__is_little_endian()) \
        for(int i = 0, ri=byte_size-1; ri>=0; ++i, --ri) \
            bytes[i].byte = ((const uint8_t*)&value)[ri]; \
    else \
        assert(!"unknown endian"); \
    \
    \ 
    enum{to_inc = '0'- (char)0}; \
	\
	size_t str_index = 0;\
	for(size_t i=0; i<byte_size; i++) \
	{ \
		union __bit_and_byte byte = bytes[i]; \
		\
		if(__bitfield_is_big()) /*비트필드 순서 처리*/\
        { \
            dest[str_index++]=byte._8+to_inc; \
            dest[str_index++]=byte._7+to_inc; \
            dest[str_index++]=byte._6+to_inc; \
            dest[str_index++]=byte._5+to_inc; \
            dest[str_index++]=byte._4+to_inc; \
            dest[str_index++]=byte._3+to_inc; \
            dest[str_index++]=byte._2+to_inc; \
            dest[str_index++]=byte._1+to_inc; \
        } \
        else \
        { \
        	dest[str_index++]=byte._1+to_inc; \
        	dest[str_index++]=byte._2+to_inc; \
        	dest[str_index++]=byte._3+to_inc; \
        	dest[str_index++]=byte._4+to_inc; \
        	dest[str_index++]=byte._5+to_inc; \
        	dest[str_index++]=byte._6+to_inc; \
        	dest[str_index++]=byte._7+to_inc; \
        	dest[str_index++]=byte._8+to_inc; \
        } \
        dest[str_index++]=' '; \
	} \
    \
    dest[--str_index] = '\0'; \
}

/*T: 바꿀 타입
/*value: 바꿀 값
/*출력만 해줌
/*	*/
#define show_as_bits(T, value) \
{ \
	char buffer[__BITS_SHOWING_BUFFER_LEN__]; \
	as_bits(T, value, buffer) \
	puts(buffer); \
}

#else
assert(!"Unsupported Compiler Version");
#endif

