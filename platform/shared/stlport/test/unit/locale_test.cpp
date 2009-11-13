#include <string>
#if !defined (STLPORT) || !defined (_STLP_USE_NO_IOSTREAMS)
#  include <sstream>
#  include <locale>
#  include <stdexcept>
#  include <memory>
#  include <algorithm>
//#  include <iostream>

#  include "cppunit/cppunit_proxy.h"

#  if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#  endif

struct ref_locale {
  const char *name;
  const char *decimal_point;
  const char *thousands_sep;
  const char *money_int_prefix;
  const char *money_int_prefix_old;
  const char *money_prefix;
  const char *money_suffix;
  const char *money_decimal_point;
  const char *money_thousands_sep;
};

// Pls, don't write #ifdef _STLP_REAL_LOCALE_IMPLEMENTED here!
// It undefined in any case!!!!!

static const ref_locale tested_locales[] = {
//{  name,         decimal_point, thousands_sep, money_int_prefix, money_int_prefix_old, money_prefix, money_suffix, money_decimal_point, money_thousands_sep},
  { "fr_FR",       ",",           "\xa0",        "EUR ",           "FRF ",               "",           "",           ",",
#  if defined (WIN32) || defined (_WIN32)
                                                                                                                                          "\xa0" },
#  else
                                                                                                                                          " " },
#  endif
  { "ru_RU.koi8r", ",",           ".",           "RUB ",           "RUR ",               "",           "\xd2\xd5\xc2", ".",               " " },
  { "en_GB",       ".",           ",",           "GBP ",           "",                   "\xa3",       "",           ".",                 "," },
  { "en_US",       ".",           ",",           "USD ",           "",                   "$",          "",           ".",                 "," },
  { "C",           ".",           ",",           "",               "",                   "",           "",           " ",                 " " },
};


//
// TestCase class
//
class LocaleTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(LocaleTest);
#  if defined (STLPORT) && !defined (_STLP_USE_EXCEPTIONS)
  CPPUNIT_IGNORE;
#  endif
  CPPUNIT_TEST(locale_by_name);
  CPPUNIT_STOP_IGNORE;
  CPPUNIT_TEST(loc_has_facet);
  CPPUNIT_TEST(num_put_get);
  CPPUNIT_TEST(money_put_get);
  CPPUNIT_TEST(money_put_X_bug);
  CPPUNIT_TEST(time_put_get);
#  if defined (__DMC__) && defined (_DLL)
  CPPUNIT_IGNORE;
#  endif
  CPPUNIT_TEST(collate_facet);
  CPPUNIT_TEST(ctype_facet);
#  if defined (STLPORT) && defined (_STLP_NO_MEMBER_TEMPLATES)
  CPPUNIT_IGNORE;
#  endif
  CPPUNIT_TEST(locale_init_problem);
  CPPUNIT_STOP_IGNORE;
  CPPUNIT_TEST(default_locale);
#  if !defined (STLPORT)
  CPPUNIT_IGNORE;
#  endif
  CPPUNIT_TEST(facet_id);
  CPPUNIT_STOP_IGNORE;
#  if defined (STLPORT) && \
     (!defined (_STLP_USE_EXCEPTIONS) || defined (_STLP_NO_MEMBER_TEMPLATES) || defined (_STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS))
  CPPUNIT_IGNORE;
#  endif
  CPPUNIT_TEST(combine);
  CPPUNIT_TEST_SUITE_END();

public:
  void locale_by_name();
  void loc_has_facet();
  void num_put_get();
  void money_put_get();
  void time_put_get();
  void collate_facet();
  void ctype_facet();
  void locale_init_problem();
  void money_put_X_bug();
  void default_locale();
  void facet_id();
  void combine();
private:
  void _loc_has_facet( const locale&, const ref_locale& );
  void _num_put_get( const locale&, const ref_locale& );
  void _money_put_get( const locale&, const ref_locale& );
  void _money_put_get2( const locale& loc, const locale& streamLoc, const ref_locale& );
  void _time_put_get( const locale&, const ref_locale& );
  void _ctype_facet( const locale&, const ref_locale& );
  void _locale_init_problem( const locale&, const ref_locale& );
  void _money_put_X_bug( const locale&, const ref_locale& );
};

CPPUNIT_TEST_SUITE_REGISTRATION(LocaleTest);

//
// tests implementation
//
void LocaleTest::_num_put_get( const locale& loc, const ref_locale& rl ) {
  CPPUNIT_ASSERT( has_facet<numpunct<char> >(loc) );
  numpunct<char> const& npct = use_facet<numpunct<char> >(loc);
  CPPUNIT_ASSERT( npct.decimal_point() == *rl.decimal_point );

  float val = 1234.56f;
  ostringstream fostr;
  fostr.imbue(loc);
  fostr << val;

  string ref = "1";
  if (!npct.grouping().empty()) {
    ref += npct.thousands_sep();
  }
  ref += "234";
  ref += npct.decimal_point();
  ref += "56";
  //cout << "In " << loc.name() << " 1234.56 is written: " << fostr.str() << endl;
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 12345678.9f;
  ref = "1";
  ref += npct.decimal_point();
  ref += "23457e+07";
  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 1000000000.0f;
  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == "1e+09" );

  val = 1234.0f;
  ref = "1";
  if (!npct.grouping().empty()) {
    ref += npct.thousands_sep();
  }
  ref += "234";
  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == ref );

  val = 10000001.0f;
  fostr.str("");
  fostr << val;
  CPPUNIT_ASSERT( fostr.str() == "1e+07" );
}

void LocaleTest::_money_put_get( const locale& loc, const ref_locale& rl )
{
  _money_put_get2(loc, loc, rl);
}

