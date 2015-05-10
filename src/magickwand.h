#ifndef _MAGICKWAND_H
#define _MAGICKWAND_H 

#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include <node.h>
#include <nan.h>
#include <node_buffer.h>
#include <stdlib.h>
#include <string.h>
#include <wand/MagickWand.h>

using v8::FunctionTemplate;
using v8::Local;
using v8::Null;
using v8::Value;
using v8::Handle;
using v8::Object;
using v8::Integer;
using v8::String;
using v8::Function;

NAN_METHOD(resizeAsync);
NAN_METHOD(thumbnailAsync);

#endif /* _MAGICKWAND_H */
