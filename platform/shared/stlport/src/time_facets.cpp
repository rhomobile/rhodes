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

#include "stlport_prefix.h"

#include <cstdio>
#include <locale>
#include <istream>
#include <cstdio>

#include "c_locale.h"

_STLP_BEGIN_NAMESPACE

_STLP_MOVE_TO_PRIV_NAMESPACE

// default "C" values for month and day names

const char default_dayname[][14] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
  "Friday", "Saturday"};

const char default_monthname[][24] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
  "January", "February", "March", "April", "May", "June",
  "July", "August", "September", "October", "November", "December"};

// _Init_time_info: initialize table with
// "C" values (note these are not defined in the C standard, so this
// is somewhat arbitrary).

void _STLP_CALL _Init_timeinfo(_Time_Info& table) {
  int i;
  for (i = 0; i < 14; ++i)
    table._M_dayname[i] = default_dayname[i];
  for (i = 0; i < 24; ++i)
    table._M_monthname[i] = default_monthname[i];
  table._M_am_pm[0] = "AM";
  table._M_am_pm[1] = "PM";
  table._M_time_format = "%H:%M:%S";
  table._M_date_format = "%m/%d/%y";
  table._M_date_time_format = "%m/%d/%y";
}

void _STLP_CALL _Init_timeinfo(_Time_Info& table, _Locale_time * time) {
  if(!time)
    locale::_M_throw_runtime_error();

  int i;
  for (i = 0; i < 7; ++i)
    table._M_dayname[i] = _Locale_abbrev_dayofweek(time, i);
  for (i = 0; i < 7; ++i)
    table._M_dayname[i+7] = _Locale_full_dayofweek(time, i);
  for (i = 0; i < 12; ++i)
    table._M_monthname[i] = _Locale_abbrev_monthname(time, i);
  for (i = 0; i < 12; ++i)
    table._M_monthname[i+12] = _Locale_full_monthname(time, i);
  table._M_am_pm[0] = _Locale_am_str(time);
  table._M_am_pm[1] = _Locale_pm_str(time);
  table._M_time_format = _Locale_t_fmt(time);
  if ( table._M_time_format == "%T" ) {
    table._M_time_format = "%H:%M:%S";
  } else if ( table._M_time_format == "%r" ) {
    table._M_time_format = "%I:%M:%S %p";
  } else if ( table._M_time_format == "%R" ) {
    table._M_time_format = "%H:%M";
  }
  table._M_date_format = _Locale_d_fmt(time);
  table._M_date_time_format = _Locale_d_t_fmt(time);
  table._M_long_date_format = _Locale_long_d_fmt(time);
  table._M_long_date_time_format = _Locale_long_d_t_fmt(time);
}

inline char* __subformat(const string& format, char*& buf, size_t buf_size,
                         const _Time_Info&  table, const tm* t) {
  const char * cp = format.data();
  const char * cp_end = cp + format.size();
  while (cp != cp_end) {
    if (*cp == '%') {
      char mod = 0;
      ++cp;
      if(*cp == '#') {
        mod = *cp; ++cp;
      }
      char *former_buf = buf;
      buf = __write_formatted_time(buf, buf_size, *cp++, mod, table, t);
      buf_size -= (buf - former_buf);
    } else
      *buf++ = *cp++;
  }
  return buf;
}

#if defined (__GNUC__)
/* The number of days from the first day of the first ISO week of this
   year to the year day YDAY with week day WDAY.  ISO weeks start on
   Monday; the first ISO week has the year's first Thursday.  YDAY may
   be as small as YDAY_MINIMUM.  */
