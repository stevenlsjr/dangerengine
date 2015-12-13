//
//  SLSAppState.h
//  dangerengine
//
//  Created by Steven on 11/28/15.
//
//

#import <Foundation/Foundation.h>
#import <state/slsAppState.h>

@interface SLSAppState : NSObject

@property(readonly, nonnull) slsAppState *state;

- (instancetype)initWithCState:(slsAppState *)state;
+ (instancetype)stateWithCState:(slsAppState *)state;

@end
