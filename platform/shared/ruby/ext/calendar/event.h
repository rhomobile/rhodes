#ifndef RUBY_EVENT_H
#define RUBY_EVENT_H 1

#ifdef __cplusplus
extern "C" {
#endif

/********************************************
 * List of supported event's fields (visible in ruby)
 ********************************************/

#define RUBY_EV_ID "id"
#define RUBY_EV_TITLE "title"
#define RUBY_EV_CANCELED "canceled"
#define RUBY_EV_ORGANIZER "organizer"
#define RUBY_EV_START_DATE "start_date"
#define RUBY_EV_END_DATE "end_date"
#define RUBY_EV_LAST_MODIFIED "last_modified"
#define RUBY_EV_LOCATION "location"
#define RUBY_EV_NOTES "notes"
#define RUBY_EV_PRIVACY "privacy"
#define RUBY_EV_REMINDER "reminder"
#define RUBY_EV_RECURRENCE "recurrence"
#define RUBY_EV_RECURRENCE_FREQUENCY "frequency"
#define RUBY_EV_RECURRENCE_FREQUENCY_DAILY "daily"
#define RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY "weekly"
#define RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY "monthly"
#define RUBY_EV_RECURRENCE_FREQUENCY_YEARLY "yearly"
#define RUBY_EV_RECURRENCE_INTERVAL "interval"
#define RUBY_EV_RECURRENCE_COUNT  "count"
#define RUBY_EV_RECURRENCE_END  "end_date"
#define RUBY_EV_RECURRENCE_DAYOFMONTH "day_of_month"
#define RUBY_EV_RECURRENCE_MONTHS  "months"
#define RUBY_EV_RECURRENCE_DAYS  "days"
#define RUBY_FETCH_include_repeating "include_repeating"

#ifdef __cplusplus
}
#endif

#endif
