# Makefile for piir

# Copyright (C) 1994-2013 Free Software Foundation, Inc.

# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



LIBS=-lm

DEPS=linux-i2c.o

%.o: %.c %.h
	cc -ggdb -c -o $@ $<

mlxd: mlxd.c $(DEPS)
	cc -ggdb -o mlxd mlxd.c $(LIBS) $(DEPS)
