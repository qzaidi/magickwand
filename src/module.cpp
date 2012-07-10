#include "./magickwand.h"

extern "C" void init (Handle<Object> target) {
  HandleScope scope;
  MagickWandGenesis();
  NODE_SET_METHOD(target, "resize", resizeAsync);
  NODE_SET_METHOD(target, "thumbnail", thumbnailAsync);
}
