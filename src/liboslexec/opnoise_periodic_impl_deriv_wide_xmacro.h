/*
Copyright (c) 2009-2010 Sony Pictures Imageworks Inc., et al.
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of Sony Pictures Imageworks nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifdef __OSL_XMACRO_ARGS

#define __OSL_XMACRO_OPNAME __OSL_EXPAND(__OSL_XMACRO_ARG1 __OSL_XMACRO_ARGS)
#define __OSL_XMACRO_IMPLNAME __OSL_EXPAND(__OSL_XMACRO_ARG2 __OSL_XMACRO_ARGS)
#define __OSL_XMACRO_LANE_COUNT __OSL_EXPAND(__OSL_XMACRO_ARG3 __OSL_XMACRO_ARGS)

#endif

#ifndef __OSL_XMACRO_OPNAME
#error must define __OSL_XMACRO_OPNAME to name of noise operation before including this header
#endif

#ifndef __OSL_XMACRO_IMPLNAME
#error must define __OSL_XMACRO_IMPLNAME to name of noise implementation before including this header
#endif

#ifndef __OSL_XMACRO_LANE_COUNT
#error must define __OSL_XMACRO_LANE_COUNT to number of SIMD lanes before including this header
#endif

#define __WV __OSL_CONCAT3(w,__OSL_XMACRO_LANE_COUNT,v)
#define __WF __OSL_CONCAT3(w,__OSL_XMACRO_LANE_COUNT,f)

#define __WDV __OSL_CONCAT3(w,__OSL_XMACRO_LANE_COUNT,dv)
#define __WDF __OSL_CONCAT3(w,__OSL_XMACRO_LANE_COUNT,df)

#define __OSL_PNOISE_OP3(A, B, C) __OSL_CONCAT7(osl_,__OSL_XMACRO_OPNAME,_,A,B,C,_masked)
#define __OSL_PNOISE_OP5(A, B, C, D, E) __OSL_CONCAT9(osl_,__OSL_XMACRO_OPNAME,_,A,B,C,D,E,_masked)

OSL_SHADEOP void __OSL_PNOISE_OP3(__WDF,__WDF, __WF) (
    char *r_ptr, char *x_ptr, char *px_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wx(x_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpx(px_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(WidthT))*/
        OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<float> x = wx[i];
            float px = wpx[i];
            Dual2<float> result;
            impl(result, x, px);
            wresult[i] = result;
        }
    }
}

OSL_SHADEOP void __OSL_PNOISE_OP5(__WDF, __WDF, __WDF, __WF, __WF) (
    char *r_ptr, char *x_ptr, char *y_ptr, char *px_ptr, char *py_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wx(x_ptr);
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wy(y_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpx(px_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpy(py_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(WidthT))*/
        OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<float> x = wx[i];
            Dual2<float> y = wy[i];
            float px = wpx[i];
            float py = wpy[i];
            Dual2<float> result;
            impl(result, x, y, px, py);
            wresult[i] = result;
        }
    }
}


OSL_SHADEOP void __OSL_PNOISE_OP3(__WDF, __WDV, __WV) (
    char *r_ptr, char *p_ptr, char *pp_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wp(p_ptr);
    ConstWideAccessor<Vec3,__OSL_XMACRO_LANE_COUNT> wpp(pp_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(WidthT))*/
        OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<Vec3> p = wp[i];
            Vec3 pp = wpp[i];
            Dual2<float> result;
            impl(result, p, pp);
            wresult[i] = result;
        }
    }
}

OSL_SHADEOP void __OSL_PNOISE_OP5(__WDF, __WDV, __WDF, __WV, __WF) (
    char *r_ptr, char *p_ptr, char *t_ptr, char *pp_ptr, char *pt_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wp(p_ptr);
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wt(t_ptr);
    ConstWideAccessor<Vec3,__OSL_XMACRO_LANE_COUNT> wpp(pp_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpt(pt_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(WidthT))*/
        OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<Vec3> p = wp[i];
            Dual2<float> t = wt[i];
            Vec3 pp = wpp[i];
            float pt = wpt[i];
            Dual2<float> result;
            impl(result, p, t, pp, pt);
            wresult[i] = result;
        }
    }
}

