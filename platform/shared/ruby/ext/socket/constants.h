#define pseudo_AF_FTIP pseudo_AF_RTIP
#ifdef SOCK_STREAM
    sock_define_const("SOCK_STREAM", SOCK_STREAM);
#endif

#ifdef SOCK_DGRAM
    sock_define_const("SOCK_DGRAM", SOCK_DGRAM);
#endif

#ifdef SOCK_RAW
    sock_define_const("SOCK_RAW", SOCK_RAW);
#endif

#ifdef SOCK_RDM
    sock_define_const("SOCK_RDM", SOCK_RDM);
#endif

#ifdef SOCK_SEQPACKET
    sock_define_const("SOCK_SEQPACKET", SOCK_SEQPACKET);
#endif

#ifdef SOCK_PACKET
    sock_define_const("SOCK_PACKET", SOCK_PACKET);
#endif

#ifdef AF_INET
    sock_define_const("AF_INET", AF_INET);
#endif

#ifdef PF_INET
    sock_define_const("PF_INET", PF_INET);
#endif

#ifdef AF_UNIX
    sock_define_const("AF_UNIX", AF_UNIX);
#endif

#ifdef PF_UNIX
    sock_define_const("PF_UNIX", PF_UNIX);
#endif

#ifdef AF_AX25
    sock_define_const("AF_AX25", AF_AX25);
#endif

#ifdef PF_AX25
    sock_define_const("PF_AX25", PF_AX25);
#endif

#ifdef AF_IPX
    sock_define_const("AF_IPX", AF_IPX);
#endif

#ifdef PF_IPX
    sock_define_const("PF_IPX", PF_IPX);
#endif

#ifdef AF_APPLETALK
    sock_define_const("AF_APPLETALK", AF_APPLETALK);
#endif

#ifdef PF_APPLETALK
    sock_define_const("PF_APPLETALK", PF_APPLETALK);
#endif

#ifdef AF_UNSPEC
    sock_define_const("AF_UNSPEC", AF_UNSPEC);
#endif

#ifdef PF_UNSPEC
    sock_define_const("PF_UNSPEC", PF_UNSPEC);
#endif

#ifdef AF_LOCAL
    sock_define_const("AF_LOCAL", AF_LOCAL);
#endif

#ifdef PF_LOCAL
    sock_define_const("PF_LOCAL", PF_LOCAL);
#endif

#ifdef AF_IMPLINK
    sock_define_const("AF_IMPLINK", AF_IMPLINK);
#endif

#ifdef PF_IMPLINK
    sock_define_const("PF_IMPLINK", PF_IMPLINK);
#endif

#ifdef AF_PUP
    sock_define_const("AF_PUP", AF_PUP);
#endif

#ifdef PF_PUP
    sock_define_const("PF_PUP", PF_PUP);
#endif

#ifdef AF_CHAOS
    sock_define_const("AF_CHAOS", AF_CHAOS);
#endif

#ifdef PF_CHAOS
    sock_define_const("PF_CHAOS", PF_CHAOS);
#endif

#ifdef AF_NS
    sock_define_const("AF_NS", AF_NS);
#endif

#ifdef PF_NS
    sock_define_const("PF_NS", PF_NS);
#endif

#ifdef AF_ISO
    sock_define_const("AF_ISO", AF_ISO);
#endif

#ifdef PF_ISO
    sock_define_const("PF_ISO", PF_ISO);
#endif

#ifdef AF_OSI
    sock_define_const("AF_OSI", AF_OSI);
#endif

#ifdef PF_OSI
    sock_define_const("PF_OSI", PF_OSI);
#endif

#ifdef AF_ECMA
    sock_define_const("AF_ECMA", AF_ECMA);
#endif

#ifdef PF_ECMA
    sock_define_const("PF_ECMA", PF_ECMA);
#endif

#ifdef AF_DATAKIT
    sock_define_const("AF_DATAKIT", AF_DATAKIT);
#endif

#ifdef PF_DATAKIT
    sock_define_const("PF_DATAKIT", PF_DATAKIT);
#endif

