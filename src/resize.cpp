#include "./magickwand.h"

class ResizeWorker : public NanAsyncWorker {
  public:
    ResizeWorker(NanCallback *callback, int quality, 
                int width, int height, 
                char* imagePath, char *format) : NanAsyncWorker(callback) ,
   width(width), height(height), quality(quality)
   {
      this->imagePath = imagePath;
      this->format = format;
    }

    ~ResizeWorker() {}

    void Execute() {
      MagickWand *magick_wand = NewMagickWand();
      MagickBooleanType status;
      ExceptionType severity;

      int width, height;
      float aspectRatio;

      status = MagickReadImage(magick_wand,this->imagePath);

      if (status == MagickFalse) {
        SetErrorMessage(MagickGetException(magick_wand,&severity));
        DestroyMagickWand(magick_wand);
        return;
      }

      if (this->height == 0 || this->width == 0) {
        width = MagickGetImageWidth(magick_wand);
        height = MagickGetImageHeight(magick_wand);

        aspectRatio = (width * 1.0)/height;

        if (this->height == 0)
          this->height =  this->width * (1.0/aspectRatio);
        else if (this->width == 0) 
          this->width = this->height * aspectRatio;
      }

      if (this->width && this->height)
        MagickResizeImage(magick_wand,this->width,this->height,LanczosFilter,1.0);

      if (this->format) 
        MagickSetImageFormat(magick_wand,this->format);

      if (this->quality) 
        MagickSetImageCompressionQuality(magick_wand,this->quality);

      this->resizedImage = (char *) MagickGetImageBlob(magick_wand,&this->resizedImageLen);
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
    // input
    int width;
    int height;
    int quality;
    char *imagePath;
    char *format;

    // output
    char *resizedImage;
    size_t resizedImageLen;
};

NAN_METHOD(resizeAsync) {

  NanScope();

  const char *usage = "Too few arguments: Usage: resize(pathtoimgfile,new width, new height,quality,cb)";
  if (args.Length() != 6) {
    return NanThrowError(usage);
  }

  String::Utf8Value name(args[0]);
  String::Utf8Value format(args[4]); 
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();
  int quality = args[3]->Int32Value();
  char *imageFormat = nullptr;

  NanCallback *cb = new NanCallback(args[5].As<Function>());

  if (width < 0 || height < 0) {
    return NanThrowError("Invalid width/height arguments");
  }

  if (quality < 0 || quality > 100) {
    return NanThrowError("Invalid quality parameter");
  }

  if (*format) {
    imageFormat = strdup(*format);
  }

  NanAsyncQueueWorker(new ResizeWorker(cb, quality, width, height, strdup(*name), imageFormat));
  NanReturnUndefined();
}
