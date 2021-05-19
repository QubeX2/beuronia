/**
 * Copyright (C) 2021 Mikael Andersson
 * qubex2@gmail.com
 */
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>
#include "system.h"

#define ERROR_ALLOC_MESSAGE "Insufficient memory"

#define MALLOC(p, n) \
do \
{ \
  if ( !( (p) = malloc(sizeof(*(p)) * (n)) ) ) \
  { \
    system_die(ERROR_ALLOC_MESSAGE); \
  } \
} \
while(0)

#define CALLOC(p, n) \
do \
{ \
  if ( !( (p) = calloc((n), sizeof(*(p))) ) ) \
  { \
    system_die(ERROR_ALLOC_MESSAGE); \
  } \
} \
while(0)

#define REALLOC(p, n) \
do \
{ \
  if ( !( (p) = realloc((p), sizeof(*(p)) * (n)) ) ) \
  { \
    system_die(ERROR_ALLOC_MESSAGE); \
  } \
} \
while(0)

#define FREE(p) \
do \
{ \
  free(p); \
  p = NULL; \
} \
while(0)

#endif /* __MEMORY_H__ */