#ifdef AF_CCITT
    sock_define_const("AF_CCITT", AF_CCITT);
#endif

#ifdef PF_CCITT
    sock_define_const("PF_CCITT", PF_CCITT);
#endif

#ifdef AF_SNA
    sock_define_const("AF_SNA", AF_SNA);
#endif

#ifdef PF_SNA
    sock_define_const("PF_SNA", PF_SNA);
#endif

#ifdef AF_DEC
    sock_define_const("AF_DEC", AF_DEC);
#endif

#ifdef PF_DEC
    sock_define_const("PF_DEC", PF_DEC);
#endif

#ifdef AF_DLI
    sock_define_const("AF_DLI", AF_DLI);
#endif

#ifdef PF_DLI
    sock_define_const("PF_DLI", PF_DLI);
#endif

#ifdef AF_LAT
    sock_define_const("AF_LAT", AF_LAT);
#endif

#ifdef PF_LAT
    sock_define_const("PF_LAT", PF_LAT);
#endif

#ifdef AF_HYLINK
    sock_define_const("AF_HYLINK", AF_HYLINK);
#endif

#ifdef PF_HYLINK
    sock_define_const("PF_HYLINK", PF_HYLINK);
#endif

#ifdef AF_ROUTE
    sock_define_const("AF_ROUTE", AF_ROUTE);
#endif

#ifdef PF_ROUTE
    sock_define_const("PF_ROUTE", PF_ROUTE);
#endif

#ifdef AF_LINK
    sock_define_const("AF_LINK", AF_LINK);
#endif

#ifdef PF_LINK
    sock_define_const("PF_LINK", PF_LINK);
#endif

#ifdef AF_COIP
    sock_define_const("AF_COIP", AF_COIP);
#endif

#ifdef PF_COIP
    sock_define_const("PF_COIP", PF_COIP);
#endif

#ifdef AF_CNT
    sock_define_const("AF_CNT", AF_CNT);
#endif

#ifdef PF_CNT
    sock_define_const("PF_CNT", PF_CNT);
#endif

#ifdef AF_SIP
    sock_define_const("AF_SIP", AF_SIP);
#endif

#ifdef PF_SIP
    sock_define_const("PF_SIP", PF_SIP);
#endif

#ifdef AF_NDRV
    sock_define_const("AF_NDRV", AF_NDRV);
#endif

#ifdef PF_NDRV
    sock_define_const("PF_NDRV", PF_NDRV);
#endif

#ifdef AF_ISDN
    sock_define_const("AF_ISDN", AF_ISDN);
#endif

#ifdef PF_ISDN
    sock_define_const("PF_ISDN", PF_ISDN);
#endif

#ifdef AF_NATM
    sock_define_const("AF_NATM", AF_NATM);
#endif

#ifdef PF_NATM
    sock_define_const("PF_NATM", PF_NATM);
#endif

#ifdef AF_SYSTEM
    sock_define_const("AF_SYSTEM", AF_SYSTEM);
#endif

#ifdef PF_SYSTEM
    sock_define_const("PF_SYSTEM", PF_SYSTEM);
#endif

#ifdef AF_NETBIOS
    sock_define_const("AF_NETBIOS", AF_NETBIOS);
#endif

#ifdef PF_NETBIOS
    sock_define_const("PF_NETBIOS", PF_NETBIOS);
#endif

#ifdef AF_PPP
    sock_define_const("AF_PPP", AF_PPP);
#endif

#ifdef PF_PPP
    sock_define_const("PF_PPP", PF_PPP);
#endif

#ifdef AF_ATM
    sock_define_const("AF_ATM", AF_ATM);
#endif

#ifdef PF_ATM
    sock_define_const("PF_ATM", PF_ATM);
#endif

#ifdef AF_NETGRAPH
    sock_define_const("AF_NETGRAPH", AF_NETGRAPH);
#endif

#ifdef PF_NETGRAPH
    sock_define_const("PF_NETGRAPH", PF_NETGRAPH);
#endif

#ifdef AF_MAX
    sock_define_const("AF_MAX", AF_MAX);
