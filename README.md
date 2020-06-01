# DXF Plotter

> Fast and easy to use DXF to GCode converter for laser CNC


[![Build Status](https://travis-ci.com/panzergame/dxfplotter.svg?branch=develop)](https://travis-ci.com/panzergame/dxfplotter)

![](doc/screen.png)

This application helps users converting DXF to GCode with minimal settings, fully configurable GCode command format and fast spline to arc conversion. 

It targets only laser CNC following every lines and arcs from DXF file.


## Feature

* DXF support line, arc, circle, polyline, spline
* Configurable GCode command format
* Multi selected path setting edition
* Path offseting (Tool Compensation)

## Installation

Currently only linux distributions are supported, windows will come in futur.

### Installation from AppImage

Download latest ![AppImage](https://github.com/panzergame/dxfplotter/releases) and run:

```sh
chmod +x dxfplotter-x86_64.AppImage
./dxfplotter-x86_64.AppImage
```

### Manual installation from source

This project depends on Qt5, for debian like distribution installing `qtbase5-dev` is sufficient:

```sh
sudo apt-get install qtbase5-dev
```

```sh
git submodule init
git submodule update

mkdir build
cd build
cmake ..
make
```

## Usage example

Launch GUI

```sh
build$ ./dxfplotter
```

Or with a dxf file

```sh
build$ ./dxfplotter input.dxf
```

Once opened, select path from left panel or from viewport and modify settings of selected path group or of single selected path. 

Export with `File->Export` or `Ctrl+E`

## GCode format

Simple set of GCode command is used:


| Description | Default Command | Available Variables |
| - | - | - |
| Tool On | M4 S \{S:.3f} | S F |
| Tool Off | M5 | |
| Fast Move | G0 X \{X:.3f} Y \{Y:.3f} | X Y |
| Linear Move | G1 X \{X:.3f} Y \{Y:.3f} F \{F:.3f} | S F X Y |
| CW Arc Move | G2 X \{X:.3f} Y \{Y:.3f} I \{I:.3f} J \{J:.3f} F \{F:.3f} | S F X Y I J |
| CCW Arc Move | G3 X \{X:.3f} Y \{Y:.3f} I \{I:.3f} J \{J:.3f} F \{F:.3f} | S F X Y I J |

They can be customized from Settings panel `Configuration->Settings` or from dxfplotter/config.ini file in your applications configuration folder.

Commands use variables with {#:nf} where # is one of the supported variables and n the float precision: 

| Name | Description |
| - | - |
| S | Laser intensity |
| F | Movement feedrate |
| X | Movement target absciss |
| Y | Movement target ordinate|
| I | Relative arc center absciss |
| J | Relative arc center ordinate |

Properties `S` and `F` are exposed in path settings in UI.

## Meta

Tristan Porteries

Distributed under the MIT license. See LICENSE.txt for more information.
