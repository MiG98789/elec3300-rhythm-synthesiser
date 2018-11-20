# Rhythm Synthesiser

An ELEC3300 project.

## Indentation and spacing

- Indent 2 spaces
- End file with newline

## Comments
```c
// Use single line comments even if
// the comment spans multiple lines

/* Don't use block style comments */
```

Use 80 backslashes above and below to create a title comment:

```c
////////////////////////////////////////////////////////////////////////////////
// Title comment example
////////////////////////////////////////////////////////////////////////////////
```

## Naming

### Integral types
Use integer typedefs instead of primitive types:
```c
// Include stdint header if needed
#include <stdint.h>

int8_t x; // not char x
int16_t x; // not short x
int32_t x; // not int x

uint8_t x4; // not unsigned char x
uint16_t x5; // not unsigned short x
uint32_t x6; // not unsigned int x
```

### Static functions and variables
Use lower camel case:
```c
static int32_t staticVariableExample = 0;

void staticFunctionExample(void) {}
```

### Exported functions and variables
Use uppercase with underscores:
```c
extern int32_t EXPORTED_VARIABLE_EXAMPLE;

void EXPORTED_FUNCTION_EXAMPLE(void) {}
```

### Include guards
Use uppercase with underscores, with double underscore prefix:
```c
#ifndef __INCLUDE_GUARD_EXAMPLE_H
#define __INCLUDE_GUARD_EXAMPLE_H
```

## Source file ordering

1. Include project dependencies
2. Include library dependencies
3. Define static variables
4. Define static functions
5. Define exported variables
6. Define exported functions

```c
#include "example1.h"
#include "example2.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

////////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////////

static int32_t staticVariableExample = 0;

void staticFunctionExample(void) {}

////////////////////////////////////////////////////////////////////////////////
// Exported
////////////////////////////////////////////////////////////////////////////////

int32_t EXPORTED_VARIABLE_EXAMPLE = staticVariableExample;

void EXPORTED_FUNCTION_EXAMPLE(void) {}
```
