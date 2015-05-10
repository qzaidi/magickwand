"use strict";

var mgwnd = require('./build/Release/magickwand');

var magickwand = {
  resize: function(imagefile, options, cb) {

    if (Object.keys(options).length == 0) {
      return cb(new Error('Invalid width/height/format/quality arguments'));
    }

    [ 'quality', 'width', 'height' ].forEach(function(param) {
      if (!options[param])
        options[param] = 0;
    });

    mgwnd.resize(imagefile,options.width,options.height,options.quality,options.format,cb);
  },

  thumbnail: function(imagefile, options, cb) {
    var args = {};
    if (typeof(cb) != 'function') {
      cb = options;
      options = {};
    }

    [ 'width', 'height', 'quality' ].forEach(function(param) {
      args[param] = options[param] || 0;
    });

    mgwnd.thumbnail(imagefile,args.width,args.height,args.quality,cb);
  }
};

module.exports = magickwand;
