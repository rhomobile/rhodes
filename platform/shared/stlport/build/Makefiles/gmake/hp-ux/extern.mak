# Time-stamp: <03/07/15 17:46:28 ptr>
# $Id: extern.mak 1802 2005-11-01 08:25:57Z complement $


# This file reflect versions of third-party libraries that
# used in projects

# STLport library
STLPORT_LIB_DIR ?= $(STLPORT_DIR)/${TARGET_NAME}lib
STLPORT_INCLUDE_DIR ?= $(STLPORT_DIR)/stlport
STLPORT_VER ?= 4.5.5

# boost (http://www.boost.org, http://boost.sourceforge.net)
BOOST_INCLUDE_DIR ?= ${BOOST_DIR}

BOOST_DIR ?= ${SRCROOT}/../extern/boost
