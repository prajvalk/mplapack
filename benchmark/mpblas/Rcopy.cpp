/*
 * Copyright (c) 2008-2022
 *	Nakata, Maho
 * 	All rights reserved.
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

#include <stdio.h>
#include <string.h>
#include <chrono>
#include <dlfcn.h>
#include <mpblas.h>
#include <mplapack.h>
#include <mplapack_benchmark.h>

int main(int argc, char *argv[]) {
    mplapackint n = 1;
    mplapackint incx = 1, incy = 1, STEP = 97, LOOPS = 3, TOTALSTEPS = 3092;

    REAL dummy;
    double elapsedtime;
    int i, p;
    int check_flag = 1;

    using Clock = std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::nanoseconds;

    ___MPLAPACK_INITIALIZE___

    REAL diff;
    double diffr;

    if (argc != 1) {
        for (i = 1; i < argc; i++) {
            if (strcmp("-N", argv[i]) == 0) {
                n = atoi(argv[++i]);
            } else if (strcmp("-STEP", argv[i]) == 0) {
                STEP = atoi(argv[++i]);
            } else if (strcmp("-NOCHECK", argv[i]) == 0) {
                check_flag = 0;
            } else if (strcmp("-LOOPS", argv[i]) == 0) {
                LOOPS = atoi(argv[++i]);
            } else if (strcmp("-TOTALSTEPS", argv[i]) == 0) {
                TOTALSTEPS = atoi(argv[++i]);
            }
        }
    }
    for (p = 0; p < TOTALSTEPS; p++) {
        REAL *x = new REAL[n];
        REAL *x_org = new REAL[n];
        REAL *y = new REAL[n];
        if (check_flag) {
            for (i = 0; i < n; i++) {
                x[i] = x_org[i] = randomnumber(dummy);
                y[i] = randomnumber(dummy);
            }
            auto t1 = Clock::now();
            Rcopy(n, x, incx, y, incy);
            auto t2 = Clock::now();
            elapsedtime = (double)duration_cast<nanoseconds>(t2 - t1).count() / 1.0e9;
            for (i = 0; i < n; i++) {
                x[i] = x_org[i] - y[i];
            }
            diff = Rasum(n, x, incx);
            diffr = cast2double(diff);
            printf("         n      MFLOATS      error\n");
            printf("%10d   %10.3f   %10.2e\n", (int)n, (double)n / elapsedtime * MFLOPS, diffr);
        } else {
            elapsedtime = 0.0;
            for (int j = 0; j < LOOPS; j++) {
                auto t1 = Clock::now();
                Rcopy(n, x, incx, y, incy);
                auto t2 = Clock::now();
                elapsedtime = elapsedtime + (double)duration_cast<nanoseconds>(t2 - t1).count() / 1.0e9;
            }
            elapsedtime = elapsedtime / (double)LOOPS;
            printf("         n        MFLOATS\n");
            printf("%10d     %10.3f\n", (int)n, (double)n / elapsedtime * MFLOPS);
        }
        delete[] y;
        delete[] x_org;
        delete[] x;
        n = n + STEP;
    }
}
