#include "./magickwand.h"

void InitAll(Handle<Object> exports) {
  exports->Set(NanNew<String>("resize"),
      NanNew<FunctionTemplate>(resizeAsync)->GetFunction());

  exports->Set(NanNew<String>("thumbnail"),
      NanNew<FunctionTemplate>(thumbnailAsync)->GetFunction());
}

NODE_MODULE(magickwand, InitAll)
