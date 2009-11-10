# -*- Makefile -*- Time-stamp: <03/10/27 17:20:22 ptr>
# $Id: rules-d.mak 1459 2005-04-18 21:25:32Z ptr $

# Rules for release output:

$(OUTPUT_DIR)/%.d:	$(WORD1)%.cpp
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR)

$(OUTPUT_DIR)/%.d:	$(WORD1)%.cc
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR)

$(OUTPUT_DIR)/%.d:	$(WORD1)%.c
	@$(COMPILE.c) $(CDEPFLAGS) $< $(DP_OUTPUT_DIR)

ifneq ($(OUTPUT_DIR),$(OUTPUT_DIR_A))

$(OUTPUT_DIR_A)/%.d:	$(WORD1)%.cpp
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR)

$(OUTPUT_DIR_A)/%.d:	$(WORD1)%.cc
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR)

$(OUTPUT_DIR_A)/%.d:	$(WORD1)%.c
	@$(COMPILE.c) $(CDEPFLAGS) $< $(DP_OUTPUT_DIR)

endif

# Rules for debug output:

$(OUTPUT_DIR_DBG)/%.d:	$(WORD1)%.cpp
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_DBG)

$(OUTPUT_DIR_DBG)/%.d:	$(WORD1)%.cc
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_DBG)

$(OUTPUT_DIR_DBG)/%.d:	$(WORD1)%.c
	@$(COMPILE.c) $(CDEPFLAGS) $< $(DP_OUTPUT_DIR_DBG)

ifneq ($(OUTPUT_DIR_DBG),$(OUTPUT_DIR_A_DBG))

$(OUTPUT_DIR_A_DBG)/%.d:	$(WORD1)%.cpp
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_DBG)

$(OUTPUT_DIR_A_DBG)/%.d:	$(WORD1)%.cc
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_DBG)

$(OUTPUT_DIR_A_DBG)/%.d:	$(WORD1)%.c
	@$(COMPILE.c) $(CDEPFLAGS) $< $(DP_OUTPUT_DIR_DBG)

endif

# Rules for STLport debug output:

$(OUTPUT_DIR_STLDBG)/%.d:	$(WORD1)%.cpp
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_STLDBG)

$(OUTPUT_DIR_STLDBG)/%.d:	$(WORD1)%.cc
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_STLDBG)

$(OUTPUT_DIR_STLDBG)/%.d:	$(WORD1)%.c
	@$(COMPILE.c) $(CDEPFLAGS) $< $(DP_OUTPUT_DIR_STLDBG)

ifneq ($(OUTPUT_DIR_STLDBG),$(OUTPUT_DIR_A_STLDBG))

$(OUTPUT_DIR_A_STLDBG)/%.d:	$(WORD1)%.cpp
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_STLDBG)

$(OUTPUT_DIR_A_STLDBG)/%.d:	$(WORD1)%.cc
	@$(COMPILE.cc) $(CCDEPFLAGS) $< $(DP_OUTPUT_DIR_STLDBG)

$(OUTPUT_DIR_A_STLDBG)/%.d:	$(WORD1)%.c
	@$(COMPILE.c) $(CDEPFLAGS) $< $(DP_OUTPUT_DIR_STLDBG)

endif
