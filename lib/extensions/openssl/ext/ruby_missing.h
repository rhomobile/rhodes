/*
 * 'OpenSSL for Ruby' project
 * Copyright (C) 2001-2003  Michal Rokos <m.rokos@sh.cvut.cz>
 * All rights reserved.
 */
/*
 * This program is licensed under the same licence as Ruby.
 * (See the file 'LICENCE'.)
 */
#if !defined(_OSSL_RUBY_MISSING_H_)
#define _OSSL_RUBY_MISSING_H_

#define rb_define_copy_func(klass, func) \
	rb_define_method((klass), "initialize_copy", (func), 1)

#define FPTR_TO_FD(fptr) ((fptr)->fd)

#ifndef RB_INTEGER_TYPE_P
/* for Ruby 2.3 compatibility */
#define RB_INTEGER_TYPE_P(obj) (RB_FIXNUM_P(obj) || RB_TYPE_P(obj, T_BIGNUM))
#endif

//RHO 
//for Ruby 2.3 compatibility
#define RB_INT2FIX(i) (((VALUE)(i))<<1 | RUBY_FIXNUM_FLAG)
#define RB_LONG2FIX(i) RB_INT2FIX(i)
#define RB_ST2FIX(h) RB_LONG2FIX((long)(h))
#define ST2FIX(h) RB_ST2FIX(h)
//RHO

#endif /* _OSSL_RUBY_MISSING_H_ */
