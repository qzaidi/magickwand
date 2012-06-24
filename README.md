magickwand
==========

Native bindings for imagemagick, that can be used to resize and compress images. This can be used to dynamically resize images in an express/connect based server. See [examples/cdn.js](http://github.com/qzaidi/magickwand/examples/cdn.js) for an example of connect middleware using magickwand.

Most other modules exposing imagemagick invoke the convert utility, instead of making direct API calls. While this works, API calls are much faster than invoking convert, and that is the motivation for this module.

Example
-------

``` js
var magickwand = require('magickwand');
var fs = require('fs');

magickwand.resize('<pathtoimagefile>', { width: 300, height: 200, quality: 80 } ,function(err,data) {
  fs.writeFile('/tmp/def.jpg',data,"binary");
});
```

To maintain aspect ratio while resizing, set one of the width/height parameters to 0.

```
magickwand.resize('<pathtoimagefile>', { width: 100 },function(err,data) {
  fs.writeFile('/tmp/def.jpg',data,"binary");
});
```

To convert to another supported format, use the format argument

```
magickwand.resize('<pathtoimagefile>', { format: 'png', width: 300, height: 250 },function(err,data) {
  fs.writeFile('/tmp/def.jpg',data,"binary");
});
```
See examples/cdn.js on how to use this module as a middleware in connect.

Requirements
------------

magickwand uses the C library by the same name, so libmagickwand-dev should be installed. 

On Ubuntu, for example

``` bash
sudo apt-get install libmagickwand-dev
```

On Mac - you can use homebrew to install imagemagick.

```bash
brew install pkg-config
brew install imagemagick  --disable-openmp
```

On MacOS, I had trouble using the default recipie for imagemagick, as openmp would cause node process to hang.
Make sure you have disabled openmp while building Imagemagick.If you use homebrew, use the --disable-openmp 
option while installing imagemagick.

On SmartOS, install using pkgin

```bash
pkgin ImageMagic
```

Installation
------------


``` bash
$ npm install magickwand
```

Source Install / Manual Compilation
-----------------------------------

``` bash
$ git clone git://github.com/qzaidi/magickwand.git
$ node-gyp configure build
```
