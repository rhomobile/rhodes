# Time-stamp: <05/03/02 18:57:44 ptr>
# $Id: depend.mak 1848 2005-11-18 10:30:37Z complement $

PHONY += release-static-dep release-shared-dep dbg-static-dep dbg-shared-dep \
         stldbg-static-dep stldbg-shared-dep depend compiler-dep

release-static-dep release-shared-dep:	$(DEP)

dbg-static-dep dbg-shared-dep:	$(DEP_DBG)

stldbg-static-dep stldbg-shared-dep:	$(DEP_STLDBG)

depend:	$(OUTPUT_DIRS) $(INSTALL_LIB_DIRS) compiler-dep release-shared-dep dbg-shared-dep stldbg-shared-dep
	@cat -s $(DEP) $(DEP_DBG) $(DEP_STLDBG) /dev/null > $(DEPENDS_COLLECTION)

# This target is called if the compiler needs to do some setup before
# creating the dependency file. Currently this is used by the SunPro
# compiler to setup the include files correctly.
compiler-dep ::

-include $(DEPENDS_COLLECTION)
