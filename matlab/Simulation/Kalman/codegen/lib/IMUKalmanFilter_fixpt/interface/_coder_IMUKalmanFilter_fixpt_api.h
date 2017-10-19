/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IMUKalmanFilter_fixpt_api.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 18-Oct-2017 15:29:18
 */

#ifndef _CODER_IMUKALMANFILTER_FIXPT_API_H
#define _CODER_IMUKALMANFILTER_FIXPT_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_IMUKalmanFilter_fixpt_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void IMUKalmanFilter_fixpt(int16_T Theta_1, int16_T dTheta_1, int16_T
  Bias_1, int32_T P_1[9], int16_T Gyro, int16_T AccTheta, uint16_T qw, uint16_T
  qb, uint16_T rw, uint16_T rt, int16_T *Theta, int16_T *dTheta, int16_T *Bias,
  int32_T P[9]);
extern void IMUKalmanFilter_fixpt_api(const mxArray * const prhs[10], const
  mxArray *plhs[4]);
extern void IMUKalmanFilter_fixpt_atexit(void);
extern void IMUKalmanFilter_fixpt_initialize(void);
extern void IMUKalmanFilter_fixpt_terminate(void);
extern void IMUKalmanFilter_fixpt_xil_terminate(void);

#endif

/*
 * File trailer for _coder_IMUKalmanFilter_fixpt_api.h
 *
 * [EOF]
 */