void LocaleTest::_money_put_get2( const locale& loc, const locale& streamLoc, const ref_locale& rl )
{
  CPPUNIT_ASSERT( has_facet<money_put<char> >(loc) );
  money_put<char> const& fmp = use_facet<money_put<char> >(loc);
  CPPUNIT_ASSERT( has_facet<money_get<char> >(loc) );
  money_get<char> const& fmg = use_facet<money_get<char> >(loc);

  ostringstream ostr;
  ostr.imbue(streamLoc);
  ostr << showbase;

  //Check a positive value (international format)
  {
    string str_res;
    //money_put
    {
      CPPUNIT_ASSERT( (has_facet<moneypunct<char, true> >(loc)) );
      moneypunct<char, true> const& intl_fmp = use_facet<moneypunct<char, true> >(loc);

      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, true, ostr, ' ', 123456);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      //On a positive value we skip the sign field if exists:
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }
      // international currency abbreviation, if it is before value

      /*
       * int_curr_symbol
       *
       *   The international currency symbol. The operand is a four-character
       *   string, with the first three characters containing the alphabetic
       *   international currency symbol in accordance with those specified
       *   in the ISO 4217 specification. The fourth character is the character used
       *   to separate the international currency symbol from the monetary quantity.
       *
       * (http://www.opengroup.org/onlinepubs/7990989775/xbd/locale.html)
       */
      string::size_type p = strlen( rl.money_int_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( intl_fmp.pos_format().field[fieldIndex] == money_base::symbol );
        string::size_type p_old = strlen( rl.money_int_prefix_old );
        CPPUNIT_ASSERT( (str_res.substr(index, p) == rl.money_int_prefix) ||
                        ((p_old != 0) && (str_res.substr(index, p_old) ==  rl.money_int_prefix_old)) );
        if ( str_res.substr(index, p) == rl.money_int_prefix ) {
          index += p;
        } else {
          index += p_old;
        }
        ++fieldIndex;
      }

      // space after currency
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::space ||
          intl_fmp.pos_format().field[fieldIndex] == money_base::none) {
        // iternational currency symobol has four chars, one of these chars
        // is separator, so if format has space on this place, it should
        // be skipped.
        ++fieldIndex;
      }

      // sign
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }

      // value
      CPPUNIT_ASSERT( str_res[index++] == '1' );
      if (!intl_fmp.grouping().empty()) {
        CPPUNIT_ASSERT( str_res[index++] == /* intl_fmp.thousands_sep() */ *rl.money_thousands_sep );
      }
      CPPUNIT_ASSERT( str_res[index++] == '2' );
      CPPUNIT_ASSERT( str_res[index++] == '3' );
      CPPUNIT_ASSERT( str_res[index++] == '4' );
      if (intl_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == /* intl_fmp.decimal_point() */ *rl.money_decimal_point );
      }
      CPPUNIT_ASSERT( str_res[index++] == '5' );
      CPPUNIT_ASSERT( str_res[index++] == '6' );
      ++fieldIndex;

      // sign
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }

      // space
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::space ) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      // sign
      if (intl_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        ++fieldIndex;
      }

      //as space cannot be last the only left format can be none:
      while ( fieldIndex < 3 ) {
        CPPUNIT_ASSERT( intl_fmp.pos_format().field[fieldIndex] == money_base::none );
        ++fieldIndex;
      }
    }

    //money_get
    {
      ios_base::iostate err = ios_base::goodbit;
      string digits;

      istringstream istr(str_res);
      ostr.str( "" );
      ostr.clear();
      fmg.get(istr, istreambuf_iterator<char, char_traits<char> >(), true, ostr, err, digits);
      CPPUNIT_ASSERT( (err & (ios_base::failbit | ios_base::badbit)) == 0 );
      CPPUNIT_ASSERT( digits == "123456" );
    }
  }

  ostr.str("");
  //Check a negative value (national format)
  {
    CPPUNIT_ASSERT( (has_facet<moneypunct<char, false> >(loc)) );
    moneypunct<char, false> const& dom_fmp = use_facet<moneypunct<char, false> >(loc);
    string str_res;
    //Check money_put
    {
      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, false, ostr, ' ', -123456);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      if (dom_fmp.neg_format().field[fieldIndex] == money_base::sign) {
        CPPUNIT_ASSERT( str_res.substr(index, dom_fmp.negative_sign().size()) == dom_fmp.negative_sign() );
        index += dom_fmp.negative_sign().size();
        ++fieldIndex;
      }

      string::size_type p = strlen( rl.money_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_prefix );
        index += p;
        ++fieldIndex;
      }
      if (dom_fmp.neg_format().field[fieldIndex] == money_base::space ||
          dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      CPPUNIT_ASSERT( str_res[index++] == '1' );
      if (!dom_fmp.grouping().empty()) {
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.thousands_sep() );
      }
      CPPUNIT_ASSERT( str_res[index++] == '2' );
      CPPUNIT_ASSERT( str_res[index++] == '3' );
      CPPUNIT_ASSERT( str_res[index++] == '4' );
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.decimal_point() );
      }
      CPPUNIT_ASSERT( str_res[index++] == '5' );
      CPPUNIT_ASSERT( str_res[index++] == '6' );
      ++fieldIndex;

      //space cannot be last:
      if ((fieldIndex < 3) &&
          dom_fmp.neg_format().field[fieldIndex] == money_base::space) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      if (fieldIndex == 3) {
        //If none is last we should not add anything to the resulting string:
        if (dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
          CPPUNIT_ASSERT( index == str_res.size() );
        } else {
          CPPUNIT_ASSERT( dom_fmp.neg_format().field[fieldIndex] == money_base::symbol );
          CPPUNIT_ASSERT( str_res.substr(index, strlen(rl.money_suffix)) == rl.money_suffix );
        }
      }
    }

    //money_get
    {
      ios_base::iostate err = ios_base::goodbit;
#  if defined (STLPORT)
      _STLP_LONGEST_FLOAT_TYPE val;
#  else
      long double val;
#  endif

      istringstream istr(str_res);
      fmg.get(istr, istreambuf_iterator<char, char_traits<char> >(), false, ostr, err, val);
      CPPUNIT_ASSERT( (err & (ios_base::failbit | ios_base::badbit)) == 0 );
      if (dom_fmp.negative_sign().empty()) {
        //Without negative sign there is no way to guess the resulting amount sign ("C" locale):
        CPPUNIT_ASSERT( val == 123456 );
      }
      else {
        CPPUNIT_ASSERT( val == -123456 );
      }
    }
  }
}


