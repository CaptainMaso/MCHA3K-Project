/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IMUKalmanFilter_fixpt.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 18-Oct-2017 15:29:18
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "IMUKalmanFilter_fixpt.h"

/* Function Declarations */
static boolean_T MultiWord2Bool(const unsigned long u[], int n);
static long MultiWord2sLong(const unsigned long u[]);
static unsigned long MultiWord2uLong(const unsigned long u[]);
static void MultiWordAdd(const unsigned long u1[], const unsigned long u2[],
  unsigned long y[], int n);
static void MultiWordNeg(const unsigned long u1[], unsigned long y[], int n);
static void MultiWordSetSignedMax(unsigned long y[], int n);
static void MultiWordSetSignedMin(unsigned long y[], int n);
static void MultiWordSetUnsignedMax(unsigned long y[], int n);
static void MultiWordSignedWrap(const unsigned long u1[], int n1, unsigned int
  n2, unsigned long y[]);
static void MultiWordSub(const unsigned long u1[], const unsigned long u2[],
  unsigned long y[], int n);
static void MultiWordUnsignedWrap(const unsigned long u1[], int n1, unsigned int
  n2, unsigned long y[]);
static unsigned long div_repeat_u32(unsigned long numerator, unsigned long
  denominator, unsigned int nRepeatSub);
static void sLong2MultiWord(long u, unsigned long y[], int n);
static void sMultiWord2MultiWord(const unsigned long u1[], int n1, unsigned long
  y[], int n);
static int sMultiWordCmp(const unsigned long u1[], const unsigned long u2[], int
  n);
static void sMultiWordDivFloor(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long b_y1[], int m1, unsigned long y2[], int m2,
  unsigned long t1[], int l1, unsigned long t2[], int l2);
static void sMultiWordDivZero(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long b_y1[], int m1, unsigned long y2[], int m2,
  unsigned long t1[], int l1, unsigned long t2[], int l2);
static boolean_T sMultiWordEq(const unsigned long u1[], const unsigned long u2[],
  int n);
static boolean_T sMultiWordGe(const unsigned long u1[], const unsigned long u2[],
  int n);
static void sMultiWordMul(const unsigned long u1[], int n1, const unsigned long
  u2[], int n2, unsigned long y[], int n);
static void sMultiWordShl(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n);
static void sMultiWordShr(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n);
static void ssuMultiWordMul(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long y[], int n);
static void uLong2MultiWord(unsigned long u, unsigned long y[], int n);
static void uMultiWord2MultiWord(const unsigned long u1[], int n1, unsigned long
  y[], int n);
static int uMultiWordDiv(unsigned long a[], int na, unsigned long b[], int nb,
  unsigned long q[], int nq, unsigned long r[], int nr);
static void uMultiWordDivZero(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long b_y1[], int m1, unsigned long y2[], int m2,
  unsigned long t1[], int l1, unsigned long t2[], int l2);
static void uMultiWordInc(unsigned long y[], int n);
static void uMultiWordMul(const unsigned long u1[], int n1, const unsigned long
  u2[], int n2, unsigned long y[], int n);
static void uMultiWordShl(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n);
static void uMultiWordShr(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n);

/* Function Definitions */

/*
 * Arguments    : const unsigned long u[]
 *                int n
 * Return Type  : boolean_T
 */
static boolean_T MultiWord2Bool(const unsigned long u[], int n)
{
  boolean_T y;
  int i;
  y = false;
  i = 0;
  while ((i < n) && (!y)) {
    if (u[i] != 0UL) {
      y = true;
    }

    i++;
  }

  return y;
}

/*
 * Arguments    : const unsigned long u[]
 * Return Type  : long
 */
static long MultiWord2sLong(const unsigned long u[])
{
  return (long)u[0];
}

/*
 * Arguments    : const unsigned long u[]
 * Return Type  : unsigned long
 */
static unsigned long MultiWord2uLong(const unsigned long u[])
{
  return u[0];
}

/*
 * Arguments    : const unsigned long u1[]
 *                const unsigned long u2[]
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void MultiWordAdd(const unsigned long u1[], const unsigned long u2[],
  unsigned long y[], int n)
{
  int i;
  unsigned long u1i;
  unsigned long yi;
  int carry = 0;
  int carry1;
  int carry2;
  for (i = 0; i < n; i++) {
    u1i = u1[i];
    yi = (u1i + u2[i]) + carry;
    y[i] = yi;
    carry1 = (yi <= u1i);
    carry2 = (yi < u1i);
    if ((unsigned int)carry != 0U) {
      carry = carry1;
    } else {
      carry = carry2;
    }
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void MultiWordNeg(const unsigned long u1[], unsigned long y[], int n)
{
  int i;
  unsigned long yi;
  int carry = 1;
  for (i = 0; i < n; i++) {
    yi = ~u1[i] + carry;
    y[i] = yi;
    carry = (yi < (unsigned long)carry);
  }
}

/*
 * Arguments    : unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void MultiWordSetSignedMax(unsigned long y[], int n)
{
  int n1;
  int i;
  n1 = n - 1;
  for (i = 0; i < n1; i++) {
    y[i] = MAX_uint32_T;
  }

  y[n1] = 2147483647UL;
}

/*
 * Arguments    : unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void MultiWordSetSignedMin(unsigned long y[], int n)
{
  int n1;
  int i;
  n1 = n - 1;
  for (i = 0; i < n1; i++) {
    y[i] = 0UL;
  }

  y[n1] = 2147483648UL;
}

/*
 * Arguments    : unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void MultiWordSetUnsignedMax(unsigned long y[], int n)
{
  int i;
  for (i = 0; i < n; i++) {
    y[i] = MAX_uint32_T;
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned int n2
 *                unsigned long y[]
 * Return Type  : void
 */
static void MultiWordSignedWrap(const unsigned long u1[], int n1, unsigned int
  n2, unsigned long y[])
{
  int n1m1;
  int i;
  unsigned int ns;
  unsigned long mask;
  unsigned long ys;
  n1m1 = n1 - 1;
  for (i = 0; i < n1m1; i++) {
    y[i] = u1[i];
  }

  ns = 31U - n2;
  mask = 1UL << ns;
  if ((u1[n1m1] & mask) != 0UL) {
    ys = MAX_uint32_T;
  } else {
    ys = 0UL;
  }

  mask = (mask << 1U) - 1UL;
  y[n1m1] = (u1[n1m1] & mask) | (~mask & ys);
}

/*
 * Arguments    : const unsigned long u1[]
 *                const unsigned long u2[]
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void MultiWordSub(const unsigned long u1[], const unsigned long u2[],
  unsigned long y[], int n)
{
  int i;
  unsigned long u1i;
  unsigned long yi;
  int borrow = 0;
  int borrow1;
  int borrow2;
  for (i = 0; i < n; i++) {
    u1i = u1[i];
    yi = (u1i - u2[i]) - borrow;
    y[i] = yi;
    borrow1 = (yi >= u1i);
    borrow2 = (yi > u1i);
    if ((unsigned int)borrow != 0U) {
      borrow = borrow1;
    } else {
      borrow = borrow2;
    }
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned int n2
 *                unsigned long y[]
 * Return Type  : void
 */
static void MultiWordUnsignedWrap(const unsigned long u1[], int n1, unsigned int
  n2, unsigned long y[])
{
  int n1m1;
  int i;
  unsigned int ns;
  unsigned long mask;
  n1m1 = n1 - 1;
  for (i = 0; i < n1m1; i++) {
    y[i] = u1[i];
  }

  ns = 32U - n2;
  mask = (1UL << ns) - 1UL;
  y[n1m1] = u1[n1m1] & mask;
}

/*
 * Arguments    : unsigned long numerator
 *                unsigned long denominator
 *                unsigned int nRepeatSub
 * Return Type  : unsigned long
 */
static unsigned long div_repeat_u32(unsigned long numerator, unsigned long
  denominator, unsigned int nRepeatSub)
{
  unsigned long quotient;
  unsigned int iRepeatSub;
  boolean_T numeratorExtraBit;
  if (denominator == 0UL) {
    quotient = MAX_uint32_T;
  } else {
    quotient = numerator / denominator;
    numerator %= denominator;
    for (iRepeatSub = 0U; iRepeatSub < nRepeatSub; iRepeatSub++) {
      numeratorExtraBit = (numerator >= 2147483648UL);
      numerator <<= 1UL;
      quotient <<= 1UL;
      if (numeratorExtraBit || (numerator >= denominator)) {
        quotient++;
        numerator -= denominator;
      }
    }
  }

  return quotient;
}

/*
 * Arguments    : long u
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void sLong2MultiWord(long u, unsigned long y[], int n)
{
  unsigned long yi;
  int i;
  y[0] = (unsigned long)u;
  if (u < 0L) {
    yi = MAX_uint32_T;
  } else {
    yi = 0UL;
  }

  for (i = 1; i < n; i++) {
    y[i] = yi;
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void sMultiWord2MultiWord(const unsigned long u1[], int n1, unsigned long
  y[], int n)
{
  int nm;
  int i;
  unsigned long u1i;
  if (n1 <= n) {
    nm = n1;
  } else {
    nm = n;
  }

  for (i = 0; i < nm; i++) {
    y[i] = u1[i];
  }

  if (n > n1) {
    if ((u1[n1 - 1] & 2147483648UL) != 0UL) {
      u1i = MAX_uint32_T;
    } else {
      u1i = 0UL;
    }

    for (i = nm; i < n; i++) {
      y[i] = u1i;
    }
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                const unsigned long u2[]
 *                int n
 * Return Type  : int
 */
