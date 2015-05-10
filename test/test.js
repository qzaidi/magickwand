"use strict";

var magickwand = require('../index');
var assert = require('assert');
var Magic = require('mmmagic').Magic;

var imagePath = __dirname + '/abc.jpg';

describe("Basic", function() {
  describe("Non existent image", function() {
    it('resize should generate an error', function(done) {
      magickwand.resize('./nonexistend.jpg',  { width: 128, quality:80 }, function(err,data) {
        assert(err != undefined);
        assert(data == undefined);
        done();
      });
    });
    it('thumbnail should generate an error', function(done) {
      magickwand.thumbnail('./nonexistent.jpg',  { width: 128, quality:80 }, function(err,data) {
        assert(err != undefined);
        assert(err.message.indexOf('No such') > -1);
        assert(data == undefined);
        done();
      });
    });
  });

  describe("invalid width/height", function() {
    it('resize(-10,0) should generate error', function(done) {
      try {
        magickwand.resize(imagePath,{ width: -10, height: 0 }, function(err,data) {
          assert(false);
        });
      } catch(e) {
        assert(e != undefined);
        done();
      }
    });
  });

  describe("valid resize", function() {
    it("resize should produce valid image", function(done) {
      magickwand.resize(imagePath,{ width: 50, height: 50 }, function(err,data) {
        assert(err == undefined);
        assert(data.length > 0);
        var magic = new Magic();
        magic.detect(data, function(err,res) {
          assert(err == undefined);
          assert(res.indexOf('50x50') > -1);
          assert(res.indexOf('JPEG') > -1);
          done();
        });
      });
    });
    it("thumbnail should produce valid image", function(done) {
      magickwand.thumbnail(imagePath,{ width: 50, height: 50 }, function(err,data) {
        assert(err == undefined);
        assert(data.length > 0);
        var magic = new Magic();
        magic.detect(data, function(err,res) {
          assert(err == undefined);
          assert(res.indexOf('50x50') > -1);
          assert(res.indexOf('JPEG') > -1);
          done();
        });
      });
    });

    it("resize should maintain aspect ratio", function(done) {
      magickwand.resize(imagePath, { width: 0, height: 50 }, function(err,data,info) {
        assert(err == undefined);
        assert(data.length > 0);
        var magic = new Magic();
        magic.detect(data, function(err,res) {
          assert(err == undefined);
          assert(res.indexOf('62x50') > -1);
          assert(res.indexOf('JPEG') > -1);
          assert(info != undefined && info.width == 62)
          done();
        });
      });
    });

    it("resize should covert formats", function(done) {
      magickwand.resize(imagePath, { format: "png", width: 0, height: 50 }, function(err,data,info) {
        assert(err == undefined);
        assert(data.length > 0);
        var magic = new Magic();
        magic.detect(data, function(err,res) {
          assert(err == undefined);
          assert(res.indexOf('62 x 50') > -1);
          assert(res.indexOf('PNG') > -1);
          assert(info != undefined && info.width == 62)
          done();
        });
      });
    });

  });
});
