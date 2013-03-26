#pragma once

#include "rhodes/JNIRhodes.h"

namespace rho {
namespace apiGenerator {

class MethodResultJni;

class MethodExecutorJni
{
    static const char * const METHOD_EXECUTOR_CLASS;

    static jclass s_MethodExecutorClass;
    static jmethodID s_midRun;
    static jmethodID s_midRunWithSeparateThread;
    static jmethodID s_midRunWithUiThread;

protected:
    static JNIEnv* jniInit(JNIEnv* env);
    static JNIEnv* jniInit();
    static jclass loadClass(JNIEnv* env, const char* const name);

public:
    static void run(JNIEnv* env, jobject jTask, MethodResultJni& result, bool thread, bool uiThread);

    virtual ~MethodExecutorJni() {}
};

}}
