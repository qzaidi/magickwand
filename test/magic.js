"use strict";

var fs = require('fs');
var magickwand = require('../magickwand');

magickwand.resize('/tmp/abc.jpg',0,200,function(err,data) {
 if (err) {
   console.log(err);
   return;
 }

 fs.writeFile('/tmp/def.jpg',data,"binary");

}); 
