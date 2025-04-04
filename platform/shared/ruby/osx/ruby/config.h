#include <TargetConditionals.h>
#define TARGET_OS_OSX (!(TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_WATCH))


#ifndef INCLUDE_RUBY_CONFIG_H
#define INCLUDE_RUBY_CONFIG_H 1
/* confdefs.h */
#define CANONICALIZATION_FOR_MATHN 1
#define STDC_HEADERS 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STRINGS_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_UNISTD_H 1
#define __EXTENSIONS__ 1
#define _ALL_SOURCE 1
#define _GNU_SOURCE 1
#define _POSIX_PTHREAD_SEMANTICS 1
#define _TANDEM_SOURCE 1
#define HAVE_INFINITY 1
#define HAVE_NAN 1
#define RUBY_SYMBOL_EXPORT_BEGIN _Pragma("GCC visibility push(default)")
#define RUBY_SYMBOL_EXPORT_END _Pragma("GCC visibility pop")
#define BROKEN_CRYPT 1


//RHO
#define HAVE_TYPE_SOCKLEN_T 1
#define HAVE_GETADDRINFO 1
#define HAVE_TYPE_STRUCT_SOCKADDR_STORAGE 1
#define HAVE_TYPE_STRUCT_ADDRINFO 1
#define HAVE_GAI_STRERROR 1
#define GAI_STRERROR_CONST 1
#define HAVE_NET_IF_H 1

#define IPPROTO_IPV6 1
#define HAVE_TYPE_STRUCT_IPV6_MREQ 1


//>>>
#define HAVE_CONST_AF_INET 1
#define HAVE_CONST_PF_INET6 1
//#define HAVE_CONST_TCP_MAXSEG 1
#define HAVE_CONST_SOCK_STREAM 1
#define HAVE_CONST_AI_PASSIVE 1
#define HAVE_CONST_IPPROTO_TCP 1
#define HAVE_CONST_IPPROTO_EGP 1
#define HAVE_CONST_IPPROTO_PUP 1
#define HAVE_CONST_IPPROTO_UDP 1
#define HAVE_CONST_IPPROTO_IDP 1
#define HAVE_CONST_IPPROTO_AH 1
#define HAVE_CONST_IPPROTO_ESP 1
#define HAVE_CONST_IPPROTO_RAW 1
#define HAVE_CONST_IPPROTO_MAX 1
#define HAVE_CONST_IPPORT_RESERVED 1
#define HAVE_CONST_INADDR_ANY 1
#define HAVE_CONST_INADDR_BROADCAST 1
#define HAVE_CONST_INADDR_LOOPBACK 1
#define HAVE_CONST_INADDR_NONE 1
#define HAVE_CONST_IP_OPTIONS 1
#define HAVE_CONST_IP_HDRINCL 1
#define HAVE_CONST_IP_TOS 1
#define HAVE_CONST_IP_TTL 1

#define HAVE_TYPE_STRUCT_ADDRINFO 1
#define HAVE_TYPE_STRUCT_SOCKADDR_STORAGE 1
#define HAVE_GAI_STRERROR 1
#define HAVE_INET_PTON 1
#define HAVE_INET_NTOP 1
#define HAVE_ARPA_INET_H 1

#define HAVE_TYPE_STRUCT_IP_MREQ 1
//<<<



//RHO
//#define HAVE_CRT_EXTERNS_H 1

#define HAVE_LIBDL 1
#define HAVE_DIRENT_H 1
#define HAVE__BOOL 1
#define HAVE_STDBOOL_H 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_GRP_H 1
#define HAVE_FCNTL_H 1
#define HAVE_FLOAT_H 1
#define HAVE_LANGINFO_H 1
#define HAVE_LIMITS_H 1
#define HAVE_LOCALE_H 1
#define HAVE_MALLOC_MALLOC_H 1
#define HAVE_PWD_H 1
#define HAVE_SYS_ATTR_H 1
#define HAVE_SYS_FCNTL_H 1
#define HAVE_SYS_FILE_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_SYSCALL_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TIMES_H 1
#define HAVE_SYS_UIO_H 1
#define HAVE_TIME_H 1

//RHO
//#define HAVE_UCONTEXT_H 1


#define HAVE_UTIME_H 1

//RHO
//#define HAVE_TYPEOF 1

#define HAVE_LONG_LONG 1
#define HAVE_OFF_T 1
#define SIZEOF_INT 4
#define SIZEOF_SHORT 2

