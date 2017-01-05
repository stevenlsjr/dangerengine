/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 1/4/17, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSINPUTSTATE_H
#define DANGERENGINE_SLSINPUTSTATE_H

#include <math/math-types.h>

/**
 * slsInputState
 * @brief opaque type for tracking platform input
 */
typedef struct slsInputState slsInputState;

slsInputState *sls_inputstate_new();
void sls_inputstate_delete(slsInputState *state) SLS_NONNULL(1);

void sls_inputstate_update(slsInputState *self, double dt) SLS_NONNULL(1);

slsIPoint sls_inputstate_last_mousestate( slsInputState *self, int *mask) SLS_NONNULL(1);

#endif //DANGERENGINE_SLSINPUTSTATE_H
