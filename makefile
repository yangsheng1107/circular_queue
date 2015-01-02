# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2010-11-05
#
# Changelog :
#   0.01 - first version
# ------------------------------------------------

# project name (generate executable with this name)
CC       = gcc
# compiling flags here
CFLAGS   = -Wall -I.

LINKER   = gcc -o
# linking flags here
LFLAGS   = -Wall -lpthread

# change these to set the proper directories where each files shoould be
SRCDIR   = ./src
OBJDIR   = ./obj
BINDIR   = ./bin
INCDIR   = ./include

all: dir_create circular

circular: circular_obj
	$(CC) $(OBJDIR)/circular.o $(LFLAGS) -o $(BINDIR)/circular 

circular_obj: $(SRCDIR)/circular.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $(SRCDIR)/circular.c -o $(OBJDIR)/circular.o

dir_create:
ifeq "$(wildcard $(OBJDIR) )" ""
	mkdir -p $(OBJDIR)
endif
ifeq "$(wildcard $(BINDIR) )" ""
	mkdir -p $(BINDIR)
endif

clean:
	rm -rf $(BINDIR)/circular
	rm -rf $(OBJDIR)/*.*
