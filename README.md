# Building for web

Make sure emsdk is installed and execute the following cmake commands:
```
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=~/src/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release ..
make
```

# Building for desktop

```
mkdir build && cd build
cmake ..
make
```

# Running the web build

After building the executable
