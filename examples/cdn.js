"use strict";

var parse = require('url').parse;
var magickwand = require('magickwand');
var mime = require('mime');

function cdnCache(options) {
  var config = {};

  config.path = options.path || '^/cache';
  config.mime = options.mime || '^image/';
  config.fileTypes = options.fileTypes || [ '.jpg', '.gif', '.png' ];
  config.getParams = options.getParams || function(path) {
    var params = path.split('/',3);
    var result;
    var dims;
    if (    (params.length == 3)
         && (params[1].indexOf('x'))
         && (config.fileTypes.some(function(elem) { return params[2].match(elem); })) 
       ) {
      dims = params[1].split('x');    
      result = {
        width: dims[0],
        height: dims[1],
        path: 'public/' + params[2]
      };
    }

    return result;
  };
  
  return function(req,res,next) {
    var pathname = parse(req.url).pathname;
    var resizeOpts;

    if (pathname.match(config.path) == false) {
      return next();
    }

    resizeOpts = config.getParams(req.url);
    magickwand.resize(resizeOpts.path, resizeOpts.width, resizeOpts.height, function(err,data) {
      if (!err && data) {
        res.setHeader('Content-Type',mime.lookup(resizeOpts.path));
        res.setHeader('Content-Length',data.length);
        res.end(data,"binary");
      } else {
        res.statusCode = 404;
        res.setHeader('Content-Length',0);
        res.end('<h1>404 Not Found</h1>');
      }
    });
  };
}
module.export = cdnCache;
