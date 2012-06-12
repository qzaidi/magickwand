"use strict";

var fs = require('fs');
var magickwand = require('../index');

magickwand.resize('/tmp/abc.jpg', { width: 50, height: 0 }, function(err,data) {
 if (err) {
   console.log(err);
 } else {
  console.log("Blob returned, length is " + data.length);
  fs.writeFile('/tmp/def.jpg',data,"binary");
 }
}); 
