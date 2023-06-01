

#define __KAS_C__

#include "kasdef.h"
#include <stdio.h>

#if defined(KAS_MEMORY_MANAGE)
static const int PRE_BYTE = sizeof(kas_mem_flag) + sizeof(int) * 2;
#else 
static const int PRE_BYTE = sizeof(__uint32_t) * 2;
#endif

#define __KASTRING(s) \
	((kastring)(((__uint8_t *)(s))-PRE_BYTE))

#define __LEN_(s) ((s)->len)
#define __SIZE(s) ((s)->size)
#define __STR_(s) ((s)->str)

#if defined(KAS_MEMORY_MANAGE)
#define __HASH(s) (((s)->mem_flag).hashcode)
#define _MEMON(s) (((s)->mem_flag).memon)
#endif

#define null_return(r,...) \
	do{ \
		int i = 0; \
		void * pointers[] = {__VA_ARGS__,(void *)-1}; \
		while(pointers[i] != (void *)-1) { \
			if(pointers[i] == NULL) return r; \
			i++; \
		} \
	}while(0)

#define cond_return(r, ...) \
	do { \
		int i = 0; \
		char conds[] = {__VA_ARGS__,-1}; \
		while(conds[i]!=-1) { \
			if(conds[i]==1) return r; \
			i++; \
		} \
	} while(0)

#if defined(KAS_MEMORY_MANAGE)

#define MEM_OBJECT_SIZE 32

static MEM_NODE* MEM_OBJECTS[MEM_OBJECT_SIZE];

static int hash(kastring s) {
	return ((__uint64_t)s & 31) ^ (((__uint64_t)s & 0x3E0) >> 5);
}
#endif

static void _kas_free_(kastring s);
static kastring init(size_t charnum);
static kastring grow(kastring* s);
static kastring append(kastring* dest, const char* src);
//static kastring copy(kastring* dest, kastring src);

static kastring init(size_t charnum) {
	__uint32_t size = PRE_BYTE + (sizeof(char) * charnum);
	kastring s = (kastring)malloc(size);
	__SIZE(s) = charnum;
	__LEN_(s) = 0;

#if defined(KAS_MEMORY_MANAGE)
	_MEMON(s) = 1;
	__HASH(s) = (__uint8_t)hash(s);
	MEM_NODE* p = MEM_OBJECTS[__HASH(s)];
	MEM_NODE* m = (MEM_NODE *)malloc(sizeof(MEM_NODE));
	m->str = s;
	m->next = NULL;

	while (p && p->next) p = p->next;

	if (p) p->next = m;
	else MEM_OBJECTS[__HASH(s)] = m;

#endif
	return s;
}

static void _kas_free_(kastring s) {

#if defined(KAS_MEMORY_MANAGE)
	MEM_NODE* p = MEM_OBJECTS[__HASH(s)], * q = NULL;
	while (p) {
		if (p->str == s) {
			if (!q)
				MEM_OBJECTS[__HASH(s)] = p->next;
			else
				q->next = p->next;
			free(p);
		}
		q = p;
		p = p->next;
	}
#endif
	free(s);
}

static kastring grow(kastring* s) {
	int newsize = __SIZE(*s) * 2;
	kastring newstr = init(newsize);
	append(&newstr, __STR_(*s));
#if defined(KAS_MEMORY_MANAGE)
	_MEMON(newstr) = _MEMON(*s);
#endif
	_kas_free_(*s);
	*s = newstr;
	return newstr;
}
/*
edited on 2023/5/19 
version: 0.1
copy src to dest, dest and src must be initialization
the contents of dest will be overwritten by the contents of src
2023/5/25 change
*/
//static kastring copy(kastring* dest, kastring src) {
//	kastring str = *dest;
//	__LEN_(str) = 0;
//	append(&str, __STR_(src));
//	*dest = str;
//	return str;
//}

/*
edited on 2023/5/19
version: 0.1
*/
static kastring append(kastring* dest, const char* src) {
#if				__WORDSIZE==64
#define KAS_WORD_TYPE __uint64_t
#elif			__WORDSIZE==32
#define KAS_WORD_TYPE __uint32_t
#else
#define KAS_WORD_TYPE __uint8_t
#endif
	
	kastring str = *dest;

	int n = strlen(src);
	int newlen = __LEN_(str) + n;

	KAS_WORD_TYPE* p, * q;
	char* k, * l;
	
	while (__SIZE(str) < newlen) {
		grow(&str);
	}

	p = (KAS_WORD_TYPE*)(__STR_(str) + __LEN_(str));
	q = (KAS_WORD_TYPE*)src;

	__LEN_(str) += n;
	n /= sizeof(KAS_WORD_TYPE);

	while (n--) *p++ = *q++;

	if ((char*)q == src) {
		k = (char *)p;
		l = (char *)q;
	}else {
		k = (char*)(p - 1);
		l = (char*)(q - 1);
	}

	while (*l) *k++ = *l++;
	*k = '\0';

	*dest = str;
	return str;
}

