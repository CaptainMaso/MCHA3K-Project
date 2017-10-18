/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 18-Oct-2017 15:29:18
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
/* Include Files */
#include "rt_nonfinite.h"
#include "IMUKalmanFilter_fixpt.h"
#include "main.h"
#include "IMUKalmanFilter_fixpt_terminate.h"
#include "IMUKalmanFilter_fixpt_initialize.h"

/* Function Declarations */
static void argInit_3x3_sfix32_En4(long result[9]);
static int argInit_int16(void);
static int argInit_sfix16_En2(void);
static long argInit_sfix32_En4(void);
static unsigned int argInit_ufix16_En12(void);
static unsigned int argInit_ufix16_En3(void);
static unsigned int argInit_ufix16_En4(void);
static void main_IMUKalmanFilter_fixpt(void);

/* Function Definitions */

/*
 * Arguments    : long result[9]
 * Return Type  : void
 */
static void argInit_3x3_sfix32_En4(long result[9])
{
  int idx0;
  int idx1;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 3; idx0++) {
    for (idx1 = 0; idx1 < 3; idx1++) {
      /* Set the value of the array element.
         Change this value to the value that the application requires. */
      result[idx0 + 3 * idx1] = argInit_sfix32_En4();
    }
  }
}

/*
 * Arguments    : void
 * Return Type  : int
 */
static int argInit_int16(void)
{
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : int
 */
static int argInit_sfix16_En2(void)
{
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : long
 */
static long argInit_sfix32_En4(void)
{
  return 0L;
}

/*
 * Arguments    : void
 * Return Type  : unsigned int
 */
static unsigned int argInit_ufix16_En12(void)
{
  return 0U;
}

/*
 * Arguments    : void
 * Return Type  : unsigned int
 */
static unsigned int argInit_ufix16_En3(void)
{
  return 0U;
}

/*
 * Arguments    : void
 * Return Type  : unsigned int
 */
static unsigned int argInit_ufix16_En4(void)
{
  return 0U;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_IMUKalmanFilter_fixpt(void)
{
  long iv0[9];
  int Theta;
  int dTheta;
  int Bias;
  long P[9];

  /* Initialize function 'IMUKalmanFilter_fixpt' input arguments. */
  /* Initialize function input argument 'P_1'. */
  /* Call the entry-point 'IMUKalmanFilter_fixpt'. */
  argInit_3x3_sfix32_En4(iv0);
  IMUKalmanFilter_fixpt(argInit_int16(), argInit_int16(), argInit_sfix16_En2(),
                        iv0, argInit_int16(), argInit_int16(),
                        argInit_ufix16_En3(), argInit_ufix16_En12(),
                        argInit_ufix16_En12(), argInit_ufix16_En4(), &Theta,
                        &dTheta, &Bias, P);
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* Initialize the application.
     You do not need to do this more than one time. */
  IMUKalmanFilter_fixpt_initialize();

  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_IMUKalmanFilter_fixpt();

  /* Terminate the application.
     You do not need to do this more than one time. */
  IMUKalmanFilter_fixpt_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
