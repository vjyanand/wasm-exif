#include "header.h"

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::class_<ExifParser>("ExifParser")
            .constructor<std::string, std::string>()
            .function("exif_read", &ExifParser::exif_read)
            .function("exif_delete", &ExifParser::exif_delete)
            .function("exif_delete_all", &ExifParser::exif_delete_all)
            .function("exif_update", &ExifParser::exif_update)
            .function("exif_add", &ExifParser::exif_add)
            ;
}