#endif

#ifdef PF_MAX
    sock_define_const("PF_MAX", PF_MAX);
#endif

#ifdef AF_E164
    sock_define_const("AF_E164", AF_E164);
#endif

#ifdef PF_XTP
    sock_define_const("PF_XTP", PF_XTP);
#endif

#ifdef PF_RTIP
    sock_define_const("PF_RTIP", PF_RTIP);
#endif

#ifdef PF_PIP
    sock_define_const("PF_PIP", PF_PIP);
#endif

#ifdef PF_KEY
    sock_define_const("PF_KEY", PF_KEY);
#endif

#ifdef MSG_OOB
    sock_define_const("MSG_OOB", MSG_OOB);
#endif

#ifdef MSG_PEEK
    sock_define_const("MSG_PEEK", MSG_PEEK);
#endif

#ifdef MSG_DONTROUTE
    sock_define_const("MSG_DONTROUTE", MSG_DONTROUTE);
#endif

#ifdef MSG_EOR
    sock_define_const("MSG_EOR", MSG_EOR);
#endif

#ifdef MSG_TRUNC
    sock_define_const("MSG_TRUNC", MSG_TRUNC);
#endif

#ifdef MSG_CTRUNC
    sock_define_const("MSG_CTRUNC", MSG_CTRUNC);
#endif

#ifdef MSG_WAITALL
    sock_define_const("MSG_WAITALL", MSG_WAITALL);
#endif

#ifdef MSG_DONTWAIT
    sock_define_const("MSG_DONTWAIT", MSG_DONTWAIT);
#endif

#ifdef MSG_EOF
    sock_define_const("MSG_EOF", MSG_EOF);
#endif

#ifdef MSG_FLUSH
    sock_define_const("MSG_FLUSH", MSG_FLUSH);
#endif

#ifdef MSG_HOLD
    sock_define_const("MSG_HOLD", MSG_HOLD);
#endif

#ifdef MSG_SEND
    sock_define_const("MSG_SEND", MSG_SEND);
#endif

#ifdef MSG_HAVEMORE
    sock_define_const("MSG_HAVEMORE", MSG_HAVEMORE);
#endif

#ifdef MSG_RCVMORE
    sock_define_const("MSG_RCVMORE", MSG_RCVMORE);
#endif

#ifdef MSG_COMPAT
    sock_define_const("MSG_COMPAT", MSG_COMPAT);
#endif

#ifdef SOL_SOCKET
    sock_define_const("SOL_SOCKET", SOL_SOCKET);
#endif

#ifdef SOL_IP
    sock_define_const("SOL_IP", SOL_IP);
#endif

#ifdef SOL_IPX
    sock_define_const("SOL_IPX", SOL_IPX);
#endif

#ifdef SOL_AX25
    sock_define_const("SOL_AX25", SOL_AX25);
#endif

#ifdef SOL_ATALK
    sock_define_const("SOL_ATALK", SOL_ATALK);
#endif

#ifdef SOL_TCP
    sock_define_const("SOL_TCP", SOL_TCP);
#endif

#ifdef SOL_UDP
    sock_define_const("SOL_UDP", SOL_UDP);
#endif

#ifdef IPPROTO_IP
    sock_define_const("IPPROTO_IP", IPPROTO_IP);
#else
    sock_define_const("IPPROTO_IP", 0);
#endif

#ifdef IPPROTO_ICMP
    sock_define_const("IPPROTO_ICMP", IPPROTO_ICMP);
#else
    sock_define_const("IPPROTO_ICMP", 1);
#endif

#ifdef IPPROTO_IGMP
    sock_define_const("IPPROTO_IGMP", IPPROTO_IGMP);
#endif

#ifdef IPPROTO_GGP
    sock_define_const("IPPROTO_GGP", IPPROTO_GGP);
#endif

#ifdef IPPROTO_TCP
    sock_define_const("IPPROTO_TCP", IPPROTO_TCP);
#else
    sock_define_const("IPPROTO_TCP", 6);
#endif

#ifdef IPPROTO_EGP
    sock_define_const("IPPROTO_EGP", IPPROTO_EGP);
