/******************************************************************************

FILE: SpiritLogType.h

DESCRIPTION:    In this file types and macro definitions 
for work with a broad gully are described.



This file is a part of Spirit Embedded Media Engine.



Copyright SPIRIT DSP 2006.                                 www.spiritdsp.com

******************************************************************************/

#ifndef _SPIRIT_LOG_TYPE_H_
#define _SPIRIT_LOG_TYPE_H_

// Spirit functions and log functions errors
#define SPIRIT_LOG_SUCCESS              (0x0) // Function was executed successfully
#define SPIRIT_LOG_FAIL                 (0x1) // Function was executed unsuccessfully
#define SPIRIT_LOG_ID_NOT_FOUND         (0x2) // If id not fount in log
#define SPIRIT_LOG_NO_ENOUGH_MEMORY     (0x3) // There is no memory
#define SPIRIT_LOG_INVALID_PARAMETERS   (0x4) // If parameters invalid

// Event levels
#define SPIRIT_LOG_LEVEL_NOT_EVENT      (0x00) // Not added event in log
#define SPIRIT_LOG_LEVEL_INFO           (0x01) // Print information event
#define SPIRIT_LOG_LEVEL_ERROR          (0x02) // Errors event
#define SPIRIT_LOG_LEVEL_WARNING        (0x04) // Warnings level
#define SPIRIT_LOG_LEVEL_DEBUG          (0x08) // Debug event
#define SPIRIT_LOG_LEVEL_PROFILING      (0x10) // Prifling event
#define SPIRIT_LOG_LEVEL_MAX            (0x10) // Max event (or any events)
#define SPIRIT_LOG_LEVEL_ANY            (0x1F) // Any level

// Default levels
#define SPIRIT_LOG_LEVEL_DEFAULT  (SPIRIT_LOG_LEVEL_ERROR | SPIRIT_LOG_LEVEL_INFO) // Default level

#define SPIRIT_LOG_LEVEL(level)         ((uint32)(level & SPIRIT_LOG_LEVEL_ANY))



// Callback function
typedef unsigned int (*tSpiritSettingsCallback)(void*, char*, unsigned int);

// eSPIRIT_LOG_FAST – log data is stored in the memory buffer and 
//    written to the file storage after completion of realtime
//    processing. Spirit recommends to use this mode always.
// eSPIRIT_LOG_CACHED, eSPIRIT_LOG_SYNC – both of these modes 
//    perform direct write to the file. These modes are intended
//    for debugging of engine internals. Please avoid using of these modes.

// Spirit log type
typedef enum tSpiritLogType
{
  eSPIRIT_LOG_FAST = 0,
  eSPIRIT_LOG_CACHED = 1,
  eSPIRIT_LOG_SYNC = 2,
} tSpiritLogType;

// SpiritLog structure
typedef struct _tSpiritLog
{
  // Log functions
  unsigned int (*RegisterEvent)(void*, unsigned int, unsigned int*, char*); // Register event
  unsigned int (*Trace)(void*, unsigned int, char*, ...);           // Write message in log                                              // Write message in log with format
  unsigned int (*Printf)(void*, unsigned int, char*, ...);          // Print string
  unsigned int (*Flush)(void*);                                     // Flush buffer in file

  // Get/Set flags
  unsigned int (*SetFlags)(void*, unsigned int);                    // Set flags
  unsigned int (*GetFlags)(void*, unsigned int*);                   // Get flags

}tSpiritLog;

// Auxiliary a macro definitions
#ifndef SPIRIT_LOG_DISABLE

// Register event
#define SPIRIT_LOG_REGISTER_EVENT(log, id, name)              \
  do{if(log != 0 && id != 0 && name != 0) log->RegisterEvent(log, SPIRIT_LOG_LEVEL_DEFAULT, id, name);}while(0)
// Register event with level
#define SPIRIT_LOG_REGISTER_EVENT_LEVEL(log, level, id, name) \
  do{if(log != 0 && id != 0 && name != 0 &&                   \
  SPIRIT_LOG_LEVEL(level))                                    \
  log->RegisterEvent(log, level, id, name);}while(0)
// Trace with format. Char in format 's' and 'd' (example format: "sdss")

#if defined(_MSC_VER) & _MSC_VER < 1300

