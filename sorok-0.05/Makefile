# Makefile for sorok/unsorok 0.05
#
# created by : mypapit (papit58@yahoo.com)
# compatible with GNU make and BSD make
# 
# for latest version of sorok and TinySorok please visit
# http://papit.cjb.net
#
# This software is licensed under PPSplit Public Licence 1.1 
# This License  can be obtained in the software source codes or
# visiting http://papit.cjb.net
#
# Have a nice day!
#
# 



FLAGS = -Wall -O2
CC = cc
INSTALL = install
GZEXE = gzexe

all : sorok1 unsorok1


sorok1 : encode.c sorok.h
	$(CC) $(FLAGS) -o sorok encode.c

unsorok1 : decode.c sorok.h
	$(CC) $(FLAGS) -o unsorok decode.c

clean :
	rm -rf *~
	rm -rf sorok
	rm -rf unsorok
	rm -rf encode.o
	rm -rf decode.o

install :
	$(INSTALL) -m 0755 sorok /usr/local/bin
	$(INSTALL) -m 0755 unsorok /usr/local/bin

strip : sorok unsorok
	strip sorok
	strip unsorok

gzexe : strip sorok unsorok
	$(GZEXE) sorok
	$(GZEXE) unsorok
	


	