#endif

#ifdef IPPROTO_PUP
    sock_define_const("IPPROTO_PUP", IPPROTO_PUP);
#endif

#ifdef IPPROTO_UDP
    sock_define_const("IPPROTO_UDP", IPPROTO_UDP);
#else
    sock_define_const("IPPROTO_UDP", 17);
#endif

#ifdef IPPROTO_IDP
    sock_define_const("IPPROTO_IDP", IPPROTO_IDP);
#endif

#ifdef IPPROTO_HELLO
    sock_define_const("IPPROTO_HELLO", IPPROTO_HELLO);
#endif

#ifdef IPPROTO_ND
    sock_define_const("IPPROTO_ND", IPPROTO_ND);
#endif

#ifdef IPPROTO_TP
    sock_define_const("IPPROTO_TP", IPPROTO_TP);
#endif

#ifdef IPPROTO_XTP
    sock_define_const("IPPROTO_XTP", IPPROTO_XTP);
#endif

#ifdef IPPROTO_EON
    sock_define_const("IPPROTO_EON", IPPROTO_EON);
#endif

#ifdef IPPROTO_BIP
    sock_define_const("IPPROTO_BIP", IPPROTO_BIP);
#endif

#ifdef IPPROTO_RAW
    sock_define_const("IPPROTO_RAW", IPPROTO_RAW);
#else
    sock_define_const("IPPROTO_RAW", 255);
#endif

#ifdef IPPROTO_MAX
    sock_define_const("IPPROTO_MAX", IPPROTO_MAX);
#endif

#ifdef IPPORT_RESERVED
    sock_define_const("IPPORT_RESERVED", IPPORT_RESERVED);
#else
    sock_define_const("IPPORT_RESERVED", 1024);
#endif

#ifdef IPPORT_USERRESERVED
    sock_define_const("IPPORT_USERRESERVED", IPPORT_USERRESERVED);
#else
    sock_define_const("IPPORT_USERRESERVED", 5000);
#endif

#ifdef INADDR_ANY
    sock_define_const("INADDR_ANY", INADDR_ANY);
#else
    sock_define_const("INADDR_ANY", 0x00000000);
#endif

#ifdef INADDR_BROADCAST
    sock_define_const("INADDR_BROADCAST", INADDR_BROADCAST);
#else
    sock_define_const("INADDR_BROADCAST", 0xffffffff);
#endif

#ifdef INADDR_LOOPBACK
    sock_define_const("INADDR_LOOPBACK", INADDR_LOOPBACK);
#else
    sock_define_const("INADDR_LOOPBACK", 0x7F000001);
#endif

#ifdef INADDR_UNSPEC_GROUP
    sock_define_const("INADDR_UNSPEC_GROUP", INADDR_UNSPEC_GROUP);
#else
    sock_define_const("INADDR_UNSPEC_GROUP", 0xe0000000);
#endif

#ifdef INADDR_ALLHOSTS_GROUP
    sock_define_const("INADDR_ALLHOSTS_GROUP", INADDR_ALLHOSTS_GROUP);
#else
    sock_define_const("INADDR_ALLHOSTS_GROUP", 0xe0000001);
#endif

#ifdef INADDR_MAX_LOCAL_GROUP
    sock_define_const("INADDR_MAX_LOCAL_GROUP", INADDR_MAX_LOCAL_GROUP);
#else
    sock_define_const("INADDR_MAX_LOCAL_GROUP", 0xe00000ff);
#endif

#ifdef INADDR_NONE
    sock_define_const("INADDR_NONE", INADDR_NONE);
#else
    sock_define_const("INADDR_NONE", 0xffffffff);
#endif

#ifdef IP_OPTIONS
    sock_define_const("IP_OPTIONS", IP_OPTIONS);
#endif

#ifdef IP_HDRINCL
    sock_define_const("IP_HDRINCL", IP_HDRINCL);
#endif

#ifdef IP_TOS
    sock_define_const("IP_TOS", IP_TOS);
#endif

