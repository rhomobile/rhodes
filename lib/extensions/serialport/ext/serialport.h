/* Ruby/SerialPort
 * Guillaume Pierronnet <moumar@netcourrier.com>
 * Alan Stern <stern@rowland.harvard.edu>
 * Daniel E. Shipton <dshipton@redshiptechnologies.com>
 * Tobin Richard <tobin.richard@gmail.com>
 * Ryan C. Payne <rpayne-oss@bullittsystems.com>
 *
 * This code is hereby licensed for public consumption under either the
 * GNU GPL v2 or greater.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * For documentation on serial programming, see the excellent:
 * "Serial Programming Guide for POSIX Operating Systems"
 * written Michael R. Sweet.
 * http://www.easysw.com/~mike/serial/
 */

#ifndef _RUBY_SERIAL_PORT_H_
#define _RUBY_SERIAL_PORT_H_

#include <ruby.h>    /* ruby inclusion */
#ifdef HAVE_RUBY_IO_H      /* ruby io inclusion */
   #include <ruby/io.h>
#else
   #include <rubyio.h>
#endif

struct modem_params
{
   int data_rate;
   int data_bits;
   int stop_bits;
   int parity;
};

struct line_signals
{
   int rts;
   int dtr;
   int cts;
   int dsr;
   int dcd;
   int ri;
};

#define NONE   0
#define HARD   1
#define SOFT   2

#if defined(OS_MSWIN) || defined(OS_BCCWIN) || defined(OS_MINGW)
   #define SPACE  SPACEPARITY
   #define MARK   MARKPARITY
   #define EVEN   EVENPARITY
   #define ODD    ODDPARITY

   #ifndef RB_SERIAL_EXPORT
	 #ifndef HAVE_RUBY_IO_H
     	#define RB_SERIAL_EXPORT __declspec(dllexport)
	 #else
		#define RB_SERIAL_EXPORT
	 #endif
   #endif

#else
   #define EVEN   1
   #define ODD    2
   #define SPACE  3
   #define MARK   4

   #define RB_SERIAL_EXPORT
#endif

extern VALUE sBaud, sDataBits, sStopBits, sParity; /* strings */
extern VALUE sRts, sDtr, sCts, sDsr, sDcd, sRi;

/* Implementation specific functions. */
VALUE RB_SERIAL_EXPORT sp_create_impl(VALUE class, VALUE _port);
VALUE RB_SERIAL_EXPORT sp_set_modem_params_impl(int argc, VALUE *argv, VALUE self);
void RB_SERIAL_EXPORT get_modem_params_impl(VALUE self, struct modem_params *mp);
VALUE RB_SERIAL_EXPORT sp_set_flow_control_impl(VALUE self, VALUE val);
VALUE RB_SERIAL_EXPORT sp_get_flow_control_impl(VALUE self);
VALUE RB_SERIAL_EXPORT sp_set_read_timeout_impl(VALUE self, VALUE val);
VALUE RB_SERIAL_EXPORT sp_get_read_timeout_impl(VALUE self);
VALUE RB_SERIAL_EXPORT sp_set_write_timeout_impl(VALUE self, VALUE val);
VALUE RB_SERIAL_EXPORT sp_get_write_timeout_impl(VALUE self);
VALUE RB_SERIAL_EXPORT sp_break_impl(VALUE self, VALUE time);
void RB_SERIAL_EXPORT get_line_signals_helper_impl(VALUE obj, struct line_signals *ls);
VALUE RB_SERIAL_EXPORT set_signal_impl(VALUE obj, VALUE val, int sig);
VALUE RB_SERIAL_EXPORT sp_set_rts_impl(VALUE self, VALUE val);
VALUE RB_SERIAL_EXPORT sp_set_dtr_impl(VALUE self, VALUE val);
VALUE RB_SERIAL_EXPORT sp_get_rts_impl(VALUE self);
VALUE RB_SERIAL_EXPORT sp_get_dtr_impl(VALUE self);

VALUE RB_SERIAL_EXPORT sp_flush_input_data_impl(VALUE self);
VALUE RB_SERIAL_EXPORT sp_flush_output_data_impl(VALUE self);

#endif
