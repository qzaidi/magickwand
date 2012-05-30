#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <string.h>
#include <wand/MagickWand.h>

using namespace node;
using namespace v8;


extern "C" void init (Handle<Object>);

struct magickReq {
  Persistent<Function> cb;
  unsigned char *resizedImage;
  char *exception;
  size_t resizedImageLen;

  int width;
  int height;
  char imagefilepath[1];
};

/* Resize image here */
static void resize (eio_req *req) {
  struct magickReq *mgr = (struct magickReq *)req->data;
  ExceptionType severity;
  MagickWand *magick_wand = NewMagickWand();
  MagickBooleanType status;
  int width, height;
  float aspectRatio;
 
  status = MagickReadImage(magick_wand,mgr->imagefilepath);

  //MagickGetPage();

  if (status == MagickFalse) {
    mgr->exception = MagickGetException(magick_wand,&severity);
    DestroyMagickWand(magick_wand);
    return;
  }

  width = MagickGetImageWidth(magick_wand);
  height = MagickGetImageHeight(magick_wand);
  
  aspectRatio = width/height;

  if (mgr->height == 0)
    mgr->height = mgr->width / aspectRatio;
  else if (mgr->width == 0) 
    mgr->width = mgr->height * aspectRatio;

  MagickResetIterator(magick_wand);
  while (MagickNextImage(magick_wand) != MagickFalse)
    MagickResizeImage(magick_wand,mgr->width,mgr->height,LanczosFilter,1.0);
  MagickResetIterator(magick_wand);

  mgr->resizedImage = MagickGetImageBlob(magick_wand,&mgr->resizedImageLen);
  if (!mgr->resizedImage) {
    mgr->exception = MagickGetException(magick_wand,&severity);
  }

  DestroyMagickWand(magick_wand);
}

/* this is for the callback */
static int postResize(eio_req *req) {
  HandleScope scope;
  ev_unref(EV_DEFAULT_UC);
  struct magickReq *mgr = (struct magickReq *)req->data;

  Handle<Value> argv[2];

  if (mgr->exception) {
    argv[0] = Exception::Error(String::New(mgr->exception));
    argv[1] = Undefined();
    MagickRelinquishMemory(mgr->exception);
  } else {
    argv[0] = Undefined();
    Buffer *buf = Buffer::New(mgr->resizedImageLen + 1);
    memcpy(Buffer::Data(buf), mgr->resizedImage, mgr->resizedImageLen);
    argv[1] = buf->handle_;
  }

  TryCatch try_catch;

  mgr->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  mgr->cb.Dispose();
  MagickRelinquishMemory(mgr->resizedImage);
  free(mgr);
  return 0;
}

static Handle<Value> resizeAsync (const Arguments& args) {
  HandleScope scope;
  const char *usage = "Too few arguments: Usage: resize(pathtoimgfile,new width, new height,cb)";
  if (args.Length() != 4) {
    return ThrowException(Exception::Error(String::New(usage)));
  }

  String::Utf8Value name(args[0]);
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();
  Local<Function> cb = Local<Function>::Cast(args[3]);

  if ((width == 0 && height == 0) || width < 0 || height < 0) {
    return ThrowException(Exception::Error(String::New("Invalid width/height arguments")));
  }

  struct magickReq *mgr = (struct magickReq *) calloc(1,sizeof(struct magickReq) + name.length());

  mgr->cb = Persistent<Function>::New(cb);
  mgr->width = width;
  mgr->height = height;
  strncpy(mgr->imagefilepath,*name,name.length() + 1);

  eio_custom(resize, EIO_PRI_DEFAULT, postResize, mgr);
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
}


extern "C" void init (Handle<Object> target) {
  HandleScope scope;
  MagickWandGenesis();
  NODE_SET_METHOD(target, "resize", resizeAsync);
}