/*
edited on 2023/5/19
version: 0.1
*/
static kastring clone(kastring s) {
	kastring dest = init(__SIZE(s));
#if defined(KAS_MEMORY_MANAGE)
	_MEMON(dest) = _MEMON(s);
#endif
	append(&dest, __STR_(s));
	return dest;
}
/*
edited on 2023/5/19
version: 0.1
*/
static kastring clear(kastring s) {
	__STR_(s)[0] = '\0';
	__LEN_(s) = 0;
	return s;
}
/*
edited on 2023/5/19
version: 0.1
*/
kas kas_new(const char * str) {
	kastring newstr = init(STRING_INIT_DEFAULT_SIZE);
	return str ? __STR_(append(&newstr, str)) : __STR_(newstr);
}

/*
edited on 2023/5/19
version: 0.1
*/
void kas_free(kas* s) {
	null_return(, s);
	null_return(, *s);
	kastring str = __KASTRING(*s);
	_kas_free_(str);
	*s = NULL;
}

/*
edited on 2023/5/19
version: 0.1
*/
kas kas_append(kas* dest, const char* src) {
	null_return(NULL, dest, (void *)src);
	null_return(NULL, *dest);
	kastring str = __KASTRING(*dest);
	append(&str, src);
	*dest = __STR_(str);
	return __STR_(str);
}

/*
edited on 2023/5/19
version: 0.1
*/
kas kas_concat(const kas s0, const kas s1) {
	null_return(NULL, s0, s1);
	kastring oldstr = __KASTRING(s0);
	kastring newstr = init(__SIZE(oldstr));
	append(&newstr, s0);
	return __STR_(append(&newstr, s1));
}

/*
edited on 2023/5/19
version: 0.1
*/
int kas_len(const kas s) {
	null_return(-1, s);
	kastring str = __KASTRING(s);
	return __LEN_(str);
}

/*
edited on 2023/5/19
version: 0.1
*/
bool kas_empty(const kas s) {
	if (kas_len(s)>0) return false;
	return true;
}

/*
edited on 2023/5/22
version: 0.1
*/
kas kas_clone(const kas s) {
	null_return(NULL, s);
	return __STR_(clone(__KASTRING(s)));
}

/*
edited on 2023/5/22
version: 0.1
*/
kas kas_sub(kas s, __uint32_t b, __uint32_t e) {
	null_return(NULL, s);

	int len = strlen(s); // __LEN_(__KASTRING(s));
	if (e > len) e = len;
	len = e - b;

	cond_return(NULL, len <= 0);

	kastring newstr = __KASTRING(kas_new(""));
	char* p = (s) + b, *q = (s) + e;
	while (p != q) {
		__STR_(newstr)[__LEN_(newstr)++] = *p++;
	}
	__STR_(newstr)[__LEN_(newstr)] = '\0';
	return __STR_(newstr);
}


/* 
edited on 2023/5/22
version: 0.1
return the index of the first occurrence of c
*/ 
int kas_index(const kas s, const char* c) {
	null_return(-2, s, (void *)c);
	const char* p, *q, *t=s;
	while (*t) {
		if (*t == *c) {
			p = t;
			q = c;
			while (*p && *q && *p++ == *q++);
			if (*q == '\0') return t - s;
		}
		t++;
	}
	return -1;
}

/*
edited on 2023/5/22
version: 0.1
*/
bool kas_find(const kas s, const char* c) {
	if (kas_index(s, c) < 0) {
		return false;
	}
	return true;
}

/*
edited on 2023/5/22
version: 0.1
*/
int kas_count(const kas s, const char* c) {
	null_return(-1, s, (void *)c);
	const char* p=s, * q=c;
	int num = 0;
	while (*p) {
		if (*p == *q) {
			while (*p && *q && *p == *q) p++, q++;
			if (!*q) {
				num++;
				q = c;
				continue;
			}
			if (!*p) break;
		}
		p++;
	}
	return num;
}

/*
edited on 2023/5/22
version: 0.1
*/
bool kas_startswith(const kas s, const char* c) {
	null_return(false, s);
	null_return(true, (void *)c);
	const char* p = s, * q = c;
	while (*p && *q && *p++ == *q++);
	if (!*q) return true;
	return false;
}

