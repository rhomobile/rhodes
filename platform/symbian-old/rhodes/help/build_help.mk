# ============================================================================
#  Name	 : build_help.mk
#  Part of  : rhodes
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : rhodes
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : rhodes.hlp
rhodes.hlp : rhodes.xml rhodes.cshlp Custom.xml
	cshlpcmp rhodes.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy rhodes.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del rhodes.hlp
	del rhodes.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo rhodes.hlp

FINAL : do_nothing
