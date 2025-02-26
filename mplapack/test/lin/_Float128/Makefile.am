check_PROGRAMS = $(mplapack__Float128_test_PROGRAMS)

mplapack__Float128_test_PROGRAMS = xlintstR__Float128 xlintstC__Float128 xlintstrfR__Float128 xlintstrfC__Float128
mplapack__Float128_testdir = $(prefix)/lib/$(target)/mplapack/test/lin

$(check_PROGRAMS): ../liblin__Float128.la ../liblin_override__Float128.la

if IS_MINGW
libdepends = -Wl,--allow-multiple-definition -Wl,--whole-archive,$(top_builddir)/mplapack/test/lin/.libs/liblin__Float128.a,$(top_builddir)/mplapack/test/lin/.libs/liblin_override__Float128.a,$(top_builddir)/mplapack/test/matgen/.libs/libmatgen__Float128.a,$(top_builddir)/mpblas/reference/.libs/libmpblas__Float128.a,$(top_builddir)/mplapack/reference/.libs/libmplapack__Float128.a,--no-whole-archive
else
libdepends = -L.. -llin__Float128 -llin_override__Float128 -L$(top_builddir)/mplapack/test/matgen -lmatgen__Float128 -L$(top_builddir)/mplapack/reference -lmplapack__Float128 -L$(top_builddir)/mpblas/reference -lmpblas__Float128
endif

if WANT_QUADMATH
mplibs = -lm -lquadmath
else
mplibs = -lm
endif
cxxflags = $(OPENMP_CXXFLAGS) -I$(top_srcdir)/include -D___MPLAPACK_BUILD_WITH__FLOAT128___ -D___MPLAPACK_INTERNAL___ -I$(top_srcdir)/mplapack/test/fable

xlintstR__Float128_SOURCES  = ../common/Rchkaa.cpp
xlintstR__Float128_LDFLAGS  = $(libdepends) $(mplibs) -L$(LAPACK_LIBDIR) -llapacke -llapack -lblas -lgfortran
xlintstR__Float128_LDFLAGS += -Wl,-rpath,$(libdir)
xlintstR__Float128_CXXFLAGS = $(cxxflags) 

xlintstC__Float128_SOURCES  = ../common/Cchkaa.cpp
xlintstC__Float128_LDFLAGS  = $(libdepends) $(mplibs) -L$(LAPACK_LIBDIR) -llapacke -llapack -lblas -lgfortran
xlintstC__Float128_LDFLAGS += -Wl,-rpath,$(libdir)
xlintstC__Float128_CXXFLAGS = $(cxxflags)

xlintstrfR__Float128_SOURCES  = ../common/Rchkrfp.cpp
xlintstrfR__Float128_LDFLAGS  = $(libdepends) $(mplibs) -L$(LAPACK_LIBDIR) -llapacke -llapack -lblas -lgfortran
xlintstrfR__Float128_LDFLAGS += -Wl,-rpath,$(libdir)
xlintstrfR__Float128_CXXFLAGS = $(cxxflags)

xlintstrfC__Float128_SOURCES  = ../common/Cchkrfp.cpp
xlintstrfC__Float128_LDFLAGS  = $(libdepends) $(mplibs) -L$(LAPACK_LIBDIR) -llapacke -llapack -lblas -lgfortran
xlintstrfC__Float128_LDFLAGS += -Wl,-rpath,$(libdir)
xlintstrfC__Float128_CXXFLAGS = $(cxxflags)

check: Rtest.out Ctest.out Rtest_rfp.out Ctest_rfp.out

clean-local:
	rm -f *.out *~ \#*

Rtest.out: ../Rtest.in xlintstR__Float128
	@echo Testing Real MPLAPACK linear equation routines
	./xlintstR__Float128 < ../Rtest.in > $@ 2>&1

Ctest.out: ../Ctest.in xlintstC__Float128
	@echo Testing Complex MPLAPACK linear equation routines
	./xlintstC__Float128 < ../Ctest.in > $@ 2>&1

Rtest_rfp.out: ../Rtest_rfp.in xlintstrfR__Float128
	@echo Testing Real MPLAPACK RFP prototype linear equation routines
	./xlintstrfR__Float128 < ../Rtest_rfp.in > $@ 2>&1

Ctest_rfp.out: ../Ctest_rfp.in xlintstrfC__Float128
	@echo Testing Complex MPLAPACK RFP prototype linear equation routines
	./xlintstrfC__Float128 < ../Ctest_rfp.in > $@ 2>&1
