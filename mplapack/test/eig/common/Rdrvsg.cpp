/*
 * Copyright (c) 2021
 *      Nakata, Maho
 *      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <mpblas.h>
#include <mplapack.h>

#include <fem.hpp> // Fortran EMulation library of fable module
using namespace fem::major_types;
using fem::common;

#include <mplapack_matgen.h>
#include <mplapack_eig.h>

#include <mplapack_debug.h>

void Rdrvsg(INTEGER const nsizes, INTEGER *nn, INTEGER const ntypes, bool *dotype, INTEGER *iseed, REAL const thresh, INTEGER const nounit, REAL *a, INTEGER const lda, REAL *b, INTEGER const ldb, REAL *d, REAL *z, INTEGER const ldz, REAL *ab, REAL *bb, REAL *ap, REAL *bp, REAL *work, INTEGER const nwork, INTEGER *iwork, INTEGER const liwork, REAL *result, INTEGER &info) {
    INTEGER ldab = lda;
    INTEGER ldbb = ldb;
    common cmn;
    common_write write(cmn);
    const INTEGER maxtyp = 21;
    INTEGER ktype[21] = {1, 2, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 8, 8, 8, 9, 9, 9, 9, 9, 9};
    INTEGER kmagn[21] = {1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 1, 2, 3, 1, 1, 1, 1, 1, 1};
    INTEGER kmode[21] = {0, 0, 4, 3, 1, 4, 4, 4, 3, 1, 4, 4, 0, 0, 0, 4, 4, 4, 4, 4, 4};
    char buf[1024];
    INTEGER ntestt = 0;
    bool badnn = false;
    INTEGER nmax = 0;
    INTEGER j = 0;
    REAL unfl = 0.0;
    REAL ovfl = 0.0;
    REAL ulp = 0.0;
    const REAL one = 1.0;
    REAL ulpinv = 0.0;
    REAL rtunfl = 0.0;
    REAL rtovfl = 0.0;
    INTEGER i = 0;
    INTEGER iseed2[4];
    INTEGER nerrs = 0;
    INTEGER nmats = 0;
    INTEGER jsize = 0;
    INTEGER n = 0;
    REAL aninv = 0.0;
    INTEGER mtypes = 0;
    INTEGER ka9 = 0;
    INTEGER kb9 = 0;
    INTEGER jtype = 0;
    INTEGER ntest = 0;
    INTEGER ioldsd[4];
    INTEGER itype = 0;
    INTEGER imode = 0;
    REAL anorm = 0.0;
    INTEGER iinfo = 0;
    REAL cond = 0.0;
    INTEGER ka = 0;
    INTEGER kb = 0;
    const REAL zero = 0.0;
    INTEGER jcol = 0;
    INTEGER idumma[1];
    REAL abstol = 0.0;
    INTEGER il = 0;
    INTEGER iu = 0;
    INTEGER itemp = 0;
    INTEGER ibtype = 0;
    INTEGER ibuplo = 0;
    char uplo;
    const REAL ten = 10.0;
    REAL vl = 0.0;
    REAL vu = 0.0;
    INTEGER m = 0;
    INTEGER ij = 0;
    static const char *format_9999 = "(' Rdrvsg: ',a,' returned INFO=',i6,'.',/,9x,'N=',i6,', JTYPE=',i6,"
                                     "', ISEED=(',3(i5,','),i5,')')";
    //
    //     1)      Check for errors
    //
    ntestt = 0;
    info = 0;
    //
    badnn = false;
    nmax = 0;
    for (j = 1; j <= nsizes; j = j + 1) {
        nmax = max(nmax, nn[j - 1]);
        if (nn[j - 1] < 0) {
            badnn = true;
        }
    }
    //
    //     Check for errors
    //
    if (nsizes < 0) {
        info = -1;
    } else if (badnn) {
        info = -2;
    } else if (ntypes < 0) {
        info = -3;
    } else if (lda <= 1 || lda < nmax) {
        info = -9;
    } else if (ldz <= 1 || ldz < nmax) {
        info = -16;
    } else if (2 * max(nmax, (INTEGER)3) * max(nmax, (INTEGER)3) > nwork) {
        info = -21;
    } else if (2 * max(nmax, (INTEGER)3) * max(nmax, (INTEGER)3) > liwork) {
        info = -23;
    }
    //
    if (info != 0) {
        Mxerbla("Rdrvsg", -info);
        return;
    }
    //
    //     Quick return if possible
    //
    if (nsizes == 0 || ntypes == 0) {
        return;
    }
    //
    //     More Important constants
    //
    unfl = Rlamch("Safe minimum");
    ovfl = Rlamch("Overflow");
    ulp = Rlamch("Epsilon") * Rlamch("Base");
    ulpinv = one / ulp;
    rtunfl = sqrt(unfl);
    rtovfl = sqrt(ovfl);
    //
    for (i = 1; i <= 4; i = i + 1) {
        iseed2[i - 1] = iseed[i - 1];
    }
    //
    //     Loop over sizes, types
    //
    nerrs = 0;
    nmats = 0;
    //
    for (jsize = 1; jsize <= nsizes; jsize = jsize + 1) {
        n = nn[jsize - 1];
        aninv = one / castREAL(max((INTEGER)1, n));
        //
        if (nsizes != 1) {
            mtypes = min(maxtyp, ntypes);
        } else {
            mtypes = min(maxtyp + 1, ntypes);
        }
        //
        ka9 = 0;
        kb9 = 0;
        for (jtype = 1; jtype <= mtypes; jtype = jtype + 1) {
            if (!dotype[jtype - 1]) {
                goto statement_640;
            }
            nmats++;
            ntest = 0;
            //
            for (j = 1; j <= 4; j = j + 1) {
                ioldsd[j - 1] = iseed[j - 1];
            }
            //
            //           2)      Compute "A"
            //
            //                   Control parameters:
            //
            //               KMAGN  KMODE        KTYPE
            //           =1  O(1)   clustered 1  zero
            //           =2  large  clustered 2  identity
            //           =3  small  exponential  (none)
            //           =4         arithmetic   diagonal, w/ eigenvalues
            //           =5         random log   hermitian, w/ eigenvalues
            //           =6         random       (none)
            //           =7                      random diagonal
            //           =8                      random hermitian
            //           =9                      banded, w/ eigenvalues
            //
            if (mtypes > maxtyp) {
                goto statement_90;
            }
            //
            itype = ktype[jtype - 1];
            imode = kmode[jtype - 1];
            //
            //           Compute norm
            //
            switch (kmagn[jtype - 1]) {
            case 1:
                goto statement_40;
            case 2:
                goto statement_50;
            case 3:
                goto statement_60;
            default:
                break;
            }
        //
        statement_40:
            anorm = one;
            goto statement_70;
        //
        statement_50:
            anorm = (rtovfl * ulp) * aninv;
            goto statement_70;
        //
        statement_60:
            anorm = rtunfl * n * ulpinv;
            goto statement_70;
        //
        statement_70:
            //
            iinfo = 0;
            cond = ulpinv;
            //
            //           Special Matrices -- Identity & Jordan block
            //
            if (itype == 1) {
                //
                //              Zero
                //
                ka = 0;
                kb = 0;
                Rlaset("Full", lda, n, zero, zero, a, lda);
                //
            } else if (itype == 2) {
                //
                //              Identity
                //
                ka = 0;
                kb = 0;
                Rlaset("Full", lda, n, zero, zero, a, lda);
                for (jcol = 1; jcol <= n; jcol = jcol + 1) {
                    a[(jcol - 1) + (jcol - 1) * lda] = anorm;
                }
                //
            } else if (itype == 4) {
                //
                //              Diagonal Matrix, [Eigen]values Specified
                //
                ka = 0;
                kb = 0;
                Rlatms(n, n, "S", iseed, "S", work, imode, cond, anorm, 0, 0, "N", a, lda, &work[(n + 1) - 1], iinfo);
                //
            } else if (itype == 5) {
                //
                //              symmetric, eigenvalues specified
                //
                ka = max((INTEGER)0, n - 1);
                kb = ka;
                Rlatms(n, n, "S", iseed, "S", work, imode, cond, anorm, n, n, "N", a, lda, &work[(n + 1) - 1], iinfo);
                //
            } else if (itype == 7) {
                //
                //              Diagonal, random eigenvalues
                //
                ka = 0;
                kb = 0;
                Rlatmr(n, n, "S", iseed, "S", work, 6, one, one, "T", "N", &work[(n + 1) - 1], 1, one, &work[(2 * n + 1) - 1], 1, one, "N", idumma, 0, 0, zero, anorm, "NO", a, lda, iwork, iinfo);
                //
            } else if (itype == 8) {
                //
                //              symmetric, random eigenvalues
                //
                ka = max((INTEGER)0, n - 1);
                kb = ka;
                Rlatmr(n, n, "S", iseed, "H", work, 6, one, one, "T", "N", &work[(n + 1) - 1], 1, one, &work[(2 * n + 1) - 1], 1, one, "N", idumma, n, n, zero, anorm, "NO", a, lda, iwork, iinfo);
                //
            } else if (itype == 9) {
                //
                //              symmetric banded, eigenvalues specified
                //
                //              The following values are used for the half-bandwidths:
                //
                //                ka = 1   kb = 1
                //                ka = 2   kb = 1
                //                ka = 2   kb = 2
                //                ka = 3   kb = 1
                //                ka = 3   kb = 2
                //                ka = 3   kb = 3
                //
                kb9++;
                if (kb9 > ka9) {
                    ka9++;
                    kb9 = 1;
                }
                ka = max({(INTEGER)0, min(n - 1, ka9)});
                kb = max({(INTEGER)0, min(n - 1, kb9)});
                Rlatms(n, n, "S", iseed, "S", work, imode, cond, anorm, ka, ka, "N", a, lda, &work[(n + 1) - 1], iinfo);
                //
            } else {
                //
                iinfo = 1;
            }
            //
            if (iinfo != 0) {
                write(nounit, format_9999), "Generator", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                info = abs(iinfo);
                return;
            }
        //
        statement_90:
            //
            abstol = unfl + unfl;
            if (n <= 1) {
                il = 1;
                iu = n;
            } else {
                il = 1 + castINTEGER(castREAL(n - 1) * Rlarnd(1, iseed2));
                iu = 1 + castINTEGER(castREAL(n - 1) * Rlarnd(1, iseed2));
                if (il > iu) {
                    itemp = il;
                    il = iu;
                    iu = itemp;
                }
            }
            //
            //           3) Call Rsygv, Rspgv, Rsbgv, SSYGVD, SSPGVD, SSBGVD,
            //              Rsygvx, Rspgvx, and Rsbgvx, do tests.
            //
            //           loop over the three generalized problems
            //                 IBTYPE = 1: A*x = (lambda)*B*x
            //                 IBTYPE = 2: A*B*x = (lambda)*x
            //                 IBTYPE = 3: B*A*x = (lambda)*x
            //
            for (ibtype = 1; ibtype <= 3; ibtype = ibtype + 1) {
                //
                //              loop over the setting UPLO
                //
                for (ibuplo = 1; ibuplo <= 2; ibuplo = ibuplo + 1) {
                    if (ibuplo == 1) {
                        uplo = 'U';
                    }
                    if (ibuplo == 2) {
                        uplo = 'L';
                    }
                    //
                    //                 Generate random well-conditioned positive definite
                    //                 matrix B, of bandwidth not greater than that of A.
                    //
                    Rlatms(n, n, "U", iseed, "P", work, 5, ten, one, kb, kb, &uplo, b, ldb, &work[(n + 1) - 1], iinfo);
                    //
                    //                 Test Rsygv
                    //
                    ntest++;
                    //
                    Rlacpy(" ", n, n, a, lda, z, ldz);
                    Rlacpy(&uplo, n, n, b, ldb, bb, ldb);
                    //
                    Rsygv(ibtype, "V", &uplo, n, z, ldz, bb, ldb, d, work, nwork, iinfo);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rsygv(V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rsygv(V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);

                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_100;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, n, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    //                 Test Rsygvd
                    //
                    ntest++;
                    //
                    Rlacpy(" ", n, n, a, lda, z, ldz);
                    Rlacpy(&uplo, n, n, b, ldb, bb, ldb);
                    //
                    Rsygvd(ibtype, "V", &uplo, n, z, ldz, bb, ldb, d, work, nwork, iwork, liwork, iinfo);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rsygvd(V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rsygvd(V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_100;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, n, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    //                 Test Rsygvx
                    //
                    ntest++;
                    //
                    Rlacpy(" ", n, n, a, lda, ab, lda);
                    Rlacpy(&uplo, n, n, b, ldb, bb, ldb);
                    //
                    Rsygvx(ibtype, "V", "A", &uplo, n, ab, lda, bb, ldb, vl, vu, il, iu, abstol, m, d, z, ldz, work, nwork, &iwork[(n + 1) - 1], iwork, iinfo);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rsygvx(V,A,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rsygvx(V,A,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_100;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, n, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    ntest++;
                    //
                    Rlacpy(" ", n, n, a, lda, ab, lda);
                    Rlacpy(&uplo, n, n, b, ldb, bb, ldb);
                    //
                    //                 since we do not know the exact eigenvalues of this
                    //                 eigenpair, we just set VL and VU as constants.
                    //                 It is quite possible that there are no eigenvalues
                    //                 in this interval.
                    //
                    vl = zero;
                    vu = anorm;
                    Rsygvx(ibtype, "V", "V", &uplo, n, ab, lda, bb, ldb, vl, vu, il, iu, abstol, m, d, z, ldz, work, nwork, &iwork[(n + 1) - 1], iwork, iinfo);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rsygvx(V,V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rsygvx(V,V,N)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_100;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    ntest++;
                    //
                    Rlacpy(" ", n, n, a, lda, ab, lda);
                    Rlacpy(&uplo, n, n, b, ldb, bb, ldb);
                    //
                    Rsygvx(ibtype, "V", "I", &uplo, n, ab, lda, bb, ldb, vl, vu, il, iu, abstol, m, d, z, ldz, work, nwork, &iwork[(n + 1) - 1], iwork, iinfo);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rsygvx(V,I,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rsygvx(V,I,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_100;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                //
                statement_100:
                    //
                    //                 Test Rspgv
                    //
                    ntest++;
                    //
                    //                 Copy the matrices into packed storage.
                    //
                    if (Mlsame(&uplo, "U")) {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = 1; i <= j; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    } else {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = j; i <= n; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    }
                    //
                    Rspgv(ibtype, "V", &uplo, n, ap, bp, d, z, ldz, work, iinfo);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rspgv(V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rspgv(V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_310;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, n, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    //                 Test Rspgvd
                    //
                    ntest++;
                    //
                    //                 Copy the matrices into packed storage.
                    //
                    if (Mlsame(&uplo, "U")) {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = 1; i <= j; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    } else {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = j; i <= n; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    }
                    //
                    Rspgvd(ibtype, "V", &uplo, n, ap, bp, d, z, ldz, work, nwork, iwork, liwork, iinfo);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rspgvd(V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rspgvd(V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_310;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, n, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    //                 Test Rspgvx
                    //
                    ntest++;
                    //
                    //                 Copy the matrices into packed storage.
                    //
                    if (Mlsame(&uplo, "U")) {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = 1; i <= j; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    } else {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = j; i <= n; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    }
                    //
                    Rspgvx(ibtype, "V", "A", &uplo, n, ap, bp, vl, vu, il, iu, abstol, m, d, z, ldz, work, &iwork[(n + 1) - 1], iwork, info);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rspgvx(V,A,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rspgvx(V,A,N)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_310;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    ntest++;
                    //
                    //                 Copy the matrices into packed storage.
                    //
                    if (Mlsame(&uplo, "U")) {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = 1; i <= j; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    } else {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = j; i <= n; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    }
                    //
                    vl = zero;
                    vu = anorm;
                    Rspgvx(ibtype, "V", "V", &uplo, n, ap, bp, vl, vu, il, iu, abstol, m, d, z, ldz, work, &iwork[(n + 1) - 1], iwork, info);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rspgvx(V,V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rspgvx(V,V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_310;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                    //
                    ntest++;
                    //
                    //                 Copy the matrices into packed storage.
                    //
                    if (Mlsame(&uplo, "U")) {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = 1; i <= j; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    } else {
                        ij = 1;
                        for (j = 1; j <= n; j = j + 1) {
                            for (i = j; i <= n; i = i + 1) {
                                ap[ij - 1] = a[(i - 1) + (j - 1) * lda];
                                bp[ij - 1] = b[(i - 1) + (j - 1) * ldb];
                                ij++;
                            }
                        }
                    }
                    //
                    Rspgvx(ibtype, "V", "I", &uplo, n, ap, bp, vl, vu, il, iu, abstol, m, d, z, ldz, work, &iwork[(n + 1) - 1], iwork, info);
                    if (iinfo != 0) {
                        if (Mlsame(&uplo, "U"))
                            write(nounit, format_9999), "Rspgvx(V,I,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        else
                            write(nounit, format_9999), "Rspgvx(V,I,N)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                        info = abs(iinfo);
                        if (iinfo < 0) {
                            return;
                        } else {
                            result[ntest - 1] = ulpinv;
                            goto statement_310;
                        }
                    }
                    //
                    //                 Do Test
                    //
                    Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                //
                statement_310:
                    //
                    if (ibtype == 1) {
                        //
                        //                    TEST Rsbgv
                        //
                        ntest++;
                        //
                        //                    Copy the matrices into band storage.
                        //
                        if (Mlsame(&uplo, "U")) {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = max((INTEGER)1, j - ka); i <= j; i = i + 1) {
                                    ab[((ka + 1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = max((INTEGER)1, j - kb); i <= j; i = i + 1) {
                                    bb[((kb + 1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        } else {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = j; i <= min(n, j + ka); i = i + 1) {
                                    ab[((1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = j; i <= min(n, j + kb); i = i + 1) {
                                    bb[((1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        }
                        //
                        Rsbgv("V", &uplo, n, ka, kb, ab, lda, bb, ldb, d, z, ldz, work, iinfo);
                        if (iinfo != 0) {
                            if (Mlsame(&uplo, "U"))
                                write(nounit, format_9999), "Rsbgv(V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            else
                                write(nounit, format_9999), "Rsbgv(V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            info = abs(iinfo);
                            if (iinfo < 0) {
                                return;
                            } else {
                                result[ntest - 1] = ulpinv;
                                goto statement_620;
                            }
                        }
                        //
                        //                    Do Test
                        //
                        Rsgt01(ibtype, &uplo, n, n, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                        //
                        //                    TEST Rsbgvd
                        //
                        ntest++;
                        //
                        //                    Copy the matrices into band storage.
                        //
                        if (Mlsame(&uplo, "U")) {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = max((INTEGER)1, j - ka); i <= j; i = i + 1) {
                                    ab[((ka + 1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = max((INTEGER)1, j - kb); i <= j; i = i + 1) {
                                    bb[((kb + 1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        } else {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = j; i <= min(n, j + ka); i = i + 1) {
                                    ab[((1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = j; i <= min(n, j + kb); i = i + 1) {
                                    bb[((1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        }
                        //
                        Rsbgvd("V", &uplo, n, ka, kb, ab, lda, bb, ldb, d, z, ldz, work, nwork, iwork, liwork, iinfo);
                        if (iinfo != 0) {
                            if (Mlsame(&uplo, "U"))
                                write(nounit, format_9999), "Rsbgvd(V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            else
                                write(nounit, format_9999), "Rsbgvd(V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            info = abs(iinfo);
                            if (iinfo < 0) {
                                return;
                            } else {
                                result[ntest - 1] = ulpinv;
                                goto statement_620;
                            }
                        }
                        //
                        //                    Do Test
                        //
                        Rsgt01(ibtype, &uplo, n, n, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                        //
                        //                    Test Rsbgvx
                        //
                        ntest++;
                        //
                        //                    Copy the matrices into band storage.
                        //
                        if (Mlsame(&uplo, "U")) {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = max((INTEGER)1, j - ka); i <= j; i = i + 1) {
                                    ab[((ka + 1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = max((INTEGER)1, j - kb); i <= j; i = i + 1) {
                                    bb[((kb + 1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        } else {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = j; i <= min(n, j + ka); i = i + 1) {
                                    ab[((1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = j; i <= min(n, j + kb); i = i + 1) {
                                    bb[((1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        }
                        //
                        Rsbgvx("V", "A", &uplo, n, ka, kb, ab, lda, bb, ldb, bp, max((INTEGER)1, n), vl, vu, il, iu, abstol, m, d, z, ldz, work, &iwork[(n + 1) - 1], iwork, iinfo);
                        if (iinfo != 0) {
                            if (Mlsame(&uplo, "U"))
                                write(nounit, format_9999), "Rsbgvx(V,A,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            else
                                write(nounit, format_9999), "Rsbgvx(V,A,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            info = abs(iinfo);
                            if (iinfo < 0) {
                                return;
                            } else {
                                result[ntest - 1] = ulpinv;
                                goto statement_620;
                            }
                        }
                        //
                        //                    Do Test
                        //
                        Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                        //
                        ntest++;
                        //
                        //                    Copy the matrices into band storage.
                        //
                        if (Mlsame(&uplo, "U")) {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = max((INTEGER)1, j - ka); i <= j; i = i + 1) {
                                    ab[((ka + 1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = max((INTEGER)1, j - kb); i <= j; i = i + 1) {
                                    bb[((kb + 1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        } else {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = j; i <= min(n, j + ka); i = i + 1) {
                                    ab[((1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = j; i <= min(n, j + kb); i = i + 1) {
                                    bb[((1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        }
                        //
                        vl = zero;
                        vu = anorm;
                        Rsbgvx("V", "V", &uplo, n, ka, kb, ab, lda, bb, ldb, bp, max((INTEGER)1, n), vl, vu, il, iu, abstol, m, d, z, ldz, work, &iwork[(n + 1) - 1], iwork, iinfo);
                        if (iinfo != 0) {
                            if (Mlsame(&uplo, "U"))
                                write(nounit, format_9999), "Rsbgvx(V,V,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            else
                                write(nounit, format_9999), "Rsbgvx(V,V,L)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            info = abs(iinfo);
                            if (iinfo < 0) {
                                return;
                            } else {
                                result[ntest - 1] = ulpinv;
                                goto statement_620;
                            }
                        }
                        //
                        //                    Do Test
                        //
                        Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                        //
                        ntest++;
                        //
                        //                    Copy the matrices into band storage.
                        //
                        if (Mlsame(&uplo, "U")) {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = max((INTEGER)1, j - ka); i <= j; i = i + 1) {
                                    ab[((ka + 1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = max((INTEGER)1, j - kb); i <= j; i = i + 1) {
                                    bb[((kb + 1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        } else {
                            for (j = 1; j <= n; j = j + 1) {
                                for (i = j; i <= min(n, j + ka); i = i + 1) {
                                    ab[((1 + i - j) - 1) + (j - 1) * ldab] = a[(i - 1) + (j - 1) * lda];
                                }
                                for (i = j; i <= min(n, j + kb); i = i + 1) {
                                    bb[((1 + i - j) - 1) + (j - 1) * ldbb] = b[(i - 1) + (j - 1) * ldb];
                                }
                            }
                        }
                        //
                        Rsbgvx("V", "I", &uplo, n, ka, kb, ab, lda, bb, ldb, bp, max((INTEGER)1, n), vl, vu, il, iu, abstol, m, d, z, ldz, work, &iwork[(n + 1) - 1], iwork, iinfo);
                        if (iinfo != 0) {
                            if (Mlsame(&uplo, "U"))
                                write(nounit, format_9999), "Rsbgvx(V,I,U)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            else
                                write(nounit, format_9999), "Rsbgvx(V,I,N)", iinfo, n, jtype, ioldsd[0], ioldsd[1], ioldsd[2], ioldsd[3];
                            info = abs(iinfo);
                            if (iinfo < 0) {
                                return;
                            } else {
                                result[ntest - 1] = ulpinv;
                                goto statement_620;
                            }
                        }
                        //
                        //                    Do Test
                        //
                        Rsgt01(ibtype, &uplo, n, m, a, lda, b, ldb, z, ldz, d, work, &result[ntest - 1]);
                        //
                    }
                //
                statement_620:;
                }
            }
            //
            //           End of Loop -- Check for RESULT(j) > THRESH
            //
            ntestt += ntest;
            Rlafts("DSG", n, n, jtype, ntest, result, ioldsd, thresh, nounit, nerrs);
        statement_640:;
        }
    }
    //
    //     Summary
    //
    Rlasum("DSG", nounit, nerrs, ntestt);
    //
    //     End of Rdrvsg
    //
}
