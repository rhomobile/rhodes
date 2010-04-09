#ifndef __SPIRIT_CUSTOM_STRING_INC__
  #define __SPIRIT_CUSTOM_STRING_INC__

//-----------------------------------------------------------------------------

#include "common.h"

#define MAX_STRING   1024 /*symbols*/

#ifdef USE_CUSTOM_STRING
  #include <string.h>
  #define CUSTOM_STRING_NAMESPACE   spirit
  #define CUSTOM_STRING_CLASS_NAME  util_string
#else
  #include <string>
  #define CUSTOM_STRING_NAMESPACE   std
  #define CUSTOM_STRING_CLASS_NAME  string
#endif

//-----------------------------------------------------------------------------
#ifdef USE_CUSTOM_STRING

namespace CUSTOM_STRING_NAMESPACE
{
  class CUSTOM_STRING_CLASS_NAME
  {
  public:

// constructors

    CUSTOM_STRING_CLASS_NAME( void )
      : m_psz( 0 )
      , m_buf( 0 )
    {
      _alloc_string();
    }

    CUSTOM_STRING_CLASS_NAME( const char* s )
      : m_psz( 0 )
      , m_buf( 0 )
    {
      _alloc_string();
      CHAR2TCHAR( m_psz, s, MAX_STRING );
    }

#if defined( UNICODE ) || defined( _UNICODE )
    CUSTOM_STRING_CLASS_NAME( const TCHAR* s )
      : m_psz( 0 )
      , m_buf( 0 )
    {
      _alloc_string();
      _tcsncpy( m_psz, s, MAX_STRING );
    }
#endif

    CUSTOM_STRING_CLASS_NAME( const CUSTOM_STRING_CLASS_NAME& s )
      : m_psz( 0 )
      , m_buf( 0 )
    {
      _alloc_string();
      _tcsncpy( m_psz, s.m_psz, MAX_STRING );
    }

// destructor

    ~CUSTOM_STRING_CLASS_NAME()
    {
      _free_string();
    }

// operators

    CUSTOM_STRING_CLASS_NAME& operator= ( const CUSTOM_STRING_CLASS_NAME& s )
    {
      if ( &s == this ) return *this;
      _tcsncpy( m_psz, s.m_psz, MAX_STRING );
      return *this;
    }

#if defined( UNICODE ) || defined( _UNICODE )
    CUSTOM_STRING_CLASS_NAME& operator= ( const TCHAR* s )
    {
      _tcsncpy( m_psz, s, MAX_STRING );
      return *this;
    }
#endif

    CUSTOM_STRING_CLASS_NAME& operator= ( const char* s )
    {
      CHAR2TCHAR( m_psz, s, MAX_STRING );
      return *this;
    }

    CUSTOM_STRING_CLASS_NAME operator+ ( const CUSTOM_STRING_CLASS_NAME& s ) const
    {
      if ( &s == this ) return *this;
      CUSTOM_STRING_CLASS_NAME res( m_psz );
      _tcsncat( res.m_psz, s.m_psz, MAX_STRING );
      return res;
    }

    CUSTOM_STRING_CLASS_NAME operator+ ( const char* s ) const
    {
      return CUSTOM_STRING_CLASS_NAME( m_psz ) + CUSTOM_STRING_CLASS_NAME( s );
    }

#if defined( UNICODE ) || defined( _UNICODE )
    CUSTOM_STRING_CLASS_NAME operator+ ( const TCHAR* s ) const
    {
      return CUSTOM_STRING_CLASS_NAME( m_psz ) + CUSTOM_STRING_CLASS_NAME( s );
    }
#endif

    CUSTOM_STRING_CLASS_NAME& operator+= ( const CUSTOM_STRING_CLASS_NAME& s )
    {
      if ( &s == this ) return *this;
      _tcsncat( m_psz, s.m_psz, MAX_STRING );
      return *this;
    }

#if defined( UNICODE ) || defined( _UNICODE )
    CUSTOM_STRING_CLASS_NAME& operator+= ( const TCHAR* s )
    {
      _tcsncat(m_psz, s, MAX_STRING);
      return *this;
    }
#endif

    CUSTOM_STRING_CLASS_NAME& operator+= ( const char* s )
    {
      *this += CUSTOM_STRING_CLASS_NAME( s );
      return *this;
    }

    bool operator== ( const CUSTOM_STRING_CLASS_NAME& s ) const
    {
      return ( _tcsncmp( m_psz, s.m_psz, MAX_STRING ) == 0 );
    }

#if defined( UNICODE ) || defined( _UNICODE )
    bool operator== ( const TCHAR* s ) const
    {
      return ( _tcsncmp( m_psz, s, MAX_STRING ) == 0 );
    }
#endif

    bool operator== ( const char* s ) const
    {
      return ( *this == CUSTOM_STRING_CLASS_NAME( s ) );
    }

    bool operator!= ( const CUSTOM_STRING_CLASS_NAME& s ) const
    {
      return ( _tcsncmp( m_psz, s.m_psz, MAX_STRING ) != 0 );
    }

#if defined( UNICODE ) || defined( _UNICODE )
    bool operator!= ( const TCHAR* s ) const
    {
      return ( _tcsncmp( m_psz, s, MAX_STRING ) != 0 );
    }
#endif

    bool operator!= ( const char* s ) const
    {
      return ( *this != CUSTOM_STRING_CLASS_NAME( s ) );
    }

    TCHAR operator[] ( int i ) const
    {
      if ( ( i < 0 ) || ( i >= MAX_STRING ) ) return 0;
      return m_psz[ i ];
    }

// methods

    size_t size( void ) const
    {
      return _tcslen( m_psz );
    }

    bool is_empty( void ) const
    {
      return ( _tcslen( m_psz ) == 0 ); 
    }

    const TCHAR* c_str( void ) const
    {
      return m_psz;
    }

    const char* c_str( void )
    {
#if defined( UNICODE ) || defined( _UNICODE )
      TCHAR2CHAR( m_buf, m_psz, MAX_STRING );
      return m_buf;
#else
      return m_psz;
#endif
    }

  protected:
    void _alloc_string( void )
    {
      m_psz = new TCHAR[MAX_STRING];
      memset( m_psz, 0, MAX_STRING * sizeof( TCHAR ) );
#if defined( UNICODE ) || defined( _UNICODE )
      m_buf = new char[MAX_STRING];
      memset( m_buf, 0, MAX_STRING);
#endif
    }

    void _free_string( void )
    {
      if ( m_psz ) delete[] m_psz, m_psz = 0;
      if ( m_buf ) delete[] m_buf, m_buf = 0;
    }

  private:
    TCHAR* m_psz;
    char*  m_buf;

  }; // class

} // namespace

#endif // USE_CUSTOM_STRING

//-----------------------------------------------------------------------------

typedef CUSTOM_STRING_NAMESPACE::CUSTOM_STRING_CLASS_NAME util_string;

//-----------------------------------------------------------------------------

#endif //__SPIRIT_CUSTOM_STRING_INC__
