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
 */

#include "serialport.h"

VALUE cSerialPort; /* serial port class */

VALUE sBaud, sDataBits, sStopBits, sParity; /* strings */
VALUE sRts, sDtr, sCts, sDsr, sDcd, sRi;

/*
 * @api private
 *
 * @see SerialPort#new
 * @see SerialPort#open
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
 * ["baud"] Integer from 50 to 256000, depending on platform
 * ["data_bits"] Integer from 5 to 8 (4 is allowed on Windows too)
 * ["stop_bits"] Integer, only allowed values are 1 or 2 (1.5 is not supported)
 * ["parity"] One of the constants NONE, EVEN or ODD (Windows allows also MARK and SPACE)
 *
 * When using separate arguments, they are interpreted as:
 *    (baud, data_bits = 8, stop_bits = 1, parity = (previous_databits == 8 ? NONE : EVEN))
 * A baudrate of nil will keep the old value.
 * The default parity depends on the number of databits configured before this function call.
 * 
 * @overload set_modem_params(baud, data_bits, stop_bits, parity)
 *    @param baud [Integer] the baud rate
 *    @param data_bits [Integer] the number of data bits
 *    @param stop_bits [Integer] the number of stop bits
 *    @param parity [Integer] the type of parity checking
 * @overload set_modem_params(hash)
 *    @param opts [Hash] the options to configure port
 *
 * @return [Hash] the original paramters
 * @raise [ArgumentError] if values are invalide or unsupported
 */
static VALUE sp_set_modem_params(argc, argv, self)
   int argc;
   VALUE *argv, self;
{
   return sp_set_modem_params_impl(argc, argv, self);
}

/*
 * Send a break for the given time
 *
 * @param time [Integer] break time in tenths-of-a-second
 * @return [nil]
 * @note (POSIX) this value is very approximate
 */
static VALUE sp_break(self, time)
   VALUE self, time;
{
   return sp_break_impl(self, time);
}

/*
 * Get the state of the DTR line 
 *
 * @note (Windows) DTR is not available
 * @return [Integer] the state of DTR line, 0 or 1
 */
static VALUE sp_get_dtr(self)
   VALUE self;
{
   return sp_get_dtr_impl(self);
}

/*
 * Get the flow control flag
 *
 * @return [Integer] the flow control flag
 * @see SerialPort#set_flow_control
 */
static VALUE sp_get_flow_control(self)
   VALUE self;
{
   return sp_get_flow_control_impl(self);
}

/*
 * Get the read timeout value
 *
 * @return [Integer] the read timeout, in milliseconds
 * @see SerialPort#set_read_timeout
 */
static VALUE sp_get_read_timeout(self)
   VALUE self;
{
   return sp_get_read_timeout_impl(self);
}

/*
 * Get the state of the RTS line
 *
 * @return [Integer] the state of RTS line, 0 or 1
 * @note (Windows) RTS is not available
 */
static VALUE sp_get_rts(self)
   VALUE self;
{
   return sp_get_rts_impl(self);
}

/*
 * Get the write timeout
 *
 * @return [Integer] the write timeout, in milliseconds
 * @note (POSIX) write timeouts are not implemented
 */
static VALUE sp_get_write_timeout(self)
   VALUE self;
{
   return sp_get_write_timeout_impl(self);
}

/*
 * Set the state of the DTR line
 *
 * @param val [Integer] the desired state of the DTR line, 0 or 1
 * @return [Integer] the original +val+ parameter
 */
static VALUE sp_set_dtr(self, val)
   VALUE self, val;
{
   return sp_set_dtr_impl(self, val);
}

/*
 * Set the flow control
 *
 * @param val [Integer] the flow control flag,
 *    +NONE+, +HARD+, +SOFT+, or (+HARD+ | +SOFT+)
 * @return [Integer] the original +val+ parameter
 * @note SerialPort::HARD mode is not supported on all platforms.
 * @note SerialPort::HARD uses RTS/CTS handshaking.
 *    DSR/DTR is not supported.
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
 * @param timeout [Integer] the read timeout in milliseconds
 * @return [Integer] the original +timeout+ parameter
 * @note Read timeouts don't mix well with multi-threading
 */
static VALUE sp_set_read_timeout(self, val)
   VALUE self, val;
{
   return sp_set_read_timeout_impl(self, val);
}

/*
 * Set the state of the RTS line
 *
 * @param val [Integer] the state of RTS line, 0 or 1
 * @return [Integer] the original +val+ parameter
 */
static VALUE sp_set_rts(self, val)
   VALUE self, val;
{
   return sp_set_rts_impl(self, val);
}

/*
 * Set a write timeout
 *
 * @param val [Integer] the write timeout in milliseconds
 * @return [Integer] the original +val+ parameter
 * @note (POSIX) write timeouts are not implemented
 */
