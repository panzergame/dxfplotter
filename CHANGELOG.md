
<a name="v1.3.4"></a>
## [v1.3.4](https://github.com/panzergame/dxfplotter/compare/v1.3.3...v1.3.4)

> 2022-10-21

### Chore

* move cleaner/assembler/sorter to filter directory

### Feat

* allow manual workflow triggering
* Filter to remove exact duplicate polylines

### Fix

* windows ci qt5 install
* Spatial index not update after pruning loop.


<a name="v1.3.3"></a>
## [v1.3.3](https://github.com/panzergame/dxfplotter/compare/v1.3.2...v1.3.3)

> 2022-09-12

### Chore

* switch back to qt5

### Feat

* add an option to sort path by length
* tool to set origin to a bounding box corner of selected paths

### Fix

* qt5 link libraries
* qt5 uic
* no versionless qt_add_resources in qt5
* qtbase 5 package name
* root config node displayed multiple times
* path not draw properly after transformation
* missing icons


<a name="v1.3.2"></a>
## [v1.3.2](https://github.com/panzergame/dxfplotter/compare/v1.3.1...v1.3.2)

> 2022-08-06

### Chore

* ignore clangd cache
* skip zero-length bezier
* skip zero-length bezier

### Feat

* compute bounding box of polyline
* compile with qt6

### Fix

* fuse needed to run appimage
* qt6 svg missing during appimage build
* visibility icons in layer and path lists
* viewport not updated when transforming paths
* compiling with qt6.2
* install qt6 svg
* install qt6 svg
* disable pocket action when no document opened
* request qt svg at compilation for icons
* pruning of small biarc
* disable auto uic
* ci for qt6
* ci for qt6
* ci for qt6
* ci install qt6
* ci install qt6
* ci install qt6
* minor compilation issues


<a name="v1.3.1"></a>
## [v1.3.1](https://github.com/panzergame/dxfplotter/compare/v1.3...v1.3.1)

> 2022-04-02

### Chore

* suffix artifact by OS name
* update pocket gif to a more realistic example

### Fix

* prune small biarc before polyline conversion
* prune small biarc before polyline conversion


<a name="v1.3"></a>
## [v1.3](https://github.com/panzergame/dxfplotter/compare/v1.2...v1.3)

> 2022-03-18

### Chore

* add logo and icons
* centre logo image
* add README logo
* document new feature and update screenshots
* manage pocket cutting direction
* Make pocket polylines retain original border orientation
* hide polyline to/from cavc conversion by making them private
* fix code smells
* simplify cmake configure and build procedure

### Feat

* UI and logic to create pocket
* pocket POC using cavc
* compute polyline orientation (cw/ccw)

### Fix

* README gif width


<a name="v1.2"></a>
## [v1.2](https://github.com/panzergame/dxfplotter/compare/v1-windows...v1.2)

> 2022-02-15

### Chore

* update badges

### Fix

* github ci action apt update
* point not positionned


<a name="v1-windows"></a>
## [v1-windows](https://github.com/panzergame/dxfplotter/compare/v1.1...v1-windows)

> 2022-02-11

### Chore

* use a script for windows deploying
* deploy on tag
* use cmake variable to setup /MT
* avoid building tests during deploy
* debug
* embed icons as ressources
* embbed dark theme
* disable export and save action when none file loaded
* versionning dxfplot classes
* remove external dependencies in CI + error on warning
* update yamlcpp import in cmake
* update libdxfrw
* update badges

### Feat

* deploy windows binaries
* introduce windows ci
* use dark palette
* duplicate a configuration item
* Cutting direction setting
* sort path by increasing length
* mirror operator
* display info message for 2second when saving a file

### Fix

* deploy in release mode
* windeployqt invocation
* copy bynary to release directory
* release file
* windows deploy script
* window deploy script
* windows deploy
* windows deploy
* windows deploy
* multiline script
* multithreading library linkage
* jinja2 install for windows build
* cmake target name
* config tree model root parent
* missing windows include
* cmake target name
* windows compilation compatibility
* cmake wextra flag on windows
* windows ci
* install msbuild
* windows ci
* cleanup
* windows ci
* windows ci
* windows ci
* icons
* invalid name popup when cancelling operation
* code smells
* exclude thirdparty from static analysis
* exitting setting panel with ESC
* move to polyline real start when polyline is inverted depending on cutting direction
* polyline self merging + cavc precision
* yaml include dir
* point not positionned
* saving dxfplot and ngc filename cache

### Test

* windeployqt


<a name="v1.1"></a>
## [v1.1](https://github.com/panzergame/dxfplotter/compare/v1.0...v1.1)

