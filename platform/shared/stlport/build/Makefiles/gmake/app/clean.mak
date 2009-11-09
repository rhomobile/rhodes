# -*- makefile -*- Time-stamp: <05/12/07 00:54:14 ptr>

ifneq ($(OSNAME),windows)
clobber::
	@-rm -f ${PRG}
	@-rm -f ${PRG_DBG}
	@-rm -f ${PRG_STLDBG}

distclean::
	@-rm -f $(INSTALL_BIN_DIR)/$(PRG)
	@-rm -f $(INSTALL_BIN_DIR_DBG)/$(PRG_DBG)
	@-rm -f $(INSTALL_BIN_DIR_STLDBG)/$(PRG_STLDBG)
else
clobber::
	@if exist $(PRG) del /f /q $(PRG)
	@if exist $(PRG_DBG) del /f /q $(PRG_DBG)
	@if exist $(PRG_STLDBG) del /f /q $(PRG_STLDBG)

distclean::
	@if exist $(INSTALL_BIN_DIR)/$(PRGNAME)$(EXE) del /f /q $(subst /,\,$(INSTALL_BIN_DIR)/$(PRGNAME)$(EXE))
	@if exist $(INSTALL_BIN_DIR_DBG)/$(PRGNAME)$(EXE) del /f /q $(subst /,\,$(INSTALL_BIN_DIR_DBG)/$(PRGNAME)$(EXE))
	@if exist $(INSTALL_BIN_DIR_STLDBG)/$(PRGNAME)$(EXE) del /f /q $(subst /,\,$(INSTALL_BIN_DIR_STLDBG)/$(PRGNAME)$(EXE))
ifeq (bcc, $(COMPILER_NAME))
#remove STLport EXE directories, unless they contains DLLs
	@if exist $(INSTALL_BIN_DIR) if not exist $(subst /,\,$(INSTALL_BIN_DIR)/*.dll) rd /s /q $(subst /,\,$(INSTALL_BIN_DIR))
	@if exist $(INSTALL_BIN_DIR_DBG) if not exist $(subst /,\,$(INSTALL_BIN_DIR_DBG)/*.dll) rd /s /q $(subst /,\,$(INSTALL_BIN_DIR_DBG))
	@if exist $(INSTALL_BIN_DIR_STLDBG) if not exist $(subst /,\,$(INSTALL_BIN_DIR_STLDBG)/*.dll) rd /s /q $(subst /,\,$(INSTALL_BIN_DIR_STLDBG))
endif
endif
