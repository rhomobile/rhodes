/* ans_push.i */
%module AnsPush
%{
#include "ruby/ext/rho/rhoruby.h"

extern void anspush_ans_register(const char* username, const char* password, const char* session);
extern void anspush_ans_unregister(const char* session);
extern void anspush_check_ans_registration(const char* session);
extern void anspush_set_push_notification(const char* callback_uri, const char* options);

#define ans_register anspush_ans_register
#define ans_unregister anspush_ans_unregister
#define check_ans_registration anspush_check_ans_registration
#define set_push_notification anspush_set_push_notification

%}

extern void ans_register(const char* username, const char* password, const char* session);
extern void ans_unregister(const char* session);
extern void check_ans_registration(const char* session);
extern void set_push_notification(const char* callback_uri, const char* options);