static int sMultiWordCmp(const unsigned long u1[], const unsigned long u2[], int
  n)
{
  int y;
  unsigned long su1;
  unsigned long su2;
  int i;
  su1 = u1[n - 1] & 2147483648UL;
  su2 = u2[n - 1] & 2147483648UL;
  if ((su1 ^ su2) != 0UL) {
    if (su1 != 0UL) {
      y = -1;
    } else {
      y = 1;
    }
  } else {
    y = 0;
    i = n;
    while ((y == 0) && (i > 0)) {
      i--;
      su1 = u1[i];
      su2 = u2[i];
      if (su1 != su2) {
        if (su1 > su2) {
          y = 1;
        } else {
          y = -1;
        }
      }
    }
  }

  return y;
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                const unsigned long u2[]
 *                int n2
 *                unsigned long b_y1[]
 *                int m1
 *                unsigned long y2[]
 *                int m2
 *                unsigned long t1[]
 *                int l1
 *                unsigned long t2[]
 *                int l2
 * Return Type  : void
 */
static void sMultiWordDivFloor(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long b_y1[], int m1, unsigned long y2[], int m2,
  unsigned long t1[], int l1, unsigned long t2[], int l2)
{
  boolean_T numNeg;
  boolean_T denNeg;
  numNeg = ((u1[n1 - 1] & 2147483648UL) != 0UL);
  denNeg = ((u2[n2 - 1] & 2147483648UL) != 0UL);
  if (numNeg) {
    MultiWordNeg(u1, t1, n1);
  } else {
    sMultiWord2MultiWord(u1, n1, t1, l1);
  }

  if (denNeg) {
    MultiWordNeg(u2, t2, n2);
  } else {
    sMultiWord2MultiWord(u2, n2, t2, l2);
  }

  if (uMultiWordDiv(t1, l1, t2, l2, b_y1, m1, y2, m2) < 0) {
    if (numNeg) {
      MultiWordSetSignedMin(b_y1, m1);
    } else {
      MultiWordSetSignedMax(b_y1, m1);
    }
  } else {
    if ((boolean_T)(numNeg ^ denNeg)) {
      if (MultiWord2Bool(y2, m2)) {
        uMultiWordInc(b_y1, m1);
      }

      MultiWordNeg(b_y1, b_y1, m1);
    }
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                const unsigned long u2[]
 *                int n2
 *                unsigned long b_y1[]
 *                int m1
 *                unsigned long y2[]
 *                int m2
 *                unsigned long t1[]
 *                int l1
 *                unsigned long t2[]
 *                int l2
 * Return Type  : void
 */
static void sMultiWordDivZero(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long b_y1[], int m1, unsigned long y2[], int m2,
  unsigned long t1[], int l1, unsigned long t2[], int l2)
{
  boolean_T numNeg;
  boolean_T denNeg;
  numNeg = ((u1[n1 - 1] & 2147483648UL) != 0UL);
  denNeg = ((u2[n2 - 1] & 2147483648UL) != 0UL);
  if (numNeg) {
    MultiWordNeg(u1, t1, n1);
  } else {
    sMultiWord2MultiWord(u1, n1, t1, l1);
  }

  if (denNeg) {
    MultiWordNeg(u2, t2, n2);
  } else {
    sMultiWord2MultiWord(u2, n2, t2, l2);
  }

  if (uMultiWordDiv(t1, l1, t2, l2, b_y1, m1, y2, m2) < 0) {
    if (numNeg) {
      MultiWordSetSignedMin(b_y1, m1);
    } else {
      MultiWordSetSignedMax(b_y1, m1);
    }
  } else {
    if ((boolean_T)(numNeg ^ denNeg)) {
      MultiWordNeg(b_y1, b_y1, m1);
    }
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                const unsigned long u2[]
 *                int n
 * Return Type  : boolean_T
 */
static boolean_T sMultiWordEq(const unsigned long u1[], const unsigned long u2[],
  int n)
{
  return sMultiWordCmp(u1, u2, n) == 0;
}

/*
 * Arguments    : const unsigned long u1[]
 *                const unsigned long u2[]
 *                int n
 * Return Type  : boolean_T
 */
static boolean_T sMultiWordGe(const unsigned long u1[], const unsigned long u2[],
  int n)
{
  return sMultiWordCmp(u1, u2, n) >= 0;
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                const unsigned long u2[]
 *                int n2
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void sMultiWordMul(const unsigned long u1[], int n1, const unsigned long
  u2[], int n2, unsigned long y[], int n)
{
  boolean_T isNegative1;
  boolean_T isNegative2;
  int cb1;
  int k;
  int i;
  unsigned long cb;
  unsigned long u1i;
  unsigned int a1;
  unsigned long yk;
  unsigned int a0;
  int cb2;
  int nj;
  int j;
  unsigned int b1;
  unsigned int b0;
  unsigned long w01;
  unsigned long t;
  isNegative1 = ((u1[n1 - 1] & 2147483648UL) != 0UL);
  isNegative2 = ((u2[n2 - 1] & 2147483648UL) != 0UL);
  cb1 = 1;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0UL;
  }

  for (i = 0; i < n1; i++) {
    cb = 0UL;
    u1i = u1[i];
    if (isNegative1) {
      u1i = ~u1i + cb1;
      cb1 = (u1i < (unsigned long)cb1);
    }

    a1 = (unsigned int)(u1i >> 16U);
    a0 = (unsigned int)u1i & MAX_uint16_T;
    cb2 = 1;
    k = n - i;
    if (n2 <= k) {
      nj = n2;
    } else {
      nj = k;
    }

    k = i;
    for (j = 0; j < nj; j++) {
      yk = y[k];
      u1i = u2[j];
      if (isNegative2) {
        u1i = ~u1i + cb2;
        cb2 = (u1i < (unsigned long)cb2);
      }

      b1 = (unsigned int)(u1i >> 16U);
      b0 = (unsigned int)u1i & MAX_uint16_T;
      u1i = (unsigned long)a1 * b0;
      w01 = (unsigned long)a0 * b1;
      yk += cb;
      cb = (unsigned long)(yk < cb);
      t = (unsigned long)a0 * b0;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      t = u1i << 16U;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      t = w01 << 16U;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      y[k] = yk;
      cb += u1i >> 16U;
      cb += w01 >> 16U;
      cb += (unsigned long)a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  /* Apply sign */
  if (isNegative1 != isNegative2) {
    cb = 1UL;
    for (k = 0; k < n; k++) {
      yk = ~y[k] + cb;
      y[k] = yk;
      cb = (unsigned long)(yk < cb);
    }
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned int n2
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void sMultiWordShl(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n)
{
  int nb;
  unsigned long ys;
  int nc;
  unsigned long u1i;
  int i;
  unsigned int nl;
  unsigned int nr;
  unsigned long yi;
  nb = (int)n2 / 32;
  if ((u1[n1 - 1] & 2147483648UL) != 0UL) {
    ys = MAX_uint32_T;
  } else {
    ys = 0UL;
  }

  if (nb > n) {
    nc = n;
  } else {
    nc = nb;
  }

  u1i = 0UL;
  for (i = 0; i < nc; i++) {
    y[i] = 0UL;
  }

  if (nb < n) {
    nl = n2 - nb * 32U;
    nb += n1;
    if (nb > n) {
      nb = n;
    }

    nb -= i;
    if (nl > 0U) {
      nr = 32U - nl;
      for (nc = 0; nc < nb; nc++) {
        yi = u1i >> nr;
        u1i = u1[nc];
        y[i] = yi | u1i << nl;
        i++;
      }

      if (i < n) {
        y[i] = u1i >> nr | ys << nl;
        i++;
      }
    } else {
      for (nc = 0; nc < nb; nc++) {
        y[i] = u1[nc];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned int n2
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void sMultiWordShr(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n)
{
  int nb;
  int i;
  unsigned long ys;
  int nc;
  unsigned int nr;
  int i1;
  unsigned int nl;
  unsigned long u1i;
  unsigned long yi;
  nb = (int)n2 / 32;
  i = 0;
  if ((u1[n1 - 1] & 2147483648UL) != 0UL) {
    ys = MAX_uint32_T;
  } else {
    ys = 0UL;
  }

  if (nb < n1) {
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - nb * 32U;
    if (nr > 0U) {
      nl = 32U - nr;
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        yi = u1i >> nr;
        u1i = u1[i1];
        y[i] = yi | u1i << nl;
        i++;
      }

      yi = u1i >> nr;
      if (nc < n1) {
        u1i = u1[nc];
      } else {
        u1i = ys;
      }

      y[i] = yi | u1i << nl;
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                const unsigned long u2[]
 *                int n2
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void ssuMultiWordMul(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long y[], int n)
{
  boolean_T isNegative1;
  int cb1;
  int k;
  int i;
  unsigned long cb;
  unsigned long u1i;
  unsigned int a1;
  unsigned long yk;
  unsigned int a0;
  int nj;
  int j;
  unsigned int b1;
  unsigned int b0;
  unsigned long w01;
  unsigned long t;
  isNegative1 = ((u1[n1 - 1] & 2147483648UL) != 0UL);
  cb1 = 1;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0UL;
  }

  for (i = 0; i < n1; i++) {
    cb = 0UL;
    u1i = u1[i];
    if (isNegative1) {
      u1i = ~u1i + cb1;
      cb1 = (u1i < (unsigned long)cb1);
    }

    a1 = (unsigned int)(u1i >> 16U);
    a0 = (unsigned int)u1i & MAX_uint16_T;
    k = n - i;
    if (n2 <= k) {
      nj = n2;
    } else {
      nj = k;
    }

    k = i;
    for (j = 0; j < nj; j++) {
      yk = y[k];
      u1i = u2[j];
      b1 = (unsigned int)(u1i >> 16U);
      b0 = (unsigned int)u1i & MAX_uint16_T;
      u1i = (unsigned long)a1 * b0;
      w01 = (unsigned long)a0 * b1;
      yk += cb;
      cb = (unsigned long)(yk < cb);
      t = (unsigned long)a0 * b0;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      t = u1i << 16U;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      t = w01 << 16U;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      y[k] = yk;
      cb += u1i >> 16U;
      cb += w01 >> 16U;
      cb += (unsigned long)a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  /* Apply sign */
  if (isNegative1) {
    cb = 1UL;
    for (k = 0; k < n; k++) {
      yk = ~y[k] + cb;
      y[k] = yk;
      cb = (unsigned long)(yk < cb);
    }
  }
}

/*
 * Arguments    : unsigned long u
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void uLong2MultiWord(unsigned long u, unsigned long y[], int n)
{
  int i;
  y[0] = u;
  for (i = 1; i < n; i++) {
    y[i] = 0UL;
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void uMultiWord2MultiWord(const unsigned long u1[], int n1, unsigned long
  y[], int n)
{
  int nm;
  int i;
  if (n1 <= n) {
    nm = n1;
  } else {
    nm = n;
  }

  for (i = 0; i < nm; i++) {
    y[i] = u1[i];
  }

  if (n > n1) {
    for (i = nm; i < n; i++) {
      y[i] = 0UL;
    }
  }
}

/*
 * Arguments    : unsigned long a[]
 *                int na
 *                unsigned long b[]
 *                int nb
 *                unsigned long q[]
 *                int nq
 *                unsigned long r[]
 *                int nr
 * Return Type  : int
 */
static int uMultiWordDiv(unsigned long a[], int na, unsigned long b[], int nb,
  unsigned long q[], int nq, unsigned long r[], int nr)
{
  int y;
  int nzb;
  int tpi;
  int nza;
  int kr;
  int nb1;
  int na1;
  unsigned long ak;
  unsigned int kbb;
  unsigned long bk;
  unsigned long t;
  unsigned int kba;
  unsigned int nba;
  unsigned int nbb;
  unsigned long mask;
  unsigned int kbs;
  int kb;
  unsigned int tnb;
  int ka;
  unsigned long b_nb1;
  unsigned int nbq;
  unsigned long b_kb;
  int bb2;
  nzb = nb;
  tpi = nb - 1;
  while ((nzb > 0) && (b[tpi] == 0UL)) {
    nzb--;
    tpi--;
  }

  if (nzb > 0) {
    nza = na;
    for (tpi = 0; tpi < nq; tpi++) {
      q[tpi] = 0UL;
    }

    tpi = na - 1;
    while ((nza > 0) && (a[tpi] == 0UL)) {
      nza--;
      tpi--;
    }

    if ((nza > 0) && (nza >= nzb)) {
      nb1 = nzb - 1;
      na1 = nza - 1;
      for (kr = 0; kr < nr; kr++) {
        r[kr] = 0UL;
      }

      /* Quick return if dividend and divisor fit into single word. */
      if (nza == 1) {
        ak = a[0];
        bk = b[0];
        t = ak / bk;
        q[0] = t;
        r[0] = ak - t * bk;
        y = 7;
      } else {
        /* Remove leading zeros from both, dividend and divisor. */
        kbb = 1U;
        t = b[nb1] >> 1U;
        while (t != 0UL) {
          kbb++;
          t >>= 1U;
        }

        kba = 1U;
        t = a[na1] >> 1U;
        while (t != 0UL) {
          kba++;
          t >>= 1U;
        }

        /* Quick return if quotient is zero. */
        if ((nza > nzb) || (kba >= kbb)) {
          nba = na1 * 32U + kba;
          nbb = nb1 * 32U + kbb;

          /* Normalize b. */
          if (kbb != 32U) {
            bk = b[nb1];
            kbs = 32U - kbb;
            for (kb = nb1; kb > 0; kb--) {
              t = bk << kbs;
              bk = b[kb - 1];
              t |= bk >> kbb;
              b[kb] = t;
            }

            b[0] = bk << kbs;
            mask = ~((1UL << kbs) - 1UL);
          } else {
            mask = MAX_uint32_T;
          }

          /* Initialize quotient to zero. */
          tnb = 0U;
          y = 0;

          /* Until exit conditions have been met, do */
          do {
            /* Normalize a */
            if (kba != 32U) {
              kbs = 32U - kba;
              tnb += kbs;
              ak = a[na1];
              for (ka = na1; ka > 0; ka--) {
                t = ak << kbs;
                ak = a[ka - 1];
                t |= ak >> kba;
                a[ka] = t;
              }

              a[0] = ak << kbs;
            }

            /* Compare b against the a. */
            ak = a[na1];
            bk = b[nb1];
            if (nb1 == 0) {
              b_nb1 = mask;
            } else {
              b_nb1 = MAX_uint32_T;
            }

            if ((ak & b_nb1) == bk) {
              tpi = 0;
              ka = na1;
              kb = nb1;
              while ((tpi == 0) && (kb > 0)) {
                ka--;
                ak = a[ka];
                kb--;
                bk = b[kb];
                if (kb == 0) {
                  b_kb = mask;
                } else {
                  b_kb = MAX_uint32_T;
                }

                if ((ak & b_kb) != bk) {
                  if (ak > bk) {
                    tpi = 1;
                  } else {
                    tpi = -1;
                  }
                }
              }
            } else if (ak > bk) {
              tpi = 1;
            } else {
              tpi = -1;
            }

            /* If the remainder in a is still greater or equal to b, subtract normalized divisor from a. */
            if ((tpi >= 0) || (nba > nbb)) {
              nbq = nba - nbb;

              /* If the remainder and the divisor are equal, set remainder to zero. */
              if (tpi == 0) {
                ka = na1;
                for (kb = nb1; kb > 0; kb--) {
                  a[ka] = 0UL;
                  ka--;
                }

                a[ka] -= b[0];
              } else {
                /* Otherwise, subtract the divisor from the remainder */
                if (tpi < 0) {
                  ak = a[na1];
                  kba = 31U;
                  for (ka = na1; ka > 0; ka--) {
                    t = ak << 1U;
                    ak = a[ka - 1];
                    t |= ak >> 31U;
                    a[ka] = t;
                  }

                  a[0] = ak << 1U;
                  tnb++;
                  nbq--;
                }

                tpi = 0;
                ka = na1 - nb1;
                for (kb = 0; kb < nzb; kb++) {
                  bk = b[kb];
                  t = a[ka];
                  ak = t;
                  ak = (ak - bk) - tpi;
                  kr = (ak >= t);
                  bb2 = (ak > t);
                  if ((unsigned int)tpi != 0U) {
                    tpi = kr;
                  } else {
                    tpi = bb2;
                  }

                  a[ka] = ak;
                  ka++;
                }
              }

              /* Update the quotient. */
              tpi = (int)nbq / 32;
              nbq -= tpi * 32U;
              q[tpi] |= 1UL << nbq;

              /* Remove leading zeros from the remainder and check whether the exit conditions have been met. */
              tpi = na1;
              while ((nza > 0) && (a[tpi] == 0UL)) {
                nza--;
                tpi--;
              }

              if (nza >= nzb) {
                na1 = nza - 1;
                kba = 1U;
                t = a[na1] >> 1U;
                while (t != 0UL) {
                  kba++;
                  t >>= 1U;
                }

                nba = (na1 * 32U + kba) - tnb;
                if (nba < nbb) {
                  y = 2;
                }
              } else if (nza == 0) {
                y = 1;
              } else {
                na1 = nza - 1;
                y = 4;
              }
            } else {
              y = 3;
            }
          } while (y == 0);

          /* Return the remainder. */
          if (y == 1) {
            r[0] = a[0];
          } else {
            tpi = (int)tnb / 32;
            nbq = tnb - tpi * 32U;
            if (nbq == 0U) {
              ka = tpi;
              for (kr = 0; kr <= nb1; kr++) {
                r[kr] = a[ka];
                ka++;
              }
            } else {
              kbs = 32U - nbq;
              ak = a[tpi];
              kr = 0;
              for (ka = tpi + 1; ka <= na1; ka++) {
                t = ak >> nbq;
                ak = a[ka];
                t |= ak << kbs;
                r[kr] = t;
                kr++;
              }

              r[kr] = ak >> nbq;
            }
          }

          /* Restore b. */
          if (kbb != 32U) {
            bk = b[0];
            kbs = 32U - kbb;
            for (kb = 0; kb < nb1; kb++) {
              t = bk >> kbs;
              bk = b[kb + 1];
              t |= bk << kbb;
              b[kb] = t;
            }

            b[kb] = bk >> kbs;
          }
        } else {
          for (kr = 0; kr < nr; kr++) {
            r[kr] = a[kr];
          }

          y = 6;
        }
      }
    } else {
      for (kr = 0; kr < nr; kr++) {
        r[kr] = a[kr];
      }

      y = 5;
    }
  } else {
    y = -1;
  }

  return y;
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                const unsigned long u2[]
 *                int n2
 *                unsigned long b_y1[]
 *                int m1
 *                unsigned long y2[]
 *                int m2
 *                unsigned long t1[]
 *                int l1
 *                unsigned long t2[]
 *                int l2
 * Return Type  : void
 */
static void uMultiWordDivZero(const unsigned long u1[], int n1, const unsigned
  long u2[], int n2, unsigned long b_y1[], int m1, unsigned long y2[], int m2,
  unsigned long t1[], int l1, unsigned long t2[], int l2)
{
  uMultiWord2MultiWord(u1, n1, t1, l1);
  uMultiWord2MultiWord(u2, n2, t2, l2);
  if (uMultiWordDiv(t1, l1, t2, l2, b_y1, m1, y2, m2) < 0) {
    MultiWordSetUnsignedMax(b_y1, m1);
  }
}

/*
 * Arguments    : unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void uMultiWordInc(unsigned long y[], int n)
{
  int i;
  unsigned long yi;
  int carry = 1;
  for (i = 0; i < n; i++) {
    yi = y[i] + carry;
    y[i] = yi;
    carry = (yi < (unsigned long)carry);
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                const unsigned long u2[]
 *                int n2
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void uMultiWordMul(const unsigned long u1[], int n1, const unsigned long
  u2[], int n2, unsigned long y[], int n)
{
  int k;
  int i;
  unsigned long cb;
  unsigned long u1i;
  unsigned int a1;
  unsigned int a0;
  int nj;
  int j;
  unsigned long yk;
  unsigned int b1;
  unsigned int b0;
  unsigned long w01;
  unsigned long t;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0UL;
  }

  for (i = 0; i < n1; i++) {
    cb = 0UL;
    u1i = u1[i];
    a1 = (unsigned int)(u1i >> 16U);
    a0 = (unsigned int)u1i & MAX_uint16_T;
    k = n - i;
    if (n2 <= k) {
      nj = n2;
    } else {
      nj = k;
    }

    k = i;
    for (j = 0; j < nj; j++) {
      yk = y[k];
      u1i = u2[j];
      b1 = (unsigned int)(u1i >> 16U);
      b0 = (unsigned int)u1i & MAX_uint16_T;
      u1i = (unsigned long)a1 * b0;
      w01 = (unsigned long)a0 * b1;
      yk += cb;
      cb = (unsigned long)(yk < cb);
      t = (unsigned long)a0 * b0;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      t = u1i << 16U;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      t = w01 << 16U;
      yk += t;
      cb = (unsigned long)((int)cb + (yk < t));
      y[k] = yk;
      cb += u1i >> 16U;
      cb += w01 >> 16U;
      cb += (unsigned long)a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned int n2
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void uMultiWordShl(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n)
{
  int nb;
  unsigned long ys;
  int nc;
  unsigned long u1i;
  int i;
  unsigned int nl;
  unsigned int nr;
  unsigned long yi;
  nb = (int)n2 / 32;
  if ((u1[n1 - 1] & 2147483648UL) != 0UL) {
    ys = MAX_uint32_T;
  } else {
    ys = 0UL;
  }

  if (nb > n) {
    nc = n;
  } else {
    nc = nb;
  }

  u1i = 0UL;
  for (i = 0; i < nc; i++) {
    y[i] = 0UL;
  }

  if (nb < n) {
    nl = n2 - nb * 32U;
    nb += n1;
    if (nb > n) {
      nb = n;
    }

    nb -= i;
    if (nl > 0U) {
      nr = 32U - nl;
      for (nc = 0; nc < nb; nc++) {
        yi = u1i >> nr;
        u1i = u1[nc];
        y[i] = yi | u1i << nl;
        i++;
      }

      if (i < n) {
        y[i] = u1i >> nr | ys << nl;
        i++;
      }
    } else {
      for (nc = 0; nc < nb; nc++) {
        y[i] = u1[nc];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

/*
 * Arguments    : const unsigned long u1[]
 *                int n1
 *                unsigned int n2
 *                unsigned long y[]
 *                int n
 * Return Type  : void
 */
static void uMultiWordShr(const unsigned long u1[], int n1, unsigned int n2,
  unsigned long y[], int n)
{
  int nb;
  int i;
  int nc;
  unsigned int nr;
  int i1;
  unsigned int nl;
  unsigned long u1i;
  unsigned long yi;
  nb = (int)n2 / 32;
  i = 0;
  if (nb < n1) {
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - nb * 32U;
    if (nr > 0U) {
      nl = 32U - nr;
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        yi = u1i >> nr;
        u1i = u1[i1];
        y[i] = yi | u1i << nl;
        i++;
      }

      yi = u1i >> nr;
      if (nc < n1) {
        yi |= u1[nc] << nl;
      }

      y[i] = yi;
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = 0UL;
    i++;
  }
}

/*
 * Arguments    : int Theta_1
 *                int dTheta_1
 *                int Bias_1
 *                const long P_1[9]
 *                int Gyro
 *                int AccTheta
 *                unsigned int qw
 *                unsigned int qb
 *                unsigned int rw
 *                unsigned int rt
 *                int *Theta
 *                int *dTheta
 *                int *Bias
 *                long P[9]
 * Return Type  : void
 */
void IMUKalmanFilter_fixpt(int Theta_1, int dTheta_1, int Bias_1, const long
  P_1[9], int Gyro, int AccTheta, unsigned int qw, unsigned int qb, unsigned int
  rw, unsigned int rt, int *Theta, int *dTheta, int *Bias, long P[9])
{
  int Pwb_p;
  unsigned int Pwt_p;
  unsigned int Pww_p;
  uint64m_T r0;
  uint64m_T r1;
  uint64m_T r2;
  uint64m_T r3;
  uint64m_T r4;
  uint64m_T r5;
  unsigned int Pbb_p;
  int64m_T r6;
  int64m_T r7;
  int64m_T r8;
  int64m_T r9;
  int64m_T r10;
  int64m_T r11;
  int64m_T r12;
  int Ptb_p;
  uint64m_T r13;
  uint64m_T r14;
  uint64m_T r15;
  uint64m_T r16;
  uint64m_T r17;
  unsigned int b_Pwt_p;
  unsigned int b_Pww_p;
  unsigned int u0;
  long i0;
  long i1;
  long i2;
  long i3;
  long c;
  long i4;
  long b_c;
  long c_c;
  int64m_T r18;
  static int64m_T r19 = { { 0UL, 4026531840UL } };

  static int64m_T r20 = { { MAX_uint32_T, 268435455UL } };

  int96m_T r21;
  unsigned int kw;
  long i5;
  long i6;
  long i7;
  long i8;
  long d_c;
  long e_c;
  int64m_T r22;
  int64m_T r23;
  int kt;
  long i9;
  long i10;
  long i11;
  long i12;
  long f_c;
  long g_c;
  int64m_T r24;
  int64m_T r25;
  unsigned int kb;
  int i13;
  int i14;
  long i15;
  long i16;
  long h_c;
  long i_c;
  long i17;
  long i18;
  long i19;
  long i20;
  long i21;
  int64m_T r26;
  int64m_T r27;
  int64m_T r28;
  int64m_T r29;
  int64m_T r30;
  int64m_T r31;
  int64m_T r32;
  int64m_T r33;
  unsigned long u1;
  unsigned long u2;
  long i22;
  long i23;
  long i24;
  long i25;
  long i26;
  long i27;
  long i28;
  int96m_T r34;
  int96m_T r35;
  int96m_T r36;
  int96m_T r37;
  int96m_T r38;
  int96m_T r39;
  int96m_T r40;
  int96m_T r41;
  int96m_T r42;
  int96m_T r43;
  uint128m_T r44;
  uint128m_T r45;
  static uint64m_T r46 = { { 171798691UL, 41943UL } };

  uint128m_T r47;
  uint128m_T r48;
  uint128m_T r49;
  uint128m_T r50;
  int96m_T r51;
  int96m_T r52;
  static int64m_T r53 = { { 2233382993UL, 20971UL } };

  int64m_T r54;
  static int64m_T r55 = { { 0UL, 0UL } };

  int128m_T r56;
  int128m_T r57;
  int64m_T r58;
  static int64m_T r59 = { { 0UL, 4294934528UL } };

  static int64m_T r60 = { { MAX_uint32_T, 32767UL } };

  int160m_T r61;
  int128m_T r62;
  int128m_T r63;
  int128m_T r64;
  int96m_T r65;
  int96m_T r66;
  int96m_T r67;
  int b_Ptb_p;
  int64m_T r68;
  int64m_T r69;
  int64m_T r70;
  int64m_T r71;
  int64m_T r72;
  int64m_T r73;
  int64m_T r74;
  int64m_T r75;
  int64m_T r76;
  int64m_T r77;
  uint96m_T r78;
  uint96m_T r79;
  uint96m_T r80;
  uint96m_T r81;
  uint96m_T r82;
  uint96m_T r83;
  uint96m_T r84;
  uint96m_T r85;
  uint96m_T r86;
  uint96m_T r87;
  unsigned int Ptt_p;
  long i29;
  int b_Pwb_p;
  int64m_T r88;
  int64m_T r89;
  int64m_T r90;
  int96m_T r91;
  int96m_T r92;
  uint64m_T r93;
  unsigned int b_kt;
  int64m_T r94;
  static int64m_T r95 = { { 0UL, 4294836224UL } };

  static int64m_T r96 = { { MAX_uint32_T, 131071UL } };

  int b_kb;
  long i30;
  long i31;
  int64m_T j_c;
  long i32;
  long i33;
  int64m_T k_c;
  static uint64m_T r97 = { { 67108864UL, 0UL } };

  static int64m_T r98 = { { 67108864UL, 0UL } };

  unsigned int Pwt;
  int64m_T r99;
  int64m_T r100;
  static int64m_T r101 = { { 268435456UL, 0UL } };

  int96m_T r102;
  int96m_T r103;
  int Pwb;
  int64m_T l_c;
  int64m_T r104;
  int64m_T r105;
  int Ptb;
  int64m_T m_c;
  long i34;
  long i35;
  int64m_T r106;
  int64m_T r107;
  int64m_T r108;
  int96m_T r109;
  int96m_T r110;
  int64m_T r111;
  int64m_T r112;
  int96m_T r113;
  int96m_T r114;

  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /*                                                                           % */
  /*            Generated by MATLAB 9.0 and Fixed-Point Designer 5.2           % */
  /*                                                                           % */
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /*  State Update */
  /*  Covariance Update */
  Pwb_p = (int)P_1[6] << 8;
  Pwt_p = (unsigned int)P_1[3] << 6;
  Pww_p = (unsigned int)P_1[0] << 9;
  *dTheta = (dTheta_1 << 4) >> 4;
  *Bias = (Bias_1 << 7) >> 7;
  uLong2MultiWord((unsigned int)P_1[8] << 9, &r0.chunks[0U], 2);
  uMultiWordShl(&r0.chunks[0U], 2, 22U, &r1.chunks[0U], 2);
  MultiWordUnsignedWrap(&r1.chunks[0U], 2, 25U, &r2.chunks[0U]);
  uLong2MultiWord(41943UL * qb, &r0.chunks[0U], 2);
  MultiWordUnsignedWrap(&r0.chunks[0U], 2, 25U, &r1.chunks[0U]);
  MultiWordAdd(&r2.chunks[0U], &r1.chunks[0U], &r3.chunks[0U], 2);
  MultiWordUnsignedWrap(&r3.chunks[0U], 2, 25U, &r4.chunks[0U]);
  uMultiWordShr(&r4.chunks[0U], 2, 22U, &r5.chunks[0U], 2);
  Pbb_p = (unsigned int)((int)((unsigned int)MultiWord2uLong(&r5.chunks[0U]) >>
    4) >> 5) << 9;
  sLong2MultiWord(20971L * Pwb_p, &r6.chunks[0U], 2);
  MultiWordSignedWrap(&r6.chunks[0U], 2, 22U, &r7.chunks[0U]);
  sLong2MultiWord((int)P_1[7] << 5, &r8.chunks[0U], 2);
  sMultiWordShl(&r8.chunks[0U], 2, 25U, &r9.chunks[0U], 2);
  MultiWordSignedWrap(&r9.chunks[0U], 2, 22U, &r6.chunks[0U]);
  MultiWordAdd(&r7.chunks[0U], &r6.chunks[0U], &r10.chunks[0U], 2);
  MultiWordSignedWrap(&r10.chunks[0U], 2, 22U, &r11.chunks[0U]);
  sMultiWordShr(&r11.chunks[0U], 2, 25U, &r12.chunks[0U], 2);
  Ptb_p = ((int)MultiWord2sLong(&r12.chunks[0U]) >> 5) << 5;
  Pwb_p = (Pwb_p >> 8) << 8;
  uLong2MultiWord(41943UL * Pww_p, &r13.chunks[0U], 2);
  MultiWordUnsignedWrap(&r13.chunks[0U], 2, 21U, &r14.chunks[0U]);
  uLong2MultiWord(Pwt_p, &r15.chunks[0U], 2);
  uMultiWordShl(&r15.chunks[0U], 2, 26U, &r16.chunks[0U], 2);
  MultiWordUnsignedWrap(&r16.chunks[0U], 2, 21U, &r13.chunks[0U]);
  MultiWordAdd(&r14.chunks[0U], &r13.chunks[0U], &r17.chunks[0U], 2);
  MultiWordUnsignedWrap(&r17.chunks[0U], 2, 21U, &r0.chunks[0U]);
  MultiWordUnsignedWrap(&r0.chunks[0U], 2, 20U, &r1.chunks[0U]);
  uLong2MultiWord(53687UL * qw, &r14.chunks[0U], 2);
  uMultiWordShl(&r14.chunks[0U], 2, 1U, &r17.chunks[0U], 2);
  MultiWordUnsignedWrap(&r17.chunks[0U], 2, 20U, &r0.chunks[0U]);
  MultiWordAdd(&r1.chunks[0U], &r0.chunks[0U], &r2.chunks[0U], 2);
  MultiWordUnsignedWrap(&r2.chunks[0U], 2, 20U, &r3.chunks[0U]);
  uMultiWordShr(&r3.chunks[0U], 2, 26U, &r4.chunks[0U], 2);
  b_Pwt_p = (unsigned int)((int)((unsigned int)MultiWord2uLong(&r4.chunks[0U]) >>
    5) >> 1) << 6;
  uLong2MultiWord((unsigned long)Pww_p << 13, &r17.chunks[0U], 2);
  MultiWordUnsignedWrap(&r17.chunks[0U], 2, 31U, &r0.chunks[0U]);
  uLong2MultiWord(41943UL * qw, &r14.chunks[0U], 2);
  MultiWordUnsignedWrap(&r14.chunks[0U], 2, 31U, &r17.chunks[0U]);
  MultiWordAdd(&r0.chunks[0U], &r17.chunks[0U], &r1.chunks[0U], 2);
  MultiWordUnsignedWrap(&r1.chunks[0U], 2, 31U, &r2.chunks[0U]);
  uMultiWordShr(&r2.chunks[0U], 2, 13U, &r3.chunks[0U], 2);
  b_Pww_p = (unsigned int)((int)((unsigned int)MultiWord2uLong(&r3.chunks[0U]) >>
    8) >> 1) << 9;

  /*     %% Kalman Gain Update */
  u0 = b_Pww_p;
  i0 = (long)Pwb_p << 1;
  if ((i0 & 8388608L) != 0L) {
    i1 = i0 | -8388608L;
  } else {
    i1 = i0 & 8388607L;
  }

  i0 = i1 << 1;
  if ((i0 & 67108864L) != 0L) {
    i2 = i0 | -67108864L;
  } else {
    i2 = i0 & 67108863L;
  }

  i0 = u0 + i2;
  if ((i0 & 67108864L) != 0L) {
    i0 |= -67108864L;
  } else {
    i0 &= 67108863L;
  }

  u0 = Pbb_p;
  if ((i0 & 268435456L) != 0L) {
    i3 = i0 | -268435456L;
  } else {
    i3 = i0 & 268435455L;
  }

  i0 = i3 + u0;
  if ((i0 & 268435456L) != 0L) {
    i0 |= -268435456L;
  } else {
    i0 &= 268435455L;
  }

  c = (long)(unsigned int)div_repeat_u32((unsigned long)rw << 16, 41943UL, 7U) <<
    1;
  if ((i0 & 1073741824L) != 0L) {
    i4 = i0 | -1073741824L;
  } else {
    i4 = i0 & 1073741823L;
  }

  if ((c & 1073741824L) != 0L) {
    b_c = c | -1073741824L;
  } else {
    b_c = c;
  }

  i0 = i4 + b_c;
  if ((i0 & 1073741824L) != 0L) {
    i0 |= -1073741824L;
  } else {
    i0 &= 1073741823L;
  }

  u0 = b_Pww_p;
  c = (long)Pwb_p << 1;
  if ((c & 262144L) != 0L) {
    c_c = c | -262144L;
  } else {
    c_c = c & 262143L;
  }

  c = u0 + c_c;
  if ((c & 262144L) != 0L) {
    c |= -262144L;
  } else {
    c &= 262143L;
  }

  if (i0 == 0L) {
    if (c >= 0L) {
      r18 = r20;
    } else {
      r18 = r19;
    }
  } else {
    sLong2MultiWord(c, &r7.chunks[0U], 2);
    sMultiWordShl(&r7.chunks[0U], 2, 24U, &r10.chunks[0U], 2);
    sLong2MultiWord(i0, &r7.chunks[0U], 2);
    sMultiWordDivZero(&r10.chunks[0U], 2, &r7.chunks[0U], 2, &r21.chunks[0U], 3,
                      &r6.chunks[0U], 2, &r9.chunks[0U], 2, &r8.chunks[0U], 2);
    sMultiWord2MultiWord(&r21.chunks[0U], 3, &r11.chunks[0U], 2);
    MultiWordSignedWrap(&r11.chunks[0U], 2, 3U, &r18.chunks[0U]);
  }

  sMultiWordShr(&r18.chunks[0U], 2, 8U, &r11.chunks[0U], 2);
  kw = (unsigned int)MultiWord2uLong(&r11.chunks[0U]);
  i0 = (long)Pwb_p << 1;
  if ((i0 & 8388608L) != 0L) {
    i5 = i0 | -8388608L;
  } else {
    i5 = i0 & 8388607L;
  }

  i0 = i5 << 1;
  if ((i0 & 67108864L) != 0L) {
    i6 = i0 | -67108864L;
  } else {
    i6 = i0 & 67108863L;
  }

  i0 = u0 + i6;
  if ((i0 & 67108864L) != 0L) {
    i0 |= -67108864L;
  } else {
    i0 &= 67108863L;
  }

  u0 = Pbb_p;
  if ((i0 & 268435456L) != 0L) {
    i7 = i0 | -268435456L;
  } else {
    i7 = i0 & 268435455L;
  }

  i0 = i7 + u0;
  if ((i0 & 268435456L) != 0L) {
    i0 |= -268435456L;
  } else {
    i0 &= 268435455L;
  }

  c = (long)(unsigned int)div_repeat_u32((unsigned long)rw << 16, 41943UL, 7U) <<
    1;
  if ((i0 & 1073741824L) != 0L) {
    i8 = i0 | -1073741824L;
  } else {
    i8 = i0 & 1073741823L;
  }

  if ((c & 1073741824L) != 0L) {
    d_c = c | -1073741824L;
  } else {
    d_c = c;
  }

  i0 = i8 + d_c;
  if ((i0 & 1073741824L) != 0L) {
    i0 |= -1073741824L;
  } else {
    i0 &= 1073741823L;
  }

  u0 = b_Pwt_p;
  c = (long)Ptb_p << 1;
  if ((c & 262144L) != 0L) {
    e_c = c | -262144L;
  } else {
    e_c = c & 262143L;
  }

  c = u0 + e_c;
  if ((c & 262144L) != 0L) {
    c |= -262144L;
  } else {
    c &= 262143L;
  }

  if (i0 == 0L) {
    if (c >= 0L) {
      r22 = r20;
    } else {
      r22 = r19;
    }
  } else {
    sLong2MultiWord(c, &r6.chunks[0U], 2);
    sMultiWordShl(&r6.chunks[0U], 2, 27U, &r7.chunks[0U], 2);
    sLong2MultiWord(i0, &r6.chunks[0U], 2);
    sMultiWordDivZero(&r7.chunks[0U], 2, &r6.chunks[0U], 2, &r21.chunks[0U], 3,
                      &r9.chunks[0U], 2, &r8.chunks[0U], 2, &r23.chunks[0U], 2);
    sMultiWord2MultiWord(&r21.chunks[0U], 3, &r10.chunks[0U], 2);
    MultiWordSignedWrap(&r10.chunks[0U], 2, 3U, &r22.chunks[0U]);
  }

  sMultiWordShr(&r22.chunks[0U], 2, 8U, &r10.chunks[0U], 2);
  kt = (int)MultiWord2sLong(&r10.chunks[0U]);
  u0 = b_Pww_p;
  i0 = (long)Pwb_p << 1;
  if ((i0 & 8388608L) != 0L) {
    i9 = i0 | -8388608L;
  } else {
    i9 = i0 & 8388607L;
  }

  i0 = i9 << 1;
  if ((i0 & 67108864L) != 0L) {
    i10 = i0 | -67108864L;
  } else {
    i10 = i0 & 67108863L;
  }

  i0 = u0 + i10;
  if ((i0 & 67108864L) != 0L) {
    i0 |= -67108864L;
  } else {
    i0 &= 67108863L;
  }

  u0 = Pbb_p;
  if ((i0 & 268435456L) != 0L) {
    i11 = i0 | -268435456L;
  } else {
    i11 = i0 & 268435455L;
  }

  i0 = i11 + u0;
  if ((i0 & 268435456L) != 0L) {
    i0 |= -268435456L;
  } else {
    i0 &= 268435455L;
  }

  c = (long)(unsigned int)div_repeat_u32((unsigned long)rw << 16, 41943UL, 7U) <<
    1;
  if ((i0 & 1073741824L) != 0L) {
    i12 = i0 | -1073741824L;
  } else {
    i12 = i0 & 1073741823L;
  }

  if ((c & 1073741824L) != 0L) {
    f_c = c | -1073741824L;
  } else {
    f_c = c;
  }

  i0 = i12 + f_c;
  if ((i0 & 1073741824L) != 0L) {
    i0 |= -1073741824L;
  } else {
    i0 &= 1073741823L;
  }

  c = (long)Pwb_p << 1;
  if ((c & 262144L) != 0L) {
    g_c = c | -262144L;
  } else {
    g_c = c & 262143L;
  }

  c = g_c + u0;
  if ((c & 262144L) != 0L) {
    c |= -262144L;
  } else {
    c &= 262143L;
  }

  if (i0 == 0L) {
    if (c >= 0L) {
      r24 = r20;
    } else {
      r24 = r19;
    }
  } else {
    sLong2MultiWord(c, &r9.chunks[0U], 2);
    sMultiWordShl(&r9.chunks[0U], 2, 24U, &r6.chunks[0U], 2);
    sLong2MultiWord(i0, &r9.chunks[0U], 2);
    sMultiWordDivZero(&r6.chunks[0U], 2, &r9.chunks[0U], 2, &r21.chunks[0U], 3,
                      &r8.chunks[0U], 2, &r23.chunks[0U], 2, &r25.chunks[0U], 2);
    sMultiWord2MultiWord(&r21.chunks[0U], 3, &r7.chunks[0U], 2);
    MultiWordSignedWrap(&r7.chunks[0U], 2, 3U, &r24.chunks[0U]);
  }

  sMultiWordShr(&r24.chunks[0U], 2, 4U, &r7.chunks[0U], 2);
  kb = (unsigned int)MultiWord2uLong(&r7.chunks[0U]);

  /*     %% State Update */
  i13 = Gyro;
  i14 = *dTheta;
  if ((i13 & 65536L) != 0L) {
    i15 = i13 | -65536L;
  } else {
    i15 = i13 & 65535L;
  }

  if ((i14 & 65536L) != 0L) {
    i16 = i14 | -65536L;
  } else {
    i16 = i14 & 65535L;
  }

  i0 = i15 - i16;
  if ((i0 & 65536L) != 0L) {
    c = i0 | -65536L;
  } else {
    c = i0 & 65535L;
  }

  /*     %% Covariance Update */
  u0 = kw;
  i0 = u0 - 65536L;
  if ((i0 & 33554432L) != 0L) {
    h_c = i0 | -33554432L;
  } else {
    h_c = i0 & 33554431L;
  }

  u0 = kb;
  i0 = u0 - 1048576L;
  if ((i0 & 536870912L) != 0L) {
    i_c = i0 | -536870912L;
  } else {
    i_c = i0 & 536870911L;
  }

  if ((i13 & 65536L) != 0L) {
    i17 = i13 | -65536L;
  } else {
    i17 = i13 & 65535L;
  }

  if ((i14 & 65536L) != 0L) {
    i18 = i14 | -65536L;
  } else {
    i18 = i14 & 65535L;
  }

  i0 = i17 - i18;
  if ((i0 & 65536L) != 0L) {
    i19 = i0 | -65536L;
  } else {
    i19 = i0 & 65535L;
  }

  i0 = i19 << 2;
  i13 = *Bias;
  if ((i0 & 524288L) != 0L) {
    i20 = i0 | -524288L;
  } else {
    i20 = i0 & 524287L;
  }

  if ((i13 & 524288L) != 0L) {
    i21 = i13 | -524288L;
  } else {
    i21 = i13 & 524287L;
  }

  i0 = i20 - i21;
  sLong2MultiWord(20971L * dTheta_1, &r26.chunks[0U], 2);
  MultiWordSignedWrap(&r26.chunks[0U], 2, 25U, &r27.chunks[0U]);
  sLong2MultiWord(Theta_1, &r28.chunks[0U], 2);
  sMultiWordShl(&r28.chunks[0U], 2, 22U, &r29.chunks[0U], 2);
  MultiWordSignedWrap(&r29.chunks[0U], 2, 25U, &r26.chunks[0U]);
  MultiWordAdd(&r27.chunks[0U], &r26.chunks[0U], &r30.chunks[0U], 2);
  MultiWordSignedWrap(&r30.chunks[0U], 2, 25U, &r31.chunks[0U]);
  sMultiWordShr(&r31.chunks[0U], 2, 18U, &r32.chunks[0U], 2);
  sLong2MultiWord((int)MultiWord2sLong(&r32.chunks[0U]) >> 4, &r33.chunks[0U], 2);
  sMultiWordShl(&r33.chunks[0U], 2, 18U, &r25.chunks[0U], 2);
  MultiWordSignedWrap(&r25.chunks[0U], 2, 27U, &r23.chunks[0U]);
  u1 = (unsigned long)kt;
  if ((i0 & 524288L) != 0L) {
    u2 = (unsigned long)(i0 | -524288L);
  } else {
    u2 = (unsigned long)(i0 & 524287L);
  }

  sMultiWordMul(&u1, 1, &u2, 1, &r33.chunks[0U], 2);
  MultiWordSignedWrap(&r33.chunks[0U], 2, 27U, &r25.chunks[0U]);
  MultiWordAdd(&r23.chunks[0U], &r25.chunks[0U], &r8.chunks[0U], 2);
  MultiWordSignedWrap(&r8.chunks[0U], 2, 27U, &r9.chunks[0U]);
  sMultiWordShr(&r9.chunks[0U], 2, 14U, &r6.chunks[0U], 2);
  *Theta = (int)MultiWord2sLong(&r6.chunks[0U]) >> 4;
  i13 = Gyro;
  if ((i13 & 65536L) != 0L) {
    i22 = i13 | -65536L;
  } else {
    i22 = i13 & 65535L;
  }

  if ((i14 & 65536L) != 0L) {
    i23 = i14 | -65536L;
  } else {
    i23 = i14 & 65535L;
  }

  i0 = i22 - i23;
  if ((i0 & 65536L) != 0L) {
    i24 = i0 | -65536L;
  } else {
    i24 = i0 & 65535L;
  }

  i0 = i24 << 2;
  i13 = *Bias;
  if ((i0 & 524288L) != 0L) {
    i25 = i0 | -524288L;
  } else {
    i25 = i0 & 524287L;
  }

  if ((i13 & 524288L) != 0L) {
    i26 = i13 | -524288L;
  } else {
    i26 = i13 & 524287L;
  }

  i0 = i25 - i26;
  sLong2MultiWord(*dTheta, &r31.chunks[0U], 2);
  sMultiWordShl(&r31.chunks[0U], 2, 18U, &r33.chunks[0U], 2);
  MultiWordSignedWrap(&r33.chunks[0U], 2, 27U, &r25.chunks[0U]);
  if ((i0 & 524288L) != 0L) {
    u1 = (unsigned long)(i0 | -524288L);
  } else {
    u1 = (unsigned long)(i0 & 524287L);
  }

  u2 = kw;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r31.chunks[0U], 2);
  MultiWordSignedWrap(&r31.chunks[0U], 2, 27U, &r33.chunks[0U]);
  MultiWordAdd(&r25.chunks[0U], &r33.chunks[0U], &r23.chunks[0U], 2);
  MultiWordSignedWrap(&r23.chunks[0U], 2, 27U, &r8.chunks[0U]);
  sMultiWordShr(&r8.chunks[0U], 2, 14U, &r9.chunks[0U], 2);
  *dTheta = (int)MultiWord2sLong(&r9.chunks[0U]) >> 4;
  i0 = c << 2;
  if ((i0 & 524288L) != 0L) {
    i27 = i0 | -524288L;
  } else {
    i27 = i0 & 524287L;
  }

  if ((i13 & 524288L) != 0L) {
    i28 = i13 | -524288L;
  } else {
    i28 = i13 & 524287L;
  }

  i0 = i27 - i28;
  sLong2MultiWord(*Bias, &r30.chunks[0U], 2);
  sMultiWordShl(&r30.chunks[0U], 2, 20U, &r31.chunks[0U], 2);
  MultiWordSignedWrap(&r31.chunks[0U], 2, 27U, &r33.chunks[0U]);
  if ((i0 & 524288L) != 0L) {
    u1 = (unsigned long)(i0 | -524288L);
  } else {
    u1 = (unsigned long)(i0 & 524287L);
  }

  u2 = kb;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r30.chunks[0U], 2);
  MultiWordSignedWrap(&r30.chunks[0U], 2, 27U, &r31.chunks[0U]);
  MultiWordAdd(&r33.chunks[0U], &r31.chunks[0U], &r25.chunks[0U], 2);
  MultiWordSignedWrap(&r25.chunks[0U], 2, 27U, &r23.chunks[0U]);
  sMultiWordShr(&r23.chunks[0U], 2, 13U, &r8.chunks[0U], 2);
  *Bias = (int)MultiWord2sLong(&r8.chunks[0U]) >> 7;
  i0 = u0 - 1048576L;
  u1 = (unsigned long)kb * kb;
  u2 = b_Pww_p;
  uMultiWordMul(&u1, 1, &u2, 1, &r2.chunks[0U], 2);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r34.chunks[0U], 3);
  MultiWordSignedWrap(&r34.chunks[0U], 3, 23U, &r35.chunks[0U]);
  if ((i0 & 536870912L) != 0L) {
    u1 = (unsigned long)(i0 | -536870912L);
  } else {
    u1 = (unsigned long)(i0 & 536870911L);
  }

  u2 = (unsigned long)kb << 1;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r23.chunks[0U], 2);
  u1 = (unsigned long)Pwb_p;
  sMultiWordMul(&r23.chunks[0U], 2, &u1, 1, &r36.chunks[0U], 3);
  sMultiWordShl(&r36.chunks[0U], 3, 1U, &r37.chunks[0U], 3);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 23U, &r34.chunks[0U]);
  MultiWordAdd(&r35.chunks[0U], &r34.chunks[0U], &r38.chunks[0U], 3);
  MultiWordSignedWrap(&r38.chunks[0U], 3, 23U, &r39.chunks[0U]);
  MultiWordSignedWrap(&r39.chunks[0U], 3, 19U, &r40.chunks[0U]);
  u1 = (unsigned long)i_c;
  u2 = (unsigned long)i_c;
  sMultiWordMul(&u1, 1, &u2, 1, &r23.chunks[0U], 2);
  u1 = Pbb_p;
  ssuMultiWordMul(&r23.chunks[0U], 2, &u1, 1, &r38.chunks[0U], 3);
  MultiWordSignedWrap(&r38.chunks[0U], 3, 19U, &r39.chunks[0U]);
  MultiWordAdd(&r40.chunks[0U], &r39.chunks[0U], &r41.chunks[0U], 3);
  MultiWordSignedWrap(&r41.chunks[0U], 3, 19U, &r42.chunks[0U]);
  MultiWordSignedWrap(&r42.chunks[0U], 3, 17U, &r43.chunks[0U]);
  u1 = (unsigned long)kb * kb;
  u2 = rw;
  uMultiWordMul(&u1, 1, &u2, 1, &r17.chunks[0U], 2);
  uMultiWord2MultiWord(&r17.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordShl(&r44.chunks[0U], 4, 55U, &r45.chunks[0U], 4);
  r17 = r46;
  uMultiWord2MultiWord(&r17.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordDivZero(&r45.chunks[0U], 4, &r44.chunks[0U], 4, &r47.chunks[0U], 4,
                    &r48.chunks[0U], 4, &r49.chunks[0U], 4, &r50.chunks[0U], 4);
  uMultiWord2MultiWord(&r47.chunks[0U], 4, &r0.chunks[0U], 2);
  MultiWordUnsignedWrap(&r0.chunks[0U], 2, 16U, &r1.chunks[0U]);
  uMultiWordShl(&r1.chunks[0U], 2, 1U, &r2.chunks[0U], 2);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r41.chunks[0U], 3);
  MultiWordSignedWrap(&r41.chunks[0U], 3, 17U, &r42.chunks[0U]);
  MultiWordAdd(&r43.chunks[0U], &r42.chunks[0U], &r51.chunks[0U], 3);
  MultiWordSignedWrap(&r51.chunks[0U], 3, 17U, &r52.chunks[0U]);
  sMultiWordShr(&r52.chunks[0U], 3, 40U, &r21.chunks[0U], 3);
  P[8] = (int)((unsigned int)MultiWord2uLong(&r21.chunks[0U]) >> 4) >> 5;
  i0 = -(long)u0;
  c = u0 - 1048576L;
  i_c = u0 - 1048576L;
  r6 = r53;
  u1 = (unsigned long)((long)kb * kt);
  u2 = rw;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r54.chunks[0U], 2);
  r23 = r55;
  if (sMultiWordEq(&r6.chunks[0U], &r23.chunks[0U], 2)) {
    r25 = r55;
    if (sMultiWordGe(&r54.chunks[0U], &r25.chunks[0U], 2)) {
      r58 = r60;
    } else {
      r58 = r59;
    }
  } else {
    sMultiWord2MultiWord(&r54.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordShl(&r56.chunks[0U], 4, 54U, &r57.chunks[0U], 4);
    sMultiWord2MultiWord(&r6.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordDivFloor(&r57.chunks[0U], 4, &r56.chunks[0U], 4, &r61.chunks[0U],
                       5, &r62.chunks[0U], 4, &r63.chunks[0U], 4, &r64.chunks[0U],
                       4);
    sMultiWord2MultiWord(&r61.chunks[0U], 5, &r25.chunks[0U], 2);
    MultiWordSignedWrap(&r25.chunks[0U], 2, 16U, &r58.chunks[0U]);
  }

  u1 = (unsigned long)((long)kb * kt);
  u2 = b_Pww_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r27.chunks[0U], 2);
  MultiWordSignedWrap(&r27.chunks[0U], 2, 11U, &r30.chunks[0U]);
  if ((i0 & 65536L) != 0L) {
    u1 = (unsigned long)(i0 | -65536L);
  } else {
    u1 = (unsigned long)(i0 & 65535L);
  }

  u2 = b_Pwt_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r29.chunks[0U], 2);
  sMultiWordShl(&r29.chunks[0U], 2, 19U, &r26.chunks[0U], 2);
  MultiWordSignedWrap(&r26.chunks[0U], 2, 11U, &r27.chunks[0U]);
  MultiWordAdd(&r30.chunks[0U], &r27.chunks[0U], &r31.chunks[0U], 2);
  MultiWordSignedWrap(&r31.chunks[0U], 2, 11U, &r33.chunks[0U]);
  sMultiWord2MultiWord(&r33.chunks[0U], 2, &r65.chunks[0U], 3);
  MultiWordSignedWrap(&r65.chunks[0U], 3, 31U, &r66.chunks[0U]);
  sLong2MultiWord((long)kb * kt, &r29.chunks[0U], 2);
  MultiWordSignedWrap(&r29.chunks[0U], 2, 17U, &r26.chunks[0U]);
  u1 = (unsigned long)kt;
  if ((c & 536870912L) != 0L) {
    u2 = (unsigned long)(c | -536870912L);
  } else {
    u2 = (unsigned long)(c & 536870911L);
  }

  sMultiWordMul(&u1, 1, &u2, 1, &r28.chunks[0U], 2);
  MultiWordSignedWrap(&r28.chunks[0U], 2, 17U, &r29.chunks[0U]);
  MultiWordAdd(&r26.chunks[0U], &r29.chunks[0U], &r27.chunks[0U], 2);
  MultiWordSignedWrap(&r27.chunks[0U], 2, 17U, &r30.chunks[0U]);
  u1 = (unsigned long)Pwb_p;
  sMultiWordMul(&r30.chunks[0U], 2, &u1, 1, &r31.chunks[0U], 2);
  sMultiWordShl(&r31.chunks[0U], 2, 1U, &r33.chunks[0U], 2);
  sMultiWord2MultiWord(&r33.chunks[0U], 2, &r67.chunks[0U], 3);
  MultiWordSignedWrap(&r67.chunks[0U], 3, 31U, &r65.chunks[0U]);
  MultiWordAdd(&r66.chunks[0U], &r65.chunks[0U], &r36.chunks[0U], 3);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 31U, &r37.chunks[0U]);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 30U, &r34.chunks[0U]);
  u1 = (unsigned long)Ptb_p;
  u2 = 1048576UL - kb;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r33.chunks[0U], 2);
  sMultiWord2MultiWord(&r33.chunks[0U], 2, &r66.chunks[0U], 3);
  sMultiWordShl(&r66.chunks[0U], 3, 20U, &r36.chunks[0U], 3);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 30U, &r37.chunks[0U]);
  MultiWordAdd(&r34.chunks[0U], &r37.chunks[0U], &r35.chunks[0U], 3);
  MultiWordSignedWrap(&r35.chunks[0U], 3, 30U, &r38.chunks[0U]);
  MultiWordSignedWrap(&r38.chunks[0U], 3, 29U, &r39.chunks[0U]);
  u1 = (unsigned long)kt;
  if ((i_c & 536870912L) != 0L) {
    u2 = (unsigned long)(i_c | -536870912L);
  } else {
    u2 = (unsigned long)(i_c & 536870911L);
  }

  sMultiWordMul(&u1, 1, &u2, 1, &r31.chunks[0U], 2);
  u1 = Pbb_p;
  ssuMultiWordMul(&r31.chunks[0U], 2, &u1, 1, &r33.chunks[0U], 2);
  sMultiWord2MultiWord(&r33.chunks[0U], 2, &r35.chunks[0U], 3);
  MultiWordSignedWrap(&r35.chunks[0U], 3, 29U, &r38.chunks[0U]);
  MultiWordAdd(&r39.chunks[0U], &r38.chunks[0U], &r40.chunks[0U], 3);
  MultiWordSignedWrap(&r40.chunks[0U], 3, 29U, &r41.chunks[0U]);
  MultiWordSignedWrap(&r41.chunks[0U], 3, 28U, &r42.chunks[0U]);
  sMultiWordShl(&r58.chunks[0U], 2, 1U, &r33.chunks[0U], 2);
  sMultiWord2MultiWord(&r33.chunks[0U], 2, &r40.chunks[0U], 3);
  MultiWordSignedWrap(&r40.chunks[0U], 3, 28U, &r41.chunks[0U]);
  MultiWordAdd(&r42.chunks[0U], &r41.chunks[0U], &r43.chunks[0U], 3);
  MultiWordSignedWrap(&r43.chunks[0U], 3, 28U, &r51.chunks[0U]);
  sMultiWordShr(&r51.chunks[0U], 3, 40U, &r52.chunks[0U], 3);
  b_Ptb_p = ((int)MultiWord2sLong(&r52.chunks[0U]) >> 5) << 5;
  i0 = -2L * kt;
  c = -2L * kt;
  r6 = r53;
  u1 = (unsigned long)((long)kt * kt);
  u2 = rw;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r68.chunks[0U], 2);
  r33 = r55;
  if (sMultiWordEq(&r6.chunks[0U], &r33.chunks[0U], 2)) {
    r31 = r55;
    if (sMultiWordGe(&r68.chunks[0U], &r31.chunks[0U], 2)) {
      r69 = r60;
    } else {
      r69 = r59;
    }
  } else {
    sMultiWord2MultiWord(&r68.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordShl(&r56.chunks[0U], 4, 54U, &r57.chunks[0U], 4);
    sMultiWord2MultiWord(&r6.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordDivFloor(&r57.chunks[0U], 4, &r56.chunks[0U], 4, &r61.chunks[0U],
                       5, &r62.chunks[0U], 4, &r63.chunks[0U], 4, &r64.chunks[0U],
                       4);
    sMultiWord2MultiWord(&r61.chunks[0U], 5, &r31.chunks[0U], 2);
    MultiWordSignedWrap(&r31.chunks[0U], 2, 16U, &r69.chunks[0U]);
  }

  u1 = (unsigned long)((long)kt * kt);
  u2 = b_Pww_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r70.chunks[0U], 2);
  MultiWordSignedWrap(&r70.chunks[0U], 2, 4U, &r71.chunks[0U]);
  if ((i0 & 8388608L) != 0L) {
    u1 = (unsigned long)(i0 | -8388608L);
  } else {
    u1 = (unsigned long)(i0 & 8388607L);
  }

  u2 = b_Pwt_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r72.chunks[0U], 2);
  sMultiWordShl(&r72.chunks[0U], 2, 19U, &r73.chunks[0U], 2);
  MultiWordSignedWrap(&r73.chunks[0U], 2, 4U, &r70.chunks[0U]);
  MultiWordAdd(&r71.chunks[0U], &r70.chunks[0U], &r74.chunks[0U], 2);
  MultiWordSignedWrap(&r74.chunks[0U], 2, 4U, &r75.chunks[0U]);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 3U, &r76.chunks[0U]);
  sLong2MultiWord((long)kt * kt, &r72.chunks[0U], 2);
  sMultiWordShl(&r72.chunks[0U], 2, 1U, &r73.chunks[0U], 2);
  MultiWordSignedWrap(&r73.chunks[0U], 2, 24U, &r70.chunks[0U]);
  u1 = (unsigned long)Pwb_p;
  sMultiWordMul(&r70.chunks[0U], 2, &u1, 1, &r71.chunks[0U], 2);
  sMultiWordShl(&r71.chunks[0U], 2, 1U, &r74.chunks[0U], 2);
  MultiWordSignedWrap(&r74.chunks[0U], 2, 3U, &r75.chunks[0U]);
  MultiWordAdd(&r76.chunks[0U], &r75.chunks[0U], &r77.chunks[0U], 2);
  MultiWordSignedWrap(&r77.chunks[0U], 2, 3U, &r28.chunks[0U]);
  MultiWordSignedWrap(&r28.chunks[0U], 2, 1U, &r29.chunks[0U]);
  uLong2MultiWord((unsigned int)P_1[4] << 2, &r78.chunks[0U], 3);
  uMultiWordShl(&r78.chunks[0U], 3, 38U, &r79.chunks[0U], 3);
  uMultiWord2MultiWord(&r79.chunks[0U], 3, &r16.chunks[0U], 2);
  MultiWordUnsignedWrap(&r16.chunks[0U], 2, 9U, &r13.chunks[0U]);
  uLong2MultiWord(53687UL * Pww_p, &r15.chunks[0U], 2);
  MultiWordUnsignedWrap(&r15.chunks[0U], 2, 9U, &r16.chunks[0U]);
  MultiWordAdd(&r13.chunks[0U], &r16.chunks[0U], &r14.chunks[0U], 2);
  MultiWordUnsignedWrap(&r14.chunks[0U], 2, 9U, &r17.chunks[0U]);
  MultiWordUnsignedWrap(&r17.chunks[0U], 2, 8U, &r0.chunks[0U]);
  uLong2MultiWord(41943UL * Pwt_p, &r13.chunks[0U], 2);
  uMultiWordShl(&r13.chunks[0U], 2, 12U, &r14.chunks[0U], 2);
  MultiWordUnsignedWrap(&r14.chunks[0U], 2, 8U, &r17.chunks[0U]);
  MultiWordAdd(&r0.chunks[0U], &r17.chunks[0U], &r1.chunks[0U], 2);
  MultiWordUnsignedWrap(&r1.chunks[0U], 2, 8U, &r2.chunks[0U]);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r80.chunks[0U], 3);
  uMultiWordShl(&r80.chunks[0U], 3, 14U, &r81.chunks[0U], 3);
  MultiWordUnsignedWrap(&r81.chunks[0U], 3, 25U, &r82.chunks[0U]);
  u1 = 34359UL * qw;
  u2 = 43690UL;
  uMultiWordMul(&u1, 1, &u2, 1, &r2.chunks[0U], 2);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r80.chunks[0U], 3);
  MultiWordUnsignedWrap(&r80.chunks[0U], 3, 25U, &r81.chunks[0U]);
  MultiWordAdd(&r82.chunks[0U], &r81.chunks[0U], &r83.chunks[0U], 3);
  MultiWordUnsignedWrap(&r83.chunks[0U], 3, 25U, &r84.chunks[0U]);
  uMultiWordShr(&r84.chunks[0U], 3, 52U, &r85.chunks[0U], 3);
  uLong2MultiWord((unsigned int)((int)((unsigned int)MultiWord2uLong
    (&r85.chunks[0U]) >> 1) >> 1) << 2, &r86.chunks[0U], 3);
  uMultiWordShl(&r86.chunks[0U], 3, 39U, &r87.chunks[0U], 3);
  uMultiWord2MultiWord(&r87.chunks[0U], 3, &r77.chunks[0U], 2);
  MultiWordSignedWrap(&r77.chunks[0U], 2, 1U, &r28.chunks[0U]);
  MultiWordAdd(&r29.chunks[0U], &r28.chunks[0U], &r26.chunks[0U], 2);
  MultiWordSignedWrap(&r26.chunks[0U], 2, 1U, &r27.chunks[0U]);
  if ((c & 8388608L) != 0L) {
    u1 = (unsigned long)(c | -8388608L);
  } else {
    u1 = (unsigned long)(c & 8388607L);
  }

  u2 = (unsigned long)Ptb_p;
  sMultiWordMul(&u1, 1, &u2, 1, &r29.chunks[0U], 2);
  sMultiWordShl(&r29.chunks[0U], 2, 20U, &r26.chunks[0U], 2);
  MultiWordAdd(&r27.chunks[0U], &r26.chunks[0U], &r30.chunks[0U], 2);
  sMultiWord2MultiWord(&r30.chunks[0U], 2, &r35.chunks[0U], 3);
  MultiWordSignedWrap(&r35.chunks[0U], 3, 31U, &r38.chunks[0U]);
  u1 = (unsigned long)((long)kt * kt);
  u2 = Pbb_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r30.chunks[0U], 2);
  sMultiWord2MultiWord(&r30.chunks[0U], 2, &r34.chunks[0U], 3);
  MultiWordSignedWrap(&r34.chunks[0U], 3, 31U, &r35.chunks[0U]);
  MultiWordAdd(&r38.chunks[0U], &r35.chunks[0U], &r39.chunks[0U], 3);
  MultiWordSignedWrap(&r39.chunks[0U], 3, 31U, &r40.chunks[0U]);
  MultiWordSignedWrap(&r40.chunks[0U], 3, 30U, &r41.chunks[0U]);
  sMultiWordShl(&r69.chunks[0U], 2, 1U, &r30.chunks[0U], 2);
  sMultiWord2MultiWord(&r30.chunks[0U], 2, &r39.chunks[0U], 3);
  MultiWordSignedWrap(&r39.chunks[0U], 3, 30U, &r40.chunks[0U]);
  MultiWordAdd(&r41.chunks[0U], &r40.chunks[0U], &r42.chunks[0U], 3);
  MultiWordSignedWrap(&r42.chunks[0U], 3, 30U, &r43.chunks[0U]);
  sMultiWordShr(&r43.chunks[0U], 3, 39U, &r51.chunks[0U], 3);
  Ptt_p = (unsigned int)((int)((unsigned int)MultiWord2uLong(&r51.chunks[0U]) >>
    1) >> 1) << 2;
  u0 = kw;
  i0 = u0 - 65536L;
  u0 = kb;
  c = u0 - 1048576L;
  u0 = kw;
  i_c = u0 - 65536L;
  u0 = kb;
  i29 = u0 - 1048576L;
  if ((i0 & 33554432L) != 0L) {
    u1 = (unsigned long)(i0 | -33554432L);
  } else {
    u1 = (unsigned long)(i0 & 33554431L);
  }

  u2 = kb;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r27.chunks[0U], 2);
  u1 = b_Pww_p;
  ssuMultiWordMul(&r27.chunks[0U], 2, &u1, 1, &r30.chunks[0U], 2);
  sMultiWord2MultiWord(&r30.chunks[0U], 2, &r66.chunks[0U], 3);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 20U, &r36.chunks[0U]);
  uLong2MultiWord((unsigned long)kb * kw, &r29.chunks[0U], 2);
  MultiWordSignedWrap(&r29.chunks[0U], 2, 6U, &r26.chunks[0U]);
  if ((c & 536870912L) != 0L) {
    u1 = (unsigned long)(c | -536870912L);
  } else {
    u1 = (unsigned long)(c & 536870911L);
  }

  if ((i_c & 33554432L) != 0L) {
    u2 = (unsigned long)(i_c | -33554432L);
  } else {
    u2 = (unsigned long)(i_c & 33554431L);
  }

  sMultiWordMul(&u1, 1, &u2, 1, &r28.chunks[0U], 2);
  MultiWordSignedWrap(&r28.chunks[0U], 2, 6U, &r29.chunks[0U]);
  MultiWordAdd(&r26.chunks[0U], &r29.chunks[0U], &r27.chunks[0U], 2);
  MultiWordSignedWrap(&r27.chunks[0U], 2, 6U, &r30.chunks[0U]);
  u1 = (unsigned long)Pwb_p;
  sMultiWordMul(&r30.chunks[0U], 2, &u1, 1, &r67.chunks[0U], 3);
  sMultiWordShl(&r67.chunks[0U], 3, 1U, &r65.chunks[0U], 3);
  MultiWordSignedWrap(&r65.chunks[0U], 3, 20U, &r66.chunks[0U]);
  MultiWordAdd(&r36.chunks[0U], &r66.chunks[0U], &r37.chunks[0U], 3);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 20U, &r34.chunks[0U]);
  MultiWordSignedWrap(&r34.chunks[0U], 3, 19U, &r35.chunks[0U]);
  if ((i29 & 536870912L) != 0L) {
    u1 = (unsigned long)(i29 | -536870912L);
  } else {
    u1 = (unsigned long)(i29 & 536870911L);
  }

  u2 = kw;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r27.chunks[0U], 2);
  u1 = Pbb_p;
  ssuMultiWordMul(&r27.chunks[0U], 2, &u1, 1, &r30.chunks[0U], 2);
  sMultiWord2MultiWord(&r30.chunks[0U], 2, &r37.chunks[0U], 3);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 19U, &r34.chunks[0U]);
  MultiWordAdd(&r35.chunks[0U], &r34.chunks[0U], &r38.chunks[0U], 3);
  MultiWordSignedWrap(&r38.chunks[0U], 3, 19U, &r39.chunks[0U]);
  MultiWordSignedWrap(&r39.chunks[0U], 3, 17U, &r40.chunks[0U]);
  u1 = (unsigned long)kb * kw;
  u2 = rw;
  uMultiWordMul(&u1, 1, &u2, 1, &r17.chunks[0U], 2);
  uMultiWord2MultiWord(&r17.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordShl(&r44.chunks[0U], 4, 55U, &r45.chunks[0U], 4);
  r17 = r46;
  uMultiWord2MultiWord(&r17.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordDivZero(&r45.chunks[0U], 4, &r44.chunks[0U], 4, &r47.chunks[0U], 4,
                    &r48.chunks[0U], 4, &r49.chunks[0U], 4, &r50.chunks[0U], 4);
  uMultiWord2MultiWord(&r47.chunks[0U], 4, &r0.chunks[0U], 2);
  MultiWordUnsignedWrap(&r0.chunks[0U], 2, 16U, &r1.chunks[0U]);
  uMultiWordShl(&r1.chunks[0U], 2, 1U, &r2.chunks[0U], 2);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r38.chunks[0U], 3);
  MultiWordSignedWrap(&r38.chunks[0U], 3, 17U, &r39.chunks[0U]);
  MultiWordAdd(&r40.chunks[0U], &r39.chunks[0U], &r41.chunks[0U], 3);
  MultiWordSignedWrap(&r41.chunks[0U], 3, 17U, &r42.chunks[0U]);
  sMultiWordShr(&r42.chunks[0U], 3, 37U, &r43.chunks[0U], 3);
  b_Pwb_p = ((int)MultiWord2sLong(&r43.chunks[0U]) >> 8) << 8;
  u0 = kw;
  i0 = u0 - 65536L;
  c = u0 - 65536L;
  i_c = -(long)u0;
  r6 = r53;
  u1 = (unsigned long)((long)kt * kw);
  u2 = rw;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r88.chunks[0U], 2);
  r30 = r55;
  if (sMultiWordEq(&r6.chunks[0U], &r30.chunks[0U], 2)) {
    r27 = r55;
    if (sMultiWordGe(&r88.chunks[0U], &r27.chunks[0U], 2)) {
      r89 = r60;
    } else {
      r89 = r59;
    }
  } else {
    sMultiWord2MultiWord(&r88.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordShl(&r56.chunks[0U], 4, 54U, &r57.chunks[0U], 4);
    sMultiWord2MultiWord(&r6.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordDivFloor(&r57.chunks[0U], 4, &r56.chunks[0U], 4, &r61.chunks[0U],
                       5, &r62.chunks[0U], 4, &r63.chunks[0U], 4, &r64.chunks[0U],
                       4);
    sMultiWord2MultiWord(&r61.chunks[0U], 5, &r27.chunks[0U], 2);
    MultiWordSignedWrap(&r27.chunks[0U], 2, 16U, &r89.chunks[0U]);
  }

  u1 = (unsigned long)kt;
  if ((i0 & 33554432L) != 0L) {
    u2 = (unsigned long)(i0 | -33554432L);
  } else {
    u2 = (unsigned long)(i0 & 33554431L);
  }

  sMultiWordMul(&u1, 1, &u2, 1, &r70.chunks[0U], 2);
  u1 = b_Pww_p;
  ssuMultiWordMul(&r70.chunks[0U], 2, &u1, 1, &r71.chunks[0U], 2);
  MultiWordSignedWrap(&r71.chunks[0U], 2, 2U, &r74.chunks[0U]);
  u1 = 65536UL - kw;
  u2 = b_Pwt_p;
  uMultiWordMul(&u1, 1, &u2, 1, &r1.chunks[0U], 2);
  uMultiWordShl(&r1.chunks[0U], 2, 19U, &r2.chunks[0U], 2);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r70.chunks[0U], 2);
  MultiWordSignedWrap(&r70.chunks[0U], 2, 2U, &r71.chunks[0U]);
  MultiWordAdd(&r74.chunks[0U], &r71.chunks[0U], &r75.chunks[0U], 2);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 2U, &r76.chunks[0U]);
  MultiWordSignedWrap(&r76.chunks[0U], 2, 1U, &r77.chunks[0U]);
  sLong2MultiWord((long)kt * kw, &r72.chunks[0U], 2);
  MultiWordSignedWrap(&r72.chunks[0U], 2, 21U, &r73.chunks[0U]);
  u1 = (unsigned long)kt;
  if ((c & 33554432L) != 0L) {
    u2 = (unsigned long)(c | -33554432L);
  } else {
    u2 = (unsigned long)(c & 33554431L);
  }

  sMultiWordMul(&u1, 1, &u2, 1, &r90.chunks[0U], 2);
  MultiWordSignedWrap(&r90.chunks[0U], 2, 21U, &r72.chunks[0U]);
  MultiWordAdd(&r73.chunks[0U], &r72.chunks[0U], &r70.chunks[0U], 2);
  MultiWordSignedWrap(&r70.chunks[0U], 2, 21U, &r71.chunks[0U]);
  u1 = (unsigned long)Pwb_p;
  sMultiWordMul(&r71.chunks[0U], 2, &u1, 1, &r74.chunks[0U], 2);
  sMultiWordShl(&r74.chunks[0U], 2, 1U, &r75.chunks[0U], 2);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 1U, &r76.chunks[0U]);
  MultiWordAdd(&r77.chunks[0U], &r76.chunks[0U], &r28.chunks[0U], 2);
  MultiWordSignedWrap(&r28.chunks[0U], 2, 1U, &r29.chunks[0U]);
  if ((i_c & 65536L) != 0L) {
    u1 = (unsigned long)(i_c | -65536L);
  } else {
    u1 = (unsigned long)(i_c & 65535L);
  }

  u2 = (unsigned long)Ptb_p;
  sMultiWordMul(&u1, 1, &u2, 1, &r77.chunks[0U], 2);
  sMultiWordShl(&r77.chunks[0U], 2, 20U, &r28.chunks[0U], 2);
  MultiWordAdd(&r29.chunks[0U], &r28.chunks[0U], &r26.chunks[0U], 2);
  sMultiWord2MultiWord(&r26.chunks[0U], 2, &r37.chunks[0U], 3);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 31U, &r34.chunks[0U]);
  u1 = (unsigned long)((long)kt * kw);
  u2 = Pbb_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r26.chunks[0U], 2);
  sMultiWord2MultiWord(&r26.chunks[0U], 2, &r36.chunks[0U], 3);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 31U, &r37.chunks[0U]);
  MultiWordAdd(&r34.chunks[0U], &r37.chunks[0U], &r35.chunks[0U], 3);
  MultiWordSignedWrap(&r35.chunks[0U], 3, 31U, &r38.chunks[0U]);
  MultiWordSignedWrap(&r38.chunks[0U], 3, 30U, &r39.chunks[0U]);
  sMultiWordShl(&r89.chunks[0U], 2, 1U, &r26.chunks[0U], 2);
  sMultiWord2MultiWord(&r26.chunks[0U], 2, &r35.chunks[0U], 3);
  MultiWordSignedWrap(&r35.chunks[0U], 3, 30U, &r38.chunks[0U]);
  MultiWordAdd(&r39.chunks[0U], &r38.chunks[0U], &r40.chunks[0U], 3);
  MultiWordSignedWrap(&r40.chunks[0U], 3, 30U, &r41.chunks[0U]);
  sMultiWordShr(&r41.chunks[0U], 3, 35U, &r42.chunks[0U], 3);
  Pwt_p = (unsigned int)((int)((unsigned int)MultiWord2uLong(&r42.chunks[0U]) >>
    5) >> 1) << 6;
  i0 = u0 - 65536L;
  u1 = (unsigned long)h_c;
  u2 = (unsigned long)h_c;
  sMultiWordMul(&u1, 1, &u2, 1, &r26.chunks[0U], 2);
  u1 = b_Pww_p;
  ssuMultiWordMul(&r26.chunks[0U], 2, &u1, 1, &r67.chunks[0U], 3);
  MultiWordSignedWrap(&r67.chunks[0U], 3, 27U, &r65.chunks[0U]);
  if ((i0 & 33554432L) != 0L) {
    u1 = (unsigned long)(i0 | -33554432L);
  } else {
    u1 = (unsigned long)(i0 & 33554431L);
  }

  u2 = (unsigned long)kw << 1;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r26.chunks[0U], 2);
  u1 = (unsigned long)Pwb_p;
  sMultiWordMul(&r26.chunks[0U], 2, &u1, 1, &r91.chunks[0U], 3);
  sMultiWordShl(&r91.chunks[0U], 3, 1U, &r92.chunks[0U], 3);
  MultiWordSignedWrap(&r92.chunks[0U], 3, 27U, &r67.chunks[0U]);
  MultiWordAdd(&r65.chunks[0U], &r67.chunks[0U], &r66.chunks[0U], 3);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 27U, &r36.chunks[0U]);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 25U, &r37.chunks[0U]);
  u1 = (unsigned long)kw * kw;
  u2 = Pbb_p;
  uMultiWordMul(&u1, 1, &u2, 1, &r2.chunks[0U], 2);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r66.chunks[0U], 3);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 25U, &r36.chunks[0U]);
  MultiWordAdd(&r37.chunks[0U], &r36.chunks[0U], &r34.chunks[0U], 3);
  MultiWordSignedWrap(&r34.chunks[0U], 3, 25U, &r35.chunks[0U]);
  MultiWordSignedWrap(&r35.chunks[0U], 3, 23U, &r38.chunks[0U]);
  u1 = (unsigned long)kw * kw;
  u2 = rw;
  uMultiWordMul(&u1, 1, &u2, 1, &r17.chunks[0U], 2);
  uMultiWord2MultiWord(&r17.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordShl(&r44.chunks[0U], 4, 55U, &r45.chunks[0U], 4);
  r17 = r46;
  uMultiWord2MultiWord(&r17.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordDivZero(&r45.chunks[0U], 4, &r44.chunks[0U], 4, &r47.chunks[0U], 4,
                    &r48.chunks[0U], 4, &r49.chunks[0U], 4, &r50.chunks[0U], 4);
  uMultiWord2MultiWord(&r47.chunks[0U], 4, &r0.chunks[0U], 2);
  MultiWordUnsignedWrap(&r0.chunks[0U], 2, 16U, &r1.chunks[0U]);
  uMultiWordShl(&r1.chunks[0U], 2, 1U, &r2.chunks[0U], 2);
  uMultiWord2MultiWord(&r2.chunks[0U], 2, &r34.chunks[0U], 3);
  MultiWordSignedWrap(&r34.chunks[0U], 3, 23U, &r35.chunks[0U]);
  MultiWordAdd(&r38.chunks[0U], &r35.chunks[0U], &r39.chunks[0U], 3);
  MultiWordSignedWrap(&r39.chunks[0U], 3, 23U, &r40.chunks[0U]);
  sMultiWordShr(&r40.chunks[0U], 3, 32U, &r41.chunks[0U], 3);
  Pww_p = (unsigned int)((int)((unsigned int)MultiWord2uLong(&r41.chunks[0U]) >>
    8) >> 1) << 9;

  /*     %% Kalman Gain Update */
  uLong2MultiWord(Pwt_p, &r14.chunks[0U], 2);
  uMultiWordShl(&r14.chunks[0U], 2, 20U, &r17.chunks[0U], 2);
  uLong2MultiWord(Ptt_p + ((unsigned long)(unsigned int)div_repeat_u32((unsigned
    long)rt << 16, 41943UL, 7U) << 2), &r14.chunks[0U], 2);
  uMultiWordDivZero(&r17.chunks[0U], 2, &r14.chunks[0U], 2, &r0.chunks[0U], 2,
                    &r13.chunks[0U], 2, &r16.chunks[0U], 2, &r15.chunks[0U], 2);
  MultiWordUnsignedWrap(&r0.chunks[0U], 2, 15U, &r1.chunks[0U]);
  uMultiWordShl(&r1.chunks[0U], 2, 5U, &r2.chunks[0U], 2);
  kw = (unsigned int)MultiWord2uLong(&r2.chunks[0U]);
  uLong2MultiWord(Ptt_p, &r13.chunks[0U], 2);
  uMultiWordShl(&r13.chunks[0U], 2, 24U, &r14.chunks[0U], 2);
  uLong2MultiWord(Ptt_p + ((unsigned long)(unsigned int)div_repeat_u32((unsigned
    long)rt << 16, 41943UL, 7U) << 2), &r13.chunks[0U], 2);
  uMultiWordDivZero(&r14.chunks[0U], 2, &r13.chunks[0U], 2, &r17.chunks[0U], 2,
                    &r16.chunks[0U], 2, &r15.chunks[0U], 2, &r93.chunks[0U], 2);
  MultiWordUnsignedWrap(&r17.chunks[0U], 2, 11U, &r0.chunks[0U]);
  uMultiWordShl(&r0.chunks[0U], 2, 2U, &r1.chunks[0U], 2);
  b_kt = (unsigned int)MultiWord2uLong(&r1.chunks[0U]);
  u1 = Ptt_p + ((unsigned long)(unsigned int)div_repeat_u32((unsigned long)rt <<
    16, 41943UL, 7U) << 2);
  i13 = b_Ptb_p;
  if ((long)u1 == 0L) {
    if (i13 >= 0) {
      r94 = r96;
    } else {
      r94 = r95;
    }
  } else {
    sLong2MultiWord(i13, &r28.chunks[0U], 2);
    sMultiWordShl(&r28.chunks[0U], 2, 21U, &r29.chunks[0U], 2);
    uLong2MultiWord(u1, &r28.chunks[0U], 2);
    sMultiWordDivZero(&r29.chunks[0U], 2, &r28.chunks[0U], 2, &r40.chunks[0U], 3,
                      &r77.chunks[0U], 2, &r76.chunks[0U], 2, &r75.chunks[0U], 2);
    sMultiWord2MultiWord(&r40.chunks[0U], 3, &r26.chunks[0U], 2);
    MultiWordSignedWrap(&r26.chunks[0U], 2, 14U, &r94.chunks[0U]);
  }

  sMultiWordShl(&r94.chunks[0U], 2, 4U, &r26.chunks[0U], 2);
  b_kb = (int)MultiWord2sLong(&r26.chunks[0U]);

  /*     %% State Update */
  i13 = AccTheta;
  i14 = *Theta;
  if ((i13 & 65536L) != 0L) {
    i30 = i13 | -65536L;
  } else {
    i30 = i13 & 65535L;
  }

  if ((i14 & 65536L) != 0L) {
    i31 = i14 | -65536L;
  } else {
    i31 = i14 & 65535L;
  }

  i0 = i30 - i31;
  if ((i0 & 65536L) != 0L) {
    u1 = (unsigned long)(i0 | -65536L);
  } else {
    u1 = (unsigned long)(i0 & 65535L);
  }

  u2 = kw;
  ssuMultiWordMul(&u1, 1, &u2, 1, &j_c.chunks[0U], 2);
  if ((i13 & 65536L) != 0L) {
    i32 = i13 | -65536L;
  } else {
    i32 = i13 & 65535L;
  }

  if ((i14 & 65536L) != 0L) {
    i33 = i14 | -65536L;
  } else {
    i33 = i14 & 65535L;
  }

  i0 = i32 - i33;
  u1 = (unsigned long)b_kb;
  if ((i0 & 65536L) != 0L) {
    u2 = (unsigned long)(i0 | -65536L);
  } else {
    u2 = (unsigned long)(i0 & 65535L);
  }

  sMultiWordMul(&u1, 1, &u2, 1, &k_c.chunks[0U], 2);

  /*     %% Covariance Update */
  r13 = r97;
  uLong2MultiWord(b_kt, &r15.chunks[0U], 2);
  MultiWordUnsignedWrap(&r15.chunks[0U], 2, 29U, &r16.chunks[0U]);
  MultiWordSub(&r13.chunks[0U], &r16.chunks[0U], &r14.chunks[0U], 2);
  MultiWordUnsignedWrap(&r14.chunks[0U], 2, 29U, &r17.chunks[0U]);
  u1 = Pwt_p;
  uMultiWordMul(&r17.chunks[0U], 2, &u1, 1, &r0.chunks[0U], 2);
  uMultiWord2MultiWord(&r0.chunks[0U], 2, &r86.chunks[0U], 3);
  uMultiWordShl(&r86.chunks[0U], 3, 25U, &r87.chunks[0U], 3);
  uMultiWord2MultiWord(&r87.chunks[0U], 3, &r66.chunks[0U], 3);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 18U, &r36.chunks[0U]);
  uLong2MultiWord(b_kt, &r75.chunks[0U], 2);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 28U, &r76.chunks[0U]);
  r75 = r98;
  MultiWordSub(&r76.chunks[0U], &r75.chunks[0U], &r77.chunks[0U], 2);
  MultiWordSignedWrap(&r77.chunks[0U], 2, 28U, &r28.chunks[0U]);
  u1 = kw;
  ssuMultiWordMul(&r28.chunks[0U], 2, &u1, 1, &r29.chunks[0U], 2);
  u1 = Ptt_p;
  ssuMultiWordMul(&r29.chunks[0U], 2, &u1, 1, &r65.chunks[0U], 3);
  MultiWordSignedWrap(&r65.chunks[0U], 3, 18U, &r66.chunks[0U]);
  MultiWordAdd(&r36.chunks[0U], &r66.chunks[0U], &r37.chunks[0U], 3);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 18U, &r34.chunks[0U]);
  MultiWordSignedWrap(&r34.chunks[0U], 3, 16U, &r35.chunks[0U]);
  u1 = (unsigned long)b_kt * kw;
  u2 = rt;
  uMultiWordMul(&u1, 1, &u2, 1, &r13.chunks[0U], 2);
  uMultiWord2MultiWord(&r13.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordShl(&r44.chunks[0U], 4, 55U, &r45.chunks[0U], 4);
  r13 = r46;
  uMultiWord2MultiWord(&r13.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordDivZero(&r45.chunks[0U], 4, &r44.chunks[0U], 4, &r47.chunks[0U], 4,
                    &r48.chunks[0U], 4, &r49.chunks[0U], 4, &r50.chunks[0U], 4);
  uMultiWord2MultiWord(&r47.chunks[0U], 4, &r14.chunks[0U], 2);
  MultiWordUnsignedWrap(&r14.chunks[0U], 2, 16U, &r17.chunks[0U]);
  uMultiWordShl(&r17.chunks[0U], 2, 2U, &r0.chunks[0U], 2);
  uMultiWord2MultiWord(&r0.chunks[0U], 2, &r37.chunks[0U], 3);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 16U, &r34.chunks[0U]);
  MultiWordAdd(&r35.chunks[0U], &r34.chunks[0U], &r38.chunks[0U], 3);
  MultiWordSignedWrap(&r38.chunks[0U], 3, 16U, &r39.chunks[0U]);
  sMultiWordShr(&r39.chunks[0U], 3, 51U, &r40.chunks[0U], 3);
  Pwt = (unsigned int)MultiWord2uLong(&r40.chunks[0U]);
  i13 = b_kb;
  if ((i13 & 65536L) != 0L) {
    i0 = -(i13 | -65536L);
  } else {
    i0 = -(i13 & 65535L);
  }

  r6 = r53;
  u1 = (unsigned long)((long)b_kb * kw);
  u2 = rt;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r99.chunks[0U], 2);
  r29 = r55;
  if (sMultiWordEq(&r6.chunks[0U], &r29.chunks[0U], 2)) {
    r28 = r55;
    if (sMultiWordGe(&r99.chunks[0U], &r28.chunks[0U], 2)) {
      r100 = r60;
    } else {
      r100 = r59;
    }
  } else {
    sMultiWord2MultiWord(&r99.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordShl(&r56.chunks[0U], 4, 54U, &r57.chunks[0U], 4);
    sMultiWord2MultiWord(&r6.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordDivFloor(&r57.chunks[0U], 4, &r56.chunks[0U], 4, &r61.chunks[0U],
                       5, &r62.chunks[0U], 4, &r63.chunks[0U], 4, &r64.chunks[0U],
                       4);
    sMultiWord2MultiWord(&r61.chunks[0U], 5, &r28.chunks[0U], 2);
    MultiWordSignedWrap(&r28.chunks[0U], 2, 16U, &r100.chunks[0U]);
  }

  if ((i0 & 65536L) != 0L) {
    u1 = (unsigned long)(i0 | -65536L);
  } else {
    u1 = (unsigned long)(i0 & 65535L);
  }

  u2 = Pww_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r71.chunks[0U], 2);
  sMultiWordShl(&r71.chunks[0U], 2, 26U, &r74.chunks[0U], 2);
  MultiWordSignedWrap(&r74.chunks[0U], 2, 4U, &r75.chunks[0U]);
  u1 = (unsigned long)((long)b_kb * kw);
  u2 = Pwt_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r71.chunks[0U], 2);
  MultiWordSignedWrap(&r71.chunks[0U], 2, 4U, &r74.chunks[0U]);
  MultiWordAdd(&r75.chunks[0U], &r74.chunks[0U], &r76.chunks[0U], 2);
  MultiWordSignedWrap(&r76.chunks[0U], 2, 4U, &r77.chunks[0U]);
  sMultiWord2MultiWord(&r77.chunks[0U], 2, &r91.chunks[0U], 3);
  MultiWordSignedWrap(&r91.chunks[0U], 3, 14U, &r92.chunks[0U]);
  r74 = r101;
  sLong2MultiWord(b_kb, &r70.chunks[0U], 2);
  MultiWordSignedWrap(&r70.chunks[0U], 2, 26U, &r71.chunks[0U]);
  MultiWordSub(&r74.chunks[0U], &r71.chunks[0U], &r75.chunks[0U], 2);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 26U, &r76.chunks[0U]);
  u1 = (unsigned long)b_Pwb_p;
  sMultiWordMul(&r76.chunks[0U], 2, &u1, 1, &r77.chunks[0U], 2);
  sMultiWord2MultiWord(&r77.chunks[0U], 2, &r102.chunks[0U], 3);
  sMultiWordShl(&r102.chunks[0U], 3, 27U, &r103.chunks[0U], 3);
  MultiWordSignedWrap(&r103.chunks[0U], 3, 14U, &r91.chunks[0U]);
  MultiWordAdd(&r92.chunks[0U], &r91.chunks[0U], &r67.chunks[0U], 3);
  MultiWordSignedWrap(&r67.chunks[0U], 3, 14U, &r65.chunks[0U]);
  MultiWordSignedWrap(&r65.chunks[0U], 3, 13U, &r66.chunks[0U]);
  sLong2MultiWord(b_kb, &r71.chunks[0U], 2);
  MultiWordSignedWrap(&r71.chunks[0U], 2, 26U, &r74.chunks[0U]);
  r71 = r101;
  MultiWordSub(&r74.chunks[0U], &r71.chunks[0U], &r75.chunks[0U], 2);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 26U, &r76.chunks[0U]);
  u1 = kw;
  ssuMultiWordMul(&r76.chunks[0U], 2, &u1, 1, &r77.chunks[0U], 2);
  u1 = (unsigned long)b_Ptb_p;
  sMultiWordMul(&r77.chunks[0U], 2, &u1, 1, &r92.chunks[0U], 3);
  sMultiWordShl(&r92.chunks[0U], 3, 1U, &r67.chunks[0U], 3);
  MultiWordSignedWrap(&r67.chunks[0U], 3, 13U, &r65.chunks[0U]);
  MultiWordAdd(&r66.chunks[0U], &r65.chunks[0U], &r36.chunks[0U], 3);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 13U, &r37.chunks[0U]);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 12U, &r34.chunks[0U]);
  sMultiWordShl(&r100.chunks[0U], 2, 6U, &r77.chunks[0U], 2);
  sMultiWord2MultiWord(&r77.chunks[0U], 2, &r36.chunks[0U], 3);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 12U, &r37.chunks[0U]);
  MultiWordAdd(&r34.chunks[0U], &r37.chunks[0U], &r35.chunks[0U], 3);
  MultiWordSignedWrap(&r35.chunks[0U], 3, 12U, &r38.chunks[0U]);
  sMultiWordShr(&r38.chunks[0U], 3, 55U, &r39.chunks[0U], 3);
  Pwb = (int)MultiWord2sLong(&r39.chunks[0U]);
  uLong2MultiWord(b_kt, &r75.chunks[0U], 2);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 28U, &r76.chunks[0U]);
  r75 = r98;
  MultiWordSub(&r76.chunks[0U], &r75.chunks[0U], &r77.chunks[0U], 2);
  MultiWordSignedWrap(&r77.chunks[0U], 2, 28U, &l_c.chunks[0U]);
  r6 = r53;
  u1 = (unsigned long)((long)b_kb * b_kt);
  u2 = rt;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r104.chunks[0U], 2);
  r77 = r55;
  if (sMultiWordEq(&r6.chunks[0U], &r77.chunks[0U], 2)) {
    r76 = r55;
    if (sMultiWordGe(&r104.chunks[0U], &r76.chunks[0U], 2)) {
      r105 = r60;
    } else {
      r105 = r59;
    }
  } else {
    sMultiWord2MultiWord(&r104.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordShl(&r56.chunks[0U], 4, 54U, &r57.chunks[0U], 4);
    sMultiWord2MultiWord(&r6.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordDivFloor(&r57.chunks[0U], 4, &r56.chunks[0U], 4, &r61.chunks[0U],
                       5, &r62.chunks[0U], 4, &r63.chunks[0U], 4, &r64.chunks[0U],
                       4);
    sMultiWord2MultiWord(&r61.chunks[0U], 5, &r76.chunks[0U], 2);
    MultiWordSignedWrap(&r76.chunks[0U], 2, 16U, &r105.chunks[0U]);
  }

  u1 = (unsigned long)b_kb;
  uLong2MultiWord(b_kt, &r73.chunks[0U], 2);
  MultiWordSignedWrap(&r73.chunks[0U], 2, 28U, &r70.chunks[0U]);
  r73 = r98;
  MultiWordSub(&r70.chunks[0U], &r73.chunks[0U], &r71.chunks[0U], 2);
  MultiWordSignedWrap(&r71.chunks[0U], 2, 28U, &r74.chunks[0U]);
  sMultiWordMul(&u1, 1, &r74.chunks[0U], 2, &r75.chunks[0U], 2);
  u1 = Pwt_p;
  ssuMultiWordMul(&r75.chunks[0U], 2, &u1, 1, &r67.chunks[0U], 3);
  MultiWordSignedWrap(&r67.chunks[0U], 3, 4U, &r65.chunks[0U]);
  sLong2MultiWord(b_kb, &r70.chunks[0U], 2);
  MultiWordSignedWrap(&r70.chunks[0U], 2, 26U, &r71.chunks[0U]);
  r70 = r101;
  MultiWordSub(&r71.chunks[0U], &r70.chunks[0U], &r74.chunks[0U], 2);
  MultiWordSignedWrap(&r74.chunks[0U], 2, 26U, &r75.chunks[0U]);
  uLong2MultiWord(b_kt, &r73.chunks[0U], 2);
  MultiWordSignedWrap(&r73.chunks[0U], 2, 28U, &r70.chunks[0U]);
  r73 = r98;
  MultiWordSub(&r70.chunks[0U], &r73.chunks[0U], &r71.chunks[0U], 2);
  MultiWordSignedWrap(&r71.chunks[0U], 2, 28U, &r74.chunks[0U]);
  sMultiWordMul(&r75.chunks[0U], 2, &r74.chunks[0U], 2, &r103.chunks[0U], 3);
  u1 = (unsigned long)b_Ptb_p;
  sMultiWordMul(&r103.chunks[0U], 3, &u1, 1, &r91.chunks[0U], 3);
  sMultiWordShl(&r91.chunks[0U], 3, 1U, &r92.chunks[0U], 3);
  MultiWordSignedWrap(&r92.chunks[0U], 3, 4U, &r67.chunks[0U]);
  MultiWordAdd(&r65.chunks[0U], &r67.chunks[0U], &r66.chunks[0U], 3);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 4U, &r36.chunks[0U]);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 3U, &r37.chunks[0U]);
  sMultiWordShl(&r105.chunks[0U], 2, 6U, &r75.chunks[0U], 2);
  sMultiWord2MultiWord(&r75.chunks[0U], 2, &r66.chunks[0U], 3);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 3U, &r36.chunks[0U]);
  MultiWordAdd(&r37.chunks[0U], &r36.chunks[0U], &r34.chunks[0U], 3);
  MultiWordSignedWrap(&r34.chunks[0U], 3, 3U, &r35.chunks[0U]);
  sMultiWordShr(&r35.chunks[0U], 3, 55U, &r38.chunks[0U], 3);
  Ptb = (int)MultiWord2sLong(&r38.chunks[0U]);
  sLong2MultiWord(b_kb, &r71.chunks[0U], 2);
  MultiWordSignedWrap(&r71.chunks[0U], 2, 26U, &r74.chunks[0U]);
  r71 = r101;
  MultiWordSub(&r74.chunks[0U], &r71.chunks[0U], &r75.chunks[0U], 2);
  MultiWordSignedWrap(&r75.chunks[0U], 2, 26U, &m_c.chunks[0U]);
  i13 = AccTheta;
  if ((i13 & 65536L) != 0L) {
    i34 = i13 | -65536L;
  } else {
    i34 = i13 & 65535L;
  }

  if ((i14 & 65536L) != 0L) {
    i35 = i14 | -65536L;
  } else {
    i35 = i14 & 65535L;
  }

  i0 = i34 - i35;
  sLong2MultiWord(*Theta, &r72.chunks[0U], 2);
  sMultiWordShl(&r72.chunks[0U], 2, 26U, &r73.chunks[0U], 2);
  MultiWordSignedWrap(&r73.chunks[0U], 2, 21U, &r70.chunks[0U]);
  if ((i0 & 65536L) != 0L) {
    u1 = (unsigned long)(i0 | -65536L);
  } else {
    u1 = (unsigned long)(i0 & 65535L);
  }

  u2 = b_kt;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r72.chunks[0U], 2);
  MultiWordSignedWrap(&r72.chunks[0U], 2, 21U, &r73.chunks[0U]);
  MultiWordAdd(&r70.chunks[0U], &r73.chunks[0U], &r71.chunks[0U], 2);
  MultiWordSignedWrap(&r71.chunks[0U], 2, 21U, &r74.chunks[0U]);
  sMultiWordShr(&r74.chunks[0U], 2, 22U, &r75.chunks[0U], 2);
  *Theta = (int)MultiWord2sLong(&r75.chunks[0U]) >> 4;
  sLong2MultiWord(*dTheta, &r90.chunks[0U], 2);
  sMultiWordShl(&r90.chunks[0U], 2, 29U, &r72.chunks[0U], 2);
  MultiWordSignedWrap(&r72.chunks[0U], 2, 18U, &r73.chunks[0U]);
  MultiWordSignedWrap(&j_c.chunks[0U], 2, 18U, &r72.chunks[0U]);
  MultiWordAdd(&r73.chunks[0U], &r72.chunks[0U], &r70.chunks[0U], 2);
  MultiWordSignedWrap(&r70.chunks[0U], 2, 18U, &r71.chunks[0U]);
  sMultiWordShr(&r71.chunks[0U], 2, 25U, &r74.chunks[0U], 2);
  *dTheta = (int)MultiWord2sLong(&r74.chunks[0U]) >> 4;
  sLong2MultiWord(*Bias, &r106.chunks[0U], 2);
  sMultiWordShl(&r106.chunks[0U], 2, 26U, &r90.chunks[0U], 2);
  MultiWordSignedWrap(&r90.chunks[0U], 2, 21U, &r72.chunks[0U]);
  MultiWordSignedWrap(&k_c.chunks[0U], 2, 21U, &r90.chunks[0U]);
  MultiWordAdd(&r72.chunks[0U], &r90.chunks[0U], &r73.chunks[0U], 2);
  MultiWordSignedWrap(&r73.chunks[0U], 2, 21U, &r70.chunks[0U]);
  sMultiWordShr(&r70.chunks[0U], 2, 19U, &r71.chunks[0U], 2);
  *Bias = (int)MultiWord2sLong(&r71.chunks[0U]) >> 7;
  i0 = -2L * kw;
  uLong2MultiWord(Pww_p, &r17.chunks[0U], 2);
  uMultiWordShl(&r17.chunks[0U], 2, 26U, &r0.chunks[0U], 2);
  uMultiWord2MultiWord(&r0.chunks[0U], 2, &r90.chunks[0U], 2);
  MultiWordSignedWrap(&r90.chunks[0U], 2, 20U, &r72.chunks[0U]);
  if ((i0 & 8388608L) != 0L) {
    u1 = (unsigned long)(i0 | -8388608L);
  } else {
    u1 = (unsigned long)(i0 & 8388607L);
  }

  u2 = Pwt_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r106.chunks[0U], 2);
  MultiWordSignedWrap(&r106.chunks[0U], 2, 20U, &r90.chunks[0U]);
  MultiWordAdd(&r72.chunks[0U], &r90.chunks[0U], &r73.chunks[0U], 2);
  MultiWordSignedWrap(&r73.chunks[0U], 2, 20U, &r70.chunks[0U]);
  sMultiWord2MultiWord(&r70.chunks[0U], 2, &r91.chunks[0U], 3);
  sMultiWordShl(&r91.chunks[0U], 3, 25U, &r92.chunks[0U], 3);
  MultiWordSignedWrap(&r92.chunks[0U], 3, 25U, &r67.chunks[0U]);
  u1 = (unsigned long)kw * kw;
  u2 = Ptt_p;
  uMultiWordMul(&u1, 1, &u2, 1, &r0.chunks[0U], 2);
  uMultiWord2MultiWord(&r0.chunks[0U], 2, &r91.chunks[0U], 3);
  MultiWordSignedWrap(&r91.chunks[0U], 3, 25U, &r92.chunks[0U]);
  MultiWordAdd(&r67.chunks[0U], &r92.chunks[0U], &r65.chunks[0U], 3);
  MultiWordSignedWrap(&r65.chunks[0U], 3, 25U, &r66.chunks[0U]);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 23U, &r36.chunks[0U]);
  u1 = (unsigned long)kw * kw;
  u2 = rt;
  uMultiWordMul(&u1, 1, &u2, 1, &r13.chunks[0U], 2);
  uMultiWord2MultiWord(&r13.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordShl(&r44.chunks[0U], 4, 55U, &r45.chunks[0U], 4);
  r13 = r46;
  uMultiWord2MultiWord(&r13.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordDivZero(&r45.chunks[0U], 4, &r44.chunks[0U], 4, &r47.chunks[0U], 4,
                    &r48.chunks[0U], 4, &r49.chunks[0U], 4, &r50.chunks[0U], 4);
  uMultiWord2MultiWord(&r47.chunks[0U], 4, &r14.chunks[0U], 2);
  MultiWordUnsignedWrap(&r14.chunks[0U], 2, 16U, &r17.chunks[0U]);
  uMultiWordShl(&r17.chunks[0U], 2, 2U, &r0.chunks[0U], 2);
  uMultiWord2MultiWord(&r0.chunks[0U], 2, &r65.chunks[0U], 3);
  MultiWordSignedWrap(&r65.chunks[0U], 3, 23U, &r66.chunks[0U]);
  MultiWordAdd(&r36.chunks[0U], &r66.chunks[0U], &r37.chunks[0U], 3);
  MultiWordSignedWrap(&r37.chunks[0U], 3, 23U, &r34.chunks[0U]);
  sMultiWordShr(&r34.chunks[0U], 3, 51U, &r35.chunks[0U], 3);
  P[0] = (int)((unsigned int)MultiWord2uLong(&r35.chunks[0U]) >> 8) >> 1;
  P[3] = (int)(Pwt >> 5) >> 1;
  P[6] = Pwb >> 8;
  P[1] = (int)(Pwt >> 5) >> 1;
  sMultiWordMul(&l_c.chunks[0U], 2, &l_c.chunks[0U], 2, &r67.chunks[0U], 3);
  u1 = Ptt_p;
  ssuMultiWordMul(&r67.chunks[0U], 3, &u1, 1, &r65.chunks[0U], 3);
  MultiWordSignedWrap(&r65.chunks[0U], 3, 6U, &r66.chunks[0U]);
  u1 = (unsigned long)b_kt * b_kt;
  u2 = rt;
  uMultiWordMul(&u1, 1, &u2, 1, &r13.chunks[0U], 2);
  uMultiWord2MultiWord(&r13.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordShl(&r44.chunks[0U], 4, 55U, &r45.chunks[0U], 4);
  r13 = r46;
  uMultiWord2MultiWord(&r13.chunks[0U], 2, &r44.chunks[0U], 4);
  uMultiWordDivZero(&r45.chunks[0U], 4, &r44.chunks[0U], 4, &r47.chunks[0U], 4,
                    &r48.chunks[0U], 4, &r49.chunks[0U], 4, &r50.chunks[0U], 4);
  uMultiWord2MultiWord(&r47.chunks[0U], 4, &r14.chunks[0U], 2);
  MultiWordUnsignedWrap(&r14.chunks[0U], 2, 16U, &r17.chunks[0U]);
  uMultiWordShl(&r17.chunks[0U], 2, 2U, &r0.chunks[0U], 2);
  uMultiWord2MultiWord(&r0.chunks[0U], 2, &r67.chunks[0U], 3);
  MultiWordSignedWrap(&r67.chunks[0U], 3, 6U, &r65.chunks[0U]);
  MultiWordAdd(&r66.chunks[0U], &r65.chunks[0U], &r36.chunks[0U], 3);
  MultiWordSignedWrap(&r36.chunks[0U], 3, 6U, &r37.chunks[0U]);
  sMultiWordShr(&r37.chunks[0U], 3, 52U, &r34.chunks[0U], 3);
  P[4] = (int)((unsigned int)MultiWord2uLong(&r34.chunks[0U]) >> 1) >> 1;
  P[7] = Ptb >> 5;
  P[2] = Pwb >> 8;
  P[5] = Ptb >> 5;
  i0 = (long)b_kb << 1;
  r6 = r53;
  u1 = (unsigned long)((long)b_kb * b_kb);
  u2 = rt;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r107.chunks[0U], 2);
  r70 = r55;
  if (sMultiWordEq(&r6.chunks[0U], &r70.chunks[0U], 2)) {
    r73 = r55;
    if (sMultiWordGe(&r107.chunks[0U], &r73.chunks[0U], 2)) {
      r108 = r60;
    } else {
      r108 = r59;
    }
  } else {
    sMultiWord2MultiWord(&r107.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordShl(&r56.chunks[0U], 4, 54U, &r57.chunks[0U], 4);
    sMultiWord2MultiWord(&r6.chunks[0U], 2, &r56.chunks[0U], 4);
    sMultiWordDivFloor(&r57.chunks[0U], 4, &r56.chunks[0U], 4, &r61.chunks[0U],
                       5, &r62.chunks[0U], 4, &r63.chunks[0U], 4, &r64.chunks[0U],
                       4);
    sMultiWord2MultiWord(&r61.chunks[0U], 5, &r73.chunks[0U], 2);
    MultiWordSignedWrap(&r73.chunks[0U], 2, 16U, &r108.chunks[0U]);
  }

  u1 = (unsigned long)((long)b_kb * b_kb);
  u2 = Pww_p;
  ssuMultiWordMul(&u1, 1, &u2, 1, &r72.chunks[0U], 2);
  sMultiWord2MultiWord(&r72.chunks[0U], 2, &r109.chunks[0U], 3);
  MultiWordSignedWrap(&r109.chunks[0U], 3, 16U, &r110.chunks[0U]);
  if ((i0 & 8388608L) != 0L) {
    u1 = (unsigned long)(i0 | -8388608L);
  } else {
    u1 = (unsigned long)(i0 & 8388607L);
  }

  sLong2MultiWord(b_kb, &r111.chunks[0U], 2);
  MultiWordSignedWrap(&r111.chunks[0U], 2, 26U, &r112.chunks[0U]);
  r111 = r101;
  MultiWordSub(&r112.chunks[0U], &r111.chunks[0U], &r106.chunks[0U], 2);
  MultiWordSignedWrap(&r106.chunks[0U], 2, 26U, &r90.chunks[0U]);
  sMultiWordMul(&u1, 1, &r90.chunks[0U], 2, &r72.chunks[0U], 2);
  u1 = (unsigned long)b_Pwb_p;
  sMultiWordMul(&r72.chunks[0U], 2, &u1, 1, &r113.chunks[0U], 3);
  sMultiWordShl(&r113.chunks[0U], 3, 1U, &r114.chunks[0U], 3);
  MultiWordSignedWrap(&r114.chunks[0U], 3, 16U, &r109.chunks[0U]);
  MultiWordAdd(&r110.chunks[0U], &r109.chunks[0U], &r102.chunks[0U], 3);
  MultiWordSignedWrap(&r102.chunks[0U], 3, 16U, &r103.chunks[0U]);
  MultiWordSignedWrap(&r103.chunks[0U], 3, 3U, &r91.chunks[0U]);
  sMultiWordMul(&m_c.chunks[0U], 2, &m_c.chunks[0U], 2, &r110.chunks[0U], 3);
  u1 = (unsigned int)P[8] << 9;
  ssuMultiWordMul(&r110.chunks[0U], 3, &u1, 1, &r102.chunks[0U], 3);
  MultiWordSignedWrap(&r102.chunks[0U], 3, 3U, &r103.chunks[0U]);
  MultiWordAdd(&r91.chunks[0U], &r103.chunks[0U], &r92.chunks[0U], 3);
  MultiWordSignedWrap(&r92.chunks[0U], 3, 3U, &r67.chunks[0U]);
  MultiWordSignedWrap(&r67.chunks[0U], 3, 2U, &r65.chunks[0U]);
  sMultiWordShl(&r108.chunks[0U], 2, 9U, &r72.chunks[0U], 2);
  sMultiWord2MultiWord(&r72.chunks[0U], 2, &r92.chunks[0U], 3);
  MultiWordSignedWrap(&r92.chunks[0U], 3, 2U, &r67.chunks[0U]);
  MultiWordAdd(&r65.chunks[0U], &r67.chunks[0U], &r66.chunks[0U], 3);
  MultiWordSignedWrap(&r66.chunks[0U], 3, 2U, &r36.chunks[0U]);
  sMultiWordShr(&r36.chunks[0U], 3, 56U, &r37.chunks[0U], 3);
  P[8] = (int)((unsigned int)MultiWord2uLong(&r37.chunks[0U]) >> 4) >> 5;
}

/*
 * File trailer for IMUKalmanFilter_fixpt.c
 *
 * [EOF]
 */
