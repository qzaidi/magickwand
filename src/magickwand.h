#ifndef _MAGICKWAND_H
#define _MAGICKWAND_H 

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <string.h>
#include <wand/MagickWand.h>

using namespace node;
using namespace v8;

extern "C" void init(Handle<Object>);

Handle<Value> resizeAsync (const Arguments& args);
Handle<Value> thumbnailAsync (const Arguments& args);

#endif /* _MAGICKWAND_H */
