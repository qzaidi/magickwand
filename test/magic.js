"use strict";

var fs = require('fs');
var magickwand = require('../index');

magickwand.resize('./abc.jpg',  { width: 128, quality:80 }, function(err,data,info) {
 if (err) {
   console.log(err);
 } else {
  console.log("Blob returned from resize, length is " + data.length);
  console.log(info);
  fs.writeFile('/tmp/def.jpg',data,"binary");
 }
}); 

magickwand.thumbnail('./abc.jpg', { width: 126 }, function(err,data,info) {
 if (err) {
   console.log(err);
 } else {
  console.log("Blob returned from thumbnail, length is " + data.length);
  console.log(info);
  fs.writeFile('/tmp/ghi.jpg',data,"binary");
 }
});
