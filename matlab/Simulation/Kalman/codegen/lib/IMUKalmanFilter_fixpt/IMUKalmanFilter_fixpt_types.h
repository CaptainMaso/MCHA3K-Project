/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IMUKalmanFilter_fixpt_types.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 18-Oct-2017 15:29:18
 */

#ifndef IMUKALMANFILTER_FIXPT_TYPES_H
#define IMUKALMANFILTER_FIXPT_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_int128m_T
#define typedef_int128m_T

typedef struct {
  unsigned long chunks[4];
} int128m_T;

#endif                                 /*typedef_int128m_T*/

#ifndef typedef_int160m_T
#define typedef_int160m_T

typedef struct {
  unsigned long chunks[5];
} int160m_T;

#endif                                 /*typedef_int160m_T*/

#ifndef typedef_int64m_T
#define typedef_int64m_T

typedef struct {
  unsigned long chunks[2];
} int64m_T;

#endif                                 /*typedef_int64m_T*/

#ifndef typedef_int96m_T
#define typedef_int96m_T

typedef struct {
  unsigned long chunks[3];
} int96m_T;

#endif                                 /*typedef_int96m_T*/

#ifndef typedef_uint128m_T
#define typedef_uint128m_T

typedef struct {
  unsigned long chunks[4];
} uint128m_T;

#endif                                 /*typedef_uint128m_T*/

#ifndef typedef_uint64m_T
#define typedef_uint64m_T

typedef struct {
  unsigned long chunks[2];
} uint64m_T;

#endif                                 /*typedef_uint64m_T*/

#ifndef typedef_uint96m_T
#define typedef_uint96m_T

typedef struct {
  unsigned long chunks[3];
} uint96m_T;

#endif                                 /*typedef_uint96m_T*/
#endif

/*
 * File trailer for IMUKalmanFilter_fixpt_types.h
 *
 * [EOF]
 */
