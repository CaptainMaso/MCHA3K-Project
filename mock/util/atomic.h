#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdint.h>
#include "avr/io.h"
#include "avr/interrupt.h"

static __inline__ uint8_t __iSeiRetVal(void)
{
    sei();
    return 1;
}

static __inline__ uint8_t __iCliRetVal(void)
{
    cli();
    return 1;
}

static __inline__ void __iSeiParam(const uint8_t *__s)
{
    sei();
    (void)__s;
}

static __inline__ void __iCliParam(const uint8_t *__s)
{
    cli();
    (void)__s;
}

static __inline__ void __iRestore(const uint8_t *__s)
{
    SREG = *__s;
}

#define ATOMIC_BLOCK(type)      for ( type, __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0, (void)sreg_save )
#define ATOMIC_RESTORESTATE     uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = SREG
#define ATOMIC_FORCEON          uint8_t sreg_save __attribute__((__cleanup__(__iSeiParam))) = 0    

#define NONATOMIC_BLOCK(type)   for ( type, __ToDo = __iSeiRetVal(); __ToDo ; __ToDo = 0, (void)sreg_save )
#define NONATOMIC_RESTORESTATE  uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = SREG
#define NONATOMIC_FORCEOFF      uint8_t sreg_save __attribute__((__cleanup__(__iCliParam))) = 0


#endif