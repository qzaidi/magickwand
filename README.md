magickwand
==========

magickwand is the native port of imagemagick/convert.

Most other ports of imagemagick invoke the convert binary, instead of making direct API calls. While this works, API calls 
would be faster than invoking convert, and that is the motivation for this module.