#if 0


OSL_SHADEOP void __OSL_PNOISE_OP3(__WDV, __WDF, __WF) (
    char *r_ptr, char *x_ptr, char *px_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wx(x_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpx(px_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(WidthT))*/
        OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<float> x = wx[i];
            float px = wpx[i];
            Dual2<Vec3> result;
            impl(result, x, px);
            wresult[i] = result;
        }
    }
}

OSL_SHADEOP void __OSL_PNOISE_OP5(__WDV, __WDF, __WDF, __WF, __WF) (
    char *r_ptr, char *x_ptr, char *y_ptr, char *px_ptr, char *py_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wx(x_ptr);
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wy(y_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpx(px_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpy(py_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(WidthT))*/
        OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<float> x = wx[i];
            Dual2<float> y = wy[i];
            float px = wpx[i];
            float py = wpy[i];
            Dual2<Vec3> result;
            impl(result, x, y, px, py);
            wresult[i] = result;
        }
    }
}

OSL_SHADEOP void __OSL_PNOISE_OP3(__WDV, __WDV, __WV) (
    char *r_ptr, char *p_ptr, char *pp_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wp(p_ptr);
    ConstWideAccessor<Vec3,__OSL_XMACRO_LANE_COUNT> wpp(pp_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(WidthT))*/
        OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<Vec3> p = wp[i];
            Vec3 pp = wpp[i];
            Dual2<Vec3> result;
            impl(result, p, pp);
            wresult[i] = result;
        }
    }
}


OSL_SHADEOP void __OSL_PNOISE_OP5(__WDV, __WDV, __WDF, __WV, __WF) (
    char *r_ptr, char *p_ptr, char *t_ptr, char *pp_ptr, char *pt_ptr, unsigned int mask_value) {
    __OSL_XMACRO_IMPLNAME impl;
    MaskedAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wresult(r_ptr, Mask(mask_value));
    ConstWideAccessor<Dual2<Vec3>,__OSL_XMACRO_LANE_COUNT> wp(p_ptr);
    ConstWideAccessor<Dual2<float>,__OSL_XMACRO_LANE_COUNT> wt(t_ptr);
    ConstWideAccessor<Vec3,__OSL_XMACRO_LANE_COUNT> wpp(pp_ptr);
    ConstWideAccessor<float,__OSL_XMACRO_LANE_COUNT> wpt(pt_ptr);
    OSL_INTEL_PRAGMA(forceinline recursive)
    {
#ifndef __OSL_XMACRO_NO_SIMD_FOR_WDV_WDV_WDF
        /* TODO: figure out why clang is "loop not vectorized: cannot identify array bounds" */
        /*OSL_OMP_AND_CLANG_PRAGMA(clang loop vectorize(assume_safety) vectorize_width(__OSL_XMACRO_LANE_COUNT))*/
        /*OSL_OMP_NOT_CLANG_PRAGMA(omp simd simdlen(__OSL_XMACRO_LANE_COUNT))*/
        OSL_OMP_PRAGMA(omp simd)
#endif
        for(int i=0; i< __OSL_XMACRO_LANE_COUNT; ++i) {
            Dual2<Vec3> p = wp[i];
            Dual2<float> t = wt[i];
            Vec3 pp = wpp[i];
            float pt = wpt[i];
            Dual2<Vec3> result;
            impl(result, p, t, pp, pt);
            wresult[i] = result;
        }
    }
}
#endif

#undef __OSL_XMACRO_ARGS
#undef __OSL_XMACRO_OPNAME
#undef __OSL_XMACRO_IMPLNAME
#undef __OSL_XMACRO_LANE_COUNT
#undef __OSL_XMACRO_NO_SIMD_FOR_WDV_WDV_WDF

#undef __WV
#undef __WF
#undef __WDV
#undef __WDF
#undef __OSL_PNOISE_OP3
#undef __OSL_PNOISE_OP5
