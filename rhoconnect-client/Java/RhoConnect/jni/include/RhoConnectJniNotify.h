#ifndef RHOCONNECTJNINOTIFY_H_INCLUDED
#define RHOCONNECTJNINOTIFY_H_INCLUDED

#include "rhodes/JNIRhodes.h"

namespace rho { namespace connect_jni {

//template <class Ret, class P>
//class Bind1
//{
//public:
//    typedef P param_t;
//    typedef Ret return_t;
//    typedef return_t (*callback_t)(param_t);
//private:
//    callback_t m_callback;
//    param_t m_param;
//public:
//    Bind1(callback_t fn, param_t param) : m_callback(fn), m_param(param) {}
//    return_t operator()() { return m_callback(m_param); }
//};
//
//template <class Ret, class P0, class P1>
//class Bind2
//{
//public:
//    typedef P0 param0_t;
//    typedef P1 param1_t;
//    typedef Ret return_t;
//    typedef return_t (*callback_t)(param0_t, param1_t);
//private:
//    callback_t m_callback;
//    param0_t m_param0;
//    param1_t m_param1;
//public:
//    Bind2(callback_t fn, param0_t param0, param1_t param1) : m_callback(fn), m_param0(param0), m_param1(param1) {}
//    return_t operator()() { return m_callback(m_param0, m_param1); }
//};
//
//template <class Ret, class P0, class P1, class P2>
//class Bind3
//{
//public:
//    typedef P0 param0_t;
//    typedef P1 param1_t;
//    typedef P2 param2_t;
//    typedef Ret return_t;
//    typedef return_t (*callback_t)(param0_t, param1_t, param2_t);
//private:
//    callback_t m_callback;
//    param0_t m_param0;
//    param1_t m_param1;
//    param2_t m_param2;
//public:
//    Bind3(callback_t fn, param0_t param0, param1_t param1, param2_t param2)
//    : m_callback(fn), m_param0(param0), m_param1(param1), m_param2(param2) {}
//    return_t operator()() { return m_callback(m_param0, m_param1, m_param2); }
//};
//
//template <class Ret, class P>
//Bind1<Ret, P> make_bind(typename Bind1<Ret, P>::callback_t fn, typename Bind1<Ret, P>::param_t param)
//{
//    return Bind1<Ret, P>(fn, param);
//}
//
//template <class Ret, class P0, class P1>
//Bind2<Ret, P0, P1> make_bind(typename Bind2<Ret, P0, P1>::callback_t fn,
//                             typename Bind2<Ret, P0, P1>::param0_t param0,
//                             typename Bind2<Ret, P0, P1>::param1_t param1)
//{
//    return Bind2<Ret, P0, P1>(fn, param0, param1);
//}
//
//template <class Ret, class P0, class P1, class P2>
//Bind3<Ret, P0, P1, P2> make_bind(typename Bind3<Ret, P0, P1, P2>::callback_t fn,
//                             typename Bind3<Ret, P0, P1, P2>::param0_t param0,
//                             typename Bind3<Ret, P0, P1, P2>::param1_t param1,
//                             typename Bind3<Ret, P0, P1, P2>::param2_t param2)
//{
//    return Bind3<Ret, P0, P1, P2>(fn, param0, param1, param2);
//}

jhobject rhoconnect_jni_parsenotify(JNIEnv * env, const char* res);

//template <class functor>
//jhobject rhoconnect_call(JNIEnv * env, functor fn)
//{
//    return rhoconnect_jni_parsenotify(env, reinterpret_cast<char*>(fn()));
//}

}}

#endif // RHOCONNECTJNINOTIFY_H_INCLUDED
