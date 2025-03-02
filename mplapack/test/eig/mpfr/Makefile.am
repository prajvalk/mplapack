check_PROGRAMS = $(mplapack_mpfr_test_PROGRAMS)

mplapack_mpfr_test_PROGRAMS = xeigtstR_mpfr xeigtstC_mpfr
mplapack_mpfr_testdir = $(prefix)/lib/$(target)/mplapack/test/eig

$(check_PROGRAMS): ../libeig_mpfr.la ../libeig_override_mpfr.la

if IS_MINGW
libdepends = -Wl,--allow-multiple-definition -Wl,--whole-archive,$(top_builddir)/mplapack/test/eig/.libs/libeig_mpfr.a,$(top_builddir)/mplapack/test/eig/.libs/libeig_override_mpfr.a,$(top_builddir)/mplapack/test/matgen/.libs/libmatgen_mpfr.a,$(top_builddir)/mpblas/reference/.libs/libmpblas_mpfr.a,$(top_builddir)/mplapack/reference/.libs/libmplapack_mpfr.a,--no-whole-archive
else
libdepends = -L$(top_builddir)/mplapack/test/eig -leig_mpfr -leig_override_mpfr -L$(top_builddir)/mplapack/test/matgen -lmatgen_mpfr -L$(top_builddir)/mplapack/reference -lmplapack_mpfr -L$(top_builddir)/mpblas/reference -lmpblas_mpfr
endif

mplibs = -L$(MPC_LIBDIR) -L$(MPFR_LIBDIR) -L$(GMP_LIBDIR) -lmpc -lmpfr -lgmpxx -lgmp -lm
cxxflags = $(OPENMP_CXXFLAGS) -I$(top_srcdir)/include -I$(LAPACK_INCLUDEDIR) -I$(GMP_INCLUDEDIR) -I$(MPFR_INCLUDEDIR) -I$(MPC_INCLUDEDIR) -I$(top_srcdir)/mpfrc++ -D___MPLAPACK_BUILD_WITH_MPFR___ -D___MPLAPACK_INTERNAL___ -I$(top_srcdir)/mplapack/test/fable

xeigtstR_mpfr_SOURCES  = ../common/Rchkee.cpp
xeigtstR_mpfr_LDFLAGS  = $(libdepends) $(mplibs) -L$(LAPACK_LIBDIR) -llapacke -llapack -lblas -lgfortran
xeigtstR_mpfr_LDFLAGS += -Wl,-rpath,$(libdir)
xeigtstR_mpfr_CXXFLAGS = $(cxxflags)

xeigtstC_mpfr_SOURCES  = ../common/Cchkee.cpp
xeigtstC_mpfr_LDFLAGS  = $(libdepends) $(mplibs) -L$(LAPACK_LIBDIR) -llapacke -llapack -lblas -lgfortran
xeigtstC_mpfr_LDFLAGS += -Wl,-rpath,$(libdir)
xeigtstC_mpfr_CXXFLAGS = $(cxxflags)

install-data-hook:
if IS_MACOS
	bash $(top_builddir)/misc/fix_dylib_macOS.sh $(mplapack_mpfr_testdir) $(prefix)
endif

check: Rnep.out Rsep.out Rse2.out Rsvd.out Rec.out Red.out Rgg.out Rgd.out Rsb.out Rsg.out Rbal.out Rbak.out Rgbal.out Rgbak.out Rbb.out Rglm.out Rgqr.out Rgsv.out Rcsd.out Rlse.out \
       Cnep.out Csep.out Cse2.out Csvd.out Cec.out Ced.out Cgg.out Cgd.out Csb.out Csg.out Cbal.out Cbak.out Cgbal.out Cgbak.out Cbb.out Cglm.out Cgqr.out Cgsv.out Ccsd.out Clse.out

clean-local:
	rm -f *.out *~ \#*

Rnep.out: ../nep.in xeigtstR_mpfr
	@echo NEP: Testing Nonsymmetric Eigenvalue Problem routines
	./xeigtstR_mpfr < ../nep.in > $@ 2>&1

Rsep.out: ../sep.in xeigtstR_mpfr
	@echo SEP: Testing Symmetric Eigenvalue Problem routines
	./xeigtstR_mpfr < ../sep.in > $@ 2>&1

