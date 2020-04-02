bin_PROGRAMS = joes_crazy_bankers
joes_crazy_bankers_SOURCES = main.c

# These might be a good idea
.POSIX:
AM_CFLAGS = -std=c99 -pedantic -D_POSIX_C_SOURCE=200809L -Wall -Wextra
AM_LDFLAGS = 

# This might be how to add CPP stuff
# AM_CPPFLAGS =  -pedantic -Wall -Wextra
# AM_CPPFLAGS+= -std=c++11

# Here's an example for adding things
# This might not be the best way to handle adding these.
# Quoting these doesn't seem to be a good idea.

AM_CFLAGS += -pthread
AM_LDFLAGS += -lpthread