#  define __ISO_WEEK_START_WDAY 1 /* Monday */
#  define __ISO_WEEK1_WDAY 4 /* Thursday */
#  define __YDAY_MINIMUM (-366)
#  define __TM_YEAR_BASE 1900
static int
__iso_week_days(int yday, int wday) {
  /* Add enough to the first operand of % to make it nonnegative.  */
  int big_enough_multiple_of_7 = (-__YDAY_MINIMUM / 7 + 2) * 7;
  return (yday
          - (yday - wday + __ISO_WEEK1_WDAY + big_enough_multiple_of_7) % 7
          + __ISO_WEEK1_WDAY - __ISO_WEEK_START_WDAY);
}

#  define __is_leap(year)\
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

#endif

#define __hour12(hour) \
  (((hour) % 12 == 0) ? (12) : (hour) % 12)

#if !defined (_STLP_USE_SAFE_STRING_FUNCTIONS)
#  define _STLP_SPRINTF(B, BS, F, D) sprintf(B, F, D)
#else
#  define _STLP_SPRINTF(B, BS, F, D) sprintf_s(B, BS, F, D)
#endif

char * _STLP_CALL __write_formatted_time(char* buf, size_t buf_size, char format, char modifier,
                                         const _Time_Info& table, const tm* t) {
  switch (format) {
    case 'a':
      return copy(table._M_dayname[t->tm_wday].begin(),
                  table._M_dayname[t->tm_wday].end(),
                  buf);

    case 'A':
      return copy(table._M_dayname[t->tm_wday+7].begin(),
                  table._M_dayname[t->tm_wday+7].end(),
                  buf);

    case 'b':
      return copy(table._M_monthname[t->tm_mon].begin(),
                  table._M_monthname[t->tm_mon].end(),
                  buf);

    case 'B':
      return copy(table._M_monthname[t->tm_mon+12].begin(),
                  table._M_monthname[t->tm_mon+12].end(),
                  buf);

    case 'c': {
      const char *cp = (modifier != '#') ?
        table._M_date_time_format.data() :
        table._M_long_date_time_format.data();
      const char* cp_end = cp +
        ((modifier != '#') ? table._M_date_time_format.size() :
         table._M_long_date_time_format.size() );
      char mod;
      while (cp != cp_end) {
        if (*cp == '%') {
          ++cp; if(*cp == '#') mod = *cp++; else mod = 0;
          char *buf_pos = buf;
          buf = __write_formatted_time(buf, buf_size, *cp++, mod, table, t);
          buf_size -= (buf - buf_pos);
        }
        else {
          *buf++ = *cp++; --buf_size;
        }
      }
      return buf;
    }

    case 'd':
      _STLP_SPRINTF(buf, buf_size, (modifier != '#')?"%.2ld":"%ld", (long)t->tm_mday);
      return ((long)t->tm_mday < 10L && modifier == '#')?buf+1:buf + 2;

    case 'e':
      _STLP_SPRINTF(buf, buf_size, "%2ld", (long)t->tm_mday);
      return buf + 2;

    case 'H':
      _STLP_SPRINTF(buf, buf_size, (modifier != '#')?"%.2ld":"%ld", (long)t->tm_hour);
      return ((long)t->tm_hour < 10L && modifier == '#')?buf+1:buf + 2;

    case 'I':
      _STLP_SPRINTF(buf, buf_size, (modifier != '#')?"%.2ld":"%ld", (long)__hour12(t->tm_hour));
      return ((long)__hour12(t->tm_hour) < 10L && modifier == '#')?buf+1:buf + 2;

    case 'j':
      return __write_integer(buf, 0, (long)((long)t->tm_yday + 1));

    case 'm':
      _STLP_SPRINTF(buf, buf_size, (modifier != '#')?"%.2ld":"%ld", (long)t->tm_mon + 1);
      return ((long)(t->tm_mon + 1) < 10L && modifier == '#')?buf+1:buf + 2;

    case 'M':
      _STLP_SPRINTF(buf, buf_size, (modifier != '#')?"%.2ld":"%ld", (long)t->tm_min);
      return ((long)t->tm_min < 10L && modifier == '#')?buf+1:buf + 2;

    case 'p':
      return copy(table._M_am_pm[t->tm_hour/12].begin(),
                  table._M_am_pm[t->tm_hour/12].end(),
                  buf);

    case 'S': // pad with zeros
       _STLP_SPRINTF(buf, buf_size, (modifier != '#')?"%.2ld":"%ld", (long)t->tm_sec);
       return ((long)t->tm_sec < 10L && modifier == '#')?buf+1:buf + 2;

    case 'U':
      return __write_integer(buf, 0,
                             long((t->tm_yday - t->tm_wday + 7) / 7));
      //      break;

    case 'w':
      return __write_integer(buf, 0, (long)t->tm_wday);
      //      break;

    case 'W':
      return __write_integer(buf, 0,
                             (long)(t->tm_wday == 0 ? (t->tm_yday + 1) / 7 :
                                                      (t->tm_yday + 8 - t->tm_wday) / 7));

    case'x': {
      const char * cp = (modifier != '#') ? table._M_date_format.data():
                                            table._M_long_date_format.data();
      const char* cp_end = (modifier != '#') ? cp + table._M_date_format.size():
                                               cp + table._M_long_date_format.size();
      char mod;
      while (cp != cp_end) {
        if (*cp == '%') {
          ++cp; if(*cp == '#') mod = *cp++; else mod = 0;
          char *buf_pos = buf;
          buf = __write_formatted_time(buf, buf_size, *cp++, mod, table, t);
          buf_size -= (buf - buf_pos);
        }
        else {
          *buf++ = *cp++; --buf_size;
        }
      }
      return buf;
    }

    case 'X': {
      const char * cp = table._M_time_format.data();
      const char* cp_end = cp + table._M_time_format.size();
      char mod;
      while (cp != cp_end) {
        if (*cp == '%') {
          ++cp; if(*cp == '#') mod = *cp++; else mod = 0;
          char *buf_pos = buf;
          buf = __write_formatted_time(buf, buf_size, *cp++, mod, table, t);
          buf_size -= (buf - buf_pos);
        }
        else {
          *buf++ = *cp++; --buf_size;
        }
      }
      return buf;
    }
    case 'y':
      return __write_integer(buf, 0, (long)((long)(t->tm_year + 1900) % 100));

    case 'Y':
      return __write_integer(buf, 0, (long)((long)t->tm_year + 1900));

    case '%':
      *buf++ = '%';
      return buf;

#if defined (__GNUC__)
      // fbp : at least on SUN
#  if defined (_STLP_UNIX) && !defined (__linux__)
#    define __USE_BSD 1
#  endif

   /*********************************************
    *     JGS, handle various extensions        *
    *********************************************/

    case 'h': /* POSIX.2 extension */
      // same as 'b', abbrev month name
      return copy(table._M_monthname[t->tm_mon].begin(),
                  table._M_monthname[t->tm_mon].end(),
                  buf);

    case 'C': /* POSIX.2 extension */
      // same as 'd', the day
      _STLP_SPRINTF(buf, buf_size, "%2ld", (long)t->tm_mday);
      return buf + 2;

    case 'D': /* POSIX.2 extension */
      // same as 'x'
      return __subformat(table._M_date_format, buf, buf_size, table, t);

    case 'k': /* GNU extension */
      _STLP_SPRINTF(buf, buf_size, "%2ld", (long)t->tm_hour);
      return buf + 2;

    case 'l': /* GNU extension */
      _STLP_SPRINTF(buf, buf_size, "%2ld", (long)t->tm_hour % 12);
      return buf + 2;

    case 'n': /* POSIX.2 extension */
      *buf++ = '\n';
      return buf;

    case 'R': /* GNU extension */
      return __subformat("%H:%M", buf, buf_size, table, t);

    case 'r': /* POSIX.2 extension */
      return __subformat("%I:%M:%S %p", buf, buf_size, table, t);

    case 'T': /* POSIX.2 extension.  */
      return __subformat("%H:%M:%S", buf, buf_size, table, t);

    case 't': /* POSIX.2 extension.  */
      *buf++ = '\t';
      return buf;

    case 'u': /* POSIX.2 extension.  */
      return __write_integer(buf, 0, long((t->tm_wday - 1 + 7)) % 7 + 1);

    case 's': {
      time_t __t;
      __t = mktime(__CONST_CAST(tm*, t));
      return __write_integer(buf, 0, (long)__t );
    }
    case 'g': /* GNU extension */
    case 'G': {
      int year = t->tm_year + __TM_YEAR_BASE;
      int days = __iso_week_days (t->tm_yday, t->tm_wday);
      if (days < 0) {
        /* This ISO week belongs to the previous year.  */
        year--;
        days = __iso_week_days (t->tm_yday + (365 + __is_leap (year)), t->tm_wday);
      }
      else {
        int d = __iso_week_days (t->tm_yday - (365 + __is_leap (year)), t->tm_wday);
        if (0 <= d) {
          /* This ISO week belongs to the next year.  */
          ++year;
          days = d;
        }
      }
      switch (format) {
      case 'g':
        return __write_integer(buf, 0, (long)(year % 100 + 100) % 100);
      case 'G':
        return __write_integer(buf, 0, (long)year);
      default:
        return __write_integer(buf, 0, (long)days / 7 + 1);
      }
    }

#  if defined (_STLP_USE_GLIBC) && ! defined (__CYGWIN__)
    case 'z':   /* GNU extension.  */
      if (t->tm_isdst < 0)
        break;
      {
        int diff;
#    if defined (__USE_BSD) || defined (__BEOS__)
        diff = t->tm_gmtoff;
#    else
        diff = t->__tm_gmtoff;
#    endif
        if (diff < 0) {
          *buf++ = '-';
          diff = -diff;
        } else
          *buf++ = '+';
        diff /= 60;
        _STLP_SPRINTF(buf, buf_size, "%.4d", (diff / 60) * 100 + diff % 60);
        return buf + 4;
      }
#  endif /* __GLIBC__ */
#endif /* __GNUC__ */

    default:
      //      return buf;
      break;
  }
  return buf;
}

