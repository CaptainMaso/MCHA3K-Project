/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IMUKalmanFilter_fixpt.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 18-Oct-2017 15:29:18
 */

#ifndef IMUKALMANFILTER_FIXPT_H
#define IMUKALMANFILTER_FIXPT_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "IMUKalmanFilter_fixpt_types.h"

/* Function Declarations */
extern void IMUKalmanFilter_fixpt(int Theta_1, int dTheta_1, int Bias_1, const
  long P_1[9], int Gyro, int AccTheta, unsigned int qw, unsigned int qb,
  unsigned int rw, unsigned int rt, int *Theta, int *dTheta, int *Bias, long P[9]);

#endif

/*
 * File trailer for IMUKalmanFilter_fixpt.h
 *
 * [EOF]
 */
