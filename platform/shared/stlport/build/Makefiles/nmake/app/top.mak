# -*- makefile -*- Time-stamp: <03/09/28 18:46:10 ptr>
# $Id: top.mak 1459 2005-04-18 21:25:32Z ptr $

!ifndef LDFLAGS
LDFLAGS = 
!endif

!include $(RULESBASE)/$(USE_MAKE)/app/macro.mak
!include $(RULESBASE)/$(USE_MAKE)/app/$(COMPILER_NAME).mak
!include $(RULESBASE)/$(USE_MAKE)/app/rules-exe-$(OSNAME).mak

!include $(RULESBASE)/$(USE_MAKE)/app/rules-install-so.mak
!include $(RULESBASE)/$(USE_MAKE)/app/rules-install-a.mak

!include $(RULESBASE)/$(USE_MAKE)/app/clean.mak
