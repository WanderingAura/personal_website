Game-like portfolio page for my WIP website.

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

# Running the embedded portfolio site (Go + htmx)

The raylib gallery is embedded inline on the portfolio site's landing page, served by a small Go backend
in `web/` (About/Projects/Contact are also rendered there, with htmx handling client-side navigation).

```
web/scripts/build.sh   # builds the web target above and copies the WASM artifacts into web/static/wasm/
cd web && go run .
```

Then visit `http://localhost:8080`.

# TODO
- I want a room class where I can specify a size and a set of directions using bit flags representing which
directions the doors are in
- I want a light class that is a light point source and uses UpdateLightValues to update its lights.
- I want a display case for the boids with sliders underneath that you can zoom in on to adjust.