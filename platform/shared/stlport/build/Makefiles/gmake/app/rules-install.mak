# -*- makefile -*- Time-stamp: <05/12/07 01:03:13 ptr>

install:	install-shared

INSTALL_PRGNAME := ${PRGNAME}${EXE}

ifneq ($(OSNAME),windows)
install-release-shared: release-shared $(INSTALL_BIN_DIR)
	$(INSTALL_EXE) ${PRG} $(INSTALL_BIN_DIR)/${INSTALL_PRGNAME}

install-dbg-shared: dbg-shared $(INSTALL_BIN_DIR_DBG)
	$(INSTALL_EXE) ${PRG_DBG} $(INSTALL_BIN_DIR_DBG)/${INSTALL_PRGNAME}

install-stldbg-shared: stldbg-shared $(INSTALL_BIN_DIR_STLDBG)
	$(INSTALL_EXE) ${PRG_STLDBG} $(INSTALL_BIN_DIR_STLDBG)/${INSTALL_PRGNAME}

install-release-static: release-static $(INSTALL_BIN_DIR)
	$(INSTALL_EXE) ${PRG} $(INSTALL_BIN_DIR)/${INSTALL_PRGNAME}

install-dbg-static: dbg-static $(INSTALL_BIN_DIR_DBG)
	$(INSTALL_EXE) $(PRG_DBG) $(INSTALL_BIN_DIR_DBG)/$(INSTALL_PRGNAME)

install-stldbg-static: stldbg-static $(INSTALL_BIN_DIR_STLDBG)
	$(INSTALL_EXE) ${PRG_STLDBG} $(INSTALL_BIN_DIR_STLDBG)/${INSTALL_PRGNAME}
else
install-release-shared: release-shared $(INSTALL_BIN_DIR)
	$(INSTALL_EXE) $(PRG) $(subst /,\,$(INSTALL_BIN_DIR)/$(INSTALL_PRGNAME))

install-dbg-shared: dbg-shared $(INSTALL_BIN_DIR_DBG)
	$(INSTALL_EXE) $(PRG_DBG) $(subst /,\,$(INSTALL_BIN_DIR_DBG)/$(INSTALL_PRGNAME))

install-stldbg-shared: stldbg-shared $(INSTALL_BIN_DIR_STLDBG)
	$(INSTALL_EXE) $(PRG_STLDBG) $(subst /,\,$(INSTALL_BIN_DIR_STLDBG)/$(INSTALL_PRGNAME))

install-release-static: release-static $(INSTALL_BIN_DIR)
	$(INSTALL_EXE) $(PRG) $(subst /,\,$(INSTALL_BIN_DIR)/$(INSTALL_PRGNAME))

install-dbg-static: dbg-static $(INSTALL_BIN_DIR_DBG)
	$(INSTALL_EXE) $(PRG_DBG) $(subst /,\,$(INSTALL_BIN_DIR_DBG)/$(INSTALL_PRGNAME))

install-stldbg-static: stldbg-static $(INSTALL_BIN_DIR_STLDBG)
	$(INSTALL_EXE) $(PRG_STLDBG) $(subst /,\,$(INSTALL_BIN_DIR_STLDBG)/$(INSTALL_PRGNAME))
endif