Rse2.out: ../se2.in xeigtstR_mpfr
	@echo SEP: Testing Symmetric Eigenvalue Problem routines
	./xeigtstR_mpfr < ../se2.in > $@ 2>&1

Rsvd.out: ../svd.in xeigtstR_mpfr
	@echo SVD: Testing Singular Value Decomposition routines
	./xeigtstR_mpfr < ../svd.in > $@ 2>&1

Rec.out: ../Rec.in xeigtstR_mpfr
	@echo DEC: Testing Eigen Condition Routines
	./xeigtstR_mpfr < ../Rec.in > $@ 2>&1

Red.out: ../Red.in xeigtstR_mpfr
	@echo DEV: Testing Nonsymmetric Eigenvalue Driver
	./xeigtstR_mpfr < ../Red.in > $@ 2>&1

Rgg.out: ../Rgg.in xeigtstR_mpfr
	@echo DGG: Testing Nonsymmetric Generalized Eigenvalue Problem routines
	./xeigtstR_mpfr < ../Rgg.in > $@ 2>&1

Rgd.out: ../Rgd.in xeigtstR_mpfr
	@echo DGD: Testing Nonsymmetric Generalized Eigenvalue Problem driver routines
	./xeigtstR_mpfr < ../Rgd.in > $@ 2>&1

Rsb.out: ../Rsb.in xeigtstR_mpfr
	@echo DSB: Testing Symmetric Eigenvalue Problem routines
	./xeigtstR_mpfr < ../Rsb.in > $@ 2>&1

Rsg.out: ../Rsg.in xeigtstR_mpfr
	@echo DSG: Testing Symmetric Generalized Eigenvalue Problem routines
	./xeigtstR_mpfr < ../Rsg.in > $@ 2>&1

Rbal.out: ../Rbal.in xeigtstR_mpfr
	@echo Rgebal: Testing the balancing of a general matrix
	./xeigtstR_mpfr < ../Rbal.in > $@ 2>&1

Rbak.out: ../Rbak.in xeigtstR_mpfr
	@echo Rgebak: Testing the back transformation of balanced matrix
	./xeigtstR_mpfr < ../Rbak.in > $@ 2>&1

Rgbal.out: ../Rgbal.in xeigtstR_mpfr
	@echo Rggbal: Testing the balancing of a pair of general matrices
	./xeigtstR_mpfr < ../Rgbal.in > $@ 2>&1

Rgbak.out: ../Rgbak.in xeigtstR_mpfr
	@echo Rggbak: Testing the back transformation of a pair of balanced matrices
	./xeigtstR_mpfr < ../Rgbak.in > $@ 2>&1

Rbb.out: ../Rbb.in xeigtstR_mpfr
	@echo DBB: Testing banded Singular Value Decomposition routines
	./xeigtstR_mpfr < ../Rbb.in > $@ 2>&1

Rglm.out: ../glm.in xeigtstR_mpfr
	@echo GLM: Testing Generalized Linear Regression Model routines
	./xeigtstR_mpfr < ../glm.in > $@ 2>&1

Rgqr.out: ../gqr.in xeigtstR_mpfr
	@echo GQR: Testing Generalized QR and RQ factorization routines
	./xeigtstR_mpfr < ../gqr.in > $@ 2>&1

Rgsv.out: ../gsv.in xeigtstR_mpfr
	@echo GSV: Testing Generalized Singular Value Decomposition routines
	./xeigtstR_mpfr < ../gsv.in > $@ 2>&1

Rcsd.out: ../csd.in xeigtstR_mpfr
	@echo CSD: Testing CS Decomposition routines
	./xeigtstR_mpfr < ../csd.in > $@ 2>&1

Rlse.out: ../lse.in xeigtstR_mpfr
	@echo LSE: Testing Constrained Linear Least Squares routines
	./xeigtstR_mpfr < ../lse.in > $@ 2>&1

Cnep.out: ../nep.in ./xeigtstC_mpfr
	@echo NEP: Testing Nonsymmetric Eigenvalue Problem routines
	./xeigtstC_mpfr < ../nep.in > $@ 2>&1

