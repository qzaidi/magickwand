magickwand
==========

magickwand is the native port of imagemagick/convert.

Most other ports of imagemagick invoke the convert binary, instead of making direct API calls. While this works, API calls 
would be faster than invoking convert, and that is the motivation for this module.

Example
-------

``` js
var magickwand = require('magickwand');
var fs = require('fs');

magickwand.resize('<pathtoimagefile>',newWidth,newHeight,function(err,data) {
  fs.writeFile('/tmp/def.jpg',data,"binary");
});
```

See examples/cdn.js on how to use this module as a middleware in connect.

Requirements
------------

I have tested this on Linux and MacOS.

On MacOS - make sure you have disabled openmp while building Imagemagick. Otherwise, imagemagick will hang the node process.
If you use homebrew, use the --disable-openmp option while installing imagemagick.

Installation
------------

TODO: Publish this as an npm module

``` bash
$ npm install magickwand
```

Source Install / Manual Compilation
-----------------------------------
TODO
