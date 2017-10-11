#include "__cf_motor_freerun.h"
#include <math.h>
#include "motor_freerun_acc.h"
#include "motor_freerun_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
static void mdlOutputs ( SimStruct * S , int_T tid ) { real_T nsbpiu4gus ;
real_T jyv55g5kh2 ; real_T jljtpusbdb ; p4ecftnajw * _rtB ; a4tp0s2zv5 * _rtP
; elxoupzbnb * _rtDW ; _rtDW = ( ( elxoupzbnb * ) ssGetRootDWork ( S ) ) ;
_rtP = ( ( a4tp0s2zv5 * ) ssGetModelRtp ( S ) ) ; _rtB = ( ( p4ecftnajw * )
_ssGetModelBlockIO ( S ) ) ; ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 0 , 1 , SS_CALL_MDL_OUTPUTS
) ; ssCallAccelRunBlock ( S , 0 , 2 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 0 , 3 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock
( S , 0 , 4 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 0 , 5 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 0 , 6 , SS_CALL_MDL_OUTPUTS
) ; ssCallAccelRunBlock ( S , 0 , 7 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 0 , 8 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> lqveyzvsr2
= muDoubleScalarSin ( _rtP -> P_314 * ssGetTaskTime ( S , 0 ) + _rtP -> P_315
) * _rtP -> P_312 + _rtP -> P_313 ; _rtB -> c4da1skt1p = _rtB -> lqveyzvsr2 ;
_rtB -> l4hvm3tkav = _rtB -> c4da1skt1p - _rtDW -> apau5a4xdf ; nsbpiu4gus =
ssGetT ( S ) ; _rtB -> jqyl0iqmyq = ssGetTaskTime ( S , 1 ) ; _rtB ->
abswcrldmp = ( nsbpiu4gus - _rtB -> jqyl0iqmyq ) * _rtP -> P_317 ; _rtB ->
dx4zm3x3bh = _rtB -> l4hvm3tkav * _rtB -> abswcrldmp + _rtB -> c4da1skt1p ;
ssCallAccelRunBlock ( S , 0 , 19 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> pfea0x2o3n
= muDoubleScalarRound ( _rtB -> dx4zm3x3bh / 12.0 * 65535.0 *
muDoubleScalarAbs ( _rtB -> dx4zm3x3bh / 12.0 ) / _rtP -> P_318 ) * _rtP ->
P_318 ; ssCallAccelRunBlock ( S , 0 , 22 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
avebi5l445 = muDoubleScalarSin ( _rtP -> P_390 * ssGetTaskTime ( S , 0 ) +
_rtP -> P_391 ) * _rtP -> P_388 + _rtP -> P_389 ; _rtB -> kemcuogzob = _rtB
-> avebi5l445 ; _rtB -> mjhe3cpjum = _rtB -> kemcuogzob - _rtDW -> o1vuc3jtrj
; jyv55g5kh2 = ssGetT ( S ) ; _rtB -> cy3nejcg4e = ssGetTaskTime ( S , 1 ) ;
_rtB -> biudwsrqed = ( jyv55g5kh2 - _rtB -> cy3nejcg4e ) * _rtP -> P_393 ;
_rtB -> lk1xqz34b2 = _rtB -> mjhe3cpjum * _rtB -> biudwsrqed + _rtB ->
kemcuogzob ; ssCallAccelRunBlock ( S , 0 , 34 , SS_CALL_MDL_OUTPUTS ) ; _rtB
-> fwt5yqevtz = muDoubleScalarRound ( _rtB -> lk1xqz34b2 / 12.0 * 65535.0 *
muDoubleScalarAbs ( _rtB -> lk1xqz34b2 / 12.0 ) / _rtP -> P_394 ) * _rtP ->
P_394 ; ssCallAccelRunBlock ( S , 0 , 37 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
lkczehtq2y = ssGetT ( S ) ; _rtB -> ddchauyfku = _rtB -> lkczehtq2y ; _rtB ->
aypz45bqm4 = _rtB -> ddchauyfku - _rtDW -> g0cognx2kg ; jljtpusbdb = ssGetT (
S ) ; _rtB -> hiip2e3abu = ssGetTaskTime ( S , 1 ) ; _rtB -> pi5eg1efub = (
jljtpusbdb - _rtB -> hiip2e3abu ) * _rtP -> P_465 ; _rtB -> o2bulfhvka = _rtB
-> aypz45bqm4 * _rtB -> pi5eg1efub + _rtB -> ddchauyfku ; ssCallAccelRunBlock
( S , 0 , 49 , SS_CALL_MDL_OUTPUTS ) ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { p4ecftnajw * _rtB ;
elxoupzbnb * _rtDW ; _rtDW = ( ( elxoupzbnb * ) ssGetRootDWork ( S ) ) ; _rtB
= ( ( p4ecftnajw * ) _ssGetModelBlockIO ( S ) ) ; _rtDW -> apau5a4xdf = _rtB
-> lqveyzvsr2 ; _rtDW -> o1vuc3jtrj = _rtB -> avebi5l445 ; _rtDW ->
g0cognx2kg = _rtB -> lkczehtq2y ; UNUSED_PARAMETER ( tid ) ; } static void
mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 , 3421896733U
) ; ssSetChecksumVal ( S , 1 , 2576260230U ) ; ssSetChecksumVal ( S , 2 ,
1437528750U ) ; ssSetChecksumVal ( S , 3 , 1411631449U ) ; { mxArray *
slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" ) ;
char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1
, & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField
( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; }
else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; } }
mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "8.7" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( elxoupzbnb ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( p4ecftnajw ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
a4tp0s2zv5 ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & edn3skvqjt ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
