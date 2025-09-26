# Carbon Cluster Ionization

A high-performance C++ simulation code for modeling the interaction of carbon particles with focused Gaussian laser pulses. The simulation uses molecular dynamics and field ionization physics to study cluster dynamics and electron emission.

## Overview

The simulation consists of two main stages:

1. **Equilibration**: Carbon particles are initially placed on a 3D grid and interact through Lennard-Jones potential until reaching thermal equilibrium
2. **Ionization**: The equilibrated cluster is exposed to an intense laser pulse, which can ionize the particles (remove electrons). The code tracks particle positions, velocities, and ionization states throughout the laser interaction

## Key Features

- **Parallel Processing**: Utilizes OpenMP for multi-threaded performance
- **Physical Accuracy**: Implements proper Lennard-Jones interactions and field ionization physics
- **Flexible Parameters**: Configurable laser parameters, grid dimensions, and simulation timesteps
- **Real-time Visualization**: Generates output suitable for analysis and visualization


## Requirements

- **C++ Compiler**: Supporting C++20 standard (GCC 10+ or Clang 12+)
- **CMake**: Version 3.10 or higher
- **OpenMP**: For parallel processing support (optional but recommended)
- **Operating System**: Linux, macOS, or Windows with appropriate build tools

## Installation

1. Clone the repository:
```bash
git clone https://github.com/AnAdisorn/Carbon-cluster-ionization.git
cd Carbon-cluster-ionization
```

2. Create and enter build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
make
```

4. Verify installation by running tests:
```bash
make test
```

## Usage

### 1. Initializing and Equilibrating Carbon Cluster

First, create and equilibrate a carbon cluster using the Lennard-Jones potential:

```bash
./initialise_cluster <grid_size_x> <grid_size_y> <grid_size_z> [spacing] <steps> <dt>
```

**Parameters:**
- `grid_size_x`, `grid_size_y`, `grid_size_z`: Number of particles along each axis
- `spacing` (optional): Initial spacing between particles (default: optimized value)
- `steps`: Number of equilibration timesteps
- `dt`: Timestep size for integration

**Example:**
```bash
./initialise_cluster 10 10 10 1.5 10000 0.001
```

This creates a 10×10×10 cluster with 1.5 unit spacing, runs for 10,000 steps with a timestep of 0.001.

https://github.com/user-attachments/assets/e6147f3f-eb0e-45b2-bcc6-8572f2ac020e

![potential](https://github.com/user-attachments/assets/e67059a8-b6dc-4081-b154-182652c0d670)

### 2. Simulating Cluster Ionization

After equilibration, simulate the laser ionization process:

```bash
./ionise_cluster <e0> <w> <b> <dt> <cycles> <polarization> <method>
```

**Parameters:**
- `e0`: Peak electric field strength (V/m)
- `w`: Laser frequency (rad/s)
- `b`: Gaussian beam waist parameter
- `dt`: Timestep for ionization simulation
- `cycles`: Number of laser cycles
- `polarization`: Laser polarization (0=linear, 1=circular)
- `method`: Integration method (0=Euler, 1=RK4)

**Example:**
```bash
./ionise_cluster 1e12 2.3e15 10.0 0.0001 5 0 1
```

This simulates ionization with 10¹² V/m field strength, frequency 2.3×10¹⁵ rad/s, 10-unit beam waist, for 5 laser cycles using RK4 integration.

https://github.com/user-attachments/assets/8fe7228d-4558-4ebb-bec1-9c2d3e3dd37c

![image](https://github.com/user-attachments/assets/7f084bd9-2ec8-49e2-996c-ab705f2ce2d3)

## Testing

The project includes unit tests to verify core functionality. To run the test suite:

```bash
cd build
make test
```

Or run tests individually:

```bash
./test_ionisation_map
```

**Test Coverage:**
- Parameter validation and reference handling
- Ionization mapping accuracy
- Physical constant verification

## Output Files

The simulation generates several output files for analysis:

- **positions.dat**: Particle positions throughout the simulation
- **velocities.dat**: Particle velocity data
- **ionization.dat**: Ionization states and electron counts
- **energy.dat**: System energy evolution
- **field.dat**: Electric field values at particle positions

## Support

For questions, issues, or support:

- **GitHub Issues**: Report bugs or request features at [GitHub Issues](https://github.com/AnAdisorn/Carbon-cluster-ionization/issues)
- **Documentation**: Check the inline code documentation for detailed API reference
- **Email**: Contact the maintainer for research collaboration inquiries

## Contributing

We welcome contributions to improve the simulation accuracy, performance, and usability. To contribute:

1. **Fork the repository** and create a feature branch
2. **Follow coding standards**: Use consistent C++20 style and include appropriate tests
3. **Run tests**: Ensure all tests pass before submitting
4. **Submit a pull request** with a clear description of changes

### Development Setup

```bash
# Install development dependencies
sudo apt-get install cmake g++ libomp-dev

# Build with debug symbols
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Run tests
make test
```

### Code Quality

- Follow modern C++ best practices
- Include unit tests for new features
- Document public APIs
- Ensure thread safety for parallel regions

## Authors and Acknowledgments

- **Primary Author**: Adisorn Panasawatwong
- **Institution**: MPI-PKS
- **Research Group**: Finite systems

Special thanks to contributors and the computational physics community for theoretical foundations and code improvements.

## License

This project is licensed under MIT License - see the LICENSE file for details.

## Project Status

**Active Development** - This project is actively maintained and developed for ongoing research in laser-matter interaction physics. New features and improvements are regularly added.
