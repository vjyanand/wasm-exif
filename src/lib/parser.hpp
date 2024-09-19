#include <iostream>
#include <string>
#include <exiv2/exiv2.hpp>
#include "nlohmann/json.hpp"

class ExifParser {
    std::string checksum;
    std::string fileName;
private:
    static nlohmann::json internal_exif_read(Exiv2::ExifData &exifData);

    static bool internal_exif_delete(Exiv2::ExifData &exifData, const std::string &key);

    static bool internal_exif_update(Exiv2::ExifData &exifData, const std::string &key, const std::string &value);

    static bool internal_exif_add(Exiv2::ExifData &exifData, const std::string &key, const std::string &value, int type);

public:
    explicit ExifParser(const std::string &file, const std::string &cSum);

    std::string exif_read();

    std::string exif_delete(const std::string &key);

    std::string exif_update(const std::string &key, const std::string &value);

    std::string exif_add(const std::string &key, const std::string &value, const int type);

    bool exif_delete_all();

    ~ExifParser() {

    }
};
