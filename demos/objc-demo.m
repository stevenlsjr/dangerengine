#import <ObjFW/ObjFW.h>
#import "../src/objc/DangerEngineFW.h"


int main(int argc, char const *argv[])
{
  @autoreleasepool
  {
    SLSContext * ctx = [SLSContext
    contextWithCaption:@"caption"
    width:
    640
    height:
    640];

    [ctx
    run];
  }
  return 0;
}


