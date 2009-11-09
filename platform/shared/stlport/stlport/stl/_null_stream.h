/*
 * Copyright (c) 2000
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#ifndef _STLP_NULL_STREAM_H
# define _STLP_NULL_STREAM_H

_STLP_BEGIN_NAMESPACE

struct __null_stream
{
    void flush() { }
};

template <class _Tp>
__null_stream& operator <<(__null_stream& __x, const _Tp& )
{
    return __x;
}

template <class _Tp>
__null_stream& operator >>(const _Tp&, __null_stream& __x )
{
    return __x;
}

extern __null_stream cin, cout, cerr, endl, ws, hex, dec;

_STLP_END_NAMESPACE

# endif