// Test for bug in case when number of digits in value less then number
// of digits in fraction. I.e. '9' should be printed as '0.09',
// if x.frac_digits() == 2.

void LocaleTest::_money_put_X_bug( const locale& loc, const ref_locale& rl )
{
  CPPUNIT_ASSERT( has_facet<money_put<char> >(loc) );
  money_put<char> const& fmp = use_facet<money_put<char> >(loc);

  ostringstream ostr;
  ostr.imbue(loc);
  ostr << showbase;

  // ostr.str("");
  // Check value with one decimal digit:
  {
    CPPUNIT_ASSERT( (has_facet<moneypunct<char, false> >(loc)) );
    moneypunct<char, false> const& dom_fmp = use_facet<moneypunct<char, false> >(loc);
    string str_res;
    // Check money_put
    {
      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, false, ostr, ' ', 9);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      if (dom_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        CPPUNIT_ASSERT( str_res.substr(index, dom_fmp.positive_sign().size()) == dom_fmp.positive_sign() );
        index += dom_fmp.positive_sign().size();
        ++fieldIndex;
      }

      string::size_type p = strlen( rl.money_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_prefix );
        index += p;
        ++fieldIndex;
      }
      if (dom_fmp.neg_format().field[fieldIndex] == money_base::space ||
          dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == '0' );
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.decimal_point() );
        for ( int fd = 1; fd < dom_fmp.frac_digits(); ++fd ) {
          CPPUNIT_ASSERT( str_res[index++] == '0' );
        }
      }
      CPPUNIT_ASSERT( str_res[index++] == '9' );
      ++fieldIndex;

      //space cannot be last:
      if ((fieldIndex < 3) &&
          dom_fmp.neg_format().field[fieldIndex] == money_base::space) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      if (fieldIndex == 3) {
        //If none is last we should not add anything to the resulting string:
        if (dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
          CPPUNIT_ASSERT( index == str_res.size() );
        } else {
          CPPUNIT_ASSERT( dom_fmp.neg_format().field[fieldIndex] == money_base::symbol );
          CPPUNIT_ASSERT( str_res.substr(index, strlen(rl.money_suffix)) == rl.money_suffix );
        }
      }
    }
  }

  ostr.str("");
  // Check value with two decimal digit:
  {
    CPPUNIT_ASSERT( (has_facet<moneypunct<char, false> >(loc)) );
    moneypunct<char, false> const& dom_fmp = use_facet<moneypunct<char, false> >(loc);
    string str_res;
    // Check money_put
    {
      ostreambuf_iterator<char, char_traits<char> > res = fmp.put(ostr, false, ostr, ' ', 90);

      CPPUNIT_ASSERT( !res.failed() );
      str_res = ostr.str();

      size_t fieldIndex = 0;
      size_t index = 0;

      if (dom_fmp.pos_format().field[fieldIndex] == money_base::sign) {
        CPPUNIT_ASSERT( str_res.substr(index, dom_fmp.positive_sign().size()) == dom_fmp.positive_sign() );
        index += dom_fmp.positive_sign().size();
        ++fieldIndex;
      }

      string::size_type p = strlen( rl.money_prefix );
      if (p != 0) {
        CPPUNIT_ASSERT( str_res.substr(index, p) == rl.money_prefix );
        index += p;
        ++fieldIndex;
      }
      if (dom_fmp.neg_format().field[fieldIndex] == money_base::space ||
          dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == '0' );
        CPPUNIT_ASSERT( str_res[index++] == dom_fmp.decimal_point() );
        for ( int fd = 1; fd < dom_fmp.frac_digits() - 1; ++fd ) {
          CPPUNIT_ASSERT( str_res[index++] == '0' );
        }
      }
      CPPUNIT_ASSERT( str_res[index++] == '9' );
      if (dom_fmp.frac_digits() != 0) {
        CPPUNIT_ASSERT( str_res[index++] == '0' );
      }
      ++fieldIndex;

      //space cannot be last:
      if ((fieldIndex < 3) &&
          dom_fmp.neg_format().field[fieldIndex] == money_base::space) {
        CPPUNIT_ASSERT( str_res[index++] == ' ' );
        ++fieldIndex;
      }

      if (fieldIndex == 3) {
        //If none is last we should not add anything to the resulting string:
        if (dom_fmp.neg_format().field[fieldIndex] == money_base::none) {
          CPPUNIT_ASSERT( index == str_res.size() );
        } else {
          CPPUNIT_ASSERT( dom_fmp.neg_format().field[fieldIndex] == money_base::symbol );
          CPPUNIT_ASSERT( str_res.substr(index, strlen(rl.money_suffix)) == rl.money_suffix );
        }
      }
    }
  }
}

