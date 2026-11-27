# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

dxfplotter: Qt6 desktop app converting DXF files to GCode for laser/router CNC (tool compensation, pockets, multi-pass depth, drag knife, 3D simulation).

## Build & test

```sh
git submodule update --init
cmake --preset dev            # local preset (CMakeUserPresets.json): clang-20, Ninja, Qt 6.10, binary dir build/
cmake --build build -j$(nproc)
ctest --test-dir build                                   # all tests
./build/test/dxfplotter-test --gtest_filter='DragKnifeTest.*' # single test / suite
```

- Requires CMake ≥ 4.0, C++23, Python 3 + jinja2 (config code generation).
- Options: `WITH_ORTOOLS` (path order optimization, needs or-tools in `/opt`), `WITH_SIMULATION` (Qt3D view, ON by default, disabled in the Windows CI build), `CODE_COVERAGE`.
- CI presets live in `CMakePresets.json` (`ci-build-test-coverage`, `ci-deploy`); CI scripts in `ci/`, Docker image in `ci/docker/ubuntu/Dockerfile`.
- Tests are GoogleTest (fetched via FetchContent) in a single `dxfplotter-test` executable; each test source must be listed in `test/CMakeLists.txt`.
- Formatting: `.clang-format` (WebKit-based, tabs).

## C++20 modules

Every source under `src/` is a module unit, not a header/source pair. File `src/a/b/c.cpp` exports module `a.b.c` (e.g. `src/geometry/tooloperation/dragknife.cpp` → `geometry.tooloperation.dragknife`). Layout of a unit:

```cpp
module;
#include <...>          // std and Qt headers go in the global module fragment
export module geometry.foo;
import geometry.polyline;
export namespace geometry { ... }
```

- Libraries are declared with `add_module_library(name <module sources> NON_MODULE_SOURCES <impl units/headers>)` (`cmake/modules.cmake`). A new file must be added to its directory's `CMakeLists.txt`, and a new library to `LINK_LIBRARIES` in the root `CMakeLists.txt`.
- Implementation units (`module x;` without `export`, e.g. `src/config/config.cpp`, `src/exporter/gcode/exporterimpl.cpp`) go in `NON_MODULE_SOURCES`.
- Optional features are guarded with `#ifdef WITH_ORTOOLS` around `import` lines.
- MSVC/MinGW are sensitive to import order and incomplete types across modules; several past fixes import a base class module before its derived ones.

## Architecture

Layers, bottom-up (each is a static library under `src/`):

- `common` – small utilities (`Aggregable<T>` gives `Ptr`/`UPtr`/`List`/`ListPtr` aliases used everywhere).
- `geometry` – `Bulge` (segment: start, end, bulge factor; line or arc) and `Polyline` (list of bulges) are the core types. Splines are converted to arcs via biarc fitting. Offsetting/pocketing go through CavalierContours, wrapped by module `geometry.cavc` (`cavc.cpp`, which also sets cavc's precision thresholds). `filter/` cleans and assembles imported polylines.
- `geometry/tooloperation` – `ToolOperation::generate(Polyline)` returns `ToolPolyline`s (polyline + optional entry) and a cutting direction. Implementations: `Offset`, `Pocket`, `DragKnife`. The tool type chosen in config decides which operation the application applies for left/right actions.
- `config` – the whole configuration tree is declared in `template/config.xml`; `config_gen.py` + `configgen.cpp.in` (jinja2) generate `build/src/config/configgen.cpp` (module `config.config`) with typed group/list/property classes. Edit the XML to add a setting, not the generated code. Stored as YAML (yaml-cpp).
- `model` – `Document` → `Task` → `Layer`/`Path`. `Path` holds the base polyline, `PathSettings`, and an optional `ToolPath` from a tool operation. `Renderable` is the Qt-object base for selectable/visible items. `DocumentHistory` implements undo/redo by full `Document` snapshots. `model/application` holds `Application` (top-level QObject: open/save/export, apply tool operations, config, tool/profile selection) and `Simulation`.
- `importer` – `dxf` (via libdxfrw) and `dxfplot` (native project format, cereal serialization from `serializer/`).
- `exporter` – `renderer/Renderer<Visitor>` walks the task and depth passes, emitting moves to a visitor; `gcode` formats moves using per-profile fmt-style templates (`{X:.3f}`) and a post-processor; `dxfplot` saves projects.
- `view` – Qt Widgets UI. `.ui` forms live in `template/uic/` and are compiled with `qt_wrap_ui` (AUTOUIC is OFF). `view2d` is a QGraphicsView viewport; `simulation` uses Qt3D; `DocumentModelObserver` lets views react to document changes.

`src/main.cpp` wires `model::Application` and `view::MainWindow`; CLI accepts a file and `-t <tool>` / `-p <profile>`.

## Conventions

- Commit messages follow Conventional Commits (`feat:`, `fix:`, …); `CHANGELOG.md` is generated with git-chglog (`.chglog/`).
- `reference/` holds papers backing the spline-to-arc and biarc algorithms.
