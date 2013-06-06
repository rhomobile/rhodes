/* Ruby/SerialPort
 * Guillaume Pierronnet <moumar@netcourrier.com>
 * Alan Stern <stern@rowland.harvard.edu>
 * Daniel E. Shipton <dshipton@redshiptechnologies.com>
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

/* Check if we are on a posix compliant system. */
#if !defined(OS_MSWIN) && !defined(OS_BCCWIN) && !defined(OS_MINGW)

#include <stdio.h>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>

#ifdef CRTSCTS
#define HAVE_FLOWCONTROL_HARD 1
#else
#undef HAVE_FLOWCONTROL_HARD
#endif

/* on mac os x, not all baud rates are defined in termios.h but
   they are mapped to the numeric value anyway, so we define them here */
#ifdef __APPLE__
#ifndef B460800
#define B460800 460800
#endif
#ifndef B500000
#define B500000 500000
#endif
#ifndef B576000
#define B576000 576000
#endif
#ifndef B921600
#define B921600 921600
#endif
#ifndef B1000000
#define B1000000 1000000
#endif
#endif

static char sTcgetattr[] = "tcgetattr";
static char sTcsetattr[] = "tcsetattr";
static char sIoctl[] = "ioctl";


int get_fd_helper(obj)
   VALUE obj;
{
#ifdef HAVE_RUBY_IO_H
   rb_io_t *fptr;
#else
   OpenFile *fptr;
#endif
   GetOpenFile(obj, fptr);
#ifdef HAVE_RUBY_IO_H
   return (fptr->fd);
#else
   return (fileno(fptr->f));
#endif
}

VALUE sp_create_impl(class, _port)
   VALUE class, _port;
{
#ifdef HAVE_RUBY_IO_H
   rb_io_t *fp;
#else
   OpenFile *fp;
#endif

   int fd;
   int num_port;
   char *port;
   char *ports[] = {
#if defined(OS_LINUX) || defined(OS_CYGWIN)
      "/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3",
      "/dev/ttyS4", "/dev/ttyS5", "/dev/ttyS6", "/dev/ttyS7"
#elif defined(OS_FREEBSD) || defined(OS_NETBSD) || defined(OS_OPENBSD) || defined(OS_DARWIN)
      "/dev/cuaa0", "/dev/cuaa1", "/dev/cuaa2", "/dev/cuaa3",
      "/dev/cuaa4", "/dev/cuaa5", "/dev/cuaa6", "/dev/cuaa7"
#elif defined(OS_SOLARIS)
         "/dev/ttya", "/dev/ttyb", "/dev/ttyc", "/dev/ttyd",
      "/dev/ttye", "/dev/ttyf", "/dev/ttyg", "/dev/ttyh"
#elif defined(OS_AIX)
         "/dev/tty0", "/dev/tty1", "/dev/tty2", "/dev/tty3",
      "/dev/tty4", "/dev/tty5", "/dev/tty6", "/dev/tty7"
#elif defined(OS_IRIX)
         "/dev/ttyf1", "/dev/ttyf2", "/dev/ttyf3", "/dev/ttyf4",
      "/dev/ttyf5", "/dev/ttyf6", "/dev/ttyf7", "/dev/ttyf8"
#endif
   };
   struct termios params;

   NEWOBJ(sp, struct RFile);
   rb_secure(4);
   OBJSETUP(sp, class, T_FILE);
   MakeOpenFile((VALUE) sp, fp);

   switch(TYPE(_port))
   {
      case T_FIXNUM:
         num_port = FIX2INT(_port);
         if (num_port < 0 || num_port > sizeof(ports) / sizeof(ports[0]))
         {
            rb_raise(rb_eArgError, "illegal port number");
         }
         port = ports[num_port];
         break;

      case T_STRING:
         Check_SafeStr(_port);
         port = RSTRING_PTR(_port);
         break;

      default:
         rb_raise(rb_eTypeError, "wrong argument type");
         break;
   }

   fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
   if (fd == -1)
   {
      rb_sys_fail(port);
   }

   if (!isatty(fd))
   {
      close(fd);
      rb_raise(rb_eArgError, "not a serial port");
   }

   /* enable blocking read */
   fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~O_NONBLOCK);

   if (tcgetattr(fd, &params) == -1)
   {
      close(fd);
      rb_sys_fail(sTcgetattr);
   }

   params.c_oflag = 0;
   params.c_lflag = 0;
   params.c_iflag &= (IXON | IXOFF | IXANY);
   params.c_cflag |= CLOCAL | CREAD;
   params.c_cflag &= ~HUPCL;

   if (tcsetattr(fd, TCSANOW, &params) == -1)
   {
      close(fd);
      rb_sys_fail(sTcsetattr);
   }

