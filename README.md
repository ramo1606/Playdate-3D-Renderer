# 3D Software Renderer for Playdate

This project is a 3D software renderer implemented in C89 for the Playdate handheld gaming system. It provides a modular and efficient 3D graphics engine tailored for the Playdate's hardware capabilities.

## Features

- 2D and 3D vector operations
- Point projection and linear transformations
- Triangle and vertex handling
- Dynamic arrays for memory management
- Line drawing algorithms
- OBJ file parsing and rendering
- Triangle rasterization with depth sorting
- Matrix operations and 3D transformations
- Lighting and shading effects
- Texture mapping with perspective correction
- PNG file decoding for textures
- Z-buffer implementation
- Camera system and clipping
- Support for multiple meshes
- Optimized for Playdate hardware

## Requirements

- Visual Studio 2022
- CMake (version 3.10 or higher)
- Git
- Playdate SDK (version 2.5.0 or higher)

## Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/3d-software-renderer-playdate.git
   cd 3d-software-renderer-playdate
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Generate the project files:
   ```
   cmake ..
   ```

4. Open the generated solution file in Visual Studio 2022 and build the project.

## Running Tests

Unit tests are implemented using the Minunit.h library. To run the tests:

1. Build the project in Debug mode
2. Run the `run_tests` executable generated in the build directory

## Documentation

Comprehensive documentation can be found in the `docs` directory. This includes:

- API documentation
- Implementation details
- Usage instructions and examples
- Performance considerations

## Contributing

Contributions are welcome! Please read the [CONTRIBUTING.md](CONTRIBUTING.md) file for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- Playdate SDK developers
- Contributors to the Minunit.h testing library

## Contact

For any questions or concerns, please open an issue on the GitHub repository.