void LocaleTest::_time_put_get( const locale& loc, const ref_locale&)
{
  CPPUNIT_ASSERT( has_facet<time_put<char> >(loc) );
  const time_put<char>& tmp = use_facet<time_put<char> >(loc);

  struct tm xmas = { 0, 0, 12, 25, 11, 93 };
  ostringstream ostr;
  ostr.imbue(loc);
  string format = "%B %d %Y";

  time_put<char>::iter_type ret = tmp.put(ostr, ostr, ' ', &xmas, format.data(), format.data() + format.size());
  CPPUNIT_ASSERT( !ret.failed() );

  /*
   * In other words, user conformation is required for reliable parsing
   * of user-entered dates and times, but machine-generated formats can be
   * parsed reliably. This allows parsers to be aggressive about interpreting
   * user variations on standard format.
   *
   *                                             ISO/IEC 14882, 22.2.5.1
   */
  CPPUNIT_ASSERT( has_facet<time_get<char> >(loc) );
  const time_get<char>& tmg = use_facet<time_get<char> >(loc);
  basic_ios<char> io(0);
  io.imbue(loc);

  istringstream istr( ostr.str() );
  istreambuf_iterator<char, char_traits<char> > i( istr );
  istreambuf_iterator<char, char_traits<char> > e;
  ios_base::iostate err = ios_base::goodbit;
  struct tm other = { 15, 20, 9, 14, 7, 105 };

  i = tmg.get_monthname( i, e, io, err, &other );
  CPPUNIT_ASSERT( err == ios_base::goodbit );
  CPPUNIT_ASSERT( other.tm_mon == xmas.tm_mon );

  ++i; ++i; ++i; ++i; // skip day of month and spaces around it
  i = tmg.get_year( i, e, io, err, &other );

  CPPUNIT_ASSERT( err == ios_base::eofbit );
  CPPUNIT_ASSERT( other.tm_year == xmas.tm_year );

  ostringstream ostrX;
  ostrX.imbue(loc);
  format = "%x %X";

  ret = tmp.put(ostrX, ostrX, ' ', &xmas, format.data(), format.data() + format.size());
  CPPUNIT_ASSERT( !ret.failed() );

  istringstream istrX( ostrX.str() );
  istreambuf_iterator<char, char_traits<char> > j( istrX );

  err = ios_base::goodbit;

  struct tm yet_more = { 15, 20, 9, 14, 7, 105 };

  j = tmg.get_date( j, e, io, err, &yet_more );

  CPPUNIT_ASSERT( err == ios_base::goodbit );

  CPPUNIT_ASSERT( yet_more.tm_sec != xmas.tm_sec );
  CPPUNIT_ASSERT( yet_more.tm_min != xmas.tm_min );
  CPPUNIT_ASSERT( yet_more.tm_hour != xmas.tm_hour );
  CPPUNIT_ASSERT( yet_more.tm_mday == xmas.tm_mday );
  CPPUNIT_ASSERT( yet_more.tm_mon == xmas.tm_mon );
  CPPUNIT_ASSERT( yet_more.tm_year == xmas.tm_year );

  ++j; // skip space

  j = tmg.get_time( j, e, io, err, &yet_more );

  CPPUNIT_ASSERT( err == ios_base::eofbit || err == ios_base::goodbit );

  CPPUNIT_ASSERT( yet_more.tm_sec == xmas.tm_sec );
  CPPUNIT_ASSERT( yet_more.tm_min == xmas.tm_min );
  CPPUNIT_ASSERT( yet_more.tm_hour == xmas.tm_hour );
  CPPUNIT_ASSERT( yet_more.tm_mday == xmas.tm_mday );
  CPPUNIT_ASSERT( yet_more.tm_mon == xmas.tm_mon );
  CPPUNIT_ASSERT( yet_more.tm_year == xmas.tm_year );
}

