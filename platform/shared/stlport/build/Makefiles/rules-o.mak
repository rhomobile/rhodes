# -*- Makefile -*- Time-stamp: <03/10/27 17:19:13 ptr>
# $Id: rules-o.mak 1871 2005-11-26 20:55:00Z dums $

# Rules for release output:

$(OUTPUT_DIR)/%.$(OBJ_EXT):	$(WORD1)%.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR)/%.$(OBJ_EXT):	$(WORD1)%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR)/%.$(OBJ_EXT):	$(WORD1)%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

ifneq ($(OUTPUT_DIR),$(OUTPUT_DIR_A))

$(OUTPUT_DIR_A)/%.$(OBJ_EXT):	$(WORD1)%.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_A)/%.$(OBJ_EXT):	$(WORD1)%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_A)/%.$(OBJ_EXT):	$(WORD1)%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

endif

# Rules for debug output:

$(OUTPUT_DIR_DBG)/%.$(OBJ_EXT):	$(WORD1)%.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_DBG)/%.$(OBJ_EXT):	$(WORD1)%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_DBG)/%.$(OBJ_EXT):	$(WORD1)%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

ifneq ($(OUTPUT_DIR_DBG),$(OUTPUT_DIR_A_DBG))

$(OUTPUT_DIR_A_DBG)/%.$(OBJ_EXT):	$(WORD1)%.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_A_DBG)/%.$(OBJ_EXT):	$(WORD1)%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_A_DBG)/%.$(OBJ_EXT):	$(WORD1)%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

endif

# Rules for STLport debug output:

$(OUTPUT_DIR_STLDBG)/%.$(OBJ_EXT):	$(WORD1)%.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_STLDBG)/%.$(OBJ_EXT):	$(WORD1)%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_STLDBG)/%.$(OBJ_EXT):	$(WORD1)%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

ifneq ($(OUTPUT_DIR_STLDBG),$(OUTPUT_DIR_A_STLDBG))

$(OUTPUT_DIR_A_STLDBG)/%.$(OBJ_EXT):	$(WORD1)%.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_A_STLDBG)/%.$(OBJ_EXT):	$(WORD1)%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(OUTPUT_DIR_A_STLDBG)/%.$(OBJ_EXT):	$(WORD1)%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

endif
