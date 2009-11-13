# -*- makefile -*- Time-stamp: <05/03/02 18:39:22 ptr>
# $Id: rules-install-so.mak 2057 2005-12-29 20:43:09Z dums $

INSTALL_TAGS ?= install-shared

PHONY += install $(INSTALL_TAGS)

install:	$(INSTALL_TAGS)

install-release-shared:	release-shared $(INSTALL_LIB_DIR)
	@if [ -h $(INSTALL_LIB_DIR)/${SO_NAME} ] ; then \
	  rm $(INSTALL_LIB_DIR)/${SO_NAME}; \
	fi
	@if [ -h $(INSTALL_LIB_DIR)/${SO_NAMEx} ] ; then \
	  rm $(INSTALL_LIB_DIR)/${SO_NAMEx}; \
	fi
	@if [ -h $(INSTALL_LIB_DIR)/${SO_NAMExx} ] ; then \
	  rm $(INSTALL_LIB_DIR)/${SO_NAMExx}; \
	fi
	@if [ -f $(INSTALL_LIB_DIR)/${SO_NAMExxx} ] ; then \
	  rm $(INSTALL_LIB_DIR)/${SO_NAMExxx}; \
	fi
	$(INSTALL_SO) ${SO_NAME_OUTxxx} $(INSTALL_LIB_DIR)
	(cd $(INSTALL_LIB_DIR) && \
	 { ln -s ${SO_NAMExxx} ${SO_NAMExx}; \
	   ln -s ${SO_NAMExx} ${SO_NAMEx}; \
	   ln -s ${SO_NAMEx} ${SO_NAME}; } )

install-dbg-shared:	dbg-shared $(INSTALL_LIB_DIR_DBG)
	@if [ -h $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBG} ] ; then \
	  rm $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBG}; \
	fi
	@if [ -h $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBGx} ] ; then \
	  rm $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBGx}; \
	fi
	@if [ -h $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBGxx} ] ; then \
	  rm $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBGxx}; \
	fi
	@if [ -f $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBGxxx} ] ; then \
	  rm $(INSTALL_LIB_DIR_DBG)/${SO_NAME_DBGxxx}; \
	fi
	$(INSTALL_SO) ${SO_NAME_OUT_DBGxxx} $(INSTALL_LIB_DIR_DBG)
	(cd $(INSTALL_LIB_DIR_DBG) && \
	 { ln -s ${SO_NAME_DBGxxx} ${SO_NAME_DBGxx}; \
	   ln -s ${SO_NAME_DBGxx} ${SO_NAME_DBGx}; \
	   ln -s ${SO_NAME_DBGx} ${SO_NAME_DBG}; } )

install-stldbg-shared:	stldbg-shared $(INSTALL_LIB_DIR_STLDBG)
	@if [ -h $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBG} ] ; then \
	  rm $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBG}; \
	fi
	@if [ -h $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBGx} ] ; then \
	  rm $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBGx}; \
	fi
	@if [ -h $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBGxx} ] ; then \
	  rm $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBGxx}; \
	fi
	@if [ -f $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBGxxx} ] ; then \
	  rm $(INSTALL_LIB_DIR_STLDBG)/${SO_NAME_STLDBGxxx}; \
	fi
	$(INSTALL_SO) ${SO_NAME_OUT_STLDBGxxx} $(INSTALL_LIB_DIR_STLDBG)
	(cd $(INSTALL_LIB_DIR_STLDBG) && \
	 { ln -s ${SO_NAME_STLDBGxxx} ${SO_NAME_STLDBGxx}; \
	   ln -s ${SO_NAME_STLDBGxx} ${SO_NAME_STLDBGx}; \
	   ln -s ${SO_NAME_STLDBGx} ${SO_NAME_STLDBG}; } )