//RHO
#ifdef RHODES_QT_PLATFORM
#define SIZEOF_LONG 8
#else
#ifdef __LP64__
#define SIZEOF_LONG 8
#else
#define SIZEOF_LONG 4
#endif
#endif
//RHO

#define SIZEOF_LONG_LONG 8
#define SIZEOF___INT64 0
#define SIZEOF___INT128 16
#define SIZEOF_OFF_T 8

//RHO
#ifdef RHODES_EMULATOR
#define SIZEOF_VOIDP 8
#else
#ifdef __LP64__
#define SIZEOF_VOIDP 8
#else
#define SIZEOF_VOIDP 4
#endif
#endif
//RHO

#define SIZEOF_FLOAT 4
#define SIZEOF_DOUBLE 8
#define SIZEOF_TIME_T 8
#define SIZEOF_CLOCK_T 8
#define PACKED_STRUCT(x) x __attribute__((packed))
#define PACKED_STRUCT_UNALIGNED(x) x
#define PRI_LL_PREFIX "ll"
#define rb_pid_t pid_t
#define SIGNEDNESS_OF_PID_T -1
#define PIDT2NUM(v) INT2NUM(v)
#define NUM2PIDT(v) NUM2INT(v)
#define PRI_PIDT_PREFIX PRI_INT_PREFIX
#define rb_uid_t uid_t
#define SIGNEDNESS_OF_UID_T +1
#define UIDT2NUM(v) UINT2NUM(v)
#define NUM2UIDT(v) NUM2UINT(v)
#define PRI_UIDT_PREFIX PRI_INT_PREFIX
#define rb_gid_t gid_t
#define SIGNEDNESS_OF_GID_T +1
#define GIDT2NUM(v) UINT2NUM(v)
#define NUM2GIDT(v) NUM2UINT(v)
#define PRI_GIDT_PREFIX PRI_INT_PREFIX
#define rb_time_t time_t
#define SIGNEDNESS_OF_TIME_T -1
#define TIMET2NUM(v) LONG2NUM(v)
#define NUM2TIMET(v) NUM2LONG(v)
#define PRI_TIMET_PREFIX PRI_LONG_PREFIX
#define rb_dev_t dev_t
#define SIGNEDNESS_OF_DEV_T -1
#define DEVT2NUM(v) INT2NUM(v)
#define NUM2DEVT(v) NUM2INT(v)
#define PRI_DEVT_PREFIX PRI_INT_PREFIX
#define rb_mode_t mode_t
#define SIGNEDNESS_OF_MODE_T +1
#define MODET2NUM(v) UINT2NUM(v)
#define NUM2MODET(v) NUM2UINT(v)
#define PRI_MODET_PREFIX PRI_INT_PREFIX
#define rb_rlim_t rlim_t
#define SIGNEDNESS_OF_RLIM_T +1
#define RLIM2NUM(v) ULL2NUM(v)
#define NUM2RLIM(v) NUM2ULL(v)
#define PRI_RLIM_PREFIX PRI_LL_PREFIX
#define rb_off_t off_t
#define SIGNEDNESS_OF_OFF_T -1
#define OFFT2NUM(v) LL2NUM(v)
#define NUM2OFFT(v) NUM2LL(v)
#define PRI_OFFT_PREFIX PRI_LL_PREFIX
#define rb_clockid_t int
#define SIGNEDNESS_OF_CLOCKID_T -1
#define CLOCKID2NUM(v) INT2NUM(v)
#define NUM2CLOCKID(v) NUM2INT(v)
#define PRI_CLOCKID_PREFIX PRI_INT_PREFIX
#define HAVE_PROTOTYPES 1
#define TOKEN_PASTE(x,y) x##y
#define STRINGIZE(expr) STRINGIZE0(expr)
#define HAVE_STDARG_PROTOTYPES 1
#define HAVE_VA_ARGS_MACRO 1
#define CONSTFUNC(x) __attribute__ ((__const__)) x
#define PUREFUNC(x) __attribute__ ((__pure__)) x
#define NORETURN(x) __attribute__ ((__noreturn__)) x
#define DEPRECATED(x) __attribute__ ((__deprecated__)) x
#define DEPRECATED_BY(n,x) __attribute__ ((__deprecated__("by "#n))) x
#define DEPRECATED_TYPE(mesg,x) x __attribute__ ((__deprecated__ mesg))
#define NOINLINE(x) __attribute__ ((__noinline__)) x
#define ALWAYS_INLINE(x) __attribute__ ((__always_inline__)) x
#define WARN_UNUSED_RESULT(x) __attribute__ ((__warn_unused_result__)) x
#define MAYBE_UNUSED(x) __attribute__ ((__unused__)) x
#define WEAK(x) __attribute__ ((__weak__)) x
#define HAVE_FUNC_WEAK 1
#define FUNC_CDECL(x) __attribute__ ((__cdecl__)) x
#define HAVE_GCC_ATOMIC_BUILTINS 1
#define HAVE_GCC_SYNC_BUILTINS 1
#define UNREACHABLE __builtin_unreachable()
#define RUBY_FUNC_EXPORTED __attribute__ ((__visibility__("default"))) extern
#define RUBY_FUNCTION_NAME_STRING __func__
#define ENUM_OVER_INT 1
#define HAVE_DECL_SYS_NERR 1
#define HAVE_DECL_GETENV 1
#define SIZEOF_SIZE_T 8
#define SIZEOF_PTRDIFF_T 8
#define PRI_SIZE_PREFIX "z"
#define PRI_PTRDIFF_PREFIX "t"
#define HAVE_STRUCT_STAT_ST_BLKSIZE 1
#define HAVE_ST_BLKSIZE 1
#define HAVE_STRUCT_STAT_ST_BLOCKS 1
#define HAVE_ST_BLOCKS 1
#define HAVE_STRUCT_STAT_ST_RDEV 1
#define HAVE_ST_RDEV 1
#define SIZEOF_STRUCT_STAT_ST_SIZE SIZEOF_OFF_T
#define SIZEOF_STRUCT_STAT_ST_BLOCKS SIZEOF_OFF_T
#define SIZEOF_STRUCT_STAT_ST_INO SIZEOF_LONG
#define HAVE_STRUCT_STAT_ST_ATIMESPEC 1
#define HAVE_STRUCT_STAT_ST_MTIMESPEC 1
#define HAVE_STRUCT_STAT_ST_CTIMESPEC 1
#define HAVE_STRUCT_STAT_ST_BIRTHTIMESPEC 1
#define HAVE_STRUCT_TIMEVAL 1
#define SIZEOF_STRUCT_TIMEVAL_TV_SEC SIZEOF_TIME_T
#define HAVE_STRUCT_TIMESPEC 1
#define HAVE_STRUCT_TIMEZONE 1
#define HAVE_RB_FD_INIT 1
#define HAVE_INT8_T 1
#define SIZEOF_INT8_T 1
#define HAVE_UINT8_T 1
#define SIZEOF_UINT8_T 1
#define HAVE_INT16_T 1
#define SIZEOF_INT16_T 2
#define HAVE_UINT16_T 1
#define SIZEOF_UINT16_T 2
#define HAVE_INT32_T 1
#define SIZEOF_INT32_T 4
#define HAVE_UINT32_T 1
#define SIZEOF_UINT32_T 4
#define HAVE_INT64_T 1
#define SIZEOF_INT64_T 8
#define HAVE_UINT64_T 1
#define SIZEOF_UINT64_T 8

