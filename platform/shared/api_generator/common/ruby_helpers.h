#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H

VALUE rho_create_object_with_id( VALUE klass, const char* szID );
const char* rho_get_object_id( VALUE valObj );
int rho_ruby_is_string(VALUE val);
int rho_ruby_is_array(VALUE val);

#ifdef __cplusplus
}
#endif
