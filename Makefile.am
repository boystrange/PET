
SUBDIRS = src tests homepage

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pet.pc 

upload: @PACKAGE@-@VERSION@.tar.gz
	scp @PACKAGE@-@VERSION@.tar.gz padovani@pianeta.di.unito.it:public_html/Software/PET/source

