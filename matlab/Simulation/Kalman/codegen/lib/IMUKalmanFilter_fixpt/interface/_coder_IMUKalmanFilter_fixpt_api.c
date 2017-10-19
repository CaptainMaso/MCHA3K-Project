/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IMUKalmanFilter_fixpt_api.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 18-Oct-2017 15:29:18
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_IMUKalmanFilter_fixpt_api.h"
#include "_coder_IMUKalmanFilter_fixpt_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
static const mxArray *eml_mx;
static const mxArray *b_eml_mx;
static const mxArray *c_eml_mx;
static const mxArray *d_eml_mx;
static const mxArray *e_eml_mx;
static const mxArray *f_eml_mx;
static const mxArray *g_eml_mx;
emlrtContext emlrtContextGlobal = { true, false, 131434U, NULL,
  "IMUKalmanFilter_fixpt", NULL, false, { 2045744189U, 2170104910U, 2743257031U,
    4284093946U }, NULL };

static emlrtMCInfo emlrtMCI = { -1, -1, "", "" };

/* Function Declarations */
static void IMUKalmanFilter_fixpt_once(const emlrtStack *sp);
static int16_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static const mxArray *b_emlrt_marshallOut(const emlrtStack *sp, const int16_T u);
static const mxArray *b_numerictype(const emlrtStack *sp, const char * b, real_T
  c, const char * d, real_T e, const char * f, real_T g, const char * h, real_T
  i, const char * j, real_T k, emlrtMCInfo *location);
static int16_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Bias_1,
  const char_T *identifier);
static const mxArray *c_emlrt_marshallOut(const emlrtStack *sp, const int32_T u
  [9]);
static const mxArray *c_numerictype(const emlrtStack *sp, const char * b,
  boolean_T c, const char * d, const char * e, const char * f, real_T g, const
  char * h, real_T i, const char * j, real_T k, const char * l, real_T m,
  emlrtMCInfo *location);
static int16_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *P_1, const
  char_T *identifier, int32_T y[9]);
static int16_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *Theta_1,
  const char_T *identifier);
static const mxArray *emlrt_marshallOut(const emlrtStack *sp, const int16_T u);
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, int32_T y[9]);
static const mxArray *fimath(const emlrtStack *sp, const char * b, const char
  * c, const char * d, const char * e, const char * f, const char * g, const
  char * h, const char * i, const char * j, const char * k, const char * l,
  real_T m, const char * n, real_T o, const char * p, real_T q, const char * r,
  real_T s, const char * t, real_T u, const char * v, real_T w, const char * x,
  real_T y, const char * ab, const char * bb, const char * cb, real_T db, const
  char * eb, real_T fb, const char * gb, real_T hb, const char * ib, real_T jb,
  const char * kb, real_T lb, const char * mb, real_T nb, const char * ob,
  real_T pb, const char * qb, boolean_T rb, emlrtMCInfo *location);
static uint16_T g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *qw,
  const char_T *identifier);
static uint16_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static uint16_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *qb,
  const char_T *identifier);
static uint16_T j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static uint16_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *rt,
  const char_T *identifier);
static uint16_T l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static int16_T m_emlrt_marshallIn(const mxArray *src);
static int16_T n_emlrt_marshallIn(const mxArray *src);
static const mxArray *numerictype(const emlrtStack *sp, const char * b, real_T c,
  const char * d, real_T e, const char * f, real_T g, const char * h, real_T i,
  emlrtMCInfo *location);
static void o_emlrt_marshallIn(const mxArray *src, int32_T ret[9]);
static uint16_T p_emlrt_marshallIn(const mxArray *src);
static uint16_T q_emlrt_marshallIn(const mxArray *src);
static uint16_T r_emlrt_marshallIn(const mxArray *src);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 * Return Type  : void
 */
