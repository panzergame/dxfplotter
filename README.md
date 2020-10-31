# DXF Plotter

> Fast and easy to use DXF to GCode converter for laser CNC


[![Build Status](https://travis-ci.com/panzergame/dxfplotter.svg?branch=develop)](https://travis-ci.com/panzergame/dxfplotter)

![](doc/screen.png)

This application helps users converting DXF files to GCode with minimal settings, fully configurable GCode commands format and fast spline to arc conversion. 

It targets only laser and router CNC following every lines and arcs from DXF file with optional desired depth.


## Features

* DXF support of line, arc, circle, polyline, spline
* Configurable GCode command format
* Multi selected path setting edition
* Path offseting (Tool Compensation)
* Multi passes with depth

## Installation

Currently only linux distributions are supported, windows will come in futur.

### Installation from AppImage

Download latest ![AppImage](https://github.com/panzergame/dxfplotter/releases) and run:

```sh
chmod +x dxfplotter-x86_64.AppImage
./dxfplotter-x86_64.AppImage
```

### Manual installation from source

This project depends on Qt5 and yaml-cpp, for debian like distribution installing `qtbase5-dev` and `libyaml-cpp-dev` is sufficient:

```sh
sudo apt-get install qtbase5-dev libyaml-cpp-dev
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

With a specified dxf file

```sh
build$ ./dxfplotter input.dxf
```

With a specified file and tool

```sh
build$ ./dxfplotter input.dxf -t "Mill 1mm"
```

Once opened, select path from left panel or from viewport and modify settings of selected path group or of single selected path. 

Export with `File->Export` or `Ctrl+E`


## Configuration

![](doc/configuration.png)

Configuration settings are exposed in `Configuration->Settings`, the configuration is splitted in two:
* general settings aiming dxf importing or default values
* tools with settings such as radius and gcode formatting

## GCode format

Simple set of GCode command is used per tool:


| Description | Default Command | Available Variables |
| - | - | - |
| Pre Cut | M4 S \{S:.3f} | S F |
| Post Cut | M5 | |
| Plane Fast Move | G0 X \{X:.3f} Y \{Y:.3f} | X Y |
| Plane Linear Move | G1 X \{X:.3f} Y \{Y:.3f} F \{F:.3f} | S F X Y |
| Depth Fast Move | G0 Z \{Z:.3f} | Z |
| Depth Linear Move | G1 Z \{Z:.3f} | S F Z |
| CW Arc Move | G2 X \{X:.3f} Y \{Y:.3f} I \{I:.3f} J \{J:.3f} F \{F:.3f} | S F X Y I J |
| CCW Arc Move | G3 X \{X:.3f} Y \{Y:.3f} I \{I:.3f} J \{J:.3f} F \{F:.3f} | S F X Y I J |

They can be customized from tool Settings panel `Configuration->Settings->Tools->ToolName->Gcode` or from dxfplotter/config.yml file in your applications configuration folder.

Variables provided in formatting are available with {#:nf} where # is one of the supported variables and n the float precision: 

| Name | Description |
| - | - |
| S | Laser intensity |
| F | Movement feedrate |
| X | Movement X axis target |
| Y | Movement Y axis target |
| Z | Movement Z axis target |
| I | Relative arc center absciss |
| J | Relative arc center ordinate |

Properties `S` and `F` are exposed in path settings in UI.

## Meta

Tristan Porteries

Distributed under the MIT license. See LICENSE.txt for more information.
