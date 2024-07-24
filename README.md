# 35mm Film to Gif Converter
A simple GUI application to convert 35mm film negatives to GIFs. This program aims to provide a streamlined process for turning specialty camera negatives into digital GIFs or short video.

## About
The app takes a single image of a 35mm film negative and converts it into a GIF. The user can adjust the number of frames, the frame rate, and the resolution of the output GIF. The app also provides a preview of the GIF before saving it to disk.

Supported cameras (Aim in V1.0.0):
- Rensha Cardia BYU-N 16
- Nimslo 3D

Future support:
- Rensha Cardia BYU-N 8
- Nishika N8000/N9000
- Stereo Realist
- Lomography Pop 9
- Reto 3D
- Action Tracker Sports 35

## Getting Started
There are no pre-built binaries available at this time. To run the application, you will need to build it from source.

### Build Instructions
1. Clone the repository to your local machine.
2. The project is set up for VSCode with the CMake Tools extension. (But CMake can be run from the command line as well)
3. In VSCode, under the Run and Debug tab, select the Launch task that matches your system platform.
4. This should generate the necessary build files and compile the application.
   1. On first build, the project will download and build the necessary dependencies (wxWidgets).
   2. This may take some time depending on your system and internet connection.
   3. After the first build you will have to navigate to the wxWidgets folder and apply the patches described in [wxWidgetsChanges.md](wxWidgetsChanges.md).
5. The application should be output to the `build` directory!

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## License Attributions
The following repositories were used in the development of this project:
- [**`wxWidgets`**](https://github.com/wxWidgets/wxWidgets)
- [**`Halide`**](https://github.com/halide/Halide)
- [**`wx_cmake_fetchcontent_template`**](https://github.com/lszl84/wx_cmake_fetchcontent_template)

