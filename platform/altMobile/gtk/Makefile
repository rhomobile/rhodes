CC ?= gcc
CXX ?= g++
APPNAME ?= RhoRunnerGtk
OUTDIR ?= ./bin
SRCDIR := src
OBJDIR := obj

# Optional: SDK root passed from rake (absolute path to repo root)
RHO_ROOT ?=
ENABLE_CORE ?=
EXTRA_LIBS ?=

# Detect WebKitGTK (prefer 4.1, fallback to 4.0), ensure GTK4 toolchain
WEBKIT_PKG := $(shell pkg-config --exists webkitgtk-6.0 && echo webkitgtk-6.0 || (pkg-config --exists webkit2gtk-4.1 && echo webkit2gtk-4.1 || (pkg-config --exists webkit2gtk-4.0 && echo webkit2gtk-4.0)))
WEBKIT_CFLAGS := $(shell pkg-config --cflags $(WEBKIT_PKG) 2>/dev/null)
# If detected WebKit pkg is not GTK4-compatible (no gtk-4.0 in CFLAGS), disable WebKit usage
ifeq ($(findstring gtk-4.0,$(WEBKIT_CFLAGS)),)
  NO_WEBKIT := 1
endif
ifeq ($(findstring gtk-3.0,$(WEBKIT_CFLAGS)),gtk-3.0)
  NO_WEBKIT := 1
endif

PKGS := gtk4 libadwaita-1
ifeq ($(NO_WEBKIT),)
  PKGS += $(WEBKIT_PKG)
endif

PKG_CFLAGS := $(shell pkg-config --cflags $(PKGS))
CFLAGS := -O2 -Wall -Wextra -Wno-unused-parameter $(PKG_CFLAGS)
ifeq ($(NO_WEBKIT),1)
  CFLAGS += -DNO_WEBKIT
endif
# Define header switch for webkitgtk-6.0 (after base CFLAGS defined)
ifeq ($(WEBKIT_PKG),webkitgtk-6.0)
  CFLAGS += -DWEBKITGTK6
endif

# Rhodes core integration
PKG_LDFLAGS := $(shell pkg-config --libs $(PKGS))
# Some distros use --as-needed by default; ensure zlib kept for WebKit deps
LDFLAGS := -Wl,--no-as-needed $(PKG_LDFLAGS) -lz
ifeq ($(ENABLE_CORE),1)
  CFLAGS += -DRHO_ENABLE_CORE
  ifneq ($(RHO_ROOT),)
    CFLAGS += -I$(RHO_ROOT)/platform/shared \
              -I$(RHO_ROOT)/platform/shared/ruby/include \
              -I$(RHO_ROOT)/platform/shared/ruby/linux \
              -I$(RHO_ROOT)/platform/shared/ruby \
              -I$(RHO_ROOT)/platform/shared/curl/include \
              -I$(RHO_ROOT)/lib/commonAPI/coreapi/ext/shared \
              -I$(RHO_ROOT)/lib \
              -I$(RHO_ROOT)/lib/commonAPI/coreapi/ext/shared/generated/cpp
    LDFLAGS += -L$(RHO_ROOT)/platform/linux/bin/rholib \
               -L$(RHO_ROOT)/platform/linux/bin/rubylib \
               -L$(RHO_ROOT)/platform/linux/bin/sqlite3 \
               -L$(RHO_ROOT)/platform/linux/bin/syncengine \
               -L$(RHO_ROOT)/platform/linux/bin/curl \
               -L$(RHO_ROOT)/platform/linux/bin/extensions \
               -Wl,--start-group -lrholib -lrubylib -lsqlite3 -lsyncengine -lcurl -lcoreapi $(EXTRA_LIBS) -Wl,--end-group \
               -lstdc++ -lgmp -ldl -lpthread -lm
  endif
endif

CSOURCES := $(wildcard $(SRCDIR)/*.c)
CPPSOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CSOURCES)) \
           $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPPSOURCES))

TARGET := $(OUTDIR)/$(APPNAME)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(OUTDIR)/$(APPNAME)
