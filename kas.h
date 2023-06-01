
#ifndef __KAS_H__
#define __KAS_H__

#include "kasdef.h"

extern kas	kas_new		(const char* _init_string)
	__nonnull((1));

extern void	kas_free	(kas* _string)
	__nonnull((1));

extern int	kas_len		(const kas _s)
	__nonnull((1));

extern kas	kas_append	(kas* _dest_string, const char* _src_string)
	__nonnull((1, 2));

extern kas	kas_concat	(const kas _s0, const kas _s1)
	__nonnull((1,2));

extern kas kas_sub(kas s, __uint32_t b, __uint32_t e)
	__nonnull((1));

extern bool	kas_empty	(const kas _s)
	__nonnull((1));

extern kas	kas_clone		(const kas _s)
	__nonnull((1));

extern int	kas_index	(const kas _s, const char * _sub_s)
	__nonnull((1,2));

extern bool kas_find	(const kas _s, const char* _sub_s)
	__nonnull((1,2));

extern int	kas_count	(const kas _s, const char* _sub_s)
	__nonnull((1,2));

extern bool kas_startswith	(const kas _s, const char* _start)
	__nonnull((1,2));

extern bool kas_endswith	(const kas _s, const char* _ends)
	__nonnull((1,2));

/* 
The type of `s` is [ kas * ]
The type of `sep` is [ char * ], and also be use [ kas ]
`sep` mean is separator
eg: kas str = kas_join(&str, " ", "hello", "world");
*/
#define kas_join(s, sep, ...) \
	_kas_join(s, sep, ##__VA_ARGS__, NULL)

extern kas	_kas_join	(kas * _s, const char* sep, ...) \
	__nonnull((1));

/*
This function will replace the `s0` in `_s` with `s1`.
*/
extern kas	kas_replace	(kas* _s, const char* s0, const char* s1) \
	__nonnull((1,2,3));


extern kas kas_sub		(kas _s, __uint32_t begin, __uint32_t end) \
	__nonnull((1));

#define kas_reset(s, ...) \
	_kas_reset(s, (v_str){__VA_FIRST(__VA_ARGS__)})

extern kas _kas_reset	(kas* _s, v_str _c) \
	__nonnull((1));

#if defined(KAS_MEMORY_MANAGE)

extern void kas_exclfree	(size_t num, ...);

extern void kas_freeany();

extern void kas_freeall();

extern void kas_deprive		(kas _s)
	__nonnull((1));

extern void kas_liberate	(kas _s)
	__nonnull((1));

extern bool kas_isfree		(kas s);

#endif
#endif