#ifdef IP_TTL
    sock_define_const("IP_TTL", IP_TTL);
#endif

#ifdef IP_RECVOPTS
    sock_define_const("IP_RECVOPTS", IP_RECVOPTS);
#endif

#ifdef IP_RECVRETOPTS
    sock_define_const("IP_RECVRETOPTS", IP_RECVRETOPTS);
#endif

#ifdef IP_RECVDSTADDR
    sock_define_const("IP_RECVDSTADDR", IP_RECVDSTADDR);
#endif

#ifdef IP_RETOPTS
    sock_define_const("IP_RETOPTS", IP_RETOPTS);
#endif

#ifdef IP_MULTICAST_IF
    sock_define_const("IP_MULTICAST_IF", IP_MULTICAST_IF);
#endif

#ifdef IP_MULTICAST_TTL
    sock_define_const("IP_MULTICAST_TTL", IP_MULTICAST_TTL);
#endif

#ifdef IP_MULTICAST_LOOP
    sock_define_const("IP_MULTICAST_LOOP", IP_MULTICAST_LOOP);
#endif

#ifdef IP_ADD_MEMBERSHIP
    sock_define_const("IP_ADD_MEMBERSHIP", IP_ADD_MEMBERSHIP);
#endif

#ifdef IP_DROP_MEMBERSHIP
    sock_define_const("IP_DROP_MEMBERSHIP", IP_DROP_MEMBERSHIP);
#endif

#ifdef IP_DEFAULT_MULTICAST_TTL
    sock_define_const("IP_DEFAULT_MULTICAST_TTL", IP_DEFAULT_MULTICAST_TTL);
#endif

#ifdef IP_DEFAULT_MULTICAST_LOOP
    sock_define_const("IP_DEFAULT_MULTICAST_LOOP", IP_DEFAULT_MULTICAST_LOOP);
#endif

#ifdef IP_MAX_MEMBERSHIPS
    sock_define_const("IP_MAX_MEMBERSHIPS", IP_MAX_MEMBERSHIPS);
#endif

#ifdef SO_DEBUG
    sock_define_const("SO_DEBUG", SO_DEBUG);
#endif

#ifdef SO_REUSEADDR
    sock_define_const("SO_REUSEADDR", SO_REUSEADDR);
#endif

#ifdef SO_REUSEPORT
    sock_define_const("SO_REUSEPORT", SO_REUSEPORT);
#endif

#ifdef SO_TYPE
    sock_define_const("SO_TYPE", SO_TYPE);
#endif

#ifdef SO_ERROR
    sock_define_const("SO_ERROR", SO_ERROR);
#endif

#ifdef SO_DONTROUTE
    sock_define_const("SO_DONTROUTE", SO_DONTROUTE);
#endif

#ifdef SO_BROADCAST
    sock_define_const("SO_BROADCAST", SO_BROADCAST);
#endif

#ifdef SO_SNDBUF
    sock_define_const("SO_SNDBUF", SO_SNDBUF);
#endif

#ifdef SO_RCVBUF
    sock_define_const("SO_RCVBUF", SO_RCVBUF);
#endif

#ifdef SO_KEEPALIVE
    sock_define_const("SO_KEEPALIVE", SO_KEEPALIVE);
#endif

#ifdef SO_OOBINLINE
    sock_define_const("SO_OOBINLINE", SO_OOBINLINE);
#endif

#ifdef SO_NO_CHECK
    sock_define_const("SO_NO_CHECK", SO_NO_CHECK);
#endif

#ifdef SO_PRIORITY
    sock_define_const("SO_PRIORITY", SO_PRIORITY);
#endif

#ifdef SO_LINGER
    sock_define_const("SO_LINGER", SO_LINGER);
#endif

#ifdef SO_PASSCRED
    sock_define_const("SO_PASSCRED", SO_PASSCRED);
#endif

#ifdef SO_PEERCRED
    sock_define_const("SO_PEERCRED", SO_PEERCRED);
#endif

#ifdef SO_RCVLOWAT
    sock_define_const("SO_RCVLOWAT", SO_RCVLOWAT);
#endif

