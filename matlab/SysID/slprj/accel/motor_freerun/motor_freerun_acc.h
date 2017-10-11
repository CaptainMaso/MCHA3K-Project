#include "__cf_motor_freerun.h"
#ifndef RTW_HEADER_motor_freerun_acc_h_
#define RTW_HEADER_motor_freerun_acc_h_
#include <stddef.h>
#ifndef motor_freerun_acc_COMMON_INCLUDES_
#define motor_freerun_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "motor_freerun_acc_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
typedef struct { real_T hy20f3fz5n ; real_T kkj0k2rj5b ; real_T hy5s12ce2h ;
real_T nxwo2kljxx ; real_T lqveyzvsr2 ; real_T c4da1skt1p ; real_T l4hvm3tkav
; real_T jqyl0iqmyq ; real_T abswcrldmp ; real_T dx4zm3x3bh ; real_T
pfea0x2o3n ; real_T avebi5l445 ; real_T kemcuogzob ; real_T mjhe3cpjum ;
real_T cy3nejcg4e ; real_T biudwsrqed ; real_T lk1xqz34b2 ; real_T fwt5yqevtz
; real_T lkczehtq2y ; real_T ddchauyfku ; real_T aypz45bqm4 ; real_T
hiip2e3abu ; real_T pi5eg1efub ; real_T o2bulfhvka ; } p4ecftnajw ; typedef
struct { real_T apau5a4xdf ; real_T o1vuc3jtrj ; real_T g0cognx2kg ; void *
okvspyi4ca ; void * m00t1bnj3r ; void * amwytbjs2i ; void * oyvi3342u2 ; void
* kp3zuhlakr ; void * omdpp4v5td ; void * ijfzyd1vb1 ; void * mo5hdasa0l ;
void * abaixtxp4p ; void * nbyjz1xmnv ; void * orq1ljjpbo ; void * owunzwquvh
; void * bfzwootbgn ; void * buew55rplf ; } elxoupzbnb ; struct a4tp0s2zv5_ {
real_T P_0 [ 2 ] ; real_T P_1 ; real_T P_2 [ 2 ] ; real_T P_3 ; real_T P_4 [
2 ] ; real_T P_5 ; real_T P_6 [ 2 ] ; real_T P_7 ; real_T P_8 [ 2 ] ; real_T
P_9 ; real_T P_10 [ 2 ] ; real_T P_11 ; real_T P_12 [ 2 ] ; real_T P_13 ;
real_T P_14 [ 2 ] ; real_T P_15 ; real_T P_16 [ 2 ] ; real_T P_17 ; real_T
P_18 [ 2 ] ; real_T P_19 ; real_T P_20 [ 2 ] ; real_T P_21 ; real_T P_22 [ 2
] ; real_T P_23 ; real_T P_24 [ 2 ] ; real_T P_25 ; real_T P_26 [ 2 ] ;
real_T P_27 ; real_T P_28 [ 2 ] ; real_T P_29 ; real_T P_30 [ 2 ] ; real_T
P_31 [ 9 ] ; real_T P_32 [ 2 ] ; real_T P_33 ; real_T P_34 [ 2 ] ; real_T
P_35 [ 9 ] ; real_T P_36 [ 2 ] ; real_T P_37 ; real_T P_38 [ 2 ] ; real_T
P_40 [ 2 ] ; real_T P_41 ; real_T P_42 [ 2 ] ; real_T P_43 [ 15 ] ; real_T
P_44 [ 2 ] ; real_T P_45 [ 3 ] ; real_T P_46 [ 2 ] ; real_T P_47 [ 12 ] ;
real_T P_48 [ 2 ] ; real_T P_49 ; real_T P_50 [ 2 ] ; real_T P_51 [ 12 ] ;
real_T P_52 [ 2 ] ; real_T P_53 ; real_T P_54 [ 2 ] ; real_T P_55 ; real_T
P_56 [ 2 ] ; real_T P_57 ; real_T P_58 [ 2 ] ; real_T P_59 ; real_T P_60 [ 2
] ; real_T P_61 ; real_T P_62 [ 2 ] ; real_T P_63 ; real_T P_64 [ 2 ] ;
real_T P_65 ; real_T P_66 [ 2 ] ; real_T P_67 ; real_T P_68 [ 2 ] ; real_T
P_69 [ 9 ] ; real_T P_70 [ 2 ] ; real_T P_71 ; real_T P_72 [ 2 ] ; real_T
P_73 ; real_T P_74 [ 2 ] ; real_T P_75 ; real_T P_76 [ 2 ] ; real_T P_77 ;
real_T P_78 [ 2 ] ; real_T P_79 ; real_T P_80 [ 2 ] ; real_T P_81 ; real_T
P_82 [ 2 ] ; real_T P_83 ; real_T P_84 [ 2 ] ; real_T P_85 ; real_T P_86 [ 2
] ; real_T P_87 ; real_T P_88 [ 2 ] ; real_T P_89 ; real_T P_90 [ 2 ] ;
real_T P_91 ; real_T P_92 [ 2 ] ; real_T P_93 ; real_T P_94 [ 2 ] ; real_T
P_95 ; real_T P_96 [ 2 ] ; real_T P_97 ; real_T P_98 [ 2 ] ; real_T P_99 ;
real_T P_100 [ 2 ] ; real_T P_101 ; real_T P_102 [ 2 ] ; real_T P_103 ;
real_T P_104 [ 2 ] ; real_T P_105 ; real_T P_106 [ 2 ] ; real_T P_107 ;
real_T P_108 [ 2 ] ; real_T P_109 [ 9 ] ; real_T P_110 [ 2 ] ; real_T P_111 ;
real_T P_112 [ 2 ] ; real_T P_113 [ 9 ] ; real_T P_114 [ 2 ] ; real_T P_115 ;
real_T P_116 [ 2 ] ; real_T P_118 [ 2 ] ; real_T P_119 ; real_T P_120 [ 2 ] ;
real_T P_121 [ 15 ] ; real_T P_122 [ 2 ] ; real_T P_123 [ 3 ] ; real_T P_124
[ 2 ] ; real_T P_125 [ 12 ] ; real_T P_126 [ 2 ] ; real_T P_127 ; real_T
P_128 [ 2 ] ; real_T P_129 [ 12 ] ; real_T P_130 [ 2 ] ; real_T P_131 ;
real_T P_132 [ 2 ] ; real_T P_133 ; real_T P_134 [ 2 ] ; real_T P_135 ;
real_T P_136 [ 2 ] ; real_T P_137 ; real_T P_138 [ 2 ] ; real_T P_139 ;
real_T P_140 [ 2 ] ; real_T P_141 ; real_T P_142 [ 2 ] ; real_T P_143 ;
real_T P_144 [ 2 ] ; real_T P_145 ; real_T P_146 [ 2 ] ; real_T P_147 [ 9 ] ;
real_T P_148 [ 2 ] ; real_T P_149 ; real_T P_150 [ 2 ] ; real_T P_151 ;
real_T P_152 [ 2 ] ; real_T P_153 ; real_T P_154 [ 2 ] ; real_T P_155 ;
real_T P_156 [ 2 ] ; real_T P_157 ; real_T P_158 [ 2 ] ; real_T P_159 ;
real_T P_160 [ 2 ] ; real_T P_161 ; real_T P_162 [ 2 ] ; real_T P_163 ;
real_T P_164 [ 2 ] ; real_T P_165 ; real_T P_166 [ 2 ] ; real_T P_167 ;
real_T P_168 [ 2 ] ; real_T P_169 ; real_T P_170 [ 2 ] ; real_T P_171 ;
real_T P_172 [ 2 ] ; real_T P_173 ; real_T P_174 [ 2 ] ; real_T P_175 ;
real_T P_176 [ 2 ] ; real_T P_177 ; real_T P_178 [ 2 ] ; real_T P_179 ;
real_T P_180 [ 2 ] ; real_T P_181 ; real_T P_182 [ 2 ] ; real_T P_183 ;
real_T P_184 [ 2 ] ; real_T P_185 ; real_T P_186 [ 2 ] ; real_T P_187 [ 9 ] ;
real_T P_188 [ 2 ] ; real_T P_189 ; real_T P_190 [ 2 ] ; real_T P_191 [ 9 ] ;
real_T P_192 [ 2 ] ; real_T P_193 ; real_T P_194 [ 2 ] ; real_T P_196 [ 2 ] ;
real_T P_197 ; real_T P_198 [ 2 ] ; real_T P_199 [ 15 ] ; real_T P_200 [ 2 ]
; real_T P_201 [ 3 ] ; real_T P_202 [ 2 ] ; real_T P_203 [ 12 ] ; real_T
P_204 [ 2 ] ; real_T P_205 ; real_T P_206 [ 2 ] ; real_T P_207 [ 12 ] ;
real_T P_208 [ 2 ] ; real_T P_209 ; real_T P_210 [ 2 ] ; real_T P_211 ;
real_T P_212 [ 2 ] ; real_T P_213 ; real_T P_214 [ 2 ] ; real_T P_215 ;
real_T P_216 [ 2 ] ; real_T P_217 ; real_T P_218 [ 2 ] ; real_T P_219 ;
real_T P_220 [ 2 ] ; real_T P_221 ; real_T P_222 [ 2 ] ; real_T P_223 ;
real_T P_224 [ 2 ] ; real_T P_225 [ 9 ] ; real_T P_226 [ 2 ] ; real_T P_227 ;
real_T P_228 [ 2 ] ; real_T P_229 ; real_T P_230 [ 2 ] ; real_T P_231 ;
real_T P_232 [ 2 ] ; real_T P_233 ; real_T P_234 [ 2 ] ; real_T P_235 ;
real_T P_236 [ 2 ] ; real_T P_237 ; real_T P_238 [ 2 ] ; real_T P_239 ;
real_T P_240 [ 2 ] ; real_T P_241 ; real_T P_242 [ 2 ] ; real_T P_243 ;
real_T P_244 [ 2 ] ; real_T P_245 ; real_T P_246 [ 2 ] ; real_T P_247 ;
real_T P_248 [ 2 ] ; real_T P_249 ; real_T P_250 [ 2 ] ; real_T P_251 ;
real_T P_252 [ 2 ] ; real_T P_253 ; real_T P_254 [ 2 ] ; real_T P_255 ;
real_T P_256 [ 2 ] ; real_T P_257 ; real_T P_258 [ 2 ] ; real_T P_259 ;
real_T P_260 [ 2 ] ; real_T P_261 ; real_T P_262 [ 2 ] ; real_T P_263 ;
real_T P_264 [ 2 ] ; real_T P_265 [ 9 ] ; real_T P_266 [ 2 ] ; real_T P_267 ;
real_T P_268 [ 2 ] ; real_T P_269 [ 9 ] ; real_T P_270 [ 2 ] ; real_T P_271 ;
real_T P_272 [ 2 ] ; real_T P_274 [ 2 ] ; real_T P_275 ; real_T P_276 [ 2 ] ;
real_T P_277 [ 15 ] ; real_T P_278 [ 2 ] ; real_T P_279 [ 3 ] ; real_T P_280
[ 2 ] ; real_T P_281 [ 12 ] ; real_T P_282 [ 2 ] ; real_T P_283 ; real_T
P_284 [ 2 ] ; real_T P_285 [ 12 ] ; real_T P_286 [ 2 ] ; real_T P_287 ;
real_T P_288 [ 2 ] ; real_T P_289 ; real_T P_290 [ 2 ] ; real_T P_291 ;
real_T P_292 [ 2 ] ; real_T P_293 ; real_T P_294 [ 2 ] ; real_T P_295 ;
real_T P_296 [ 2 ] ; real_T P_297 ; real_T P_298 [ 2 ] ; real_T P_299 ;
real_T P_300 [ 2 ] ; real_T P_301 ; real_T P_302 [ 2 ] ; real_T P_303 [ 9 ] ;
real_T P_304 [ 2 ] ; real_T P_305 ; real_T P_306 [ 2 ] ; real_T P_307 ;
real_T P_308 [ 2 ] ; real_T P_309 ; real_T P_310 [ 2 ] ; real_T P_311 ;
real_T P_312 ; real_T P_313 ; real_T P_314 ; real_T P_315 ; real_T P_316 ;
real_T P_317 ; real_T P_318 ; real_T P_319 [ 2 ] ; real_T P_320 ; real_T
P_321 [ 2 ] ; real_T P_322 ; real_T P_323 [ 2 ] ; real_T P_324 ; real_T P_325
[ 2 ] ; real_T P_326 ; real_T P_327 [ 2 ] ; real_T P_328 ; real_T P_329 [ 2 ]
; real_T P_330 ; real_T P_331 [ 2 ] ; real_T P_332 ; real_T P_333 [ 2 ] ;
real_T P_334 ; real_T P_335 [ 2 ] ; real_T P_336 ; real_T P_337 [ 2 ] ;
real_T P_338 ; real_T P_339 [ 2 ] ; real_T P_340 ; real_T P_341 [ 2 ] ;
real_T P_342 ; real_T P_343 [ 2 ] ; real_T P_344 ; real_T P_345 [ 2 ] ;
real_T P_346 ; real_T P_347 [ 2 ] ; real_T P_348 ; real_T P_349 [ 2 ] ;
real_T P_350 [ 9 ] ; real_T P_351 [ 2 ] ; real_T P_352 ; real_T P_353 [ 2 ] ;
real_T P_354 [ 9 ] ; real_T P_355 [ 2 ] ; real_T P_356 ; real_T P_357 [ 2 ] ;
real_T P_359 [ 2 ] ; real_T P_360 ; real_T P_361 [ 2 ] ; real_T P_362 [ 15 ]
; real_T P_363 [ 2 ] ; real_T P_364 [ 3 ] ; real_T P_365 [ 2 ] ; real_T P_367
[ 2 ] ; real_T P_368 ; real_T P_369 [ 2 ] ; real_T P_370 [ 11 ] ; real_T
P_371 [ 2 ] ; real_T P_372 ; real_T P_373 [ 2 ] ; real_T P_374 ; real_T P_375
[ 2 ] ; real_T P_376 ; real_T P_377 [ 2 ] ; real_T P_378 [ 9 ] ; real_T P_379
[ 2 ] ; real_T P_380 ; real_T P_381 [ 2 ] ; real_T P_382 ; real_T P_383 [ 2 ]
; real_T P_384 ; real_T P_385 [ 2 ] ; real_T P_386 ; real_T P_387 ; real_T
P_388 ; real_T P_389 ; real_T P_390 ; real_T P_391 ; real_T P_392 ; real_T
P_393 ; real_T P_394 ; real_T P_395 [ 2 ] ; real_T P_396 ; real_T P_397 [ 2 ]
; real_T P_398 ; real_T P_399 [ 2 ] ; real_T P_400 ; real_T P_401 [ 2 ] ;
real_T P_402 ; real_T P_403 [ 2 ] ; real_T P_404 ; real_T P_405 [ 2 ] ;
real_T P_406 ; real_T P_407 [ 2 ] ; real_T P_408 ; real_T P_409 [ 2 ] ;
real_T P_410 ; real_T P_411 [ 2 ] ; real_T P_412 ; real_T P_413 [ 2 ] ;
real_T P_414 ; real_T P_415 [ 2 ] ; real_T P_416 ; real_T P_417 [ 2 ] ;
real_T P_418 ; real_T P_419 [ 2 ] ; real_T P_420 ; real_T P_421 [ 2 ] ;
real_T P_422 ; real_T P_423 [ 2 ] ; real_T P_424 ; real_T P_425 [ 2 ] ;
real_T P_426 [ 9 ] ; real_T P_427 [ 2 ] ; real_T P_428 ; real_T P_429 [ 2 ] ;
real_T P_430 [ 9 ] ; real_T P_431 [ 2 ] ; real_T P_432 ; real_T P_433 [ 2 ] ;
real_T P_435 [ 2 ] ; real_T P_436 ; real_T P_437 [ 2 ] ; real_T P_438 [ 15 ]
; real_T P_439 [ 2 ] ; real_T P_440 [ 3 ] ; real_T P_441 [ 2 ] ; real_T P_443
[ 2 ] ; real_T P_444 ; real_T P_445 [ 2 ] ; real_T P_446 [ 11 ] ; real_T
P_447 [ 2 ] ; real_T P_448 ; real_T P_449 [ 2 ] ; real_T P_450 ; real_T P_451
[ 2 ] ; real_T P_452 ; real_T P_453 [ 2 ] ; real_T P_454 [ 9 ] ; real_T P_455
[ 2 ] ; real_T P_456 ; real_T P_457 [ 2 ] ; real_T P_458 ; real_T P_459 [ 2 ]
; real_T P_460 ; real_T P_461 [ 2 ] ; real_T P_462 ; real_T P_463 ; real_T
P_464 ; real_T P_465 ; real_T P_466 ; } ; extern a4tp0s2zv5 edn3skvqjt ;
#endif
