#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void add_memref(void* pmem, const char* file, int line);
void del_memref(void* pmem);

extern "C"{
void    rho_free(void * p)
{
    del_memref(p);
    free(p);
}
void    sys_free(void *p)
{
    free(p);
}

void *  rho_malloc(size_t s, const char* file, int line)
{
    void* res = malloc(s);
    add_memref(res, file, line);

    return res;
}
void *  rho_calloc(size_t num, size_t size, const char* file, int line)
{
    void* res = calloc(num,size);
    add_memref(res, file, line);

    return res;
}

size_t  rho_msize(void *)
{
    return 0;
}
void *  rho_realloc(void *p, size_t s, const char* file, int line)
{
    void* res = realloc(p,s);
    add_memref(res, file, line);

    return res;
}

char *  rho_strdup(const char * str)
{
    char *tmp;
    int len = strlen(str) + 1;

    tmp = (char*)rho_malloc(len, "rho_strdup", 1);
    memcpy(tmp, str, len);

    return tmp;
}

wchar_t *  rho_wcsdup(const wchar_t * str)
{
    wchar_t *tmp;
    int len = (wcslen(str) + 1)*sizeof(wchar_t);

    tmp = (wchar_t*)rho_malloc(len, "rho_wcsdup", 1);
    memcpy(tmp, str, len);

    return tmp;
}

}

#include <new>
//#include <exception>

void* cpp_alloc(size_t size, bool nothrow) {
  for (;;) {
    void* p = rho_malloc(size, "cpp_alloc", 1);
#ifdef PREANSINEW
    return p;
#else
    if (p == NULL) {  // allocation failed
        if (nothrow) return 0;
        throw std::bad_alloc();
    } else {  // allocation success
      return p;
    } 
#endif
  }
}

static long g_nCount = -1;
#include <windows.h>
#include <atlbase.h>
#include <atlutil.h>

//#include <map>
//#include <string>
void* g_ptr_array[10000];
int   g_line_array[10000];
const char*  g_file_array[10000];
CStringA  g_stack_array[10000];

//typedef std::map<void*,std::string> TObjMap; 
//static TObjMap* g_Objects;
static CRITICAL_SECTION s_localLock;

long getObjectCount(){
    return g_nCount;
}
int g_nInsideadd_memref = 0;
CStringA MyDumpStack();
void add_memref(void* pmem, const char* file, int line)
{
    if (!g_nInsideadd_memref && g_nCount>=0&& strstr(file, "\\gc.c") == 0 &&
         strstr(file,"\\regexec.c") == 0 )
    {
        g_nInsideadd_memref = 1;

        CStringA str = MyDumpStack();

		::EnterCriticalSection(&s_localLock);
        g_ptr_array[g_nCount] = pmem;
        g_line_array[g_nCount] = line;
        g_file_array[g_nCount] = file;
        g_stack_array[g_nCount] = str;
		g_nCount++;

        g_nInsideadd_memref = 0;
		::LeaveCriticalSection(&s_localLock);
    }
}

void del_memref(void* pmem)
{
    if (g_nCount>=0){
        ::EnterCriticalSection(&s_localLock);

        for(int i = 0; i<g_nCount; i++)
        {
            if ( g_ptr_array[i] == pmem)
            {
                g_ptr_array[i] = 0;
                if ( i == g_nCount-1)
                    g_nCount--;

                break;
            }
        }

        ::LeaveCriticalSection(&s_localLock);
    }

}

extern "C" void StartMemMonitoring()
{
    if (g_nCount<0)
    {
    	::InitializeCriticalSection(&s_localLock);
        g_nCount = 0;
    }
}

extern "C" void StopMemMonitoring()
{
    if (g_nCount>=0)
    {
    	::DeleteCriticalSection(&s_localLock);
        g_nCount = 0;
    }
}

char buf[20];
#if !defined(_WIN32_WCE)
extern "C" void dumpAllMemObjs()
{
    OutputDebugString(L"START DUMP\r\n");

    for(int i = 0; i<g_nCount; i++)
    {
        if ( g_ptr_array[i] )
        {
            OutputDebugStringA( g_file_array[i] );

            sprintf(buf, ": %d", g_line_array[i] );
            OutputDebugStringA( buf );

            OutputDebugStringA( g_stack_array[i] );

            OutputDebugStringA( "\r\n" );
        }
    }
}
#else
extern "C" void dumpAllMemObjs()
{
}
#endif

#if !defined(_WIN32_WCE)
class CTraceClipboardData : public IStackDumpHandler
{
public:
    CStringA m_strStackA;
    bool m_bStop;
	void __stdcall OnBegin(){}
	void __stdcall OnEntry(void *pvAddress, LPCSTR szModule, LPCSTR szSymbol)
	{
        if ( m_bStop )
            return;

		char sz[40];
		sprintf_s(sz, _countof(sz), "%p: ", pvAddress);
		SendOut(sz);

        //if (szModule && strcmp(szModule, "kernel32.dll") == 0)
        //{
            //m_bStop = true;
            //return;
        //}

		if (szModule)
		{
			ATLASSERT(szSymbol);
			SendOut(szModule);
			SendOut("! ");
			SendOut(szSymbol);
		}
		else
			SendOut("symbol not found");
		SendOut("\r\n");
	}

	void __stdcall OnError(LPCSTR szError)
	{
		SendOut(szError);
	}
	void __stdcall OnEnd(){	}
	void SendOut(LPCSTR pszData);
	CTraceClipboardData();
	~CTraceClipboardData();
};

CTraceClipboardData::CTraceClipboardData()//DWORD dwTarget)
{
    m_bStop = false;
}

CTraceClipboardData::~CTraceClipboardData()
{
}

void CTraceClipboardData::SendOut(LPCSTR pszData)
{
    m_strStackA += pszData;
	return;
}

/////////////////////////////////////////////////////////////////////////////
// AfxDumpStack API

CStringA MyDumpStack()
{
	CTraceClipboardData clipboardData;

	AtlDumpStack(&clipboardData);

    return clipboardData.m_strStackA;
}
#else
CStringA MyDumpStack()
{
    return CStringA();
}
#endif 