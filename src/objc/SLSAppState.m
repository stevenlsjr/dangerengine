//
//  SLSAppState.m
//  dangerengine
//
//  Created by Steven on 11/28/15.
//
//

#import "SLSAppState.h"

@implementation SLSAppState {
@private
  slsAppState _state;
}

- (slsAppState *)state {

  return &_state;
}

- (instancetype)init {
  return [self initWithCState:NULL];
}

- (instancetype)initWithCState:(nonnull slsAppState *)state {
  self = [super init];
  if (!self) {
    return nil;
  }

  _state = *state;

  return self;
}

+ (instancetype)stateWithCState:(nonnull slsAppState *)state {

  return [[self alloc] initWithCState:state];
}

- (void)dealloc {
  sls_appstate_dtor(&_state);
}

@end
