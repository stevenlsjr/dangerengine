/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 3/28/16, Steven
 * 
 **/

#include "slsEvent.h"

static int64_t first_registered_event = -1;

int64_t sls_first_eventid()
{
  return first_registered_event;
}

slsEventType sls_eventid_to_eventtype(uint32_t id)
{
  assert(first_registered_event > -1);
  sls_check(id >= first_registered_event &&
            id < first_registered_event + SLS_EVT_LAST,
            "invalid event id %ui", id);

  int64_t evt = id - first_registered_event;

  return (slsEventType) evt;
  error:
  return SLS_INVALID_EVT_ID;
}

uint32_t sls_eventtype_to_eventid(slsEventType event)
{
  assert(first_registered_event > -1);
  assert(event < SLS_EVT_LAST);


  return (uint32_t) first_registered_event + event;
}

bool sls_init_eventsystem(void)
{
  char const *err;
  static bool events_registered = false;
  if (!events_registered) {
    int sdlevents = SDL_WasInit(SDL_INIT_EVENTS);
    sls_check(sdlevents & SDL_INIT_EVENTS,
              "SDL is not initialized");

    first_registered_event =
        SDL_RegisterEvents(SLS_EVT_LAST);

    sls_check(first_registered_event,
              "SDL_RegisterEvents(%i) could not allocate\n"
                  "events for library", SLS_EVT_LAST);


  }
  return true;

  error:
  err = SDL_GetError();
  if (err && *err != '\0') {
    sls_log_err("SDL error: %s", SDL_GetError());
  }
  return false;

}

/**
 * @brief Pushes a generic event to SDL's event queue
 */
void sls_push_dispatch(slsContext *ctx,
                       slsDispatchFn fn,
                       void *data)
{

}

void sls_push_timed_dispatch(slsContext *ctx,
                             slsDispatchFn fn,
                             void *data)
{

}

void sls_push_dispatchblock(slsContext *ctx, slsDispatchBlock blk)
{

}

