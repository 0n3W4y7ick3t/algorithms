#ifdef NDEBUG
#define TRACE_ON 0
#else
#define TRACE_ON 1
#endif

#include <stdio.h>

/* # does a string replacement for the argument passed to macro */
#define STRINGIFY(X) #X
#define STR_VAR(X) STRINGIFY(X)

/* if TRAC_ON is 0, this whole micro will be optmized away  */
/* %s limit HEAD to be a string, %Lg limits NUM to be arithmic */
#define TRACE_NUM(HEAD, NUM)                                                   \
  do {                                                                         \
    if (TRACE_ON)                                                              \
      fprintf(stderr, "%s:" STR_VAR(__LINE__) " in %s: %s %Lg(" #NUM ")\n",    \
              __FILE__, __func__, HEAD, (NUM) + 0.0L);                         \
  } while (0)

/* (void*){0} makes sure the right value can be converted to a void* */
#define TRACE_PTR(HEAD, PTR)                                                   \
  do {                                                                         \
    if (TRACE_ON)                                                              \
      fprintf(stderr, "%s:" STR_VAR(__LINE__) " in %s: %s %p(" #PTR ")\n",     \
              __FILE__, __func__, HEAD, ((void*){0} = (PTR)));                 \
  } while (0)

/* default arguments!! */
#define ZERO_DEFAULT3(...) CALL_3ARGS(__VA_ARGS__, 0, 0, ) // padding with 0
#define CALL_3ARGS(FUNC, _0, _1, _2, ...)                                      \
  FUNC(_0, _1, _2) // only takes the first three

/* micro definition is temporarily disblaed when expanded,
so there is no recursion */
#define strtoul(...)                                                           \
  ZERO_DEFAULT3(strtoul, __VA_ARGS__) // defaults to strtoul(arg1, 0, 0)


/* variadic arguments function is not type safe,
if you definitely want it, you should define it with micro: */
/* F must match with __VA_ARGS__, eg:
TRACE_PRINT("arr[%u]: %g", i, arr[i])
this not works when __VA_ARGS__ is empty. how to archive that is pretty hard
for more info, see Morden C written by Jens Gustedt
*/

#define TRACE_PRINT(F, ...)                                                    \
  do {                                                                         \
    if (TRACE_ON) {                                                            \
      fprintf(stderr, "%s:" STR_VAR(__LINE__) " in %s: " F "(%s)\n", __FILE__, \
              __func__, __VA_ARGS__, #__VA_ARGS__);                            \
    }                                                                          \
  } while (0)
