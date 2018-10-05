lib_LTLIBRARIES = libcarr.la
libcarr_la_SOURCES = src/array.h src/array.c

TESTS = test_clib_array
noinst_PROGRAMS = test_clib_array
test_clib_array_SOURCES = src/test_array.c src/array.h
test_clib_array_CFLAGS = -O0
test_clib_array_LDFLAGS = -lcmocka
test_clib_array_LDADD = libcarr.la
