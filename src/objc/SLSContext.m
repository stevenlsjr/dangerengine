#import "SLSContext.h"
#include <GLFW/glfw3.h>


/*-----------------------------*
 * context callbacks
 *-----------------------------*/


@

implementation SLSContext
{
}

@
synthesize ctx = _ctx;

/*-----------------------------*
 * initializer/destructor implementations
 *-----------------------------*/

- (instancetype)
initWithCaption: (OFString * )
caption
    width:
(size_t)
width
    height:
(size_t)height {
self =
[
super init
];
if (!self) {
return nil;
}


_ctx = sls_context_new(caption.UTF8String, width, height);


if (!_ctx) {
@throw [
OFInitializationFailedException exceptionWithClass:
[
self class
]];
}


_height = height;
_width = width;

return
self;
}

+ (instancetype)
contextWithCaption: (OFString * )
caption
    width:
(size_t)
width
    height:
(size_t)height {
return [[
self alloc
]
initWithCaption: caption
width: width
height: height
];
}

- (void)dealloc {
if (_ctx) {
sls_msg(_ctx, dtor);
}
}

/*-----------------------------*
 * property implementations
 *-----------------------------*/

- (size_t)height {
return
_height;
}

- (void)setHeight: (size_t)height {
  _height = height;
  glfwSetWindowSize(_ctx->window, (int) _height, (int) _width);

}

- (size_t)width {
return
_width;
}

- (void)setWidth: (size_t)width {
  _width = width;
  glfwSetWindowSize(_ctx->window, (int) _height, (int) _width);
}

/*-----------------------------*
 * method implementations
 *-----------------------------*/

- (void)run{
}

- (void)setup{
}
- (void)teardown{
}

- (void)iterLoop{
}

- (void)
drawDt: (
double)dt{
}
- (void)
updateDt: (
double)dt{
}

- (void)
resizeWidth: (
int)
width height:
(int)height{

}

@end
