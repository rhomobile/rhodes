/* Ruby/SerialPort
 * Guillaume Pierronnet <moumar@netcourrier.com>
 * Alan Stern <stern@rowland.harvard.edu>
 * Daniel E. Shipton <dshipton@redshiptechnologies.com>
 * Jonas Bähr <jonas.baehr@fs.ei.tum.de>
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

#include "serialport.h"

VALUE cSerialPort; /* serial port class */

VALUE sBaud, sDataBits, sStopBits, sParity; /* strings */
VALUE sRts, sDtr, sCts, sDsr, sDcd, sRi;

/*
 * :nodoc: This method is private and will be called by SerialPort#new or SerialPort#open.
 */
static VALUE sp_create(class, _port)
   VALUE class, _port;
{
   return sp_create_impl(class, _port);
}

/*
 * Configure the serial port. You can pass a hash or multiple values
 * as separate arguments. Invalid or unsupported values will raise
 * an ArgumentError.
 *
 * When using a hash the following keys are recognized:
 * ["baud"] Integer from 50 to 256000, depends on platform
 * ["data_bits"] Integer from 5 to 8 (4 is allowed on Windows too)
 * ["stop_bits"] An integer, only allowed values are 1 or 2 (1.5 is not supported)
 * ["parity"] One of the constants NONE, EVEN or ODD (Windows allows also MARK and SPACE)
 *
 * When using separate arguments, they are interpreted as:
 * (baud, data_bits = 8, stop_bits = 1, parity = (previous_databits==8 ? NONE : EVEN))
 *
 * Nota: A baudrate of nil will keep the old value. The default parity depends on the
 * number of databits configured before this function call.
 */
static VALUE sp_set_modem_params(argc, argv, self)
   int argc;
   VALUE *argv, self;
{
   return sp_set_modem_params_impl(argc, argv, self);
}

/*
 * Send a break for the given time.
 *
 * <tt>time</tt> is an integer of tenths-of-a-second for the break.
 *
 * Note: Under Posix, this value is very approximate.
 */
static VALUE sp_break(self, time)
   VALUE self, time;
{
   return sp_break_impl(self, time);
}

/*
 * Get the state (0 or 1) of the DTR line (not available on Windows)
 */
static VALUE sp_get_dtr(self)
   VALUE self;
{
   return sp_get_dtr_impl(self);
}

/*
 * Get the flow control. The result is either NONE, HARD, SOFT or (HARD | SOFT)
 */
static VALUE sp_get_flow_control(self)
   VALUE self;
{
   return sp_get_flow_control_impl(self);
}

/*
 * Get the timeout value (in milliseconds) for reading.
 * See SerialPort#set_read_timeout for details.
 */
static VALUE sp_get_read_timeout(self)
   VALUE self;
{
   return sp_get_read_timeout_impl(self);
}

/*
 * Get the state (0 or 1) of the RTS line (not available on Windows)
 */
static VALUE sp_get_rts(self)
   VALUE self;
{
   return sp_get_rts_impl(self);
}

/*
 * Get the write timeout (in milliseconds)
 *
 * Note: Under Posix, write timeouts are not implemented.
 */
static VALUE sp_get_write_timeout(self)
   VALUE self;
{
   return sp_get_write_timeout_impl(self);
}

/*
 * Set the state (0 or 1) of the DTR line
 */
static VALUE sp_set_dtr(self, val)
   VALUE self, val;
{
   return sp_set_dtr_impl(self, val);
}

/*
 * Set the flow control to either NONE, HARD, SOFT or (HARD | SOFT)
 *
 *  Note: SerialPort::HARD mode is not supported on all platforms.
 *  SerialPort::HARD uses RTS/CTS handshaking; DSR/DTR is not
 *  supported.
 */
static VALUE sp_set_flow_control(self, val)
   VALUE self, val;
{
   return sp_set_flow_control_impl(self, val);
}

/*
 * Set the timeout value (in milliseconds) for reading.
 * A negative read timeout will return all the available data without
 * waiting, a zero read timeout will not return until at least one
 * byte is available, and a positive read timeout returns when the
 * requested number of bytes is available or the interval between the
 * arrival of two bytes exceeds the timeout value.
 *
 * Note: Read timeouts don't mix well with multi-threading.
 */
static VALUE sp_set_read_timeout(self, val)
   VALUE self, val;
{
   return sp_set_read_timeout_impl(self, val);
}

/*
 * Set the state (0 or 1) of the RTS line
 */
static VALUE sp_set_rts(self, val)
   VALUE self, val;
{
   return sp_set_rts_impl(self, val);
}

/*
 * Set a write timeout (in milliseconds)
 *
 * Note: Under Posix, write timeouts are not implemented.
 */
