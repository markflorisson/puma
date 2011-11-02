#======================================================================
# Title: Makefile Template for a Directory
#
# Description:
#
# This makefile template is used to make and install a directory
# containing other component directories
#
# TARGETS Supported:
#
#   all		Builds and installs all sub-directories
#
#   install	Null target.  Provided for consistency
#
#   hdrz	Builds and installs only headers
#
#   clean	Removes by products of the build (except config info
#		like dependancies).  Cleans out install directories
#
#
#======================================================================

#======================================================================
# Default target
#======================================================================
all:		$(SUBDIRS)

#======================================================================
# Rule for building a sub-directory
#======================================================================
$(SUBDIRS):
		$(MAKE) -C $@ all

#======================================================================
# Rules for building headers
#======================================================================
hdrz:		$(SUBDIRS:%=%.hdrz)

%.hdrz:
		$(MAKE) -C $* $(subst $*.,,$@)

#======================================================================
# Rules for performing clean
#======================================================================
clean:		$(SUBDIRS:%=%.clean)

%.clean:
		$(MAKE) -C $* $(subst $*.,,$@)

#======================================================================
# Rules for performing realclean
#======================================================================
realclean:      $(SUBDIRS:%=%.realclean)

%.realclean: 
	$(MAKE) -C $* $(subst $*.,,$@)

unlink:         $(SUBDIRS:%=%.unlink)

%.unlink:
	$(MAKE) -C $* $(subst $*.,,$@)


ec_clean:       $(SUBDIRS:%=%.ec_clean)

%.ec_clean:
	 $(MAKE) -C $* $(subst $*.,,$@)


#======================================================================
# Phony targets
#======================================================================
.PHONY:         all hdrz install installonly realclean clean selfclean tags $(SUBDIRS) unlink
.PHONY: ec_clean


install:	all
