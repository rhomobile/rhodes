#
# Macros
#

CC = /usr/bin/gcc
CFLAGS = -O2 -g -Wall -Wno-parentheses -DRUBY_EXPORT -DRUBY_LIB=\"linux/ruby/config.h\"
INCLUDE = -I../shared/ruby/linux/ruby -I../shared/ruby/include
COMPILE = $(CC) $(CFLAGS) $(INCLUDE) -c
LNK_OPTIONS = 
SRC_DIR = ../shared/ruby
OBJFILES := $(patsubst $(SRC_DIR)/%.c,$(SRC_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))

#
# Build rubylinux
#

all : rubylinux

rubylinux: $(OBJFILES)
	$(CC) -o rubylinux $(OBJFILES)

clean : 
		rm -f $(SRC_DIR)/*.o \
		rubylinux

#
# Build the parts of rubylinux
#

%.o: %.c
	$(COMPILE) -o $@ $<
