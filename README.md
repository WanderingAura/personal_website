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

# Portfolio site (static, GitHub Pages)

The raylib gallery is embedded at the top of a single static page in `site/`, with About/Projects/Contact
below it. Every push to `main` builds the WASM and deploys `site/` to GitHub Pages
(`.github/workflows/pages.yml`).

To preview locally (needs `EMSDK` set, e.g. `source ~/src/emsdk/emsdk_env.sh`):
```
scripts/build_site.sh       # builds the web target and copies the WASM artifacts into site/wasm/
python -m http.server 8080  # from the repo root
```

Then visit `http://localhost:8080/site/`. Serving from the repo root puts the page under a sub-path, the same
as on GitHub Pages, so any accidental absolute path shows up locally.

# TODO
- I want a room class where I can specify a size and a set of directions using bit flags representing which
directions the doors are in
- I want a light class that is a light point source and uses UpdateLightValues to update its lights.
- I want a display case for the boids with sliders underneath that you can zoom in on to adjust.