#ifdef SO_SNDLOWAT
    sock_define_const("SO_SNDLOWAT", SO_SNDLOWAT);
#endif

#ifdef SO_RCVTIMEO
    sock_define_const("SO_RCVTIMEO", SO_RCVTIMEO);
#endif

#ifdef SO_SNDTIMEO
    sock_define_const("SO_SNDTIMEO", SO_SNDTIMEO);
#endif

#ifdef SO_ACCEPTCONN
    sock_define_const("SO_ACCEPTCONN", SO_ACCEPTCONN);
#endif

#ifdef SO_USELOOPBACK
    sock_define_const("SO_USELOOPBACK", SO_USELOOPBACK);
#endif

#ifdef SO_ACCEPTFILTER
    sock_define_const("SO_ACCEPTFILTER", SO_ACCEPTFILTER);
#endif

#ifdef SO_DONTTRUNC
    sock_define_const("SO_DONTTRUNC", SO_DONTTRUNC);
#endif

#ifdef SO_WANTMORE
    sock_define_const("SO_WANTMORE", SO_WANTMORE);
#endif

#ifdef SO_WANTOOBFLAG
    sock_define_const("SO_WANTOOBFLAG", SO_WANTOOBFLAG);
#endif

#ifdef SO_NREAD
    sock_define_const("SO_NREAD", SO_NREAD);
#endif

#ifdef SO_NKE
    sock_define_const("SO_NKE", SO_NKE);
#endif

#ifdef SO_NOSIGPIPE
    sock_define_const("SO_NOSIGPIPE", SO_NOSIGPIPE);
#endif

#ifdef SO_SECURITY_AUTHENTICATION
    sock_define_const("SO_SECURITY_AUTHENTICATION", SO_SECURITY_AUTHENTICATION);
#endif

#ifdef SO_SECURITY_ENCRYPTION_TRANSPORT
    sock_define_const("SO_SECURITY_ENCRYPTION_TRANSPORT", SO_SECURITY_ENCRYPTION_TRANSPORT);
#endif

#ifdef SO_SECURITY_ENCRYPTION_NETWORK
    sock_define_const("SO_SECURITY_ENCRYPTION_NETWORK", SO_SECURITY_ENCRYPTION_NETWORK);
#endif

#ifdef SO_BINDTODEVICE
    sock_define_const("SO_BINDTODEVICE", SO_BINDTODEVICE);
#endif

#ifdef SO_ATTACH_FILTER
    sock_define_const("SO_ATTACH_FILTER", SO_ATTACH_FILTER);
#endif

#ifdef SO_DETACH_FILTER
    sock_define_const("SO_DETACH_FILTER", SO_DETACH_FILTER);
#endif

#ifdef SO_PEERNAME
    sock_define_const("SO_PEERNAME", SO_PEERNAME);
#endif

#ifdef SO_TIMESTAMP
    sock_define_const("SO_TIMESTAMP", SO_TIMESTAMP);
#endif

#ifdef SOPRI_INTERACTIVE
    sock_define_const("SOPRI_INTERACTIVE", SOPRI_INTERACTIVE);
#endif

#ifdef SOPRI_NORMAL
    sock_define_const("SOPRI_NORMAL", SOPRI_NORMAL);
#endif

#ifdef SOPRI_BACKGROUND
    sock_define_const("SOPRI_BACKGROUND", SOPRI_BACKGROUND);
#endif

#ifdef IPX_TYPE
    sock_define_const("IPX_TYPE", IPX_TYPE);
#endif

#ifdef TCP_NODELAY
    sock_define_const("TCP_NODELAY", TCP_NODELAY);
#endif

#ifdef TCP_MAXSEG
    sock_define_const("TCP_MAXSEG", TCP_MAXSEG);
#endif

#ifdef EAI_ADDRFAMILY
    sock_define_const("EAI_ADDRFAMILY", EAI_ADDRFAMILY);
#endif

#ifdef EAI_AGAIN
    sock_define_const("EAI_AGAIN", EAI_AGAIN);
#endif

