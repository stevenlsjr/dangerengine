
#import <dangerengine.h>

int main(int argc, char **argv) {
  //
  SLSContext *ctx =
      [SLSContext contextWithCaption:@"Objc-demo" width:640 height:640];
  [ctx run];
  return 0;
}