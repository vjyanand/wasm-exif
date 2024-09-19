#rm -rf build_wasm

emcmake cmake -G Ninja . -B build_wasm -DEXIV2_ENABLE_INIH=OFF -DEXIV2_ENABLE_XMP=OFF -DBROTLI_INCLUDE_DIR=/Users/iavian/Downloads/brotli/wasm/include \
                                       -DBROTLICOMMON_LIBRARY=/Users/iavian/Downloads/brotli/wasm/lib/libbrotlicommon.a \
                                       -DBROTLIDEC_LIBRARY=/Users/iavian/Downloads/brotli/wasm/lib/libbrotlidec.a \
                                       -DIS_WASM:BOOL=ON -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_CXX_FLAGS="-std=c++14" -DCMAKE_EXE_LINKER_FLAGS="-g2 -static -sERROR_ON_UNDEFINED_SYMBOLS=1 -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1 -s ENVIRONMENT=web -s"

cd build_wasm || exit

cmake --build .

cd ..

cp build_wasm/exif-wasm.js /Users/iavian/Downloads/www-exif-wasm-new/public/assets/.

cp build_wasm/exif-wasm.wasm /Users/iavian/Downloads/www-exif-wasm-new/public/assets/.

cp build_wasm/exif-wasm.js /Users/iavian/Downloads/www-exif-wasm-new/src/.

cp build_wasm/exif-wasm.wasm /Users/iavian/Downloads/www-exif-wasm-new/src/.

python3 -m http.server -d ./www