void LocaleTest::_ctype_facet( const locale& loc, const ref_locale&)
{
#  if !(defined (__DMC__) && defined (_DLL))
  CPPUNIT_ASSERT( has_facet<ctype<char> >(loc) );
  ctype<char> const& ct = use_facet<ctype<char> >(loc);

  //is
  {
    CPPUNIT_ASSERT( ct.is(ctype_base::digit, '0') );
    CPPUNIT_ASSERT( ct.is(ctype_base::upper, 'A') );
    CPPUNIT_ASSERT( ct.is(ctype_base::lower, 'a') );
    CPPUNIT_ASSERT( ct.is(ctype_base::alpha, 'A') );
    CPPUNIT_ASSERT( ct.is(ctype_base::space, ' ') );
    CPPUNIT_ASSERT( !ct.is(ctype_base::space, '2') );
    CPPUNIT_ASSERT( ct.is(ctype_base::punct, '.') );
    CPPUNIT_ASSERT( ct.is(ctype_base::xdigit, 'a') );
  }

  //is range
  {
    char values[] = "0Aa .";
    ctype_base::mask res[sizeof(values)];
    ct.is(values, values + sizeof(values), res);
    // '0'
    CPPUNIT_ASSERT( (res[0] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[0] & ctype_base::digit) != 0 );
    CPPUNIT_ASSERT( (res[0] & ctype_base::xdigit) != 0 );
    // 'A'
    CPPUNIT_ASSERT( (res[1] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::alpha) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::xdigit) != 0 );
    CPPUNIT_ASSERT( (res[1] & ctype_base::upper) != 0 );
    // 'a'
    CPPUNIT_ASSERT( (res[2] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::alpha) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::xdigit) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::lower) != 0 );
    CPPUNIT_ASSERT( (res[2] & ctype_base::space) == 0 );
    // ' '
    CPPUNIT_ASSERT( (res[3] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[3] & ctype_base::space) != 0 );
    CPPUNIT_ASSERT( (res[3] & ctype_base::digit) == 0 );
    // '.'
    CPPUNIT_ASSERT( (res[4] & ctype_base::print) != 0 );
    CPPUNIT_ASSERT( (res[4] & ctype_base::punct) != 0 );
    CPPUNIT_ASSERT( (res[4] & ctype_base::digit) == 0 );
  }

  //scan_is
  {
    char range[] = "abAc123 .";
    const char *rbeg = range;
    const char *rend = range + sizeof(range);

    const char *res;
    res = ct.scan_is((ctype_base::mask)(ctype_base::alpha | ctype_base::lower), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'a' );

    res = ct.scan_is(ctype_base::upper, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'A' );

    res = ct.scan_is(ctype_base::punct, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == '.' );
  }

  //scan_not
  {
    char range[] = "abAc123 .";
    const char *rbeg = range;
    const char *rend = range + sizeof(range);

    const char *res;
    res = ct.scan_not((ctype_base::mask)(ctype_base::alpha | ctype_base::lower), rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == '1' );

    res = ct.scan_not(ctype_base::alpha, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == '1' );

    res = ct.scan_not(ctype_base::punct, rbeg, rend);
    CPPUNIT_ASSERT( res != rend );
    CPPUNIT_ASSERT( *res == 'a' );
  }

  //toupper
  {
    CPPUNIT_ASSERT( ct.toupper('a') == 'A' );
    CPPUNIT_ASSERT( ct.toupper('A') == 'A' );
    CPPUNIT_ASSERT( ct.toupper('1') == '1' );
  }

  //toupper range
  {
    char range[] = "abAc1";
    char expected_range[] = "ABAC1";
    ct.toupper(range, range + sizeof(range));
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), expected_range) );
  }

  //tolower
  {
    CPPUNIT_ASSERT( ct.tolower('A') == 'a' );
    CPPUNIT_ASSERT( ct.tolower('a') == 'a' );
    CPPUNIT_ASSERT( ct.tolower('1') == '1' );
  }

  //tolower range
  {
    char range[] = "ABaC1";
    char expected_range[] = "abac1";
    ct.tolower(range, range + sizeof(range));
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), expected_range) );
  }

  //widen
  {
    CPPUNIT_ASSERT( ct.widen('a') == 'a' );
  }

  //widen range
  {
    char range[] = "ABaC1";
    char res[sizeof(range)];
    ct.widen(range, range + sizeof(range), res);
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), res) );
  }

  //narrow
  {
    CPPUNIT_ASSERT( ct.narrow('a', 'b') == 'a' );
  }

  //narrow range
  {
    char range[] = "ABaC1";
    char res[sizeof(range)];
    ct.narrow(range, range + sizeof(range), 'b', res);
    CPPUNIT_ASSERT( equal(range, range + sizeof(range), res) );
  }
#  endif /* __DMC__ */
}

template <class _Tp>
void test_supported_locale(LocaleTest inst, _Tp __test) {
  size_t n = sizeof(tested_locales) / sizeof(tested_locales[0]);
  for (size_t i = 0; i < n; ++i) {
    auto_ptr<locale> loc;
#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
    try {
      loc.reset(new locale(tested_locales[i].name));
    }
    catch (runtime_error const&) {
      //This locale is not supported.
      continue;
    }
#  else
    //Without exception support we only test C locale.
    if (tested_locales[i].name[0] != 'C' ||
        tested_locales[i].name[1] != 0)
      continue;
    loc.reset(new locale(tested_locales[i].name));
#  endif
    CPPUNIT_MESSAGE( loc->name().c_str() );
    (inst.*__test)(*loc, tested_locales[i]);
  }
}

void LocaleTest::locale_by_name() {
#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  /*
   * Check of the 22.1.1.2.7 standard point. Construction of a locale
   * instance from a null pointer or an unknown name should result in
   * a runtime_error exception.
   */
  try {
    locale loc(static_cast<char const*>(0));
    CPPUNIT_ASSERT( false );
  }
  catch (runtime_error const&) {
  }
  catch (...) {
    CPPUNIT_ASSERT( false );
  }

  try {
    locale loc("yasli_language");
    CPPUNIT_ASSERT( false );
  }
  catch (runtime_error const&) {
  }
  catch (...) {
    CPPUNIT_ASSERT( false );
  }
#  endif
}

void LocaleTest::loc_has_facet() {
  locale loc("C");
  typedef numpunct<char> implemented_facet;
  CPPUNIT_ASSERT( has_facet<implemented_facet>(loc) );
  /*
  typedef num_put<char, back_insert_iterator<string> > not_implemented_facet;
  CPPUNIT_ASSERT( !has_facet<not_implemented_facet>(loc) );
  */
}

void LocaleTest::num_put_get()
{ test_supported_locale(*this, &LocaleTest::_num_put_get); }

void LocaleTest::money_put_get()
{ test_supported_locale(*this, &LocaleTest::_money_put_get); }

void LocaleTest::money_put_X_bug()
{ test_supported_locale(*this, &LocaleTest::_money_put_X_bug); }

void LocaleTest::time_put_get()
{ test_supported_locale(*this, &LocaleTest::_time_put_get); }

