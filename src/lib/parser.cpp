#include "parser.hpp"
#include <nlohmann/json.hpp>

ExifParser::ExifParser(const std::string &file, const std::string &cSum) {
    fileName = file;
    checksum = cSum;
}

bool ExifParser::exif_delete_all() const {
    Exiv2::XmpParser::initialize();
    ::atexit(Exiv2::XmpParser::terminate);
    auto image = Exiv2::ImageFactory::open(fileName);
    image->clearExifData();
    image->clearComment();

    Exiv2::ExifThumb exifThumb(image->exifData());
    std::string thumbExt = exifThumb.extension();
    if (!thumbExt.empty()) {
        exifThumb.erase();
    }
    image->writeMetadata();
    puts("Delete All called");
    return true;
}

std::string ExifParser::exif_delete(const std::string &key) const {
    nlohmann::json final_result = nlohmann::json::object();
    final_result.emplace("success", false);
    final_result.emplace("key", key);
    Exiv2::XmpParser::initialize();
    ::atexit(Exiv2::XmpParser::terminate);
    auto image = Exiv2::ImageFactory::open(fileName);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        final_result.emplace("message", "No exif data found");
        return final_result.dump();
    }
    if (internal_exif_delete(exifData, key)) {
        image->writeMetadata();
        final_result.erase("success");
        final_result.emplace("success", true);
        return final_result.dump();
    }
    final_result.emplace("message", "Delete failed");
    return final_result.dump();
}

bool ExifParser::internal_exif_delete(Exiv2::ExifData &exifData, const std::string &key) {
    try {
        Exiv2::ExifKey ek(key);
        auto pos = exifData.findKey(ek);
        if (pos == exifData.end()) {
            return false;
        }
        auto result = exifData.erase(pos);
        return true;
    } catch (...) {
        return false;
    }
}

std::string ExifParser::exif_update(const std::string &key, const std::string &value) const {
    nlohmann::json final_result = nlohmann::json::object();
    final_result.emplace("success", false);
    final_result.emplace("key", key);
    Exiv2::XmpParser::initialize();
    ::atexit(Exiv2::XmpParser::terminate);
    auto image = Exiv2::ImageFactory::open(fileName);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        final_result.emplace("message", "No exif data found");
        return final_result.dump();
    }
    if (internal_exif_update(exifData, key, value)) {
        image->writeMetadata();
        final_result.erase("success");
        final_result.emplace("success", true);
        final_result.emplace("value", value);
        return final_result.dump();
    }
    final_result.emplace("message", "Update failed");
    return final_result.dump();
}

bool ExifParser::internal_exif_update(Exiv2::ExifData &exifData, const std::string &key, const std::string &value) {
    try {
        Exiv2::ExifKey ek(key);
        auto pos = exifData.findKey(ek);
        if (pos == exifData.end()) {
            return false;
        }
        auto result = pos->setValue(value);
        if (result == 0) {
            return true;
        }
        return false;
    } catch (...) {
        return false;
    }
}

std::string ExifParser::exif_add(const std::string &key, const std::string &value, const int type) const {
    nlohmann::json final_result = nlohmann::json::object();
    final_result.emplace("success", false);
    final_result.emplace("key", key);
    Exiv2::XmpParser::initialize();
    ::atexit(Exiv2::XmpParser::terminate);
    auto image = Exiv2::ImageFactory::open(fileName);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        final_result.emplace("message", "No exif data found");
        return final_result.dump();
    }
    if (internal_exif_add(exifData, key, value, type)) {
        image->writeMetadata();
        final_result.erase("success");
        final_result.emplace("success", true);
        final_result.emplace("value", value);
        return final_result.dump();
    }
    final_result.emplace("message", "Add failed");
    return final_result.dump();
}

bool ExifParser::internal_exif_add(Exiv2::ExifData &exifData, const std::string &key, const std::string &value,
                                   const int type) {
    try {
        Exiv2::ExifKey ek(key);
        auto pos = exifData.findKey(ek);
        if (pos != exifData.end()) {
            return false;
        }
        auto exif_type = (type == 0) ? Exiv2::comment : static_cast<Exiv2::TypeId>(type);
        auto v4 = Exiv2::Value::create(exif_type);
        v4->read(value);
        exifData.add(Exiv2::ExifKey(key), v4.get());
        return true;
    } catch (const std::exception &exc) {
        return false;
    }
}

std::string ExifParser::exif_read() const {
    Exiv2::XmpParser::initialize();
    ::atexit(Exiv2::XmpParser::terminate);
    auto image = Exiv2::ImageFactory::open(fileName);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        puts("failed");
    }

    auto result = ExifParser::internal_exif_read(exifData);
    nlohmann::json final_result = nlohmann::json::object();
    final_result.emplace("byte_order", image->byteOrder());
    final_result.emplace("comment", image->comment());
    final_result.emplace("type", image->imageType());
    final_result.emplace("mime", image->mimeType());
    final_result.emplace("height", image->pixelHeight());
    final_result.emplace("width", image->pixelWidth());
    final_result.emplace("exif", result);
    return final_result.dump(1);
}

nlohmann::json ExifParser::internal_exif_read(Exiv2::ExifData &exifData) {
    nlohmann::json exif_list = nlohmann::json::array();
    auto end = exifData.end();
    for (auto i = exifData.begin(); i != end; ++i) {
        nlohmann::json exif_object = nlohmann::json::object();
        exif_object.emplace("familyName", i->familyName());
        exif_object.emplace("groupName", i->groupName());
        exif_object.emplace("typeName", i->typeName());
        exif_object.emplace("ifdName", i->ifdName());
        exif_object.emplace("key", i->key());
        exif_object.emplace("tag", i->tag());
        exif_object.emplace("desc", i->tagDesc());
        exif_object.emplace("label", i->tagLabel());
        exif_object.emplace("name", i->tagName());
        exif_object.emplace("value", i->toString());
        exif_list.push_back(exif_object);
    }
    return exif_list;
}