#ifdef HAVE_RUBY_IO_H
   fp->fd = fd;
#else
   fp->f = rb_fdopen(fd, "r+");
#endif
   fp->mode = FMODE_READWRITE | FMODE_SYNC;

   return (VALUE) sp;
}

VALUE sp_set_modem_params_impl(argc, argv, self)
   int argc;
   VALUE *argv, self;
{
   int fd;
   struct termios params;
   VALUE _data_rate, _data_bits, _parity, _stop_bits;
   int use_hash = 0;
   int data_rate, data_bits;
   _data_rate = _data_bits = _parity = _stop_bits = Qnil;

   if (argc == 0)
   {
      return self;
   }

   if (argc == 1 && T_HASH == TYPE(argv[0]))
   {
      use_hash = 1;
      _data_rate = rb_hash_aref(argv[0], sBaud);
      _data_bits = rb_hash_aref(argv[0], sDataBits);
      _stop_bits = rb_hash_aref(argv[0], sStopBits);
      _parity = rb_hash_aref(argv[0], sParity);
   }

   fd = get_fd_helper(self);
   if (tcgetattr(fd, &params) == -1)
   {
      rb_sys_fail(sTcgetattr);
   }

   if (!use_hash)
   {
      _data_rate = argv[0];
   }

   if (NIL_P(_data_rate))
   {
      goto SkipDataRate;
   }
   Check_Type(_data_rate, T_FIXNUM);

   switch(FIX2INT(_data_rate))
   {
      case 50:    data_rate = B50; break;
      case 75:    data_rate = B75; break;
      case 110:   data_rate = B110; break;
      case 134:   data_rate = B134; break;
      case 150:   data_rate = B150; break;
      case 200:   data_rate = B200; break;
      case 300:   data_rate = B300; break;
      case 600:   data_rate = B600; break;
      case 1200:  data_rate = B1200; break;
      case 1800:  data_rate = B1800; break;
      case 2400:  data_rate = B2400; break;
      case 4800:  data_rate = B4800; break;
      case 9600:  data_rate = B9600; break;
      case 19200: data_rate = B19200; break;
      case 38400: data_rate = B38400; break;
#ifdef B57600
      case 57600: data_rate = B57600; break;
#endif
#ifdef B76800
      case 76800: data_rate = B76800; break;
#endif
#ifdef B115200
      case 115200: data_rate = B115200; break;
#endif
#ifdef B230400
      case 230400: data_rate = B230400; break;
#endif
#ifdef B460800
      case 460800: data_rate = B460800; break;
#endif
#ifdef B500000
      case 500000: data_rate = B500000; break;
#endif
#ifdef B576000
      case 576000: data_rate = B576000; break;
#endif
#ifdef B921600
      case 921600: data_rate = B921600; break;
#endif
#ifdef B1000000
      case 1000000: data_rate = B1000000; break;
#endif

      default:
                   rb_raise(rb_eArgError, "unknown baud rate");
                   break;
   }
   cfsetispeed(&params, data_rate);
   cfsetospeed(&params, data_rate);

   SkipDataRate:

   if (!use_hash)
   {
      _data_bits = (argc >= 2 ? argv[1] : INT2FIX(8));
   }

   if (NIL_P(_data_bits))
   {
      goto SkipDataBits;
   }
   Check_Type(_data_bits, T_FIXNUM);

   switch(FIX2INT(_data_bits))
   {
      case 5:
         data_bits = CS5;
         break;
      case 6:
         data_bits = CS6;
         break;
      case 7:
         data_bits = CS7;
         break;
      case 8:
         data_bits = CS8;
         break;
      default:
         rb_raise(rb_eArgError, "unknown character size");
         break;
   }
   params.c_cflag &= ~CSIZE;
   params.c_cflag |= data_bits;

   SkipDataBits:

   if (!use_hash)
   {
      _stop_bits = (argc >= 3 ? argv[2] : INT2FIX(1));
   }

   if (NIL_P(_stop_bits))
   {
      goto SkipStopBits;
   }

   Check_Type(_stop_bits, T_FIXNUM);

   switch(FIX2INT(_stop_bits))
   {
      case 1:
         params.c_cflag &= ~CSTOPB;
         break;
      case 2:
         params.c_cflag |= CSTOPB;
         break;
      default:
         rb_raise(rb_eArgError, "unknown number of stop bits");
         break;
   }

   SkipStopBits:

   if (!use_hash)
   {
      _parity = (argc >= 4 ? argv[3] : ((params.c_cflag & CSIZE) == CS8 ?
               INT2FIX(NONE) : INT2FIX(EVEN)));
   }

   if (NIL_P(_parity))
   {
      goto SkipParity;
   }

   Check_Type(_parity, T_FIXNUM);

   switch(FIX2INT(_parity))
   {
      case EVEN:
         params.c_cflag |= PARENB;
         params.c_cflag &= ~PARODD;
         break;

      case ODD:
         params.c_cflag |= PARENB;
         params.c_cflag |= PARODD;
         break;

      case NONE:
         params.c_cflag &= ~PARENB;
         break;

      default:
         rb_raise(rb_eArgError, "unknown parity");
         break;
   }

   SkipParity:

   if (tcsetattr(fd, TCSANOW, &params) == -1)
   {
      rb_sys_fail(sTcsetattr);
   }
   return argv[0];
}

