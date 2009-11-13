/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
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

#ifndef FSTREAM_IMPL_H
#define FSTREAM_IMPL_H

#include "_stdio_file.h"

_STLP_BEGIN_NAMESPACE
_STLP_MOVE_TO_PRIV_NAMESPACE

bool __is_regular_file(_STLP_fd fd);
streamoff __file_size(_STLP_fd fd);

_STLP_MOVE_TO_STD_NAMESPACE
_STLP_END_NAMESPACE

#endif /* FSTREAM_IMPL_H */
