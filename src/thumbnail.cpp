#include "./magickwand.h"

class ThumbnailWorker: public NanAsyncWorker {
  public:
    ThumbnailWorker(NanCallback *callback, int quality,
        int width, int height, char *imagePath) : NanAsyncWorker(callback),
    width(width), height(height), quality(quality)
  {
    this->imagePath = imagePath;
  }

  ~ThumbnailWorker() {}

  void Execute() {
    ExceptionType severity;
    MagickWand *magick_wand = NewMagickWand();
    MagickBooleanType status;
    int width, height;

    status = MagickReadImage(magick_wand,this->imagePath);

    if (status == MagickFalse) {
      SetErrorMessage(MagickGetException(magick_wand,&severity));
      DestroyMagickWand(magick_wand);
      return;
    }

    if (this->height == 0 || this->width == 0) {
      width = MagickGetImageWidth(magick_wand);
      height = MagickGetImageHeight(magick_wand);

      if (this->width == 0 && this->height == 0) {
        this->width = width;
        this->height = height;
      } else {
        float aspectRatio = (width * 1.0)/height;
        if (this->height == 0)
          this->height =  this->width * (1.0/aspectRatio);
        else if (this->width == 0)
          this->width = this->height * aspectRatio;
      }
    }

    MagickThumbnailImage(magick_wand,this->width,this->height);

    if (this->quality) 
      MagickSetImageCompressionQuality(magick_wand,this->quality);

    this->resizedImage = (char *)MagickGetImageBlob(magick_wand,&this->resizedImageLen);

    if (!this->resizedImage) {
      SetErrorMessage(MagickGetException(magick_wand,&severity));
    }

    DestroyMagickWand(magick_wand);
  }

  void HandleOKCallback() {
    NanScope();

    Local<Object> info = NanNew<Object>(); 
    info->Set(NanNew("width"),NanNew<Integer>(this->width));
    info->Set(NanNew("height"),NanNew<Integer>(this->height));

    Local<Value> argv[] = {
      NanNull(),
      NanBufferUse(this->resizedImage, this->resizedImageLen),
      info
    };

    callback->Call(3, argv);
  }

  private: 
    int width;
    int height;
    int quality;
    char *imagePath;

    // output
    char *resizedImage;
    size_t resizedImageLen;
};

/*
typedef struct thumbnailReq {
  Persistent<Function> cb;
  unsigned char *resizedImage;
  char *exception;
  size_t resizedImageLen;

  unsigned int width;
  unsigned int height;
  unsigned int quality;
  char imagefilepath[1];
} ThumbnailReq;

static void thumbnail(uv_work_t *req) {
  ThumbnailReq *mgr = (ThumbnailReq *)req->data;
  ExceptionType severity;
  MagickWand *magick_wand = NewMagickWand();
  MagickBooleanType status;
  int width, height;
 
  status = MagickReadImage(magick_wand,mgr->imagefilepath);

  if (status == MagickFalse) {
    mgr->exception = MagickGetException(magick_wand,&severity);
    DestroyMagickWand(magick_wand);
    return;
  }

  if (mgr->height == 0 || mgr->width == 0) {
    width = MagickGetImageWidth(magick_wand);
    height = MagickGetImageHeight(magick_wand);

    if (mgr->width == 0 && mgr->height == 0) {
      mgr->width = width;
      mgr->height = height;
    } else {
      float aspectRatio = (width * 1.0)/height;
      if (mgr->height == 0)
        mgr->height =  mgr->width * (1.0/aspectRatio);
      else if (mgr->width == 0)
        mgr->width = mgr->height * aspectRatio;
    }
  }

  MagickThumbnailImage(magick_wand,mgr->width,mgr->height);

  if (mgr->quality) 
    MagickSetImageCompressionQuality(magick_wand,mgr->quality);

  mgr->resizedImage = MagickGetImageBlob(magick_wand,&mgr->resizedImageLen);

  if (!mgr->resizedImage) {
    mgr->exception = MagickGetException(magick_wand,&severity);
  }

  DestroyMagickWand(magick_wand);
}

static void postThumbnail(uv_work_t *req) {
  HandleScope scope;
  ThumbnailReq *mgr = (ThumbnailReq *)req->data;

  Handle<Value> argv[3];
  Local<Object> info = Object::New();

  if (mgr->exception) {
    argv[0] = Exception::Error(String::New(mgr->exception));
    argv[1] = argv[2] = Undefined();
    MagickRelinquishMemory(mgr->exception);
  } else {
    argv[0] = Undefined();
    Buffer *buf = Buffer::New(mgr->resizedImageLen + 1);
    memcpy(Buffer::Data(buf), mgr->resizedImage, mgr->resizedImageLen);
    argv[1] = buf->handle_;
    Local<Integer> width = Integer::New(mgr->width);
    Local<Integer> height = Integer::New(mgr->height);
    Local<Integer> quality = Integer::New(mgr->quality);
    info->Set(String::NewSymbol("width"), width);
    info->Set(String::NewSymbol("height"),height);
    info->Set(String::NewSymbol("quality"),quality);
    argv[2] = info;
  }

  TryCatch try_catch;

  mgr->cb->Call(Context::GetCurrent()->Global(), 3, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  mgr->cb.Dispose();
  MagickRelinquishMemory(mgr->resizedImage);
  free(mgr);

  delete req;
}
*/

NAN_METHOD(thumbnailAsync) { 

  NanScope();

  const char *usage = "Too few arguments: Usage: thumbnail(pathtoimgfile,width, height,quality,cb)";
  if (args.Length() != 5) {
    return NanThrowError(usage);
  }

  String::Utf8Value name(args[0]);
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();
  int quality = args[3]->Int32Value();

  NanCallback *cb = new NanCallback(args[4].As<Function>());

  if (width < 0 || height < 0) {
    return NanThrowError("Invalid width/height arguments");
  }
  
  if (quality < 0 || quality > 100) {
    return NanThrowError("Invalid quality parameter");
  }

  NanAsyncQueueWorker(new ThumbnailWorker(cb, quality, width, height, strdup(*name)));
  NanReturnUndefined();
}