#ifdef EAI_BADFLAGS
    sock_define_const("EAI_BADFLAGS", EAI_BADFLAGS);
#endif

#ifdef EAI_FAIL
    sock_define_const("EAI_FAIL", EAI_FAIL);
#endif

#ifdef EAI_FAMILY
    sock_define_const("EAI_FAMILY", EAI_FAMILY);
#endif

#ifdef EAI_MEMORY
    sock_define_const("EAI_MEMORY", EAI_MEMORY);
#endif

#ifdef EAI_NODATA
    sock_define_const("EAI_NODATA", EAI_NODATA);
#endif

#ifdef EAI_NONAME
    sock_define_const("EAI_NONAME", EAI_NONAME);
#endif

#ifdef EAI_SERVICE
    sock_define_const("EAI_SERVICE", EAI_SERVICE);
#endif

#ifdef EAI_SOCKTYPE
    sock_define_const("EAI_SOCKTYPE", EAI_SOCKTYPE);
#endif

#ifdef EAI_SYSTEM
    sock_define_const("EAI_SYSTEM", EAI_SYSTEM);
#endif

#ifdef EAI_BADHINTS
    sock_define_const("EAI_BADHINTS", EAI_BADHINTS);
#endif

#ifdef EAI_PROTOCOL
    sock_define_const("EAI_PROTOCOL", EAI_PROTOCOL);
#endif

#ifdef EAI_MAX
    sock_define_const("EAI_MAX", EAI_MAX);
#endif

#ifdef AI_PASSIVE
    sock_define_const("AI_PASSIVE", AI_PASSIVE);
#endif

#ifdef AI_CANONNAME
    sock_define_const("AI_CANONNAME", AI_CANONNAME);
#endif

#ifdef AI_NUMERICHOST
    sock_define_const("AI_NUMERICHOST", AI_NUMERICHOST);
#endif

#ifdef AI_MASK
    sock_define_const("AI_MASK", AI_MASK);
#endif

#ifdef AI_ALL
    sock_define_const("AI_ALL", AI_ALL);
#endif

#ifdef AI_V4MAPPED_CFG
    sock_define_const("AI_V4MAPPED_CFG", AI_V4MAPPED_CFG);
#endif

#ifdef AI_ADDRCONFIG
    sock_define_const("AI_ADDRCONFIG", AI_ADDRCONFIG);
#endif

#ifdef AI_V4MAPPED
    sock_define_const("AI_V4MAPPED", AI_V4MAPPED);
#endif

#ifdef AI_DEFAULT
    sock_define_const("AI_DEFAULT", AI_DEFAULT);
#endif

#ifdef NI_MAXHOST
    sock_define_const("NI_MAXHOST", NI_MAXHOST);
#endif

#ifdef NI_MAXSERV
    sock_define_const("NI_MAXSERV", NI_MAXSERV);
#endif

#ifdef NI_NOFQDN
    sock_define_const("NI_NOFQDN", NI_NOFQDN);
#endif

#ifdef NI_NUMERICHOST
    sock_define_const("NI_NUMERICHOST", NI_NUMERICHOST);
#endif

#ifdef NI_NAMEREQD
    sock_define_const("NI_NAMEREQD", NI_NAMEREQD);
#endif

#ifdef NI_NUMERICSERV
    sock_define_const("NI_NUMERICSERV", NI_NUMERICSERV);
#endif

#ifdef NI_DGRAM
    sock_define_const("NI_DGRAM", NI_DGRAM);
#endif

#ifdef SHUT_RD
    sock_define_const("SHUT_RD", SHUT_RD);
#else
    sock_define_const("SHUT_RD", 0);
#endif

#ifdef SHUT_WR
    sock_define_const("SHUT_WR", SHUT_WR);
#else
    sock_define_const("SHUT_WR", 1);
#endif

#ifdef SHUT_RDWR
    sock_define_const("SHUT_RDWR", SHUT_RDWR);
#else
    sock_define_const("SHUT_RDWR", 2);
#endif

#ifdef IPV6_JOIN_GROUP
    sock_define_const("IPV6_JOIN_GROUP", IPV6_JOIN_GROUP);
