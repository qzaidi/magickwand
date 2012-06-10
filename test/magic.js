"use strict";

var fs = require('fs');
var magickwand = require('../index');

magickwand.resize('/tmp/abc.jpg', { width: 0, height: 50, quality:50, format:'png' }, function(err,data) {
 if (err) {
   console.log(err);
   return;
 }
 fs.writeFile('/tmp/def.png',data,"binary");
}); 
