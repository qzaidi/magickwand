{
  "targets": [
    {
      "target_name": "magickwand",
       "sources": [ "src/module.cpp", "src/resize.cpp", "src/thumbnail.cpp" ],
       'libraries': [ '<!@(Wand-config --libs)' ],
       "conditions": [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(Wand-config --cflags)'
            ],
          },
          "include_dirs" : [
            "/usr/local/Cellar/imagemagick",
            "<!(node -e \"require('nan')\")"
          ]
        }, {
          'cflags': [
            '<!@(Wand-config --cflags)'
          ]
        }]
      ]
    }
  ]
}
