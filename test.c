

#include "kas.h"
#include <stdio.h>
#include <fcntl.h>

#define TEST_BEFOR(s) do { \
		printf("line: %d test %s ...\n", __LINE__, (#s) ); \
		fflush(stdout); \
	}while(0)

#define TEST_OK() do { \
		printf("\t-- Ok"); \
	}while(0)

#define TEST_ERROR() do { \
		printf("\t-- Error"); \
	}while(0)

#define ASSERT(n,e) do{ \
	TEST_BEFOR(n); \
	int _i_ = (e); \
	if (_i_) TEST_OK(); \
	else TEST_ERROR(); \
	printf("\t%s\n\n", #e); \
	fflush(stdout); \
}while(0)


int main(int argc, char* argv[]) {
#if 1
	kas str0 = kas_new("str0 ");
	kas str1 = kas_new("str1 ");
	ASSERT(kas_new, strcmp(str0, "str0 ") == 0);
	ASSERT(kas_new, strcmp(str1, "str1 ") == 0);
#endif

#if 1
	ASSERT(kas_concat, strcmp(kas_concat(str0, str1), "str0 str1 ") == 0);
#endif

#if 1
	kas_append(&str0, "abcdefghjkl");
	ASSERT(kas_append, kas_len(str0) == 16);
#endif

#if 1
	ASSERT(kas_empty, kas_empty(str0) == false);
#endif
	
#if 1
	kas str2 = kas_clone(str0);
	ASSERT(kas_clone, strcmp(str0, str2) == 0);
#endif
	
#if 1
	kas str3 = kas_sub(str0, 0, 2);
	ASSERT(kas_sub, strcmp(str3, "st") == 0);
#endif
	
#if 1
	ASSERT(kas_index, kas_index(str0, "tr") == 1);
#endif
	

#if 1
	kas str4 = kas_join(&str4, ".", "asd", "fgh");
	ASSERT(kas_join, strcmp(str4, "asd.fgh") == 0);
	kas str5 = kas_join(&str5, "-", "a", "b", "c");
	ASSERT(kas_join, strcmp(str5, "a-b-c") == 0);
#endif
#if 1
	kas str6 = kas_sub(str0, 0, 5);
	ASSERT(kas_sub, strcmp(str6, "str0 ") == 0);
#endif
#if 1
	kas_replace(&str0, "t", "abc");
	ASSERT(kas_replace, strncmp(str0, "sabcr0 ",7) == 0);
#endif
	
#if 1
	ASSERT(kas_reset, strcmp(kas_reset(&str0), "") == 0);
#endif

#if defined(KAS_MEMORY_MANAGE) && 1
	kas_liberate(str2);
	kas_liberate(str3);
	kas_exclfree(1, str0);
	ASSERT(kas_exclfree, kas_isfree(str0) == false);
	ASSERT(kas_exclfree, kas_isfree(str2) == false);
	ASSERT(kas_exclfree, kas_isfree(str3) == false);
	
	ASSERT(kas_freeany, kas_isfree(str4) == true);

	kas_freeall();
	ASSERT(kas_freeall(), kas_isfree(str0) == true);

#endif

#if 0
	ASSERT(null kas_len, kas_len(NULL) == -1);
	ASSERT(null kas_append, kas_append(NULL, NULL) == NULL);
	ASSERT(null kas_startswith, kas_startswith(str0, NULL));
	ASSERT(null kas_isfree, kas_isfree(NULL));
	kas_free(NULL);
#endif	
	return 0;
}