static VALUE sp_set_write_timeout(self, val)
   VALUE self, val;
{
   return sp_set_write_timeout_impl(self, val);
}

/*
 * @private helper
 */
static void get_modem_params(self, mp)
   VALUE self;
   struct modem_params *mp;
{
   get_modem_params_impl(self, mp);
}

/*
 * Set the baud rate
 *
 * @param data_rate [Integer] the baud rate
 * @return [Integer] the original +data_rate+ parameter
 * @see SerialPort#set_modem_params
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
 * Set the data bits
 *
 * @param data_bits [Integer] the number of data bits
 * @return [Integer] the original +data_bits+ parameter
 * @see SerialPort#set_modem_params
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
 * Set the stop bits
 *
 * @param stop_bits [Integer] the number of stop bits
 * @return [Integer] the original +stop_bits+ parameter
 * @see SerialPort#set_modem_params
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
 * Set the parity
 *
 * @param parity [Integer] the parity type
 * @return [Integer] the original +parity+ parameter
 * @see SerialPort#set_modem_params
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
 * Get the current baud rate
 *
 * @return [Integer] the current baud rate
 * @see SerialPort#set_modem_params
 */
static VALUE sp_get_data_rate(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.data_rate);
}

/*
 * Get the current data bits
 *
 * @return [Integer] the current number of data bits
 * @see SerialPort#set_modem_params
 */
static VALUE sp_get_data_bits(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.data_bits);
}

/*
 * Get the current stop bits
 *
 * @return [Integer] the current number of stop bits
 * @see SerialPort#set_modem_params for details
 */
static VALUE sp_get_stop_bits(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.stop_bits);
}

/*
 * Get the current parity
 *
 * @return [Integer] the current parity
 * @see SerialPort#set_modem_params
 */
static VALUE sp_get_parity(self)
   VALUE self;
{
   struct modem_params mp;

   get_modem_params(self, &mp);

   return INT2FIX(mp.parity);
}

/*
 * Get the configure of the serial port
 *
 * @return [Hash] the serial port configuration
 * @see SerialPort#set_modem_params
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

/*
 * @api private
 */
void get_line_signals_helper(obj, ls)
   VALUE obj;
   struct line_signals *ls;
{
   get_line_signals_helper_impl(obj, ls);
}

/*
 * Get the state of the CTS line
 *
 * @return [Integer] the state of the CTS line, 0 or 1
 * @see SerialPort#get_signals
 */
static VALUE sp_get_cts(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.cts);
}

/*
 * Get the state of the DSR line
 *
 * @return [Integer] the state of the DSR line, 0 or 1
 * @see SerialPort#get_signals
 */
static VALUE sp_get_dsr(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.dsr);
}

/*
 * Get the state of the DCD line
 *
 * @return [Integer] the state of the DCD line, 0 or 1
 * @see SerialPort#get_signals
 */
static VALUE sp_get_dcd(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.dcd);
}

/*
 * Get the state of the RI line
 *
 * @return [Integer] the state of the RI line, 0 or 1
 * @see SerialPort#get_signals
 */
static VALUE sp_get_ri(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper(self, &ls);

   return INT2FIX(ls.ri);
}

/*
 * Return a hash with the state of each line status bit.
 * Keys:
 *    "rts", "dtr", "cts", "dsr", "dcd", and "ri".
 *
 * @return [Hash] the state line info
 * @note (Windows) the rts and dtr values are not included
 * @note This method is implemented as both SerialPort#signals and SerialPort#get_signals
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

/**
 * Flush data received but not read.
 *
 * @return [Boolean] true on success or false if an error occurs.
 */
static VALUE sp_flush_input_data(self)
   VALUE self;
{
   return sp_flush_input_data_impl(self);
}

/**
 * Flush data written but not transmitted.
 *
 * @return [Boolean] true on success or false if an error occurs.
 */
static VALUE sp_flush_output_data(self)
   VALUE self;
{
   return sp_flush_output_data_impl(self);
}


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

   rb_define_method(cSerialPort, "flush_input", sp_flush_input_data, 0);
   rb_define_method(cSerialPort, "flush_output", sp_flush_output_data, 0);

   /*
    * 0
    */
   rb_define_const(cSerialPort, "NONE", INT2FIX(NONE));

   /*
    * 1
    */
   rb_define_const(cSerialPort, "HARD", INT2FIX(HARD));

   /*
    * 2
    */
   rb_define_const(cSerialPort, "SOFT", INT2FIX(SOFT));

   /*
    * 0
    */
   rb_define_const(cSerialPort, "SPACE", INT2FIX(SPACE));

   /*
    * 1
    */
   rb_define_const(cSerialPort, "MARK", INT2FIX(MARK));

   /*
    * 2
    */
   rb_define_const(cSerialPort, "EVEN", INT2FIX(EVEN));

   /*
    * 3
    */
   rb_define_const(cSerialPort, "ODD", INT2FIX(ODD));
}
