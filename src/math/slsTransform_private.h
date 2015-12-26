/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 12/19/15, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSTRANSFORM_PRIVATE_H
#define DANGERENGINE_SLSTRANSFORM_PRIVATE_H

#include <kazmath/kazmath.h>

struct slsTransform_p {

  kmMat4 __modelview;
  kmMat4 __inverse_modelview;
  kmMat4 __normalview;
};


#endif //DANGERENGINE_SLSTRANSFORM_PRIVATE_H