static VALUE sp_set_write_timeout(self, val)
   VALUE self, val;
{
   return sp_set_write_timeout_impl(self, val);
}

/*
 */
static void get_modem_params(self, mp)
   VALUE self;
   struct modem_params *mp;
{
   get_modem_params_impl(self, mp);
}

/*
 * Set the baud rate, see SerialPort#set_modem_params for details.
 */
static VALUE sp_set_data_rate(self, data_rate)
   VALUE self, data_rate;
{
   VALUE argv[4];

   argv[0] = data_rate;
   argv[1] = argv[2] = argv[3] = Qnil;
   sp_set_modem_params(4, argv, self);

   return data_rate;
}

/*
 * Set the data bits, see SerialPort#set_modem_params for details.
 */
static VALUE sp_set_data_bits(self, data_bits)
   VALUE self, data_bits;
{
   VALUE argv[4];

   argv[1] = data_bits;
   argv[0] = argv[2] = argv[3] = Qnil;
   sp_set_modem_params(4, argv, self);

   return data_bits;
}

/*
 * Set the stop bits, see SerialPort#set_modem_params for details.
 */
static VALUE sp_set_stop_bits(self, stop_bits)
   VALUE self, stop_bits;
{
   VALUE argv[4];

   argv[2] = stop_bits;
   argv[0] = argv[1] = argv[3] = Qnil;
   sp_set_modem_params(4, argv, self);

   return stop_bits;
}

/*
 * Set the parity, see SerialPort#set_modem_params for details.
 */
static VALUE sp_set_parity(self, parity)
   VALUE self, parity;
{
   VALUE argv[4];

   argv[3] = parity;
   argv[0] = argv[1] = argv[2] = Qnil;
   sp_set_modem_params(4, argv, self);

   return parity;
}

/*
 * Get the current baud rate, see SerialPort#get_modem_params for details.
 */
static VALUE sp_get_data_rate(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.data_rate);
}

/*
 * Get the current data bits, see SerialPort#get_modem_params for details.
 */
static VALUE sp_get_data_bits(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.data_bits);
}

/*
 * Get the current stop bits, see SerialPort#get_modem_params for details.
 */
static VALUE sp_get_stop_bits(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.stop_bits);
}

/*
 * Get the current parity, see SerialPort#get_modem_params for details.
 */
static VALUE sp_get_parity(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.parity);
}

/*
 * Get the configure of the serial port.
 *
 * Returned is a hash with the following keys:
 * ["baud"] Integer with the baud rate
 * ["data_bits"] Integer from 5 to 8 (4 is possible on Windows too)
 * ["stop_bits"] Integer, 1 or 2 (1.5 is not supported)
 * ["parity"] One of the constants NONE, EVEN or ODD (on Windows may also MARK or SPACE)
 */
static VALUE sp_get_modem_params(self)
   VALUE self;
{
   struct modem_params mp;
   VALUE hash;

   get_modem_params(self, &mp);

   hash = rb_hash_new();

   rb_hash_aset(hash, sBaud, INT2FIX(mp.data_rate));
   rb_hash_aset(hash, sDataBits, INT2FIX(mp.data_bits));
   rb_hash_aset(hash, sStopBits, INT2FIX(mp.stop_bits));
   rb_hash_aset(hash, sParity, INT2FIX(mp.parity));

   return hash;
}

void get_line_signals_helper(obj, ls)
   VALUE obj;
   struct line_signals *ls;
{
   get_line_signals_helper_impl(obj, ls);
}

/*
 * Get the state (0 or 1) of the CTS line
 */
static VALUE sp_get_cts(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.cts);
}

/*
 * Get the state (0 or 1) of the DSR line
 */
static VALUE sp_get_dsr(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.dsr);
}

/*
 * Get the state (0 or 1) of the DCD line
 */
static VALUE sp_get_dcd(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.dcd);
}

/*
 * Get the state (0 or 1) of the RI line
 */
static VALUE sp_get_ri(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.ri);
}

/*
 * Return a hash with the state of each line status bit.  Keys are
 * "rts", "dtr", "cts", "dsr", "dcd", and "ri".
 *
 * Note: Under Windows, the rts and dtr values are not included.
 */
static VALUE sp_signals(self)
   VALUE self;
{
   struct line_signals ls;
   VALUE hash;

   get_line_signals_helper(self, &ls);

   hash = rb_hash_new();

#if !(defined(OS_MSWIN) || defined(OS_BCCWIN) || defined(OS_MINGW))
   rb_hash_aset(hash, sRts, INT2FIX(ls.rts));
   rb_hash_aset(hash, sDtr, INT2FIX(ls.dtr));
#endif
   rb_hash_aset(hash, sCts, INT2FIX(ls.cts));
   rb_hash_aset(hash, sDsr, INT2FIX(ls.dsr));
   rb_hash_aset(hash, sDcd, INT2FIX(ls.dcd));
   rb_hash_aset(hash, sRi, INT2FIX(ls.ri));

   return hash;
}

