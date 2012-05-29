"use strict";

var parse = require('url').parse;
var mime = require('mime');
var magickwand = require('magickwand');

function cdnCache(options) {
  return function(req,res,next)
    var pathname = parse(req.url).pathname;
    var components = pathname.split('/',3);
    var mimeType = mime.lookup(components[2]);

    var dims;
    if (    (pathname.match(options.path) == false)
         || (components[1].match('.jpg') == false) ) {
      return next();
    }

    dims = components.split('x');

    magickwand.resize('public' + components[2], dims[0], dims[1], function(err,data) {
      if (!err && data) {
        res.setHeader('Content-Type',mimeType));
        res.setHeader('Content-Length',data.length);
        res.end(data,"binary");
      } else {
        res.statusCode = 404;
        res.setHeader('Content-Length',0);
        res.end('<h1>404 Not Found</h1>);
      }
    });
  };

}
