rm -rf build

cmake -G Ninja -DIS_WASM=OFF . -B build

cmake -DIS_WASM:BOOL=OFF . -B build

cd build

cmake --build .

cd ..