//RHO
/*
#define HAVE_INT128_T 1
#define int128_t __int128
#define SIZEOF_INT128_T SIZEOF___INT128
#define HAVE_UINT128_T 1
#define uint128_t unsigned __int128
#define SIZEOF_UINT128_T SIZEOF___INT128
*/
//RHO

#define HAVE_INTPTR_T 1
#define SIZEOF_INTPTR_T 8
#define HAVE_UINTPTR_T 1
#define SIZEOF_UINTPTR_T 8
#define HAVE_SSIZE_T 1
#define SIZEOF_SSIZE_T 8
#define GETGROUPS_T gid_t
#define RETSIGTYPE void
#define HAVE_ALLOCA_H 1
#define HAVE_ALLOCA 1
#define HAVE_ACOSH 1
#define HAVE_CBRT 1
#define HAVE_CRYPT 1
#define HAVE_DUP2 1
#define HAVE_ERF 1
#define HAVE_FFS 1
#define HAVE_FINITE 1
#define HAVE_FLOCK 1
#define HAVE_HYPOT 1
#define HAVE_ISINF 1
#define HAVE_ISNAN 1
#define HAVE_LGAMMA_R 1
#define HAVE_MEMMOVE 1
#define HAVE_NEXTAFTER 1
#define HAVE_STRCHR 1
#define HAVE_STRERROR 1
#define HAVE_STRLCAT 1
#define HAVE_STRLCPY 1
#define HAVE_STRSTR 1
#define HAVE_TGAMMA 1
#define SPT_TYPE SPT_REUSEARGV
#define HAVE_SIGNBIT 1
#define HAVE_FORK 1
#define vfork fork
#define HAVE_WORKING_FORK 1
#define HAVE___SYSCALL 1
#define HAVE__LONGJMP 1
#define HAVE_ARC4RANDOM_BUF 1
#define HAVE_ATAN2L 1
#define HAVE_ATAN2F 1
#define HAVE_CHROOT 1
#define HAVE_CLOCK_GETTIME 1
#define HAVE_COSH 1
#define HAVE_DIRFD 1
#define HAVE_DLOPEN 1

