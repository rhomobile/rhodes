# -*- makefile -*- Time-stamp: <04/08/23 23:07:35 ptr>

OPT += -xcode=pic32

dbg-shared:	LDFLAGS += -G -Qoption ld -z,initfirst -h$(SO_NAME_DBGxx) ${LDSEARCH} ${NOSTDLIB}
stldbg-shared:	LDFLAGS += -G -Qoption ld -z,initfirst -h$(SO_NAME_STLDBGxx) ${LDSEARCH} ${NOSTDLIB}
release-shared:	LDFLAGS += -G -Qoption ld -z,initfirst -h$(SO_NAMExx) ${LDSEARCH} ${NOSTDLIB}
dbg-static:	LDFLAGS += ${LDSEARCH}
stldbg-static:	LDFLAGS += ${LDSEARCH}
release-static:	LDFLAGS += ${LDSEARCH}

DEPENDS_COLLECTION_SUNPRO := $(DEPENDS_COLLECTION).sunpro

compiler-dep :: $(DEPENDS_COLLECTION_SUNPRO)

STD_HEADER_LISTS = $(STLPORT_ETC_DIR)/std_headers.txt \
                   $(STLPORT_ETC_DIR)/std_headers_cpp_runtime_h.txt \
                   $(STLPORT_ETC_DIR)/std_headers_c.txt \
                   $(STLPORT_ETC_DIR)/std_headers_c_h.txt \
                   $(STLPORT_ETC_DIR)/std_headers_classic_iostreams.txt \
                   $(STLPORT_ETC_DIR)/std_headers_cpp_runtime.txt

$(DEPENDS_COLLECTION_SUNPRO) : $(STD_HEADER_LISTS)
	@echo "Linking header files required for SunPro compiler"
	@for file in `cat $(STD_HEADER_LISTS)`; do \
	  echo "." | awk '{printf("%s", $$0)}' ; \
	  (cd $(STLPORT_INCLUDE_DIR) ; rm -f $$file.SUNWCCh; ln -s ./$$file $$file.SUNWCCh) ; \
        done; echo ""
	@cat $(STD_HEADER_LISTS) | awk '{print "$(STLPORT_INCLUDE_DIR)/" $$0 ".SUNWCCh : $(STLPORT_INCLUDE_DIR)/" $$0 }'  > $@
