#
# Macros
#

CC = gcc
CFLAGS = -O2 -g -Wall -Wno-parentheses
INCLUDE = -I. -I../shared/ruby -I../shared/ruby/linux -I../shared/ruby/include -I../shared -I../shared/ruby/rhoruby
LNK_OPTIONS = -lpthread
COMPILE = $(CC) $(CFLAGS) $(INCLUDE) $(LNK_OPTIONS) -DRUBY_EXPORT -c
SRC_DIR = ../shared/ruby
OBJFILES := $(patsubst $(SRC_DIR)/%.c,$(SRC_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
OBJFILES := $(OBJFILES) $(patsubst $(SRC_DIR)/enc/%.c,$(SRC_DIR)/enc/%.o,$(wildcard $(SRC_DIR)/enc/*.c))

#
# Build rubylinux
#

all : rubylinux

rubylinux: $(OBJFILES)
	$(CC) -o rubylinux $(OBJFILES)

clean : 
	find $(SRC_DIR) -name *.o | xargs rm

#
# Build the parts of rubylinux
#

%.o: %.c
	$(COMPILE) -o $@ $<
