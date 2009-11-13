# -*- makefile -*- Time-stamp: <04/07/25 16:54:59 ptr>
# $Id: rules-a.mak 2632 2006-10-05 19:34:16Z dums $

# Static libraries tags

PHONY += release-static dbg-static stldbg-static

release-static: $(OUTPUT_DIR_A) ${A_NAME_OUT}

dbg-static:	$(OUTPUT_DIR_A_DBG) ${A_NAME_OUT_DBG}

stldbg-static:	$(OUTPUT_DIR_A_STLDBG) ${A_NAME_OUT_STLDBG}


ifeq (bcc, $(COMPILER_NAME))
# Borland archive builder
# We always remove the lib before generating it to avoid warnings from
# archive builder.
${A_NAME_OUT}:	$(OBJ_A)
ifneq (windows, $(OSNAME))
	@rm -f ${A_NAME_OUT}
else
	@if exist $(subst /,\,$(A_NAME_OUT)) del /f /q $(subst /,\,$(A_NAME_OUT))
endif
	$(AR) $(AR_OUT) /P64 $(addprefix $(AR_INS_R),$(subst /,\,$(OBJ_A)))
$(A_NAME_OUT_DBG):	$(OBJ_A_DBG)
ifneq (windows, $(OSNAME))
	@rm -f $(A_NAME_OUT_DBG)
else
	@if exist $(subst /,\,$(A_NAME_OUT_DBG)) del /f /q $(subst /,\,$(A_NAME_OUT_DBG))
endif
	$(AR) $(AR_OUT) /P128 $(addprefix $(AR_INS_R),$(subst /,\,$(OBJ_A_DBG)))
$(A_NAME_OUT_STLDBG):	$(OBJ_A_STLDBG)
ifneq (windows, $(OSNAME))
	@rm -f $(A_NAME_OUT_STLDBG)
else
	@if exist $(subst /,\,$(A_NAME_OUT_STLDBG)) del /f /q $(subst /,\,$(A_NAME_OUT_STLDBG))
endif
	$(AR) $(AR_OUT) /P256 $(addprefix $(AR_INS_R),$(subst /,\,$(OBJ_A_STLDBG)))
else
ifeq (dmc, $(COMPILER_NAME))
# Digital Mars archive builder
${A_NAME_OUT}:	$(OBJ_A)
	$(AR) $(AR_INS_R) $(AR_OUT) $(subst /,\,$(OBJ_A))
${A_NAME_OUT_DBG}:	$(OBJ_A_DBG)
	$(AR) $(AR_INS_R) $(AR_OUT) $(subst /,\,$(OBJ_A_DBG))
${A_NAME_OUT_STLDBG}:	$(OBJ_A_STLDBG)
	$(AR) $(AR_INS_R) $(AR_OUT) $(subst /,\,$(OBJ_A_STLDBG))
else
# GNU archive builder
${A_NAME_OUT}:	$(OBJ_A)
	$(AR) $(AR_INS_R) $(AR_OUT) $(OBJ_A)
${A_NAME_OUT_DBG}:	$(OBJ_A_DBG)
	$(AR) $(AR_INS_R) $(AR_OUT) $(OBJ_A_DBG)
${A_NAME_OUT_STLDBG}:	$(OBJ_A_STLDBG)
	$(AR) $(AR_INS_R) $(AR_OUT) $(OBJ_A_STLDBG)
endif
endif
