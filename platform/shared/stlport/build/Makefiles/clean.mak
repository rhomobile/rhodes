# -*- Makefile -*- Time-stamp: <03/07/15 12:19:04 ptr>
# $Id: clean.mak 1914 2005-12-04 20:25:55Z dums $

PHONY += clean clobber distclean

ifneq (${OSNAME}, windows)
clean::	
	@-rm -f core core.*
	@-rm -f $(OBJ) $(DEP)
	@-rm -f $(OBJ_DBG) $(DEP_DBG)
	@-rm -f $(OBJ_STLDBG) $(DEP_STLDBG)
	@-rm -f $(OBJ_A) $(DEP_A)
	@-rm -f $(OBJ_A_DBG) $(DEP_A_DBG)
	@-rm -f $(OBJ_A_STLDBG) $(DEP_A_STLDBG)

clobber::	clean
	@-rm -f $(DEPENDS_COLLECTION)
else
clean::	
	@if exist core del /f /q core
	@if exist core.* del /f /q core.*
	@if exist $(subst /,\,$(OUTPUT_DIR)/*.$(OBJ_EXT)) del /f /q $(subst /,\,$(OUTPUT_DIR)/*.$(OBJ_EXT))
	@if exist $(subst /,\,$(OUTPUT_DIR_DBG)/*.$(OBJ_EXT)) del /f /q $(subst /,\,$(OUTPUT_DIR_DBG)/*.$(OBJ_EXT))
	@if exist $(subst /,\,$(OUTPUT_DIR_STLDBG)/*.$(OBJ_EXT)) del /f /q $(subst /,\,$(OUTPUT_DIR_STLDBG)/*.$(OBJ_EXT))
	@if exist $(subst /,\,$(OUTPUT_DIR_A)/*.$(OBJ_EXT)) del /f /q $(subst /,\,$(OUTPUT_DIR_A)/*.$(OBJ_EXT))
	@if exist $(subst /,\,$(OUTPUT_DIR_A_DBG)/*.$(OBJ_EXT)) del /f /q $(subst /,\,$(OUTPUT_DIR_A_DBG)/*.$(OBJ_EXT))
	@if exist $(subst /,\,$(OUTPUT_DIR_A_STLDBG)/*.$(OBJ_EXT)) del /f /q $(subst /,\,$(OUTPUT_DIR_A_STLDBG)/*.$(OBJ_EXT))

clobber::	clean
	@if exist $(subst /,\,$(PRE_OUTPUT_DIR)) rd /s /q $(subst /,\,$(PRE_OUTPUT_DIR))
endif

distclean::	clobber
