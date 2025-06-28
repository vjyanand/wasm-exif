#rm -rf build_wasm

emcmake cmake -G Ninja . -B build_wasm -DEXIV2_ENABLE_INIH=OFF -DEXIV2_ENABLE_XMP=OFF -DBROTLI_INCLUDE_DIR=/Users/iavian/Downloads/brotli/c/include \
                                       -DBROTLICOMMON_LIBRARY=/Users/iavian/Downloads/brotli/build_wasm/libbrotlicommon.a \
                                       -DBROTLIDEC_LIBRARY=/Users/iavian/Downloads/brotli/build_wasm/libbrotlidec.a \
                                       -DIS_WASM:BOOL=ON -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_CXX_FLAGS="-std=c++14" -DCMAKE_EXE_LINKER_FLAGS="-g0 -static -sERROR_ON_UNDEFINED_SYMBOLS=1 -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1 -s ENVIRONMENT=web"

cd build_wasm || exit

cmake --build .

cd ..

cp build_wasm/exif-wasm.js /Users/iavian/Downloads/www-exif/public/assets/.

cp build_wasm/exif-wasm.wasm /Users/iavian/Downloads/www-exif/public/assets/.

cp build_wasm/exif-wasm.js /Users/iavian/Downloads/www-exif/src/.

cp build_wasm/exif-wasm.wasm /Users/iavian/Downloads/www-exif/src/.

#python3 -m http.server -d ./www