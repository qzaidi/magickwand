{
  "targets": [
    {
      "target_name": "magickwand",
       "sources": [ "src/magickwand.cpp" ],
       'libraries': [ '<!@(MagickWand-config --libs)' ],
       "conditions": [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(MagickWand-config --cflags)'
            ],
          }
        }, {
          'cflags': [
            '<!@(MagickWand-config --cflags)'
          ]
        }]
      ]
    }
  ]
}