/*
 * This class is used for communication over a serial port.
 * In addition to the methods here, you can use everything
 * Ruby's IO-class provides (read, write, getc, readlines, ...)
 */
void Init_serialport()
{
   sBaud = rb_str_new2("baud");
   sDataBits = rb_str_new2("data_bits");
   sStopBits = rb_str_new2("stop_bits");
   sParity = rb_str_new2("parity");
   sRts = rb_str_new2("rts");
   sDtr = rb_str_new2("dtr");
   sCts = rb_str_new2("cts");
   sDsr = rb_str_new2("dsr");
   sDcd = rb_str_new2("dcd");
   sRi = rb_str_new2("ri");

   rb_gc_register_address(&sBaud);
   rb_gc_register_address(&sDataBits);
   rb_gc_register_address(&sStopBits);
   rb_gc_register_address(&sParity);
   rb_gc_register_address(&sRts);
   rb_gc_register_address(&sDtr);
   rb_gc_register_address(&sCts);
   rb_gc_register_address(&sDsr);
   rb_gc_register_address(&sDcd);
   rb_gc_register_address(&sRi);

   cSerialPort = rb_define_class("SerialPort", rb_cIO);
   rb_define_singleton_method(cSerialPort, "create", sp_create, 1);

   rb_define_method(cSerialPort, "get_modem_params", sp_get_modem_params, 0);
   rb_define_method(cSerialPort, "set_modem_params", sp_set_modem_params, -1);
   rb_define_method(cSerialPort, "modem_params", sp_get_modem_params, 0);
   rb_define_method(cSerialPort, "modem_params=", sp_set_modem_params, -1);
   rb_define_method(cSerialPort, "baud", sp_get_data_rate, 0);
   rb_define_method(cSerialPort, "baud=", sp_set_data_rate, 1);
   rb_define_method(cSerialPort, "data_bits", sp_get_data_bits, 0);
   rb_define_method(cSerialPort, "data_bits=", sp_set_data_bits, 1);
   rb_define_method(cSerialPort, "stop_bits", sp_get_stop_bits, 0);
   rb_define_method(cSerialPort, "stop_bits=", sp_set_stop_bits, 1);
   rb_define_method(cSerialPort, "parity", sp_get_parity, 0);
   rb_define_method(cSerialPort, "parity=", sp_set_parity, 1);

   rb_define_method(cSerialPort, "flow_control=", sp_set_flow_control, 1);
   rb_define_method(cSerialPort, "flow_control", sp_get_flow_control, 0);

   rb_define_method(cSerialPort, "read_timeout", sp_get_read_timeout, 0);
   rb_define_method(cSerialPort, "read_timeout=", sp_set_read_timeout, 1);
   rb_define_method(cSerialPort, "write_timeout", sp_get_write_timeout, 0);
   rb_define_method(cSerialPort, "write_timeout=", sp_set_write_timeout, 1);

   rb_define_method(cSerialPort, "break", sp_break, 1);

   rb_define_method(cSerialPort, "signals", sp_signals, 0);
   rb_define_method(cSerialPort, "get_signals", sp_signals, 0);
   rb_define_method(cSerialPort, "rts", sp_get_rts, 0);
   rb_define_method(cSerialPort, "rts=", sp_set_rts, 1);
   rb_define_method(cSerialPort, "dtr", sp_get_dtr, 0);
   rb_define_method(cSerialPort, "dtr=", sp_set_dtr, 1);
   rb_define_method(cSerialPort, "cts", sp_get_cts, 0);
   rb_define_method(cSerialPort, "dsr", sp_get_dsr, 0);
   rb_define_method(cSerialPort, "dcd", sp_get_dcd, 0);
   rb_define_method(cSerialPort, "ri", sp_get_ri, 0);

   rb_define_const(cSerialPort, "NONE", INT2FIX(NONE));
   rb_define_const(cSerialPort, "HARD", INT2FIX(HARD));
   rb_define_const(cSerialPort, "SOFT", INT2FIX(SOFT));

   rb_define_const(cSerialPort, "SPACE", INT2FIX(SPACE));
   rb_define_const(cSerialPort, "MARK", INT2FIX(MARK));
   rb_define_const(cSerialPort, "EVEN", INT2FIX(EVEN));
   rb_define_const(cSerialPort, "ODD", INT2FIX(ODD));

   /* the package's version as a string "X.Y.Z", beeing major, minor and patch level */
   rb_define_const(cSerialPort, "VERSION", rb_str_new2(RUBY_SERIAL_PORT_VERSION));
}
