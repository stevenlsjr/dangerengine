/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 2/20/16, Steven
 * 
 **/
#ifndef DANGERENGINE_CXX_INTEROP_H
#define DANGERENGINE_CXX_INTEROP_H

#include "overloads.h"


#ifdef __cplusplus
#include <iostream>

static inline
std::ostream &operator<<(std::ostream &out, slsIPoint const &p)
{
  return out << "slsIPoint{" << p.x << ", " << p.y << "}";
}

static inline
std::ostream &operator<<(std::ostream &out, kmMat4 const &m)
{
  constexpr auto dim = 4;

  for (auto j = 0; j< dim; ++j){
    float const *ptr = m.mat + j * dim;
    out << "| " <<
    ptr[0] << ' ' <<
    ptr[1] << ' ' <<
    ptr[2] << ' ' <<
    ptr[3] << ' ' << "|\n";
  }

  return out;
}

static inline
std::ostream &operator<<(std::ostream &out, kmMat3 const &m)
{

  constexpr auto dim = 3;

  for (auto j = 0; j< dim; ++j){
    float const *ptr = m.mat + j * dim;
    out << "| " <<
    ptr[0] << ' ' <<
    ptr[1] << ' ' <<
    ptr[2] << ' ' << "|\n";
  }

  return out;
}

#endif// __cplusplus

#endif //DANGERENGINE_CXX_INTEROP_H