SPIRIT_INLINE void SPIRIT_LOG_TRACE_EX(tSpiritLog*log, int id, char *format, ...)
{
  if (log || id || format)
  { 
    va_list ls;
    va_start(ls, format);

    if(log != 0 && format != 0 && id > 0)
      log->Trace(log, id, format, ls);

    va_end(ls);
  }
}

#elif !defined(__GNUC__)
#define SPIRIT_LOG_TRACE_EX(log, id, format, ...)      \
  do{if(log != 0 && format != 0 && id > 0) log->Trace(log, id, format, __VA_ARGS__);}while(0)
#else
#define SPIRIT_LOG_TRACE_EX(log, id, format, args...)      \
  do{if(log != 0 && format != 0 && id > 0) log->Trace(log, id, format, args);}while(0)
#endif

// Trace without format.
#define SPIRIT_LOG_TRACE(log, id)      \
  do{if(log != 0 && id > 0) log->Trace(log, id, "");}while(0)


#if defined(_MSC_VER) & _MSC_VER < 1300

SPIRIT_INLINE void  SPIRIT_LOG_PRINTF(tSpiritLog *log, char *format, ...)
{
  if (log ||format) 
  {

    va_list ls;
    va_start(ls, format);

    if(log != 0 && format != 0)
      log->Printf(log,SPIRIT_LOG_LEVEL_DEFAULT, format,ls);
    //                do{if(log != 0 && format != 0) log->Printf(log, SPIRIT_LOG_LEVEL_DEFAULT, format, __VA_ARGS__);}while(0)
  }
}

SPIRIT_INLINE void  SPIRIT_LOG_PRINTF_LEVEL(tSpiritLog *log, unsigned char level, char *format, ...)
{
  if(log != 0 && format != 0 && SPIRIT_LOG_LEVEL(level))
  {
    va_list ls;
    va_start(ls, format);
    log->Printf(log, level, format, ls);
  }
}


// Print
#elif !defined(__GNUC__)
// Max string size 256 chars
#define SPIRIT_LOG_PRINTF(log, format, ...)                         \
  do{if(log != 0 && format != 0) log->Printf(log, SPIRIT_LOG_LEVEL_DEFAULT, format, __VA_ARGS__);}while(0)
// Print with level
// Max string size 256 chars
#define SPIRIT_LOG_PRINTF_LEVEL(log, level, format, ...)            \
  do{if(log != 0 && format != 0 &&                                  \
  SPIRIT_LOG_LEVEL(level))                                          \
    log->Printf(log, level, format, __VA_ARGS__);}while(0)

#else
// Print
// Max string size 256 chars
#define SPIRIT_LOG_PRINTF(log, format...)                           \
  do{if(log != 0) log->Printf(log, SPIRIT_LOG_LEVEL_DEFAULT, format);}while(0)
// Print with level
// Max string size 256 chars
#define SPIRIT_LOG_PRINTF_LEVEL(log, level, format...)              \
  do{if(log != 0 &&                                                 \
  SPIRIT_LOG_LEVEL(level))                                          \
    log->Printf(log, level, format);}while(0)

#endif

// Flush
#define SPIRIT_LOG_FLUSH(log)                                       \
  do{if(log != 0) log->Flush(log);}while(0)

// Set level
#define SPIRIT_LOG_SET_LEVEL(log, level)                            \
  do{if(log != 0) log->SetFlags(log, SPIRIT_LOG_LEVEL(level));}while(0)


#else // SPIRIT_LOG_DISABLE

#define SPIRIT_LOG_REGISTER_EVENT(log, id, name)
#define SPIRIT_LOG_REGISTER_EVENT_LEVEL(log, level, id, name)
#ifndef __GNUC__
#define SPIRIT_LOG_TRACE_EX(log, id, format, ...)
#define SPIRIT_LOG_PRINTF(log, format, ...)
#define SPIRIT_LOG_PRINTF_LEVEL(log, level, format, ...)
#else
#define SPIRIT_LOG_TRACE_EX(log, id, format, args...)
#define SPIRIT_LOG_PRINTF(log, format...)
#define SPIRIT_LOG_PRINTF_LEVEL(log, level, format...)
#endif
#define SPIRIT_LOG_TRACE(log, id)
#define SPIRIT_LOG_FLUSH(log)
#define SPIRIT_LOG_SET_LEVEL(log, level)

#endif // SPIRIT_LOG_DISABLE

#endif // _SPIRIT_LOG_TYPE_H_
