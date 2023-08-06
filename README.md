# imgprocesslib - Simple Image Processing Library
<span style="color:#blue">[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](https://github.com/vidojesevic/imgprocesslib/blob/main/LICENSE)</span>

## Overview 
mgprocesslib is a simple terminal-based image processing tool that allows you to resize images quickly. It provides an easy-to-use command-line interface for resizing images to various predefined dimensions, making it convenient for preparing images for use on websites and other applications.

## Features
- Resize images to predefined dimensions suitable for various use cases.<br />
- Easy installation and usage.<br />
- Command-line interface for quick and efficient image processing.<br />

## Requirements
- GCC (GNU Compiler Collection)
- C Standard Library
- stb_image.h (Included in the repository)<br />

## Installation guide
To install imgprocesslib, follow the steps below: 
1. Clone the repository to your local machine:<br />
```
git clone https://github.com/vidojesevic/imgprocesslib
```
3. Change into the project directory:<br />
```
cd imgprocesslib
```
4. Compile the code:<br />
```
make
```
5. Install the program globally (you may need to use sudo):<br />
```
sudo make install
```
## Usage
After installing the program, you can use it from anywhere on your system. Open a terminal and run the imgprocess command, followed by the desired image processing options.

## Uninstall the program:
```
sudo make uninstall
```
## License
This project is licensed under the GNU General Public License v3.0. See the LICENSE file for details.
## Acknowledgments
mgprocesslib uses the stb_image.h library by Sean Barrett for image loading and saving. Many thanks to Sean for this excellent and lightweight library.

