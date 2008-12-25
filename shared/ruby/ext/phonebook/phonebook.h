#ifndef RUBY_PHONEBOOK_H
#define RUBY_PHONEBOOK_H 1

#ifdef __cplusplus
extern "C" {
#endif

/********************************************
 * List of supported phonebook's fields (visible in ruby)
 ********************************************/

static const char* RUBY_PB_ID = "id";
static const char* RUBY_PB_FIRST_NAME = "first_name";
static const char* RUBY_PB_LAST_NAME = "last_name";

#ifdef __cplusplus
}
#endif

#endif