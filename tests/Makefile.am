
TESTS = 2 3 4 1 5

NULL =

noinst_PROGRAMS =
if COND_LIBXML2
noinst_PROGRAMS += test_libxml2
endif

test_libxml2_SOURCES = test_libxml2.cc
test_libxml2_LDADD = $(XML_LIBS)

INCLUDES = \
  $(XML_CFLAGS) \
  -I$(top_srcdir)/src \
  $(NULL)

all:

do_tests: $(TESTS:%=do_test_%)

do_test_%: test_libxml2
	@echo "*** TEST #"$(@:do_test_%=%)
	./test_libxml2 source.xml.gz $(@:do_test_%=%)
	xsltproc --timing --repeat --stringparam test $(@:do_test_%=%) count.xsl source.xml.gz

do_fxgrep_test_1:
	fxgrep -1 -nv -k -P "//*||//''" source.xml

do_fxgrep_test_2:
	fxgrep -1 -nv -k -P "//m:mrow[@xref]" source.xml

do_fxgrep_test_3:
	fxgrep -1 -nv -k -P "//m:mrow[@xref]/''" source.xml

do_fxgrep_test_4:
	fxgrep -1 -nv -k -P "//*[@xref]['']" source.xml

