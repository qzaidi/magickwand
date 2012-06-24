{
  "targets": [
    {
      "target_name": "magickwand",
       "sources": [ "src/magickwand.cpp" ],
       'libraries': [ '<!@(Magick-config --libs)' ],
       "conditions": [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(Magick-config --cflags)'
            ],
          }
        }, {
          'cflags': [
            '<!@(Magick-config --cflags)'
          ]
        }]
      ]
    }
  ]
}
