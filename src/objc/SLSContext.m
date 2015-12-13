#import "SLSContext.h"
#include <GLFW/glfw3.h>

/*-----------------------------*
 * context callbacks
 *-----------------------------*/

@implementation SLSContext {
}

/*-----------------------------*
 * initializer/destructor implementations
 *-----------------------------*/

- (instancetype)init {
  return [self initWithCaption:@"window" width:640 height:480];
}

- (instancetype)initWithCaption:(NSString *)caption
                          width:(size_t)width
                         height:(size_t)height {
  self = [super init];
  if (!self) {
    return nil;
  }

  // initialize context
  if (!sls_context_class()->init(&_ctx, caption.UTF8String, width, height)) {
    [NSException raise:NSMallocException
                format:@"slsContext _ctx failed to initialize"];
  }
  _ctx.data = (__bridge void *)self;

  _appState = [SLSAppState stateWithCState:_ctx.state];

  _ctx.state = _appState.state;

  _height = height;
  _width = width;

  return self;
}

+ (instancetype)contextWithCaption:(NSString *)caption
                             width:(size_t)width
                            height:(size_t)height {
  return [[self alloc] initWithCaption:caption width:width height:height];
}

- (void)dealloc {
  if (_appState && _appState.state == _ctx.state) {
    _ctx.state = NULL; // state is managed by objc
  } else {
    NSAssert(NO, @"appstate must use objc bindings");
  }

  if (_ctx.dtor) {
    _ctx.dtor(&_ctx);
  }
}

#pragma mark "property implementation"

- (slsContext *)ctx {
  if (_ctx.data != (__bridge void *)self) {
    _ctx.data = (__bridge void *)self;
  }

  return &_ctx;
}

- (SDL_GLContext *)glContext {
  return &(_ctx.gl_context);
}

/*-----------------------------*
 * property implementations
 *-----------------------------*/
#pragma mark "property implementations"

- (size_t)height {
  return _height;
}

- (void)setHeight:(size_t)height {
  _height = height;
  SDL_SetWindowSize(_ctx.window, _width, _height);
}

- (size_t)width {
  return _width;
}

- (void)setWidth:(size_t)width {
  _width = width;
  SDL_SetWindowSize(_ctx.window, _width, _height);
}

/*-----------------------------*
 * method implementations
 *-----------------------------*/
#pragma mark method implementations

- (void)run {
  self.ctx->run(self.ctx);
}

- (void)setup {
  self.ctx->setup(self.ctx);
}

- (void)teardown {
  self.ctx->teardown(self.ctx);
}

- (void)drawDt:(double)dt {
  self.ctx->display(self.ctx, dt);
}
- (void)updateDt:(double)dt {
  self.ctx->update(self.ctx, dt);
}

- (void)resizeWidth:(int)width height:(int)height {
  self.ctx->resize(self.ctx, width, height);
}

@end
