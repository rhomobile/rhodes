# -*- makefile -*- Time-stamp: <05/12/07 01:51:13 ptr>

dbg-shared:	$(OUTPUT_DIR_DBG) ${PRG_DBG}
stldbg-shared:	$(OUTPUT_DIR_STLDBG) ${PRG_STLDBG}
release-shared:	$(OUTPUT_DIR) ${PRG}

dbg-static:	$(OUTPUT_DIR_DBG) ${PRG_DBG}
stldbg-static:	$(OUTPUT_DIR_STLDBG) ${PRG_STLDBG}
release-static:	$(OUTPUT_DIR) ${PRG}

ifeq ("${_C_SOURCES_ONLY}","")

ifeq (gcc, $(COMPILER_NAME))

${PRG}:	$(OBJ) $(LIBSDEP)
	$(LINK.cc) $(LINK_OUTPUT_OPTION) ${START_OBJ} $(OBJ) $(LDLIBS) ${STDLIBS} ${END_OBJ}

${PRG_DBG}:	$(OBJ_DBG) $(LIBSDEP)
	$(LINK.cc) $(LINK_OUTPUT_OPTION) ${START_OBJ} $(OBJ_DBG) $(LDLIBS) ${STDLIBS} ${END_OBJ}

${PRG_STLDBG}:	$(OBJ_STLDBG) $(LIBSDEP)
	$(LINK.cc) $(LINK_OUTPUT_OPTION) ${START_OBJ} $(OBJ_STLDBG) $(LDLIBS) ${STDLIBS} ${END_OBJ}

else

# Original Borland and Digital Mars linkers

${PRG}:	$(OBJ) $(LIBSDEP)
	$(LINK.cc) $(subst /,\,$(START_OBJ) $(OBJ) $(END_OBJ),$(LINK_OUTPUT_OPTION),$(MAP_OUTPUT_OPTION),$(LDLIBS) $(STDLIBS),,)

${PRG_DBG}:	$(OBJ_DBG) $(LIBSDEP)
	$(LINK.cc) $(subst /,\,$(START_OBJ) $(OBJ_DBG) $(END_OBJ),$(LINK_OUTPUT_OPTION),$(MAP_OUTPUT_OPTION),$(LDLIBS) $(STDLIBS),,)

${PRG_STLDBG}:	$(OBJ_STLDBG) $(LIBSDEP)
	$(LINK.cc) $(subst /,\,$(START_OBJ) $(OBJ_STLDBG) $(END_OBJ),$(LINK_OUTPUT_OPTION),$(MAP_OUTPUT_OPTION),$(LDLIBS) $(STDLIBS),,)

endif


else

# C sources only, so use LINK.c

${PRG}:	$(OBJ) $(LIBSDEP)
	$(LINK.c) $(LINK_OUTPUT_OPTION) $(OBJ) $(LDLIBS)

${PRG_DBG}:	$(OBJ_DBG) $(LIBSDEP)
	$(LINK.c) $(LINK_OUTPUT_OPTION) $(OBJ_DBG) $(LDLIBS)

${PRG_STLDBG}:	$(OBJ_STLDBG) $(LIBSDEP)
	$(LINK.c) $(LINK_OUTPUT_OPTION) $(OBJ_STLDBG) $(LDLIBS)

endif