void get_modem_params_impl(self, mp)
   VALUE self;
   struct modem_params *mp;
{
   int fd;
   struct termios params;

   fd = get_fd_helper(self);
   if (tcgetattr(fd, &params) == -1)
   {
      rb_sys_fail(sTcgetattr);
   }

   switch (cfgetospeed(&params))
   {
      case B50:    mp->data_rate = 50; break;
      case B75:    mp->data_rate = 75; break;
      case B110:   mp->data_rate = 110; break;
      case B134:   mp->data_rate = 134; break;
      case B150:   mp->data_rate = 150; break;
      case B200:   mp->data_rate = 200; break;
      case B300:   mp->data_rate = 300; break;
      case B600:   mp->data_rate = 600; break;
      case B1200:  mp->data_rate = 1200; break;
      case B1800:  mp->data_rate = 1800; break;
      case B2400:  mp->data_rate = 2400; break;
      case B4800:  mp->data_rate = 4800; break;
      case B9600:  mp->data_rate = 9600; break;
      case B19200: mp->data_rate = 19200; break;
      case B38400: mp->data_rate = 38400; break;
#ifdef B57600
      case B57600: mp->data_rate = 57600; break;
#endif
#ifdef B76800
      case B76800: mp->data_rate = 76800; break;
#endif
#ifdef B115200
      case B115200: mp->data_rate = 115200; break;
#endif
#ifdef B230400
      case B230400: mp->data_rate = 230400; break;
#endif
#ifdef B460800
      case B460800: mp->data_rate = 460800; break;
#endif
#ifdef B500000
      case B500000: mp->data_rate = 500000; break;
#endif
#ifdef B576000
      case B576000: mp->data_rate = 576000; break;
#endif
#ifdef B921600
      case B921600: mp->data_rate = 921600; break;
#endif
#ifdef B1000000
      case B1000000: mp->data_rate = 1000000; break;
#endif
   }

   switch(params.c_cflag & CSIZE)
   {
      case CS5:
         mp->data_bits = 5;
         break;
      case CS6:
         mp->data_bits = 6;
         break;
      case CS7:
         mp->data_bits = 7;
         break;
      case CS8:
         mp->data_bits = 8;
         break;
      default:
         mp->data_bits = 0;
         break;
   }

   mp->stop_bits = (params.c_cflag & CSTOPB ? 2 : 1);

   if (!(params.c_cflag & PARENB))
   {
      mp->parity = NONE;
   }
   else if (params.c_cflag & PARODD)
   {
      mp->parity = ODD;
   }
   else
   {
      mp->parity = EVEN;
   }
}

VALUE sp_set_flow_control_impl(self, val)
   VALUE self, val;
{
   int fd;
   int flowc;
   struct termios params;

   Check_Type(val, T_FIXNUM);

   fd = get_fd_helper(self);
   if (tcgetattr(fd, &params) == -1)
   {
      rb_sys_fail(sTcgetattr);
   }

   flowc = FIX2INT(val);
   if (flowc & HARD)
   {
#ifdef HAVE_FLOWCONTROL_HARD
      params.c_cflag |= CRTSCTS;
   }
   else
   {
      params.c_cflag &= ~CRTSCTS;
   }
#else
      rb_raise(rb_eIOError, "Hardware flow control not supported");
   }
#endif

   if (flowc & SOFT)
   {
      params.c_iflag |= (IXON | IXOFF | IXANY);
   }
   else
   {
      params.c_iflag &= ~(IXON | IXOFF | IXANY);
   }

   if (tcsetattr(fd, TCSANOW, &params) == -1)
   {
      rb_sys_fail(sTcsetattr);
   }

   return val;
}

