/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 3/28/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSEVENT_H
#define DANGERENGINE_SLSEVENT_H

#include <slsmacros.h>
#include <SDL2/SDL.h>

SLS_BEGIN_CDECLS

typedef struct slsContext slsContext;

typedef enum slsEventType {
  SLS_EVT_DISPATCH,
  SLS_EVT_TIMED_DISPATCH,
  SLS_EVT_LAST,
  SLS_INVALID_EVT_ID
} slsEventType;


typedef struct slsEvent slsEvent;

/**
 * @brief dispatch event callback
 * @returns If the user returns a non-null
 * value and the event is a repeated dispatch,
 * user_data is replaced by the return value
 */
typedef void *(*slsDispatchFn)(slsEvent *event, void *user_data);


typedef struct slsDispatchEvent {
  slsDispatchFn fn;
  void *data;
} slsDispatchEvent;

typedef struct slsTimedDispatchEvent {
  slsDispatchFn fn;
  void *data;

  /// number of ticks before function is first called
  clock_t ticks_to_fire;
  bool first_dispatch_fired;

  clock_t time;
} slsTimedDispatchEvent;

/**
 * @brief. An event struct subtyping SDL_Event.
 */
struct slsEvent {
  SDL_Event super;
  /**
   * @brief designates whether the event MUST be
   * handled on
   */
  bool on_gl_thread;
  union {
    slsDispatchEvent dispatch;
    struct {
      slsDispatchFn fn;
    } timed_dispatch;
  };
};


bool sls_init_eventsystem(void);

uint32_t sls_eventtype_to_eventid(slsEventType);

int64_t sls_first_eventid();

/**
 * @brief gives the registered sdl2 event id
 * for a given slsEvent
 */
uint32_t sls_eventtype_to_eventid(slsEventType);

/**
 * @brief gives corresponding event type from a registered sdl2 event
 * id.
 * @returns If the event does not correspond to an actual
 * event type, it returs SLS_INVALID_EVT_ID
 */
slsEventType sls_eventid_to_eventtype(uint32_t id);

/**
 * @brief Pushes a generic event to SDL's event queue
 */
void sls_push_dispatch(slsContext *ctx,
                       slsDispatchFn fn,
                       void *data);

void sls_push_timed_dispatch(slsContext *ctx,
                             slsDispatchFn fn,
                             void *data);



SLS_END_CDECLS


#endif //DANGERENGINE_SLSEVENT_H
