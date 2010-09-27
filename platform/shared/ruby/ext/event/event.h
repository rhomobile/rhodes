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
#define RUBY_EV_ATTENDEES "attendees"
#define RUBY_EV_START_DATE "start_date"
#define RUBY_EV_END_DATE "end_date"
#define RUBY_EV_LAST_MODIFIED "last_modified"
#define RUBY_EV_LOCATION "location"
#define RUBY_EV_NOTES "notes"
#define RUBY_EV_RECURRENCE "recurrence"
#define RUBY_EV_RECURRENCE_FREQUENCY "frequency"
#define RUBY_EV_RECURRENCE_FREQUENCY_DAILY "daily"
#define RUBY_EV_RECURRENCE_FREQUENCY_WEEKLY "weekly"
#define RUBY_EV_RECURRENCE_FREQUENCY_MONTHLY "monthly"
#define RUBY_EV_RECURRENCE_FREQUENCY_YEARLY "yearly"
#define RUBY_EV_RECURRENCE_INTERVAL "interval"

#ifdef __cplusplus
}
#endif

#endif
