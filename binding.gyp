{
  "targets": [
    {
      "target_name": "magickwand",
       "sources": [ "src/magickwand.cpp" ],
       "conditions": [
        ['OS=="mac" or OS=="linux"' , {
          "cflags": [ '<!@(pkg-config --cflags ImageMagick)' ],
          "libraries" : [ '<!@(pkg-config --libs-only-l ImageMagick)' ],
          "ldflags": [ '<!@(pkg-config --libs-only-L --libs-only-other ImageMagick)' ],
        }]
       ]
    }
  ],
}