/*
edited on 2023/5/22
version: 0.1
*/
bool kas_endswith(const kas s, const char* c) {
	null_return(false, s);
	null_return(true, (void*)c);
	int l1 = strlen(c);
	int l2 = __LEN_(__KASTRING(s));
	while (l1 && l2 && *(s + l2 - 1) == *(c + l1 - 1)) l2--, l1--;
	if (!l1) return true;
	return false;
}

/*
edited on 2023/5/22
version: 0.1
*/
kas	_kas_join(kas* s, const char* sep, ...) {
	kastring str = __KASTRING(kas_new(""));
	va_list list;
	va_start(list, sep);
	char* ss;
	while (1) {
		ss = va_arg(list, char*);
		if (ss == NULL) break;
		if(__LEN_(str)) \
		str = append(&str, sep);
		str = append(&str, ss);
	}
	va_end(list);
	*s = __STR_(str);
	return __STR_(str);
}

/*
edited on 2023/5/22
version: 0.1
*/
kas kas_replace(kas* s, const char* s0, const char* s1) {
	null_return(NULL, s);
	null_return(NULL, *s);
	null_return(*s, (void *)s0, (void *)s1);
	kastring dest = __KASTRING(*s);
	int len = __LEN_(dest);
	char* p, * q;
	const char *j, * k = s0;
	int l0 = strlen(s0), l1 = strlen(s1);
	int inc = kas_count(*s, s0) * (l1 - l0);
	while ((len + inc) >= __SIZE(dest)) { // À©ÈÝ
		grow(&dest);
	}
	if(inc > 0)
	while (len >= 0) { // ÍùºóÒÆ inc Î»
		__STR_(dest)[len + inc] = __STR_(dest)[len];
		len--;
	}
	p = __STR_(dest) + (inc > 0 ? inc : 0);
	q = __STR_(dest);
	while (*p) {
		if (*k == *p) {
			j = p;
			while (*j && *k && *j == *k) j++, k++;
			if (!*k) { // find sub string
				j = s1;
				while (*j) *q++ = *j++;
				p += l0;
				k = s0;
				continue;
			}
			else { k = s0; }
		}
		*q++ = *p++;
	}
	*q = '\0';
	__LEN_(dest) += inc;
	*s = __STR_(dest);
	return *s;
}

/*
edited on 2023/5/22
version: 0.1
*/
kas _kas_reset(kas* s, v_str c) {
	null_return(NULL, s);
	null_return(NULL, *s);
	kastring str = __KASTRING(*s);
	clear(str);
	if (c.str) append(&str, c.str);
	*s = __STR_(str);
	return __STR_(str);
}


#if defined(KAS_MEMORY_MANAGE)

/*
edited on 2023/5/23
version: 0.1
*/
void kas_freeall() {
	int i;
	MEM_NODE* node, *p;
	for (i = 0; i < MEM_OBJECT_SIZE; i++) {
		node = MEM_OBJECTS[i];
		while (node) {
			p = node->next;
			free(node->str);
			free(node);
			node = p;
		}
		MEM_OBJECTS[i] = NULL;
	}
}

/*
edited on 2023/5/23
version: 0.1
*/
void kas_freeany() {
	int i;
	bool f;
	MEM_NODE* node, *p=NULL, *q=NULL;
	for (i = 0; i < MEM_OBJECT_SIZE; i++) {
		node = MEM_OBJECTS[i];
		f = 1;
		while (node) {
			q = node->next;
			if (_MEMON((kastring)(node->str))) {
				free(node->str);
				free(node);
			}
			else {
				if (f) {
					MEM_OBJECTS[i] = node;
					f = 0;
				}
				else
					p->next = node;
				p = node;
			}
			node = q;
		}
		if (f) MEM_OBJECTS[i] = NULL;
	}
}



/*
edited on 2023/5/23
version: 0.1
*/
void kas_deprive(kas s) {
	null_return(, s);
	kastring str = __KASTRING(s);
	_MEMON(str) = 1;
}

/*
edited on 2023/5/23
version: 0.1
*/
void kas_liberate(kas s) {
	null_return(, s);
	kastring str = __KASTRING(s);
	_MEMON(str) = 0;
}

/*
edited on 2023/5/23
version: 0.1
*/
void kas_exclfree(size_t num, ...) {
	va_list list;
	int i;
	kas s;
	va_start(list, num);
	for (i = 0; i < num; i++) {
		s = va_arg(list, kas);
		kas_liberate(s);
	}
	va_end(list);
	kas_freeany();
}

/*
edited on 2023/5/23
version: 0.1
*/
bool kas_isfree(kas s) {
	null_return(true, s);
	kastring str = __KASTRING(s);
	MEM_NODE* p = MEM_OBJECTS[hash(str)];
	while (p) {
		if (p->str == str) {
			return false;
		}
		p = p->next;
	}
	return true;
}
#endif
