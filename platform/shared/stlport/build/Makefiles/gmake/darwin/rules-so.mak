# -*- makefile -*- Time-stamp: <05/12/27 10:51:51 ptr>

# Shared libraries tags

PHONY += release-shared dbg-shared stldbg-shared

release-shared:	$(OUTPUT_DIR) ${SO_NAME_OUTxxx}

dbg-shared:	$(OUTPUT_DIR_DBG) ${SO_NAME_OUT_DBGxxx}

stldbg-shared:	$(OUTPUT_DIR_STLDBG) ${SO_NAME_OUT_STLDBGxxx}

ifeq ("${_C_SOURCES_ONLY}","")

${SO_NAME_OUTxxx}:	$(OBJ) $(LIBSDEP)
	$(LINK.cc) $(LINK_OUTPUT_OPTION) ${START_OBJ} $(OBJ) $(LDLIBS) ${STDLIBS} ${END_OBJ}
	(cd $(OUTPUT_DIR); \
	 rm -f ${SO_NAMExx}; ln -s $(SO_NAMExxx) ${SO_NAMExx}; \
	 rm -f ${SO_NAMEx}; ln -s ${SO_NAMExx} ${SO_NAMEx}; \
	 rm -f ${SO_NAME}; ln -s ${SO_NAMEx} ${SO_NAME})

${SO_NAME_OUT_DBGxxx}:	$(OBJ_DBG) $(LIBSDEP)
	$(LINK.cc) $(LINK_OUTPUT_OPTION) ${START_OBJ} $(OBJ_DBG) $(LDLIBS) ${STDLIBS} ${END_OBJ}
	(cd $(OUTPUT_DIR_DBG); \
	 rm -f ${SO_NAME_DBGxx}; ln -s $(SO_NAME_DBGxxx) ${SO_NAME_DBGxx}; \
	 rm -f ${SO_NAME_DBGx}; ln -s ${SO_NAME_DBGxx} ${SO_NAME_DBGx}; \
	 rm -f ${SO_NAME_DBG}; ln -s ${SO_NAME_DBGx} ${SO_NAME_DBG})

${SO_NAME_OUT_STLDBGxxx}:	$(OBJ_STLDBG) $(LIBSDEP)
	$(LINK.cc) $(LINK_OUTPUT_OPTION) ${START_OBJ} $(OBJ_STLDBG) $(LDLIBS) ${STDLIBS} ${END_OBJ}
	(cd $(OUTPUT_DIR_STLDBG); \
	 rm -f ${SO_NAME_STLDBGxx}; ln -s $(SO_NAME_STLDBGxxx) ${SO_NAME_STLDBGxx}; \
	 rm -f ${SO_NAME_STLDBGx}; ln -s ${SO_NAME_STLDBGxx} ${SO_NAME_STLDBGx}; \
	 rm -f ${SO_NAME_STLDBG}; ln -s ${SO_NAME_STLDBGx} ${SO_NAME_STLDBG})
else

${SO_NAME_OUTxxx}:      $(OBJ) $(LIBSDEP)
	$(LINK.c) $(LINK_OUTPUT_OPTION) $(OBJ) $(LDLIBS)
	(cd $(OUTPUT_DIR); \
	 rm -f ${SO_NAMExx}; ln -s $(SO_NAMExxx) ${SO_NAMExx}; \
	 rm -f ${SO_NAMEx}; ln -s ${SO_NAMExx} ${SO_NAMEx}; \
	 rm -f ${SO_NAME}; ln -s ${SO_NAMEx} ${SO_NAME})

${SO_NAME_OUT_DBGxxx}:  $(OBJ_DBG) $(LIBSDEP)
	$(LINK.c) $(LINK_OUTPUT_OPTION) $(OBJ_DBG) $(LDLIBS)
	(cd $(OUTPUT_DIR_DBG); \
	 rm -f ${SO_NAME_DBGxx}; ln -s $(SO_NAME_DBGxxx) ${SO_NAME_DBGxx}; \
	 rm -f ${SO_NAME_DBGx}; ln -s ${SO_NAME_DBGxx} ${SO_NAME_DBGx}; \
	 rm -f ${SO_NAME_DBG}; ln -s ${SO_NAME_DBGx} ${SO_NAME_DBG})

${SO_NAME_OUT_STLDBGxxx}:       $(OBJ_STLDBG) $(LIBSDEP)
	$(LINK.c) $(LINK_OUTPUT_OPTION) $(OBJ_STLDBG) $(LDLIBS)
	(cd $(OUTPUT_DIR_STLDBG); \
	 rm -f ${SO_NAME_STLDBGxx}; ln -s $(SO_NAME_STLDBGxxx) ${SO_NAME_STLDBGxx}; \
	 rm -f ${SO_NAME_STLDBGx}; ln -s ${SO_NAME_STLDBGxx} ${SO_NAME_STLDBGx}; \
	 rm -f ${SO_NAME_STLDBG}; ln -s ${SO_NAME_STLDBGx} ${SO_NAME_STLDBG})

endif

