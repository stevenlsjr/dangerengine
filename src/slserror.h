/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/7/17, Steven
 * 
 **/

#ifndef DANGERENGINE_SLSERROR_H
#define DANGERENGINE_SLSERROR_H

#include "slsmacros.h"


SLS_BEGIN_CDECLS

typedef enum {
  SLS_ERROR_ROOM_GL = 0,
  SLS_ERROR_ROOM_DANGERENGINE,
  SLS_ERROR_ROOM_DANGERTYPES,
  SLS_ERROR_ROOM_LIBC,
  SLS_ERROR_ROOM_LAST
} slsErrorRoom;


typedef struct slsErrorPrivate slsErrorPrivate;
#define SLS_ERROR_PIMPL_MAXSIZE 0x10
typedef struct slsError {
  slsErrorRoom room;
  long code;
  char const *description;
  slsErrorPrivate *pimpl;
  char buffer[SLS_ERROR_PIMPL_MAXSIZE];

} slsError;


slsError *sls_error_init(slsError *self, slsErrorRoom room, long code,
                         char *description_alloced);

slsError *sls_error_init_noerror(slsError *self);

slsError *sls_error_dtor(slsError *self);

SLS_END_CDECLS

#endif //DANGERENGINE_SLSERROR_H
