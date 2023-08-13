# ipl - Simple Image Processing Library
<span style="color:#blue">[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](https://github.com/vidojesevic/imgprocesslib/blob/main/LICENSE)</span>

## Overview 
**ipl** is a simple terminal-based image processing tool that allows you to resize 
images quickly. It provides an easy-to-use command-line interface for resizing 
images to various predefined dimensions, making it convenient for preparing 
images for use on websites and other applications.

## Features
- Resize images to predefined dimensions suitable for various use cases.<br />
- Easy installation and usage.<br />
- Command-line interface for quick and efficient image processing.<br />

## Requirements
- GCC (GNU Compiler Collection)
- GNU Make
- stb_image.h (Included in the repository)

## Installation guide
To install ipl, follow the steps below: 
1. Clone the repository to your local machine:<br />
```
git clone https://github.com/vidojesevic/imgprocesslib
```
2. Change into the project directory:<br />
```
cd imgprocesslib
```
3. Compile the code:<br />
```
make
```
4. Install the program globally (you may need to use sudo):<br />
```
sudo make install
```
## Usage
To use **ipl**, simply type 'ipl' in your terminal, and the program will 
prompt you to enter the path to the image you want to process.
### Usage with CLI Arguments
You can also use command-line arguments to process images directly from 
the command line. For example, to resize an image and save it with a 
specific output name, you can use the following command:
```
ipl /path/to/input.png -r --favicon /path/to/output.png
```
### Usage with Absolute or Relative Paths
When prompted to enter the image's path, you can use an absolute path, such as 
'/absolute/path/to/image.png/', or a relative path from the directory where you 
call the ipl program. For example, if you are in the root directory of your 
project and want to process an image located at 'assets/img/logo.png', 
you can simply enter 'assets/img/logo.png' as the image path.<br />
Similarly, you can enter relative paths using the '**../**' notation. For example, 
if the image is located two directories above the current directory, you can 
enter '../../directory/image.png'.

## Uninstall the program:
```
sudo make uninstall
```
## License
This project is licensed under the GNU General Public License v3.0. 
See the LICENSE file for details.
## Acknowledgments
ipl uses the stb_image.h library by Sean Barrett for image loading and saving. 
Many thanks to Sean for this excellent and lightweight library.

