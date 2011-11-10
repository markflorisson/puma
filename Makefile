MF=	Makefile

SUBDIRS := 	src

include         $(PROJ_HOME)/MakeTmpl/dir-NEW2.mk

simulation:
		convert -adjoin -delay 10 -loop 10000 pumaHare_*.ppm puma.gif