//RHO
//#define HAVE_DLADDR 1

#define HAVE_DUP 1
#define HAVE_ENDGRENT 1
#define HAVE_FCHMOD 1
#define HAVE_FCHOWN 1
#define HAVE_FCNTL 1
#define HAVE_FGETATTRLIST 1
#define HAVE_FMOD 1
#define HAVE_FSYNC 1
#define HAVE_FTRUNCATE 1
#define HAVE_GETATTRLIST 1
#define HAVE_GETCWD 1
#define HAVE_GETGRNAM 1
#define HAVE_GETGRNAM_R 1
#define HAVE_GETGROUPS 1
#define HAVE_GETPGID 1
#define HAVE_GETPGRP 1
#define HAVE_GETPRIORITY 1
#define HAVE_GETPWNAM_R 1
#define HAVE_GETRLIMIT 1
#define HAVE_GETSID 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_GMTIME_R 1
#define HAVE_INITGROUPS 1
#define HAVE_IOCTL 1
#define HAVE_ISSETUGID 1
#define HAVE_KILLPG 1
#define HAVE_LCHMOD 1
#define HAVE_LCHOWN 1
#define HAVE_LINK 1
#define HAVE_LLABS 1
#define HAVE_LOCKF 1
#define HAVE_LOG2 1
#define HAVE_LSTAT 1
#define HAVE_MALLOC_SIZE 1
#define HAVE_MBLEN 1
#define HAVE_MEMSET_S 1
#define HAVE_WRITEV 1
#define HAVE_MKFIFO 1
#define HAVE_MKNOD 1
#define HAVE_MKTIME 1
#define HAVE_POLL 1
#define HAVE_POSIX_MEMALIGN 1
#define HAVE_PREAD 1
#define HAVE_QSORT_R 1
#define HAVE_READLINK 1
#define HAVE_ROUND 1
#define HAVE_SEEKDIR 1
#define HAVE_SENDFILE 1
#define HAVE_SETEGID 1
#define HAVE_SETENV 1
#define HAVE_SETEUID 1
#define HAVE_SETGID 1
#define HAVE_SETGROUPS 1
#define HAVE_SETPGID 1
#define HAVE_SETPGRP 1
#define HAVE_SETREGID 1
#define HAVE_SETREUID 1
#define HAVE_SETRGID 1
#define HAVE_SETRLIMIT 1
#define HAVE_SETRUID 1
#define HAVE_SETSID 1
#define HAVE_SETUID 1
#define HAVE_SHUTDOWN 1
#define HAVE_SIGACTION 1
#define HAVE_SIGALTSTACK 1
#define HAVE_SIGPROCMASK 1
#define HAVE_SINH 1
#define HAVE_SYMLINK 1
#define HAVE_SYSCALL 1
#define HAVE_SYSCONF 1
#define HAVE_TANH 1
#define HAVE_TELLDIR 1
#define HAVE_TIMEGM 1
#define HAVE_TIMES 1
#define HAVE_TRUNCATE 1
#define HAVE_UNSETENV 1
#define HAVE_UTIMES 1
#define HAVE_WAIT4 1
#define HAVE_WAITPID 1
#if !defined __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#endif /* !defined __STDC_WANT_LIB_EXT1__ */
#define HAVE_BUILTIN___BUILTIN_BSWAP16 1
#define HAVE_BUILTIN___BUILTIN_BSWAP32 1
#define HAVE_BUILTIN___BUILTIN_BSWAP64 1
#define HAVE_BUILTIN___BUILTIN_POPCOUNT 1
#define HAVE_BUILTIN___BUILTIN_POPCOUNTLL 1
#define HAVE_BUILTIN___BUILTIN_CLZ 1
#define HAVE_BUILTIN___BUILTIN_CLZL 1
#define HAVE_BUILTIN___BUILTIN_CLZLL 1
#define HAVE_BUILTIN___BUILTIN_CTZ 1
#define HAVE_BUILTIN___BUILTIN_CTZLL 1
#define HAVE_BUILTIN___BUILTIN_CONSTANT_P 1
#define HAVE_BUILTIN___BUILTIN_CHOOSE_EXPR 1