> 2021-10-30

### Chore

* rename namespace without upper cases
* remove build and test workflow
* use project base dir
* try sonar gcov
* sonar scanner in build script

### Feat

* print configuration at beginning of gcode files
* add dialog to transform (offset + angle) selected paths
* add version in appimage file name

### Fix

* use last opened or saved file to construct filename for futur files
* gcov path
* do not export config for test
* sonar scan
* uic include
* deploy file name
* release name generation


<a name="v1.0"></a>
## [v1.0](https://github.com/panzergame/dxfplotter/compare/v0.1...v1.0)

> 2021-10-12

### Fix

* deploy file name
* release name generation


<a name="v0.1"></a>
## [v0.1](https://github.com/panzergame/dxfplotter/compare/v0.3...v0.1)

> 2021-10-12

### Chore

* use cereal as submodule
* use fetch content for cereal
* update workflow
* update workflow
* checkout submodule
* split workflow for test and sonarcloud
* code review
* add dxfplot import exporter tests
* serialize Model::Renderable
* move serializing function in serialier module
* migrate to serializer library
* add untracked files
* fix code smells
* include cereal in thidrparty folder
* add cereal dependencies in travis CI
* code smells
* code smells
* throw exception when spline degree not implemented
* update README
* use system libfmt
* try CI targetting ubuntu bionic

### Feat

* add version in appimage file name
* add workflow to deploy
* select/deselect all in viewport with shortcut Ctrl+A/Esc
* add github workflow
* serialize offsetted path
* save visibility and config profile/tool in dxfplot
* save path stack in dxfplot
* save to file without asking file name
* implement save as action
* add save button in UI
* serialize QVector2D
* add document class
* import quadratic splines
* display error message when gcode formatting failed
* cut a first layer at 0 depth

### Fix

* do not import dxf entity block
* close at Ctrl+Q
* name column width + tooltip to display full name
* disable cereal unittest building
* sonar secrets
* sonar scan
* sonar scan
* sonar scan
* try to fiw sonar cloud analysis
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* sonar github action
* Model::PathSettings serialization
* layer and path visibility while importing dxfplot files
* gcode exporter test pointer to task
* export test
* do not link to cereal as it's a header only library
* use cmake modules to find libcereal
* exclude executable from thirdparty libraries
* quadratic bezier point count
* offsetted point must be a point
* print error when file doesn't exists when started from command line
* use fmt::to_string for more compatbility over different fmt version
* build CI on ubuntu focal...
* hide action related to opened file
* adapt test to first cut pass
* icon in AppImage
* display window icon

### Refactor

* own path in layer instead of task
* separate function for creating task
* simplify layer construction


<a name="v0.3"></a>
## [v0.3](https://github.com/panzergame/dxfplotter/compare/untagged-921068d2376f1ef1e911...v0.3)

> 2021-07-15

### Chore

* try fix deploy
* move deploy out of jobs
* deploy only for master and skip cleanup
* deploy only for tags

### Fix

* try deploy on tags


<a name="untagged-921068d2376f1ef1e911"></a>
## [untagged-921068d2376f1ef1e911](https://github.com/panzergame/dxfplotter/compare/untagged-ba3527e65c761e7268ad...untagged-921068d2376f1ef1e911)

> 2021-07-14


<a name="untagged-ba3527e65c761e7268ad"></a>
## [untagged-ba3527e65c761e7268ad](https://github.com/panzergame/dxfplotter/compare/v0.2...untagged-ba3527e65c761e7268ad)

> 2021-07-14

### Chore

* rename offseted by offsetted and use OffsettedPath class to store offsetted polylines and direction.
* cleanup some code smells
* complete sonarcloud properties
* try setup sonarcloud
* rename cut depth by depth per cut
* test exporter and fix task path order
* introduce tree model for layers

### Feat

* manage cutting direction to reduce burrs
* control path visibility based on parent layer
* display layer tree with paths
* add paths tab and layers tab
* convert layer and name paths based on their layer name

### Fix

* sonar cloud project root to look for sonar-project.properties
* uic-settings dependencies in view target
* script for sonarcloud analyze
* icon for right cutter compensation

### Refactor

* makes task return Path reference instead of pointer


<a name="v0.2"></a>
## [v0.2](https://github.com/panzergame/dxfplotter/compare/untagged-7ed6ab255cb68746c6e9...v0.2)

> 2020-10-02


<a name="untagged-7ed6ab255cb68746c6e9"></a>
## untagged-7ed6ab255cb68746c6e9

> 2020-10-02

