# 35mm Film to Gif Converter
A simple GUI application to convert 35mm film negatives to GIFs. This program aims to provide a streamlined process for turning specialty camera negatives into digital GIFs or composite images (WebM support Soon™).
For a list of supported cameras, [jump here](#supported-cameras).

### Nimslo 3D GIF Example "Wigglegram"
<table>
   <tr>
      <th>Full Negative</th>
      <th>Export Result</th>
   </tr>
   <tr>
      <td valign="center" width="60%" height="auto"><img src="https://github.com/user-attachments/assets/4967c70c-63a2-4fca-9db6-d2b1393da0ff" alt="Full Negative"/></td>
      <td valign="center" width="20%" height="auto"><img src="https://github.com/user-attachments/assets/065ad43e-c41a-4be6-aec0-e413263cdf20" alt="Final Gif"/></td>
   </tr>
</table>

### Rensha Cardia GIF Example
<table>
   <tr>
      <th>Full Negative</th>
      <th>Export Result</th>
   </tr>
   <tr>
      <td valign="center" width="60%" height="auto"><img src="https://github.com/user-attachments/assets/3af9bfd6-6f1c-4b53-968a-2acbd190dc3e" alt="Full Negative"/></td>
      <td valign="center" width="20%" height="auto"><img src="https://github.com/user-attachments/assets/9c4d51cc-6899-41d9-8d27-248203786a7b" alt="Final Gif"/></td>
   </tr>
</table>

### Example Composite Image (From a Rensha Cardia)
![SegoviaPanorama](https://github.com/user-attachments/assets/90f33312-2954-4e78-a9b5-617a56d80fe1)


### Supported cameras:
- Rensha Cardia BYU-N 16
- Nimslo 3D

### Future support:
- Rensha Cardia BYU-N 8
- Nishika N8000/N9000
- Stereo Realist
- Lomography Pop 9
- Reto 3D
- Action Tracker Sports 35
  
> If you know of or have a camera that should work with this app let me know and I will add it to the list!
> 
> *You could also save me the trouble and add support yourself following the existing templates. 😉*

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

