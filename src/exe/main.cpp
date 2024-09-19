#include <iostream>
#include <string>

#include "../lib/parser.hpp"
int main() {
    std::string file = "/Users/iavian/Downloads/a.jpeg";
    auto parser = ExifParser(file, "www");
    auto result = parser.exif_read();
    std::cout << std::setw(4) << result << std::endl;
parser.exif_delete("Exif.Image.Copyright");
    auto update_result = parser.exif_add("Exif.Image.Copyright", "vijay anand");
    std::cout << std::setw(4) << update_result << std::endl;
    //auto delete_result = parser.exif_delete("Exif.Photo.ShutterSpeedValue");
    //std::cout << std::setw(4) << delete_result << std::endl;
    //parser.exif_delete_all();
    return 0;
}
