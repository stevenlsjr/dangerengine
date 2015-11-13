/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 11/11/15, Steven
 * 
 **/
#ifndef DANGERENGINE_EARTH_H
#define DANGERENGINE_EARTH_H


#include <dangerengine.h>

typedef struct EarthModel EarthModel;

struct EarthModel {
  double aphelion;
  double perihelion;
  double semi_major_axis;
  double eccentricity;

  /**
   * @brief inclination in radians
   **/
  double inclination;
  /**
   * @orbital period, in days
   */
  double period;

};

slsContext * earth_shared_ctx();


void earth_del_ctx(slsContext *ctx) SLS_NONNULL(1);

void earth_ctx_update(slsContext *self, double dt) SLS_NONNULL(1);

void earth_ctx_display(slsContext *self, double dt SLS_NONNULL(1));

void earth_ctx_setup(slsContext *self) SLS_NONNULL(1);

void earth_ctx_teardown(slsContext *self ) SLS_NONNULL(1);

kmVec3 get_earth_position(double date, EarthModel *model)SLS_NONNULL(2);


void earth_ctx_resize(slsContext *self, int x, int y);

int earth_main(int *argcr, char **argv);


#endif //DANGERENGINE_EARTH_H
