
#import <Foundation/Foundation.h>
#import <SDL2/SDL_system.h>
#import "../slscontext.h"

#import "SLSAppState.h"

@interface SLSContext : NSObject {
@private
  size_t _height;
  size_t _width;
  slsContext _ctx;
}

@property(readonly) slsContext *ctx;
@property(readonly) SDL_Window *window;
@property(readonly) SDL_GLContext *glContext;
@property(readonly) SLSAppState *appState;

@property(readwrite) size_t height;
@property(readwrite) size_t width;

- (instancetype)initWithCaption:(NSString *)caption
                          width:(size_t)width
                         height:(size_t)height;

+ (instancetype)contextWithCaption:(NSString *)caption
                             width:(size_t)width
                            height:(size_t)height;

- (void)run;

- (void)setup;
- (void)teardown;

- (void)drawDt:(double)dt;
- (void)updateDt:(double)dt;

- (void)resizeWidth:(int)width height:(int)height;

@end
