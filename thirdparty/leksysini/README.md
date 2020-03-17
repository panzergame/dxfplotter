# Leksys' INI Parser

Crossplatform all-in-one INI-file parser, written on C++ with STL
```
+====================================================================================+
|   _         _                  _   ___ _   _ ___ ____                              |
|  | |    ___| | _____ _   _ ___( ) |_ _| \ | |_ _|  _ \ __ _ _ __ ___  ___ _ __     |
|  | |   / _ \ |/ / __| | | / __|/   | ||  \| || || |_) / _` | '__/ __|/ _ \ '__|    |
|  | |__|  __/   <\__ \ |_| \__ \    | || |\  || ||  __/ (_| | |  \__ \  __/ |       |
|  |_____\___|_|\_\___/\__, |___/   |___|_| \_|___|_|   \__,_|_|  |___/\___|_|       |
|                      |___/                                                         |
|                                                                                    |
+====================================================================================+ 
```

## Advantages

  - Residents in single file - iniparser.hpp. Simply include it to your project and use
  - Crossplatform (Windows & POSIX)
  - Nice and easy-to-use C++ interface
  - Extends standart INI interface with: 
    - Arrays (comma-separated values, i.e: "val1, val2, val3")
    - Maps (comma-separated pairs, i.e.: "key1:value, key2: value")
    - Nested sections (i.e.: "[parent.child]")
    - Nested arrays and maps (i.e.: "val1,{val2.1,val2.2},{k31:v31, k32:v32}")
    - File includes (;#include <file_path>)
 - Can be used to save and load any user-defined classes, as long as stream operators are provided 
 - Supports multilines
 - Loads & saves comments
 - MIT license

## Usage

Include provided iniparser.hpp to your C++ project and that is all. 
In your code you can use different ways to set and get values, as shown below
```cpp
#include <iostream>
#include "iniparser.hpp"

int main(int argc, char** argv)
{
	int ival;
	double dval;
	// Creating ini file object
	INI::File ft;
	// Loading from file
	if (!ft.Load("settings.ini"))
	{
		// Loading from stream
		std::ifstream op("opts.ini",std::ios::in);
		if (op.is_open())
			op >> ft;
	}
	// Set "value1" in section "Main" to 20
	ft.GetSection("Main")->SetValue("value1",20);
	// get "value1" from section "Main". If there is no such entry return default value 1
	ival = ft.GetSection("Main")->GetValue("value1",1).AsInt();
	// get "value2" from section "Main", defaulting to 2
	ival = ft.GetValue("Main:value2",2).AsInt();
	// set "value1" in subsection "subsect" of section "Main"
	ft.SetValue("Main.subsect:value1",345.5);
	// get "value1" from subsection "subsect" of section "Main"
	dval = ft.GetSection("Main")->GetSubSection("subsect")->GetValue("value1").AsDouble();
	// set 4th element of array "array" in section "Main" to 32.1
	ft.GetSection("Main")->SetArrayValue("array",4,32.1);
	// get 4th element of array "array" in section "Main"
	dval = ft.GetSection("Main")->GetValue("array").AsArray()[4].AsDouble();
	// iterate over all sections in file
	for (INI::File::sections_iter it = ft.SectionsBegin(); it != ft.SectionsEnd(); ++it)
	{
		std::cout << "Section name: " << it->first << std::endl;
		INI::Section* sect = it->second;
		// iterate over all entries in specific section
		for (INI::Section::values_iter it2 = sect->ValuesBegin(); it2 != sect->ValuesEnd(); ++it2)
			std::cout << "Entry name: " << it2->first << ", Entry value: " 
					  << it2->second.AsString() << std::endl;
	}
	// output to INI file
	ft.Save("settings.ini");
	// output to stream
	std::cout << ft;
}
```

Look in provided test_app.cpp for more examples of using Leksys' IniParser, for example how to get and set comments,
how to work with maps and how to save and load your classes.

## INI file syntax

Leksys' parser supports extended ini file syntax with comments, multilines, arrays, maps, subsections and inclusions.
Check below.
```ini
; Comment line
; File inclusion. Absolute or relative path can be used
;#include settings2.ini
; simple line
global_int = 1
; multiline
global_str = Hello! This is just a sample string \
             That is multiline \
             Simple as that

; Section
[Main]
int      = 1                                 ; Integer value
double   = 2.303                             ; Double value
str      = Hello! This is string!            ; String
bool     = true                              ; Boolean value
array    = 1, 202.56, String with space, 5   ; Array
cmp_arr  = {12,3}, {13,5}, {18,9}            ; Array of arrays
cmp_arr2 = a, b, {, String with ,}           ; Array with string, cont. separator
cmp_arr3 = a, b, /, String with /,           ; Same as array cmp_arr2
cmp_arr3 = a, b, String with /{ and /}       ; Array with string, cont. braces
map      = 1:5, 1:3, 2:9, 3:10               ; Map
cmp_map  = a:{1,5,7}, b: {str1, {2,3}}       ; Complex map with arrays as values
cmp_map2 = a:{String with ,,}, b: String     ; Map with string, cont. separator
cmp_map3 = a:String with /,/,, b: String     ; Same as cmp_map2
cmp_map3 = a:String with /{ and /}, b: Str   ; Map with string, cont. braces

; Subsection (Sub1 is considered subsection of section Main)
[Main.Sub1]
ret      = String
ret2     = 201

[Main.Sub2]
; File contents of included file will be placed here
;#include settings/sub2.ini

```

## Building Test Application

You can build test application to test parser's work. 
You need to install CMake before building test_app. CMake binary for your platform can be found at: http://www.cmake.org/cmake/resources/software.html

### POSIX (Linux, QNX, FreeBSD, MacOS)

```sh
$ mkdir build; cd build
$ cmake <this_file_dir>
$ make
```

### Windows

Use cmake-gui application for building project files for your IDE.
Alternatively, if you want to build project for Visual Studio, you can do it directly from the console in true-Linux style =):

```cmd
$ mkdir build; cd build
$ cmake <this_file_dir>
$ %VS90COMNTOOLS%\vsvars32.bat
$ nmake
```
Note, that for Visual Studio 10.0 (2010) environment variable will be %VS100COMNTOOLS% and so on.

## Running Test Application

1. Simply start application and ensure, that 10 first tests pass.
2. Start application with parameters:
    ```sh
    -i <this_file_dir>/ini-test/test1.ini -o out_file.ini
    ```
    and ensure that file *<application_dir>/out_file.ini* contains all the stuff from ini files in test directory

## Licence
MIT license