"use strict";

var mgwnd = require('./magickwand');

var magickwand = {
  resize: function(imagefile, options, cb) {
    // replace undefined with 0
    [ 'quality', 'width', 'height' ].forEach(function(param) {
      if (!options[param])
        options[param] = 0;
    });

    mgwnd.resize(imagefile,options.width,options.height,options.quality,options.format,cb);
  }
};

module.exports = magickwand;