//RHO
//#define HAVE_BUILTIN___BUILTIN_CHOOSE_EXPR_CONSTANT_P 1
//#define HAVE_BUILTIN___BUILTIN_TYPES_COMPATIBLE_P 1

#define HAVE_BSD_QSORT_R 1
#define ATAN2_INF_C99 1
#define LGAMMA_R_PM0_FIX 1
#define HAVE_CLOCK_GETRES 1
#define HAVE_STRUCT_TM_TM_ZONE 1
#define HAVE_TM_ZONE 1
#define HAVE_STRUCT_TM_TM_GMTOFF 1
#define HAVE_DAYLIGHT 1
#define HAVE_VAR_TIMEZONE 1
#define TYPEOF_VAR_TIMEZONE long
#define HAVE_TIMEZONE 1
#define TIMEZONE_VOID 1
#define NEGATIVE_TIME_T 1
#define POSIX_SIGNAL 1
#define HAVE_SIG_T 1
#define RSHIFT(x,y) ((x)>>(int)(y))
#define HAVE__SC_CLK_TCK 1
#define STACK_GROW_DIRECTION -1
#define _REENTRANT 1
#define _THREAD_SAFE 1
#define HAVE_LIBPTHREAD 1
#define HAVE_SCHED_YIELD 1
#define HAVE_PTHREAD_ATTR_SETINHERITSCHED 1
#define HAVE_PTHREAD_ATTR_GETSTACK 1
#define HAVE_PTHREAD_GET_STACKADDR_NP 1
#define HAVE_PTHREAD_GET_STACKSIZE_NP 1
#define HAVE_PTHREAD_COND_INIT 1
#define HAVE_PTHREAD_CONDATTR_INIT 1
#define HAVE_PTHREAD_SIGMASK 1
#define HAVE_PTHREAD_SETNAME_NP 1
#define HAVE_PTHREAD_ATTR_INIT 1
#define SET_CURRENT_THREAD_NAME(name) pthread_setname_np(name)
#define DEFINE_MCONTEXT_PTR(mc, uc) mcontext_t mc = (uc)->uc_mcontext
#define HAVE_EXECINFO_H 1
#define HAVE_LIBUNWIND_H 1
#define HAVE_BACKTRACE 1
#define BROKEN_BACKTRACE 1
#define DLEXT_MAXLEN 7
#define DLEXT ".bundle"
#define HAVE__SETJMP 1
#define HAVE_SIGSETJMP 1
#define RUBY_SETJMP(env) _setjmp((env))
#define RUBY_LONGJMP(env,val) _longjmp((env),val)
#define RUBY_JMP_BUF jmp_buf
#define HAVE_PTHREAD_H 1
#define RUBY_PLATFORM "x86_64-darwin16"

//RHO
//#define USE_RGENGC 0
//#define RGENGC_CHECK_MODE 4

#define LOAD_RELATIVE 1

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#define NO_INITIAL_LOAD_PATH
#define NO_RUBY_SITE_LIB
#define NO_RUBY_VENDOR_LIB

#define RHO_STRIP_LOG 999

#define USE_STD_PRINTF
#ifndef USE_STD_PRINTF
int rhoRubyFPrintf(FILE *, const char *, ...);
int rhoRubyVFPrintf(FILE *, const char *, va_list);
#define fprintf rhoRubyFPrintf
#define vfprintf rhoRubyVFPrintf
#endif

#define USE_NATIVE_THREAD_PRIORITY  1

//RHO

//RHO for encoding & convertors
#define EXTSTATIC 1


#endif /* INCLUDE_RUBY_CONFIG_H */