VALUE sp_get_flow_control_impl(self)
   VALUE self;
{
   int ret;
   int fd;
   struct termios params;

   fd = get_fd_helper(self);
   if (tcgetattr(fd, &params) == -1)
   {
      rb_sys_fail(sTcgetattr);
   }

   ret = 0;

#ifdef HAVE_FLOWCONTROL_HARD
   if (params.c_cflag & CRTSCTS)
   {
      ret += HARD;
   }
#endif

   if (params.c_iflag & (IXON | IXOFF | IXANY))
   {
      ret += SOFT;
   }

   return INT2FIX(ret);
}

VALUE sp_set_read_timeout_impl(self, val)
   VALUE self, val;
{
   int timeout;
   int fd;
   struct termios params;

   Check_Type(val, T_FIXNUM);
   timeout = FIX2INT(val);

   fd = get_fd_helper(self);
   if (tcgetattr(fd, &params) == -1)
   {
      rb_sys_fail(sTcgetattr);
   }

   if (timeout < 0)
   {
      params.c_cc[VTIME] = 0;
      params.c_cc[VMIN] = 0;
   }
   else if (timeout == 0)
   {
      params.c_cc[VTIME] = 0;
      params.c_cc[VMIN] = 1;
   }
   else
   {
      params.c_cc[VTIME] = (timeout + 50) / 100;
      params.c_cc[VMIN] = 0;
   }

   if (tcsetattr(fd, TCSANOW, &params) == -1)
   {
      rb_sys_fail(sTcsetattr);
   }

   return val;
}

VALUE sp_get_read_timeout_impl(self)
   VALUE self;
{
   int fd;
   struct termios params;

   fd = get_fd_helper(self);
   if (tcgetattr(fd, &params) == -1)
   {
      rb_sys_fail(sTcgetattr);
   }

   if (params.c_cc[VTIME] == 0 && params.c_cc[VMIN] == 0)
   {
      return INT2FIX(-1);
   }

   return INT2FIX(params.c_cc[VTIME] * 100);
}

VALUE sp_set_write_timeout_impl(self, val)
   VALUE self, val;
{
   rb_notimplement();
   return self;
}

VALUE sp_get_write_timeout_impl(self)
   VALUE self;
{
   rb_notimplement();
   return self;
}

VALUE sp_break_impl(self, time)
   VALUE self, time;
{
   int fd;

   Check_Type(time, T_FIXNUM);

   fd = get_fd_helper(self);

   if (tcsendbreak(fd, FIX2INT(time) / 3) == -1)
   {
      rb_sys_fail("tcsendbreak");
   }

   return Qnil;
}

void get_line_signals_helper_impl(obj, ls)
   VALUE obj;
   struct line_signals *ls;
{
   int fd, status;

   fd = get_fd_helper(obj);

   if (ioctl(fd, TIOCMGET, &status) == -1)
   {
      rb_sys_fail(sIoctl);
   }

   ls->rts = (status & TIOCM_RTS ? 1 : 0);
   ls->dtr = (status & TIOCM_DTR ? 1 : 0);
   ls->cts = (status & TIOCM_CTS ? 1 : 0);
   ls->dsr = (status & TIOCM_DSR ? 1 : 0);
   ls->dcd = (status & TIOCM_CD ? 1 : 0);
   ls->ri  = (status & TIOCM_RI ? 1 : 0);
}

VALUE set_signal_impl(obj, val, sig)
   VALUE obj,val;
   int sig;
{
   int status;
   int fd;
   int set;

   Check_Type(val, T_FIXNUM);
   fd = get_fd_helper(obj);

   if (ioctl(fd, TIOCMGET, &status) == -1)
   {
      rb_sys_fail(sIoctl);
   }

   set = FIX2INT(val);

   if (set == 0)
   {
      status &= ~sig;
   }
   else if (set == 1)
   {
      status |= sig;
   }
   else
   {
      rb_raise(rb_eArgError, "invalid value");
   }

   if (ioctl(fd, TIOCMSET, &status) == -1)
   {
      rb_sys_fail(sIoctl);
   }

   return val;
}

VALUE sp_set_rts_impl(self, val)
   VALUE self, val;
{
   return set_signal_impl(self, val, TIOCM_RTS);
}

VALUE sp_set_dtr_impl(self, val)
   VALUE self, val;
{
   return set_signal_impl(self, val, TIOCM_DTR);
}

VALUE sp_get_rts_impl(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper_impl(self, &ls);
   return INT2FIX(ls.rts);
}

VALUE sp_get_dtr_impl(self)
   VALUE self;
{
   struct line_signals ls;

   get_line_signals_helper_impl(self, &ls);

   return INT2FIX(ls.dtr);
}

#endif /* !defined(OS_MSWIN) && !defined(OS_BCCWIN) && !defined(OS_MINGW) */