time_base::dateorder _STLP_CALL
__get_date_order(_Locale_time* time) {
  const char * fmt = _Locale_d_fmt(time);
  char first, second, third;

  while (*fmt != 0 && *fmt != '%') ++fmt;
  if (*fmt == 0)
    return time_base::no_order;
  first = *++fmt;
  while (*fmt != 0 && *fmt != '%') ++fmt;
  if (*fmt == 0)
    return time_base::no_order;
  second = *++fmt;
  while (*fmt != 0 && *fmt != '%') ++fmt;
  if (*fmt == 0)
    return time_base::no_order;
  third = *++fmt;

  switch (first) {
    case 'd':
      return (second == 'm' && third == 'y') ? time_base::dmy
                                             : time_base::no_order;
    case 'm':
      return (second == 'd' && third == 'y') ? time_base::mdy
                                             : time_base::no_order;
    case 'y':
      switch (second) {
        case 'd':
          return third == 'm' ? time_base::ydm : time_base::no_order;
        case 'm':
          return third == 'd' ? time_base::ymd : time_base::no_order;
        default:
          return time_base::no_order;
      }
    default:
      return time_base::no_order;
  }
}

_STLP_MOVE_TO_STD_NAMESPACE

#if !defined(_STLP_NO_FORCE_INSTANTIATE)
template class time_get<char, istreambuf_iterator<char, char_traits<char> > >;
template class time_put<char, ostreambuf_iterator<char, char_traits<char> > >;

#  if !defined (_STLP_NO_WCHAR_T)
template class time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
template class time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
#  endif

#endif

_STLP_END_NAMESPACE
