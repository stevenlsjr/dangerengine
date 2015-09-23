#import <ObjFW/ObjFW.h>
#import "../slscontext.h"

@interface SLSContext : OFObject {
@private
  size_t _height;
  size_t _width;
  slsContext *_ctx;
}

@property (readonly) slsContext *ctx;

@property (readwrite) size_t height;
@property (readwrite) size_t width;

- (instancetype)initWithCaption:(OFString *)caption
                          width:(size_t)width
                         height:(size_t)height;

+ (instancetype)contextWithCaption:(OFString *)caption
                             width:(size_t)width
                            height:(size_t)height;

- (void)run;

- (void)setup;
- (void)teardown;

- (void)iterLoop;

- (void)drawDt:(double)dt;
- (void)updateDt:(double)dt;

- (void)resizeWidth:(int)width height:(int)height;

@end
