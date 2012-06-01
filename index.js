"use strict";

var mgwnd = require('./magickwand');

var magickwand = {
  resize: function(imagefile, options, cb) {
    if (!options.quality)
      options.quality = 0;

    mgwnd.resize(imagefile,options.width,options.height,options.quality,cb);
  }
};

module.exports = magickwand;