void LocaleTest::collate_facet()
{
  {
    CPPUNIT_ASSERT( has_facet<collate<char> >(locale::classic()) );
    collate<char> const& col = use_facet<collate<char> >(locale::classic());

    char const str1[] = "abcdef1";
    char const str2[] = "abcdef2";
    const size_t size1 = sizeof(str1) / sizeof(str1[0]) - 1;
    const size_t size2 = sizeof(str2) / sizeof(str2[0]) - 1;

    CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 1, str2, str2 + size2 - 1) == 0 );
    CPPUNIT_ASSERT( col.compare(str1, str1 + size1, str2, str2 + size2) == -1 );

    //Smallest string should be before largest one:
    CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 2, str2, str2 + size2 - 1) == -1 );
    CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 1, str2, str2 + size2 - 2) == 1 );
  }

#  if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  try {
    locale loc("fr_FR");
    {
      CPPUNIT_ASSERT( has_facet<collate<char> >(loc) );
      collate<char> const& col = use_facet<collate<char> >(loc);

      char const str1[] = "abcdef1";
      char const str2[] = "abcdef2";
      const size_t size1 = sizeof(str1) / sizeof(str1[0]) - 1;
      const size_t size2 = sizeof(str2) / sizeof(str2[0]) - 1;

      CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 1, str2, str2 + size2 - 1) == 0 );
      CPPUNIT_ASSERT( col.compare(str1, str1 + size1, str2, str2 + size2) == -1 );

      //Smallest string should be before largest one:
      CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 2, str2, str2 + size2 - 1) == -1 );
      CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 1, str2, str2 + size2 - 2) == 1 );
    }
    {
      CPPUNIT_ASSERT( has_facet<collate<char> >(loc) );
      collate<char> const& col = use_facet<collate<char> >(loc);

      string strs[] = {"abdd", "abçd", "abbd", "abcd"};

      string transformed[4];
      for (size_t i = 0; i < 4; ++i) {
        transformed[i] = col.transform(strs[i].data(), strs[i].data() + strs[i].size());
      }

      sort(strs, strs + 4, loc);
      CPPUNIT_ASSERT( strs[0] == "abbd" );
      CPPUNIT_ASSERT( strs[1] == "abcd" );
      CPPUNIT_ASSERT( strs[2] == "abçd" );
      CPPUNIT_ASSERT( strs[3] == "abdd" );

      sort(transformed, transformed + 4);

      CPPUNIT_ASSERT( col.transform(strs[0].data(), strs[0].data() + strs[0].size()) == transformed[0] );
      CPPUNIT_ASSERT( col.transform(strs[1].data(), strs[1].data() + strs[1].size()) == transformed[1] );
      CPPUNIT_ASSERT( col.transform(strs[2].data(), strs[2].data() + strs[2].size()) == transformed[2] );
      CPPUNIT_ASSERT( col.transform(strs[3].data(), strs[3].data() + strs[3].size()) == transformed[3] );

      // Check empty string result in empty key.
      CPPUNIT_ASSERT( col.transform(strs[0].data(), strs[0].data()).empty() );

      // Check that only characters that matter are taken into accout to build the key.
      CPPUNIT_ASSERT( col.transform(strs[0].data(), strs[0].data() + 2) == col.transform(strs[1].data(), strs[1].data() + 2) );
    }
#    if !defined (STLPORT) || !defined (_STLP_NO_WCHAR_T)
    {
      CPPUNIT_ASSERT( has_facet<collate<wchar_t> >(loc) );
      collate<wchar_t> const& col = use_facet<collate<wchar_t> >(loc);

      wchar_t const str1[] = L"abcdef1";
      wchar_t const str2[] = L"abcdef2";
      const size_t size1 = sizeof(str1) / sizeof(str1[0]) - 1;
      const size_t size2 = sizeof(str2) / sizeof(str2[0]) - 1;

      CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 1, str2, str2 + size2 - 1) == 0 );
      CPPUNIT_ASSERT( col.compare(str1, str1 + size1, str2, str2 + size2) == -1 );

      //Smallest string should be before largest one:
      CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 2, str2, str2 + size2 - 1) == -1 );
      CPPUNIT_ASSERT( col.compare(str1, str1 + size1 - 1, str2, str2 + size2 - 2) == 1 );
    }
    {
      size_t i;
      CPPUNIT_ASSERT( has_facet<collate<wchar_t> >(loc) );
      collate<wchar_t> const& col = use_facet<collate<wchar_t> >(loc);

      // Here we would like to use L"abçd" but it looks like all compilers
      // do not support storage of unicode characters in exe resulting in
      // compilation error. We avoid this test for the moment.
      wstring strs[] = {L"abdd", L"abcd", L"abbd", L"abcd"};

      wstring transformed[4];
      for (i = 0; i < 4; ++i) {
        transformed[i] = col.transform(strs[i].data(), strs[i].data() + strs[i].size());
      }

      sort(strs, strs + 4, loc);
      CPPUNIT_ASSERT( strs[0] == L"abbd" );
      CPPUNIT_ASSERT( strs[1] == L"abcd" );
      CPPUNIT_ASSERT( strs[2] == L"abcd" );
      CPPUNIT_ASSERT( strs[3] == L"abdd" );

      sort(transformed, transformed + 4);

      CPPUNIT_ASSERT( col.transform(strs[0].data(), strs[0].data() + strs[0].size()) == transformed[0] );
      CPPUNIT_ASSERT( col.transform(strs[1].data(), strs[1].data() + strs[1].size()) == transformed[1] );
      CPPUNIT_ASSERT( col.transform(strs[2].data(), strs[2].data() + strs[2].size()) == transformed[2] );
      CPPUNIT_ASSERT( col.transform(strs[3].data(), strs[3].data() + strs[3].size()) == transformed[3] );

      CPPUNIT_ASSERT( col.transform(strs[0].data(), strs[0].data()).empty() );

      CPPUNIT_ASSERT( col.transform(strs[0].data(), strs[0].data() + 2) == col.transform(strs[1].data(), strs[1].data() + 2) );
    }
