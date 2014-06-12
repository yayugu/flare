# forward compatibility
datarootdir = @datarootdir@

# main
bin_PROGRAMS = twbench
DEFS = -Wall $(FLARE_CXXFLAGS)
LDADD = ../lib/libflare.la

# src
twbench_CPPFLAGS = -I../lib
twbench_SOURCES = \
	twbench.cc
