 
#
# It is auto-generated content.
# Do not do required for this file in your application.
#
 
class Socket
  module Constants
  end

  SOCK_STREAM = 1

  SOCK_DGRAM = 2

  SOCK_RAW = 3

  SOCK_RDM = 4

  SOCK_SEQPACKET = 5

  AF_INET = 2

  PF_INET = 2

  AF_UNIX = 1

  PF_UNIX = 1

  AF_IPX = 6

  PF_IPX = 6

  AF_APPLETALK = 16

  PF_APPLETALK = 16

  AF_UNSPEC = 0

  PF_UNSPEC = 0

  AF_IMPLINK = 3

  PF_IMPLINK = 3

  AF_PUP = 4

  PF_PUP = 4

  AF_CHAOS = 5

  PF_CHAOS = 5

  AF_NS = 6

  PF_NS = 6

  AF_ISO = 7

  PF_ISO = 7

  AF_OSI = 7

  PF_OSI = 7

  AF_ECMA = 8

  PF_ECMA = 8

  AF_DATAKIT = 9

  PF_DATAKIT = 9

  AF_CCITT = 10

  PF_CCITT = 10

  AF_SNA = 11

  PF_SNA = 11

  AF_DLI = 13

  PF_DLI = 13

  AF_LAT = 14

  PF_LAT = 14

  AF_HYLINK = 15

  PF_HYLINK = 15

  AF_NETBIOS = 17

  AF_ATM = 22

  PF_ATM = 22

  AF_MAX = 32

  PF_MAX = 32

  MSG_OOB = 1

  MSG_PEEK = 2

  MSG_DONTROUTE = 4

  SOL_SOCKET = 65535

  IPPROTO_IP = 0

  IPPROTO_ICMP = 1

  IPPROTO_IGMP = 2

  IPPROTO_GGP = 3

  IPPROTO_TCP = 6

  IPPROTO_PUP = 12

  IPPROTO_UDP = 17

  IPPROTO_IDP = 22

  IPPROTO_ND = 77

  IPPROTO_RAW = 255

  IPPROTO_MAX = 256

  IPPORT_RESERVED = 1024

  IPPORT_USERRESERVED = 5000

  INADDR_ANY = 0

  INADDR_BROADCAST = -1

  INADDR_LOOPBACK = -16777215

  INADDR_UNSPEC_GROUP = -536870912

  INADDR_ALLHOSTS_GROUP = -536870911

  INADDR_MAX_LOCAL_GROUP = -536870657

  INADDR_NONE = -1

  IP_OPTIONS = 1

  IP_HDRINCL = 9

  IP_TOS = 8

  IP_TTL = 7

  IP_MULTICAST_IF = 2

  IP_MULTICAST_TTL = 3

  IP_MULTICAST_LOOP = 4

  IP_ADD_MEMBERSHIP = 5

  IP_DROP_MEMBERSHIP = 6

  SO_DEBUG = 1

  SO_REUSEADDR = 4

  SO_TYPE = 4104

  SO_ERROR = 4103

  SO_DONTROUTE = 16

  SO_BROADCAST = 32

  SO_SNDBUF = 4097

  SO_RCVBUF = 4098

  SO_KEEPALIVE = 8

  SO_OOBINLINE = 256

  SO_LINGER = 128

  SO_RCVLOWAT = 4100

  SO_SNDLOWAT = 4099

  SO_RCVTIMEO = 4102

  SO_SNDTIMEO = 4101

  SO_ACCEPTCONN = 2

  SO_USELOOPBACK = 64

  TCP_NODELAY = 1

  EAI_AGAIN = 11002

  EAI_BADFLAGS = 10022

  EAI_FAIL = 11003

  EAI_FAMILY = 10047

  EAI_MEMORY = 8

  EAI_NODATA = 11004

  EAI_NONAME = 11001

  EAI_SERVICE = 10109

  EAI_SOCKTYPE = 10044

  AI_PASSIVE = 1

  AI_CANONNAME = 2

  AI_NUMERICHOST = 4

  NI_MAXHOST = 1025

  NI_MAXSERV = 32

  NI_NOFQDN = 1

  NI_NUMERICHOST = 2

  NI_NAMEREQD = 4

  NI_NUMERICSERV = 8

  NI_DGRAM = 16

  SHUT_RD = 0

  SHUT_WR = 1

  SHUT_RDWR = 2

  IPV6_JOIN_GROUP = 12

  IPV6_LEAVE_GROUP = 13

  IPV6_MULTICAST_HOPS = 10

  IPV6_MULTICAST_IF = 9

  IPV6_MULTICAST_LOOP = 11

  IPV6_UNICAST_HOPS = 4

  IPV6_HOPLIMIT = 21

  IPV6_PKTINFO = 19

  module WaitReadable
  end

  module WaitWritable
  end

  SEEK_SET = 0

  SEEK_CUR = 1

  SEEK_END = 2

  LOCK_SH = 1

  LOCK_EX = 2

  LOCK_UN = 8

  LOCK_NB = 4

  RDONLY = 0

  WRONLY = 1

  RDWR = 2

  APPEND = 8

  CREAT = 256

  EXCL = 1024

  NONBLOCK = 4

  TRUNC = 512

  BINARY = 32768

  FNM_NOESCAPE = 1

  FNM_PATHNAME = 2

  FNM_DOTMATCH = 4

  FNM_CASEFOLD = 8

  FNM_SYSCASE = 8

  def self.socketpair
  end  
  def self.pair
  end  
  def self.gethostname
  end  
  def self.gethostbyname(req)
  end  
  def self.gethostbyaddr(rest)
  end  
  def self.getservbyname(rest)
  end  
  def self.getservbyport(rest)
  end  
  def self.getaddrinfo(rest)
  end  
  def self.getnameinfo(rest)
  end  
  def self.sockaddr_in(req,req1)
  end  
  def self.pack_sockaddr_in(req,req1)
  end  
  def self.unpack_sockaddr_in(req)
  end  
  def connect(req)
  end  
  def connect_nonblock(req)
  end  
  def bind(req)
  end  
  def listen(req)
  end  
  def accept
  end  
  def accept_nonblock
  end  
  def sysaccept
  end  
  def recvfrom(rest)
  end  
  def recvfrom_nonblock(rest)
  end  
end