#    endif
  }
  catch (runtime_error const&) {
    CPPUNIT_MESSAGE("No french locale to check collate facet");
  }
#  endif
}

void LocaleTest::ctype_facet()
{ test_supported_locale(*this, &LocaleTest::_ctype_facet); }

void LocaleTest::locale_init_problem() {
#  if !defined (STLPORT) || !defined (_STLP_NO_MEMBER_TEMPLATES)
  test_supported_locale(*this, &LocaleTest::_locale_init_problem);
#  endif
}


/*
 * Creation of a locale instance imply initialization of some STLport internal
 * static objects first. We use a static instance of locale to check that this
 * initialization is done correctly.
 */
static locale global_loc;
static locale other_loc("");

#  if !defined (STLPORT) || !defined (_STLP_NO_MEMBER_TEMPLATES)
void LocaleTest::_locale_init_problem( const locale& loc, const ref_locale&)
{
#    if !defined (__APPLE__) && !defined (__FreeBSD__) || \
        !defined(__GNUC__) || ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__> 3)))
  typedef codecvt<char,char,mbstate_t> my_facet;
#    else
// std::mbstate_t required for gcc 3.3.2 on FreeBSD...
// I am not sure what key here---FreeBSD or 3.3.2...
//      - ptr 2005-04-04
  typedef codecvt<char,char,std::mbstate_t> my_facet;
#    endif

#    if !(defined (__DMC__) && defined (_DLL))
  locale loc_ref(global_loc);
  {
    locale gloc( loc_ref, new my_facet() );
    CPPUNIT_ASSERT( has_facet<my_facet>( gloc ) );
    //The following code is just here to try to confuse the reference counting underlying mecanism:
    locale::global( locale::classic() );
    locale::global( gloc );
  }

#      if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  try {
#      endif
    ostringstream os("test") ;
    locale loc2( loc, new my_facet() );
    CPPUNIT_ASSERT( has_facet<my_facet>( loc2 ) );
    os.imbue( loc2 );
#      if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  }
  catch ( runtime_error& ) {
    CPPUNIT_ASSERT( false );
  }
  catch ( ... ) {
   CPPUNIT_ASSERT( false );
  }
#      endif

#      if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  try {
#      endif
    ostringstream os2("test2");
#      if !defined (STLPORT) || defined (_STLP_USE_EXCEPTIONS)
  }
  catch ( runtime_error& ) {
    CPPUNIT_ASSERT( false );
  }
  catch ( ... ) {
    CPPUNIT_ASSERT( false );
  }
#      endif
#    endif /* __DMC__ */
}
#  endif

void LocaleTest::default_locale()
{
  locale loc( "" );
}

