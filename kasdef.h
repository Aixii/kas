#ifndef __KADEF_H__
#define __KADEF_H__

#define __VA_FIRST(arg1, ...) arg1

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef char* kas;

/*maybe this is not necessary, but I don't want to change it*/
typedef struct {
	char *str;
}v_str;

#if defined(__KAS_C__)

#if defined(KAS_MEMORY_MANAGE)
typedef struct {
	bool memon;
	__uint8_t hashcode;
}kas_mem_flag;
#endif

#define STRING_INIT_DEFAULT_SIZE 8

typedef struct __attribute__((packed)) __kastring__ {
#if defined(KAS_MEMORY_MANAGE)
	kas_mem_flag mem_flag;
#endif
	__uint32_t size;
	__uint32_t len;
	char str[];
} * kastring;

#if defined(KAS_MEMORY_MANAGE)
typedef struct __mem_node__ {
	void* str;
	struct __mem_node__* next;
}MEM_NODE;


#endif
#endif
#endif
