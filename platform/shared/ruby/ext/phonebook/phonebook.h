#ifndef RUBY_PHONEBOOK_H
#define RUBY_PHONEBOOK_H 1

#ifdef __cplusplus
extern "C" {
#endif

/********************************************
 * List of supported phonebook's fields (visible in ruby)
 ********************************************/

#define RUBY_PB_ID "id"
#define RUBY_PB_PREFIX "prefix"
#define RUBY_PB_FIRST_NAME "first_name"
#define RUBY_PB_MIDDLE_NAME "middle_name" 
#define RUBY_PB_LAST_NAME "last_name"
#define RUBY_PB_SUFFIX "suffix"
#define RUBY_PB_NICKNAME "nickname"
	
#define RUBY_PB_NOTE "person_note"
	
#define RUBY_PB_BUSINESS_NUMBER "business_number" 
#define RUBY_PB_HOME_NUMBER "home_number"         
#define RUBY_PB_MOBILE_NUMBER "mobile_number"     

#define RUBY_PB_MAIN_MUMBER "main_number"         
#define RUBY_PB_PAGER_NUMBER "pager_number"       	
#define RUBY_PB_HOME_FAX "home_fax"               
#define RUBY_PB_WORK_FAX "work_fax"
#define RUBY_PB_ASSISTANT_NUMBER "assistant_number"
	
#define RUBY_PB_EMAIL_ADDRESS "email_address"
#define RUBY_PB_HOME_EMAIL_ADDRESS "home_email_address"
#define RUBY_PB_OTHER_EMAIL_ADDRESS "other_email_address"

#define RUBY_PB_COMPANY_NAME "company_name"
#define RUBY_PB_JOB_TITLE "job_title"
#define RUBY_PB_BIRTHDAY "birthday"

#define RUBY_PB_STREET_ADDRESS_1 "street_address_1"
#define RUBY_PB_CITY_1 "city_1"
#define RUBY_PB_STATE_1 "state_1"
#define RUBY_PB_ZIP_1 "zip_1"
#define RUBY_PB_COUNTRY_1 "country_1"

#define RUBY_PB_STREET_ADDRESS_2 "street_address_2"
#define RUBY_PB_CITY_2 "city_2"
#define RUBY_PB_STATE_2 "state_2"
#define RUBY_PB_ZIP_2 "zip_2"
#define RUBY_PB_COUNTRY_2 "country_2"

#define RUBY_PB_STREET_ADDRESS_3 "street_address_3"
#define RUBY_PB_CITY_3 "city_3"
#define RUBY_PB_STATE_3 "state_3"
#define RUBY_PB_ZIP_3 "zip_3"
#define RUBY_PB_COUNTRY_3 "country_3"
	
#define RUBY_PB_HOME_PAGE "home_page"

#define RUBY_PB_SPOUSE_NAME "spouse_name"
#define RUBY_PB_ASSISTANT_NAME "assistant_name"
	
#define RUBY_PB_ANNIVERSARY "anniversary"
#define RUBY_PB_CREATED "created"
#define RUBY_PB_UPDATED "updated"	
	
#ifdef __cplusplus
}
#endif

#endif