void LocaleTest::facet_id()
{
#  if defined (STLPORT)
  locale::id _id_01 = collate<char>::id;
  CPPUNIT_CHECK( _id_01._M_index == 1 );

  locale::id _id_02 = ctype<char>::id;
  CPPUNIT_CHECK( _id_02._M_index == 2 );

#    ifndef _STLP_NO_MBSTATE_T
  locale::id _id_03 = codecvt<char, char, mbstate_t>::id;
  CPPUNIT_CHECK( _id_03._M_index == 3 );
#    endif

  locale::id _id_04 = moneypunct<char, true>::id;
  CPPUNIT_CHECK( _id_04._M_index == 4 );

  locale::id _id_05 = moneypunct<char, false>::id;
  CPPUNIT_CHECK( _id_05._M_index == 5 );

  locale::id _id_06 = numpunct<char>::id;
  CPPUNIT_CHECK( _id_06._M_index == 6 );

  locale::id _id_07 = messages<char>::id;
  CPPUNIT_CHECK( _id_07._M_index == 7 );

  locale::id _id_08 = money_get<char, istreambuf_iterator<char, char_traits<char> > >::id;
  CPPUNIT_CHECK( _id_08._M_index == 8 );

  /*
  locale::id _id_09 = money_get<char, const char*>::id;
  CPPUNIT_CHECK( _id_09._M_index == 9 );
  */

  locale::id _id_10 = money_put<char, ostreambuf_iterator<char, char_traits<char> > >::id;
  CPPUNIT_CHECK( _id_10._M_index == 10 );

  /*
  locale::id _id_11 = money_put<char, char*>::id;
  CPPUNIT_CHECK( _id_11._M_index == 11 );
  */

  locale::id _id_12 = num_get<char, istreambuf_iterator<char, char_traits<char> > >::id;
  CPPUNIT_CHECK( _id_12._M_index == 12 );

  /*
  locale::id _id_13 = num_get<char, const char*>::id;
  CPPUNIT_CHECK( _id_13._M_index == 13 );
  */

  locale::id _id_14 = num_put<char, ostreambuf_iterator<char, char_traits<char> > >::id;
  CPPUNIT_CHECK( _id_14._M_index == 14 );

  /*
  locale::id _id_15 = num_put<char, char*>::id;
  CPPUNIT_CHECK( _id_15._M_index == 15 );
  */

  locale::id _id_16 = time_get<char, istreambuf_iterator<char, char_traits<char> > >::id;
  CPPUNIT_CHECK( _id_16._M_index == 16 );

  /*
  locale::id _id_17 = time_get<char, const char*>::id;
  CPPUNIT_CHECK( _id_17._M_index == 17 );
  */

  locale::id _id_18 = time_put<char, ostreambuf_iterator<char, char_traits<char> > >::id;
  CPPUNIT_CHECK( _id_18._M_index == 18 );

  /*
  locale::id _id_19 = time_put<char, char*>::id;
  CPPUNIT_CHECK( _id_19._M_index == 19 );
  */

#    ifndef _STLP_NO_WCHAR_T
  locale::id _id_20 = collate<wchar_t>::id;
  CPPUNIT_CHECK( _id_20._M_index == 20 );

  locale::id _id_21 = ctype<wchar_t>::id;
  CPPUNIT_CHECK( _id_21._M_index == 21 );

#      ifndef _STLP_NO_MBSTATE_T
  locale::id _id_22 = codecvt<wchar_t, char, mbstate_t>::id;
  CPPUNIT_CHECK( _id_22._M_index == 22 );
#      endif
  locale::id _id_23 = moneypunct<wchar_t, true>::id;
  CPPUNIT_CHECK( _id_23._M_index == 23 );

  locale::id _id_24 = moneypunct<wchar_t, false>::id;
  CPPUNIT_CHECK( _id_24._M_index == 24 );

  locale::id _id_25 = numpunct<wchar_t>::id;
  CPPUNIT_CHECK( _id_25._M_index == 25 );

  locale::id _id_26 = messages<wchar_t>::id;
  CPPUNIT_CHECK( _id_26._M_index == 26 );

  locale::id _id_27 = money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
  CPPUNIT_CHECK( _id_27._M_index == 27 );

  /*
  locale::id _id_28 = money_get<wchar_t, const wchar_t*>::id;
  CPPUNIT_CHECK( _id_28._M_index == 28 );
  */

  locale::id _id_29 = money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
  CPPUNIT_CHECK( _id_29._M_index == 29 );

  /*
  locale::id _id_30 = money_put<wchar_t, wchar_t*>::id;
  CPPUNIT_CHECK( _id_30._M_index == 30 );
  */

  locale::id _id_31 = num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
  CPPUNIT_CHECK( _id_31._M_index == 31 );

  /*
  locale::id _id_32 = num_get<wchar_t, const wchar_t*>::id;
  CPPUNIT_CHECK( _id_32._M_index == 32 );
  */

  locale::id _id_33 = num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > > ::id;
  CPPUNIT_CHECK( _id_33._M_index == 33 );

  /*
  locale::id _id_34 = num_put<wchar_t, wchar_t*>::id;
  CPPUNIT_CHECK( _id_34._M_index == 34 );
  */

  locale::id _id_35 = time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
  CPPUNIT_CHECK( _id_35._M_index == 35 );

  /*
  locale::id _id_36 = time_get<wchar_t, const wchar_t*>::id;
  CPPUNIT_CHECK( _id_36._M_index == 36 );
  */

  locale::id _id_37 = time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
  CPPUNIT_CHECK( _id_37._M_index == 37 );

  /*
  locale::id _id_38 = time_put<wchar_t, wchar_t*>::id;
  CPPUNIT_CHECK( _id_38._M_index == 38 );
  */
#    endif
#  endif
}

void LocaleTest::combine()
{
#  if (!defined (STLPORT) || \
       (defined (_STLP_USE_EXCEPTIONS) && !defined (_STLP_NO_MEMBER_TEMPLATES) && !defined (_STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS)))
  auto_ptr<locale> loc1, loc2;
  size_t loc1_index = 0;
  size_t n = sizeof(tested_locales) / sizeof(tested_locales[0]);
  for (size_t i = 0; i < n; ++i) {
    try {
      locale *ploc = new locale(tested_locales[i].name);
      if (loc1.get() == 0)
      {
        loc1.reset(ploc);
        loc1_index = i;
        continue;
      }
      else
      {
        loc2.reset(ploc);
      }

      //We can start the test
      ostringstream ostr;
      ostr << "combining '" << loc2->name() << "' money facets with '" << loc1->name() << "'";
      CPPUNIT_MESSAGE( ostr.str().c_str() );

      //We are going to combine money facets as all formats are different.
      {
        //We check that resulting locale has correctly acquire loc2 facets.
        locale loc = loc1->combine<moneypunct<char, true> >(*loc2);
        loc = loc.combine<moneypunct<char, false> >(*loc2);
        loc = loc.combine<money_put<char> >(*loc2);
        loc = loc.combine<money_get<char> >(*loc2);

        //Check loc has the correct facets:
        _money_put_get2(*loc2, loc, tested_locales[i]);

        //Check loc1 has not been impacted:
        _money_put_get2(*loc1, *loc1, tested_locales[loc1_index]);

        //Check loc2 has not been impacted:
        _money_put_get2(*loc2, *loc2, tested_locales[i]);
      }
      {
        //We check that resulting locale has not wrongly acquire loc1 facets that hasn't been combine:
        locale loc = loc2->combine<numpunct<char> >(*loc1);
        loc = loc.combine<time_put<char> >(*loc1);
        loc = loc.combine<time_get<char> >(*loc1);

        //Check loc has the correct facets:
        _money_put_get2(*loc2, loc, tested_locales[i]);

        //Check loc1 has not been impacted:
        _money_put_get2(*loc1, *loc1, tested_locales[loc1_index]);

        //Check loc2 has not been impacted:
        _money_put_get2(*loc2, *loc2, tested_locales[i]);
      }

      {
        // Check auto combination do not result in weird reference counting behavior 
        // (might generate a crash).
        loc1->combine<numpunct<char> >(*loc1);
      }

      loc1.reset(loc2.release());
      loc1_index = i;
    }
    catch (runtime_error const&) {
      //This locale is not supported.
      continue;
    }
  }
#  endif
}

#endif