Csep.out: ../sep.in ./xeigtstC_mpfr
	@echo SEP: Testing Symmetric Eigenvalue Problem routines
	./xeigtstC_mpfr < ../sep.in > $@ 2>&1

Cse2.out: ../se2.in ./xeigtstC_mpfr
	@echo SEP: Testing Symmetric Eigenvalue Problem routines
	./xeigtstC_mpfr < ../se2.in > $@ 2>&1

Csvd.out: ../svd.in ./xeigtstC_mpfr
	@echo SVD: Testing Singular Value Decomposition routines
	./xeigtstC_mpfr < ../svd.in > $@ 2>&1

Cec.out: ../Cec.in ./xeigtstC_mpfr
	@echo CEC: Testing COMPLEX Eigen Condition Routines
	./xeigtstC_mpfr < ../Cec.in > $@ 2>&1

Ced.out: ../Ced.in ./xeigtstC_mpfr
	@echo CES: Testing COMPLEX Nonsymmetric Schur Form Driver
	./xeigtstC_mpfr < ../Ced.in > $@ 2>&1

Cgg.out: ../Cgg.in ./xeigtstC_mpfr
	@echo CGG: Testing COMPLEX Nonsymmetric Generalized Eigenvalue Problem routines
	./xeigtstC_mpfr < ../Cgg.in > $@ 2>&1

Cgd.out: ../Cgd.in ./xeigtstC_mpfr
	@echo CGD: Testing COMPLEX Nonsymmetric Generalized Eigenvalue Problem driver routines
	./xeigtstC_mpfr < ../Cgd.in > $@ 2>&1

Csb.out: ../Csb.in ./xeigtstC_mpfr
	@echo CHB: Testing Hermitian Eigenvalue Problem routines
	./xeigtstC_mpfr < ../Csb.in > $@ 2>&1

Csg.out: ../Csg.in ./xeigtstC_mpfr
	@echo CSG: Testing Symmetric Generalized Eigenvalue Problem routines
	./xeigtstC_mpfr < ../Csg.in > $@ 2>&1

Cbal.out: ../Cbal.in ./xeigtstC_mpfr
	@echo CGEBAL: Testing the balancing of a COMPLEX general matrix
	./xeigtstC_mpfr < ../Cbal.in > $@ 2>&1

Cbak.out: ../Cbak.in ./xeigtstC_mpfr
	@echo CGEBAK: Testing the back transformation of a COMPLEX balanced matrix
	./xeigtstC_mpfr < ../Cbak.in > $@ 2>&1

Cgbal.out: ../Cgbal.in ./xeigtstC_mpfr
	@echo CGGBAL: Testing the balancing of a pair of COMPLEX general matrices
	./xeigtstC_mpfr < ../Cgbal.in > $@ 2>&1

Cgbak.out: ../Cgbak.in ./xeigtstC_mpfr
	@echo CGGBAK: Testing the back transformation of a pair of COMPLEX balanced matrices
	./xeigtstC_mpfr < ../Cgbak.in > $@ 2>&1

Cbb.out: ../Cbb.in ./xeigtstC_mpfr
	@echo CBB: Testing banded Singular Value Decomposition routines
	./xeigtstC_mpfr < ../Cbb.in > $@ 2>&1

Cglm.out: ../glm.in ./xeigtstC_mpfr
	@echo GLM: Testing Generalized Linear Regression Model routines
	./xeigtstC_mpfr < ../glm.in > $@ 2>&1

Cgqr.out: ../gqr.in ./xeigtstC_mpfr
	@echo GQR: Testing Generalized QR and RQ factorization routines
	./xeigtstC_mpfr < ../gqr.in > $@ 2>&1

Cgsv.out: ../gsv.in ./xeigtstC_mpfr
	@echo GSV: Testing Generalized Singular Value Decomposition routines
	./xeigtstC_mpfr < ../gsv.in > $@ 2>&1

Ccsd.out: ../csd.in ./xeigtstC_mpfr
	@echo CSD: Testing CS Decomposition routines
	./xeigtstC_mpfr < ../csd.in > $@ 2>&1

Clse.out: ../lse.in ./xeigtstC_mpfr
	@echo LSE: Testing Constrained Linear Least Squares routines
	./xeigtstC_mpfr < ../lse.in > $@ 2>&1