#endif

#ifdef IPV6_LEAVE_GROUP
    sock_define_const("IPV6_LEAVE_GROUP", IPV6_LEAVE_GROUP);
#endif

#ifdef IPV6_MULTICAST_HOPS
    sock_define_const("IPV6_MULTICAST_HOPS", IPV6_MULTICAST_HOPS);
#endif

#ifdef IPV6_MULTICAST_IF
    sock_define_const("IPV6_MULTICAST_IF", IPV6_MULTICAST_IF);
#endif

#ifdef IPV6_MULTICAST_LOOP
    sock_define_const("IPV6_MULTICAST_LOOP", IPV6_MULTICAST_LOOP);
#endif

#ifdef IPV6_UNICAST_HOPS
    sock_define_const("IPV6_UNICAST_HOPS", IPV6_UNICAST_HOPS);
#endif

#ifdef IPV6_V6ONLY
    sock_define_const("IPV6_V6ONLY", IPV6_V6ONLY);
#endif

#ifdef IPV6_CHECKSUM
    sock_define_const("IPV6_CHECKSUM", IPV6_CHECKSUM);
#endif

#ifdef IPV6_DONTFRAG
    sock_define_const("IPV6_DONTFRAG", IPV6_DONTFRAG);
#endif

#ifdef IPV6_DSTOPTS
    sock_define_const("IPV6_DSTOPTS", IPV6_DSTOPTS);
#endif

#ifdef IPV6_HOPLIMIT
    sock_define_const("IPV6_HOPLIMIT", IPV6_HOPLIMIT);
#endif

#ifdef IPV6_HOPOPTS
    sock_define_const("IPV6_HOPOPTS", IPV6_HOPOPTS);
#endif

#ifdef IPV6_NEXTHOP
    sock_define_const("IPV6_NEXTHOP", IPV6_NEXTHOP);
#endif

#ifdef IPV6_PATHMTU
    sock_define_const("IPV6_PATHMTU", IPV6_PATHMTU);
#endif

#ifdef IPV6_PKTINFO
    sock_define_const("IPV6_PKTINFO", IPV6_PKTINFO);
#endif

#ifdef IPV6_RECVDSTOPTS
    sock_define_const("IPV6_RECVDSTOPTS", IPV6_RECVDSTOPTS);
#endif

#ifdef IPV6_RECVHOPLIMIT
    sock_define_const("IPV6_RECVHOPLIMIT", IPV6_RECVHOPLIMIT);
#endif

#ifdef IPV6_RECVHOPOPTS
    sock_define_const("IPV6_RECVHOPOPTS", IPV6_RECVHOPOPTS);
#endif

#ifdef IPV6_RECVPKTINFO
    sock_define_const("IPV6_RECVPKTINFO", IPV6_RECVPKTINFO);
#endif

#ifdef IPV6_RECVRTHDR
    sock_define_const("IPV6_RECVRTHDR", IPV6_RECVRTHDR);
#endif

#ifdef IPV6_RECVTCLASS
    sock_define_const("IPV6_RECVTCLASS", IPV6_RECVTCLASS);
#endif

#ifdef IPV6_RTHDR
    sock_define_const("IPV6_RTHDR", IPV6_RTHDR);
#endif

#ifdef IPV6_RTHDRDSTOPTS
    sock_define_const("IPV6_RTHDRDSTOPTS", IPV6_RTHDRDSTOPTS);
#endif

#ifdef IPV6_RTHDR_TYPE_0
    sock_define_const("IPV6_RTHDR_TYPE_0", IPV6_RTHDR_TYPE_0);
#endif

#ifdef IPV6_RECVPATHMTU
    sock_define_const("IPV6_RECVPATHMTU", IPV6_RECVPATHMTU);
#endif

#ifdef IPV6_TCLASS
    sock_define_const("IPV6_TCLASS", IPV6_TCLASS);
#endif

#ifdef IPV6_USE_MIN_MTU
    sock_define_const("IPV6_USE_MIN_MTU", IPV6_USE_MIN_MTU);
#endif

