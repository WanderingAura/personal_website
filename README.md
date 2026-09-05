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

# Running the web build

After building the executable change directory into the `build/src` directory and execute
```
python -m http.server 8080
```

This should start an http server on localhost serving files from that folder. It should then be possible to
view the server by going to `http://localhost:8080/personal_gallery.html`

# TODO
- I want a room class where I can specify a size and a set of directions using bit flags representing which
directions the doors are in
- I want a light class that is a light point source and uses UpdateLightValues to update its lights.
- I want a display case for the boids with sliders underneath that you can zoom in on to adjust.