static void IMUKalmanFilter_fixpt_once(const emlrtStack *sp)
{
  emlrtAssignP(&g_eml_mx, NULL);
  emlrtAssignP(&f_eml_mx, NULL);
  emlrtAssignP(&e_eml_mx, NULL);
  emlrtAssignP(&d_eml_mx, NULL);
  emlrtAssignP(&c_eml_mx, NULL);
  emlrtAssignP(&b_eml_mx, NULL);
  emlrtAssignP(&eml_mx, NULL);
  emlrtAssignP(&g_eml_mx, c_numerictype(sp, "SignednessBool", false,
    "Signedness", "Unsigned", "FractionLength", 4.0, "BinaryPoint", 4.0, "Slope",
    0.0625, "FixedExponent", -4.0, &emlrtMCI));
  emlrtAssignP(&f_eml_mx, c_numerictype(sp, "SignednessBool", false,
    "Signedness", "Unsigned", "FractionLength", 12.0, "BinaryPoint", 12.0,
    "Slope", 0.000244140625, "FixedExponent", -12.0, &emlrtMCI));
  emlrtAssignP(&e_eml_mx, c_numerictype(sp, "SignednessBool", false,
    "Signedness", "Unsigned", "FractionLength", 3.0, "BinaryPoint", 3.0, "Slope",
    0.125, "FixedExponent", -3.0, &emlrtMCI));
  emlrtAssignP(&d_eml_mx, b_numerictype(sp, "WordLength", 32.0, "FractionLength",
    4.0, "BinaryPoint", 4.0, "Slope", 0.0625, "FixedExponent", -4.0, &emlrtMCI));
  emlrtAssignP(&c_eml_mx, numerictype(sp, "FractionLength", 2.0, "BinaryPoint",
    2.0, "Slope", 0.25, "FixedExponent", -2.0, &emlrtMCI));
  emlrtAssignP(&b_eml_mx, numerictype(sp, "FractionLength", 0.0, "BinaryPoint",
    0.0, "Slope", 1.0, "FixedExponent", 0.0, &emlrtMCI));
  emlrtAssignP(&eml_mx, fimath(sp, "RoundMode", "floor", "RoundingMethod",
    "Floor", "OverflowMode", "wrap", "OverflowAction", "Wrap", "ProductMode",
    "FullPrecision", "ProductWordLength", 32.0, "MaxProductWordLength", 128.0,
    "ProductFractionLength", 30.0, "ProductFixedExponent", -30.0, "ProductSlope",
    9.3132257461547852E-10, "ProductSlopeAdjustmentFactor", 1.0, "ProductBias",
    0.0, "SumMode", "FullPrecision", "SumWordLength", 32.0, "MaxSumWordLength",
    128.0, "SumFractionLength", 30.0, "SumFixedExponent", -30.0, "SumSlope",
    9.3132257461547852E-10, "SumSlopeAdjustmentFactor", 1.0, "SumBias", 0.0,
    "CastBeforeSum", true, &emlrtMCI));
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : int16_T
 */
static int16_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  int16_T y;
  static const int32_T dims = 0;
  emlrtCheckFiR2012b(sp, parentId, u, false, 0, &dims, eml_mx, b_eml_mx);
  y = m_emlrt_marshallIn(emlrtAlias(u));
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const int16_T u
 * Return Type  : const mxArray *
 */
static const mxArray *b_emlrt_marshallOut(const emlrtStack *sp, const int16_T u)
{
  const mxArray *y;
  const mxArray *b_y;
  const mxArray *m1;
  y = NULL;
  b_y = NULL;
  m1 = emlrtCreateNumericMatrix(1, 1, (int16_T)mxINT16_CLASS, (int16_T)mxREAL);
  *(int16_T *)mxGetData(m1) = u;
  emlrtAssign(&b_y, m1);
  emlrtAssign(&y, emlrtCreateFIR2013b(sp, eml_mx, c_eml_mx, "simulinkarray", b_y,
    true, false));
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const char * b
 *                real_T c
 *                const char * d
 *                real_T e
 *                const char * f
 *                real_T g
 *                const char * h
 *                real_T i
 *                const char * j
 *                real_T k
 *                emlrtMCInfo *location
 * Return Type  : const mxArray *
 */
static const mxArray *b_numerictype(const emlrtStack *sp, const char * b, real_T
  c, const char * d, real_T e, const char * f, real_T g, const char * h, real_T
  i, const char * j, real_T k, emlrtMCInfo *location)
{
  const mxArray *pArrays[10];
  const mxArray *m5;
  pArrays[0] = emlrtCreateString(b);
  pArrays[1] = emlrtCreateDoubleScalar(c);
  pArrays[2] = emlrtCreateString(d);
  pArrays[3] = emlrtCreateDoubleScalar(e);
  pArrays[4] = emlrtCreateString(f);
  pArrays[5] = emlrtCreateDoubleScalar(g);
  pArrays[6] = emlrtCreateString(h);
  pArrays[7] = emlrtCreateDoubleScalar(i);
  pArrays[8] = emlrtCreateString(j);
  pArrays[9] = emlrtCreateDoubleScalar(k);
  return emlrtCallMATLABR2012b(sp, 1, &m5, 10, pArrays, "numerictype", true,
    location);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *Bias_1
 *                const char_T *identifier
 * Return Type  : int16_T
 */
static int16_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Bias_1,
  const char_T *identifier)
{
  int16_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(Bias_1), &thisId);
  emlrtDestroyArray(&Bias_1);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const int32_T u[9]
 * Return Type  : const mxArray *
 */
static const mxArray *c_emlrt_marshallOut(const emlrtStack *sp, const int32_T u
  [9])
{
  const mxArray *y;
  const mxArray *b_y;
  const mxArray *m2;
  static const int32_T iv0[2] = { 3, 3 };

  int32_T *pData;
  int32_T i;
  y = NULL;
  b_y = NULL;
  m2 = emlrtCreateNumericArray(2, iv0, (int16_T)mxINT32_CLASS, (int16_T)mxREAL);
  pData = (int32_T *)mxGetData(m2);
  for (i = 0; i < 9; i++) {
    pData[i] = u[i];
  }

  emlrtAssign(&b_y, m2);
  emlrtAssign(&y, emlrtCreateFIR2013b(sp, eml_mx, d_eml_mx, "simulinkarray", b_y,
    true, false));
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const char * b
 *                boolean_T c
 *                const char * d
 *                const char * e
 *                const char * f
 *                real_T g
 *                const char * h
 *                real_T i
 *                const char * j
 *                real_T k
 *                const char * l
 *                real_T m
 *                emlrtMCInfo *location
 * Return Type  : const mxArray *
 */
static const mxArray *c_numerictype(const emlrtStack *sp, const char * b,
  boolean_T c, const char * d, const char * e, const char * f, real_T g, const
  char * h, real_T i, const char * j, real_T k, const char * l, real_T m,
  emlrtMCInfo *location)
{
  const mxArray *pArrays[12];
  const mxArray *m6;
  pArrays[0] = emlrtCreateString(b);
  pArrays[1] = emlrtCreateLogicalScalar(c);
  pArrays[2] = emlrtCreateString(d);
  pArrays[3] = emlrtCreateString(e);
  pArrays[4] = emlrtCreateString(f);
  pArrays[5] = emlrtCreateDoubleScalar(g);
  pArrays[6] = emlrtCreateString(h);
  pArrays[7] = emlrtCreateDoubleScalar(i);
  pArrays[8] = emlrtCreateString(j);
  pArrays[9] = emlrtCreateDoubleScalar(k);
  pArrays[10] = emlrtCreateString(l);
  pArrays[11] = emlrtCreateDoubleScalar(m);
  return emlrtCallMATLABR2012b(sp, 1, &m6, 12, pArrays, "numerictype", true,
    location);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : int16_T
 */
static int16_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  int16_T y;
  static const int32_T dims = 0;
  emlrtCheckFiR2012b(sp, parentId, u, false, 0, &dims, eml_mx, c_eml_mx);
  y = n_emlrt_marshallIn(emlrtAlias(u));
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *P_1
 *                const char_T *identifier
 *                int32_T y[9]
 * Return Type  : void
 */
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *P_1, const
  char_T *identifier, int32_T y[9])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  f_emlrt_marshallIn(sp, emlrtAlias(P_1), &thisId, y);
  emlrtDestroyArray(&P_1);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *Theta_1
 *                const char_T *identifier
 * Return Type  : int16_T
 */
static int16_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *Theta_1,
  const char_T *identifier)
{
  int16_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(Theta_1), &thisId);
  emlrtDestroyArray(&Theta_1);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const int16_T u
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const emlrtStack *sp, const int16_T u)
{
  const mxArray *y;
  const mxArray *b_y;
  const mxArray *m0;
  y = NULL;
  b_y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, (int16_T)mxINT16_CLASS, (int16_T)mxREAL);
  *(int16_T *)mxGetData(m0) = u;
  emlrtAssign(&b_y, m0);
  emlrtAssign(&y, emlrtCreateFIR2013b(sp, eml_mx, b_eml_mx, "simulinkarray", b_y,
    true, false));
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                int32_T y[9]
 * Return Type  : void
 */
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, int32_T y[9])
{
  static const int32_T dims[2] = { 3, 3 };

  emlrtCheckFiR2012b(sp, parentId, u, false, 2, dims, eml_mx, d_eml_mx);
  o_emlrt_marshallIn(emlrtAlias(u), y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const char * b
 *                const char * c
 *                const char * d
 *                const char * e
 *                const char * f
 *                const char * g
 *                const char * h
 *                const char * i
 *                const char * j
 *                const char * k
 *                const char * l
 *                real_T m
 *                const char * n
 *                real_T o
 *                const char * p
 *                real_T q
 *                const char * r
 *                real_T s
 *                const char * t
 *                real_T u
 *                const char * v
 *                real_T w
 *                const char * x
 *                real_T y
 *                const char * ab
 *                const char * bb
 *                const char * cb
 *                real_T db
 *                const char * eb
 *                real_T fb
 *                const char * gb
 *                real_T hb
 *                const char * ib
 *                real_T jb
 *                const char * kb
 *                real_T lb
 *                const char * mb
 *                real_T nb
 *                const char * ob
 *                real_T pb
 *                const char * qb
 *                boolean_T rb
 *                emlrtMCInfo *location
 * Return Type  : const mxArray *
 */
static const mxArray *fimath(const emlrtStack *sp, const char * b, const char
  * c, const char * d, const char * e, const char * f, const char * g, const
  char * h, const char * i, const char * j, const char * k, const char * l,
  real_T m, const char * n, real_T o, const char * p, real_T q, const char * r,
  real_T s, const char * t, real_T u, const char * v, real_T w, const char * x,
  real_T y, const char * ab, const char * bb, const char * cb, real_T db, const
  char * eb, real_T fb, const char * gb, real_T hb, const char * ib, real_T jb,
  const char * kb, real_T lb, const char * mb, real_T nb, const char * ob,
  real_T pb, const char * qb, boolean_T rb, emlrtMCInfo *location)
{
  const mxArray *pArrays[42];
  const mxArray *m3;
  pArrays[0] = emlrtCreateString(b);
  pArrays[1] = emlrtCreateString(c);
  pArrays[2] = emlrtCreateString(d);
  pArrays[3] = emlrtCreateString(e);
  pArrays[4] = emlrtCreateString(f);
  pArrays[5] = emlrtCreateString(g);
  pArrays[6] = emlrtCreateString(h);
  pArrays[7] = emlrtCreateString(i);
  pArrays[8] = emlrtCreateString(j);
  pArrays[9] = emlrtCreateString(k);
  pArrays[10] = emlrtCreateString(l);
  pArrays[11] = emlrtCreateDoubleScalar(m);
  pArrays[12] = emlrtCreateString(n);
  pArrays[13] = emlrtCreateDoubleScalar(o);
  pArrays[14] = emlrtCreateString(p);
  pArrays[15] = emlrtCreateDoubleScalar(q);
  pArrays[16] = emlrtCreateString(r);
  pArrays[17] = emlrtCreateDoubleScalar(s);
  pArrays[18] = emlrtCreateString(t);
  pArrays[19] = emlrtCreateDoubleScalar(u);
  pArrays[20] = emlrtCreateString(v);
  pArrays[21] = emlrtCreateDoubleScalar(w);
  pArrays[22] = emlrtCreateString(x);
  pArrays[23] = emlrtCreateDoubleScalar(y);
  pArrays[24] = emlrtCreateString(ab);
  pArrays[25] = emlrtCreateString(bb);
  pArrays[26] = emlrtCreateString(cb);
  pArrays[27] = emlrtCreateDoubleScalar(db);
  pArrays[28] = emlrtCreateString(eb);
  pArrays[29] = emlrtCreateDoubleScalar(fb);
  pArrays[30] = emlrtCreateString(gb);
  pArrays[31] = emlrtCreateDoubleScalar(hb);
  pArrays[32] = emlrtCreateString(ib);
  pArrays[33] = emlrtCreateDoubleScalar(jb);
  pArrays[34] = emlrtCreateString(kb);
  pArrays[35] = emlrtCreateDoubleScalar(lb);
  pArrays[36] = emlrtCreateString(mb);
  pArrays[37] = emlrtCreateDoubleScalar(nb);
  pArrays[38] = emlrtCreateString(ob);
  pArrays[39] = emlrtCreateDoubleScalar(pb);
  pArrays[40] = emlrtCreateString(qb);
  pArrays[41] = emlrtCreateLogicalScalar(rb);
  return emlrtCallMATLABR2012b(sp, 1, &m3, 42, pArrays, "fimath", true, location);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *qw
 *                const char_T *identifier
 * Return Type  : uint16_T
 */
static uint16_T g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *qw,
  const char_T *identifier)
{
  uint16_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = h_emlrt_marshallIn(sp, emlrtAlias(qw), &thisId);
  emlrtDestroyArray(&qw);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : uint16_T
 */
static uint16_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  uint16_T y;
  static const int32_T dims = 0;
  emlrtCheckFiR2012b(sp, parentId, u, false, 0, &dims, eml_mx, e_eml_mx);
  y = p_emlrt_marshallIn(emlrtAlias(u));
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *qb
 *                const char_T *identifier
 * Return Type  : uint16_T
 */
static uint16_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *qb,
  const char_T *identifier)
{
  uint16_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = j_emlrt_marshallIn(sp, emlrtAlias(qb), &thisId);
  emlrtDestroyArray(&qb);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : uint16_T
 */
static uint16_T j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  uint16_T y;
  static const int32_T dims = 0;
  emlrtCheckFiR2012b(sp, parentId, u, false, 0, &dims, eml_mx, f_eml_mx);
  y = q_emlrt_marshallIn(emlrtAlias(u));
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *rt
 *                const char_T *identifier
 * Return Type  : uint16_T
 */
static uint16_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *rt,
  const char_T *identifier)
{
  uint16_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = l_emlrt_marshallIn(sp, emlrtAlias(rt), &thisId);
  emlrtDestroyArray(&rt);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : uint16_T
 */
static uint16_T l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  uint16_T y;
  static const int32_T dims = 0;
  emlrtCheckFiR2012b(sp, parentId, u, false, 0, &dims, eml_mx, g_eml_mx);
  y = r_emlrt_marshallIn(emlrtAlias(u));
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const mxArray *src
 * Return Type  : int16_T
 */
static int16_T m_emlrt_marshallIn(const mxArray *src)
{
  int16_T ret;
  const mxArray *mxInt;
  mxInt = emlrtImportFiIntArrayR2008b(src);
  ret = *(int16_T *)mxGetData(mxInt);
  emlrtDestroyArray(&mxInt);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray *src
 * Return Type  : int16_T
 */
static int16_T n_emlrt_marshallIn(const mxArray *src)
{
  int16_T ret;
  const mxArray *mxInt;
  mxInt = emlrtImportFiIntArrayR2008b(src);
  ret = *(int16_T *)mxGetData(mxInt);
  emlrtDestroyArray(&mxInt);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const char * b
 *                real_T c
 *                const char * d
 *                real_T e
 *                const char * f
 *                real_T g
 *                const char * h
 *                real_T i
 *                emlrtMCInfo *location
 * Return Type  : const mxArray *
 */
static const mxArray *numerictype(const emlrtStack *sp, const char * b, real_T c,
  const char * d, real_T e, const char * f, real_T g, const char * h, real_T i,
  emlrtMCInfo *location)
{
  const mxArray *pArrays[8];
  const mxArray *m4;
  pArrays[0] = emlrtCreateString(b);
  pArrays[1] = emlrtCreateDoubleScalar(c);
  pArrays[2] = emlrtCreateString(d);
  pArrays[3] = emlrtCreateDoubleScalar(e);
  pArrays[4] = emlrtCreateString(f);
  pArrays[5] = emlrtCreateDoubleScalar(g);
  pArrays[6] = emlrtCreateString(h);
  pArrays[7] = emlrtCreateDoubleScalar(i);
  return emlrtCallMATLABR2012b(sp, 1, &m4, 8, pArrays, "numerictype", true,
    location);
}

/*
 * Arguments    : const mxArray *src
 *                int32_T ret[9]
 * Return Type  : void
 */
static void o_emlrt_marshallIn(const mxArray *src, int32_T ret[9])
{
  const mxArray *mxInt;
  int32_T i0;
  int32_T i1;
  mxInt = emlrtImportFiIntArrayR2008b(src);
  for (i0 = 0; i0 < 3; i0++) {
    for (i1 = 0; i1 < 3; i1++) {
      ret[i1 + 3 * i0] = (*(int32_T (*)[9])mxGetData(mxInt))[i1 + 3 * i0];
    }
  }

  emlrtDestroyArray(&mxInt);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const mxArray *src
 * Return Type  : uint16_T
 */
static uint16_T p_emlrt_marshallIn(const mxArray *src)
{
  uint16_T ret;
  const mxArray *mxInt;
  mxInt = emlrtImportFiIntArrayR2008b(src);
  ret = *(uint16_T *)mxGetData(mxInt);
  emlrtDestroyArray(&mxInt);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray *src
 * Return Type  : uint16_T
 */
static uint16_T q_emlrt_marshallIn(const mxArray *src)
{
  uint16_T ret;
  const mxArray *mxInt;
  mxInt = emlrtImportFiIntArrayR2008b(src);
  ret = *(uint16_T *)mxGetData(mxInt);
  emlrtDestroyArray(&mxInt);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray *src
 * Return Type  : uint16_T
 */
static uint16_T r_emlrt_marshallIn(const mxArray *src)
{
  uint16_T ret;
  const mxArray *mxInt;
  mxInt = emlrtImportFiIntArrayR2008b(src);
  ret = *(uint16_T *)mxGetData(mxInt);
  emlrtDestroyArray(&mxInt);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray * const prhs[10]
 *                const mxArray *plhs[4]
 * Return Type  : void
 */
void IMUKalmanFilter_fixpt_api(const mxArray * const prhs[10], const mxArray
  *plhs[4])
{
  int16_T Theta_1;
  int16_T dTheta_1;
  int16_T Bias_1;
  int32_T P_1[9];
  int16_T Gyro;
  int16_T AccTheta;
  uint16_T qw;
  uint16_T qb;
  uint16_T rw;
  uint16_T rt;
  int16_T Theta;
  int16_T dTheta;
  int16_T Bias;
  int32_T P[9];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Marshall function inputs */
  Theta_1 = emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "Theta_1");
  dTheta_1 = emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "dTheta_1");
  Bias_1 = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "Bias_1");
  e_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "P_1", P_1);
  Gyro = emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "Gyro");
  AccTheta = emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "AccTheta");
  qw = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "qw");
  qb = i_emlrt_marshallIn(&st, emlrtAliasP(prhs[7]), "qb");
  rw = i_emlrt_marshallIn(&st, emlrtAliasP(prhs[8]), "rw");
  rt = k_emlrt_marshallIn(&st, emlrtAliasP(prhs[9]), "rt");

  /* Invoke the target function */
  IMUKalmanFilter_fixpt(Theta_1, dTheta_1, Bias_1, P_1, Gyro, AccTheta, qw, qb,
                        rw, rt, &Theta, &dTheta, &Bias, P);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(&st, Theta);
  plhs[1] = emlrt_marshallOut(&st, dTheta);
  plhs[2] = b_emlrt_marshallOut(&st, Bias);
  plhs[3] = c_emlrt_marshallOut(&st, P);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void IMUKalmanFilter_fixpt_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  IMUKalmanFilter_fixpt_xil_terminate();
  emlrtDestroyArray(&eml_mx);
  emlrtDestroyArray(&b_eml_mx);
  emlrtDestroyArray(&c_eml_mx);
  emlrtDestroyArray(&d_eml_mx);
  emlrtDestroyArray(&e_eml_mx);
  emlrtDestroyArray(&f_eml_mx);
  emlrtDestroyArray(&g_eml_mx);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void IMUKalmanFilter_fixpt_initialize(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 3U, "ForceOff");
  emlrtEnterRtStackR2012b(&st);
  if (emlrtFirstTimeR2012b(emlrtRootTLSGlobal)) {
    IMUKalmanFilter_fixpt_once(&st);
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void IMUKalmanFilter_fixpt_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_IMUKalmanFilter_fixpt_api.c
 *
 * [EOF]
 */
