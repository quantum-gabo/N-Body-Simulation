# 🌌 N-Body Simulation

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![OpenMP](https://img.shields.io/badge/OpenMP-Enabled-blue.svg)](https://www.openmp.org/)

<!-- Screenshot placeholder - Replace with an actual screenshot for your repository -->
![Galaxy Simulation Screenshot](https://via.placeholder.com/800x800.png?text=N-Body+Simulation+Screenshot)

<!-- Animation placeholder - Consider hosting large media files externally -->
![Galaxy Simulation Animation](https://via.placeholder.com/800x800.png?text=Animation+Placeholder)

## 🚀 Overview
- A unified N-body gravitational simulation implementing both Direct Method and Barnes-Hut algorithm
- Real-time visualization using SDL2
- Support for multiple initial conditions: Uniform Disk, Two Galaxies, and Solar System
- Interactive visualization with pause/resume functionality
- Frame export capabilities for creating animations

## ✨ Key Features
- Multiple Force Calculation Methods:
  * Barnes-Hut Algorithm (θ = 0.5)
  * Direct N-body calculation
- Integration Methods:
  * Verlet Integration
  * Euler Integration
- OpenMP Parallelization (default: 8 threads)
- Real-time Visualization:
  * Interactive window (800x800)
  * FPS display
  * Body count and step counter
  * Optional quadtree visualization
- Performance Benchmarking Suite
- Customizable Parameters:
  * Number of bodies (1-100,000)
  * Time step (0.000001-0.001 years)
  * Integration method
  * Initial conditions

## 🔧 Technical Details
- C++20 standard
- CMake build system
- Dependencies:
  * SDL2
  * SDL2_ttf
  * OpenMP
- Optimizations:
  * -O3 optimization flag
  * Native architecture optimizations (-march=native)
  * Multi-threaded computation

## 🎮 Controls
### Simulation Controls
- `Space` : Pause/Resume simulation
- `R` : Reset simulation
- `Q` or `Esc` : Quit application
- `S` : Save current frame as screenshot
- `V` : Toggle frame saving for video generation

### Navigation Controls
- `Mouse Wheel` : Zoom in/out of the simulation
- `Mouse Drag` : Pan around the simulation space
- `Mouse Right-Click` : Reset view to centre

## 📁 Project Structure
```
n-body-simulator/
├── include/                    # Header files
│   ├── Benchmark.h             # Performance benchmarking functionality
│   ├── Body.h                  # Celestial body representation
│   ├── CLI11.hpp               # Command-line interface library
│   ├── Constants.h             # Physical constants and simulation parameters
│   ├── EulerIntegrator.h       # Euler integration method
│   ├── Forces.h                # Force calculation methods (Direct and Barnes-Hut)
│   ├── Node.h                  # Quadtree node interface
│   ├── Quad.h                  # Quadrant representation for Barnes-Hut
│   ├── Quadtree.h              # Barnes-Hut quadtree implementation
│   ├── Utils.h                 # Utility functions and initial conditions
│   ├── Vector.h                # Vector mathematics for 2D/3D computations
│   ├── VerletIntegrator.h      # Verlet integration method
│   └── Window.h                # SDL2 visualization and user interface
├── src/                        # Source files
│   ├── Benchmark.cpp           # Benchmark implementation
│   ├── Body.cpp                # Body class implementation
│   ├── EulerIntegrator.cpp     # Euler integrator implementation
│   ├── Forces.cpp              # Force calculations for both methods
│   ├── JetBrainsMonoNLNerdFontMono-Regular.ttf  # Font for UI text
│   ├── main.cpp                # Entry point and simulation loop
│   ├── Quad.cpp                # Quadrant implementation
│   ├── Quadtree.cpp            # Quadtree operations for Barnes-Hut
│   ├── Utils.cpp               # Initial conditions and utility functions
│   ├── Vector.cpp              # Vector operations implementation
│   ├── VerletIntegrator.cpp    # Verlet integrator implementation
│   └── Window.cpp              # SDL2 rendering and user interface
├── results/                    # Simulation outputs and benchmark data
│   ├── frames/                 # Exported animation frames
│   ├── *_benchmark_results.csv # Performance data
│   └── benchmark.ipynb         # Analysis notebook
├── build/                      # Build directory (generated)
├── CMakeLists.txt              # CMake build configuration
├── LICENSE                     # Project license
└── README.md                   # This file
```

## 🛠️ Build Instructions
```bash
# Install dependencies (Arch Linux)
sudo pacman -S sdl2 sdl2_ttf cmake gcc

# Build the project
mkdir build && cd build
cmake ..
make -j8
```

## 💻 Usage
```bash
# Basic usage
./n-body-simulator

# Custom configuration
./n-body-simulator -N 50000 -t 0.00001 -i 0 -m barnes-hut -I verlet

# Available options:
-N, --n-bodies     : Number of bodies (1-100000)
-t, --time-step    : Integration time step (0.000001-0.001)
-i, --init-system  : Initial system (0: Uniform Disk, 1: Two Galaxies, 2: Solar System)
-m, --method       : Force calculation method (barnes-hut, direct)
-I, --integrator   : Integration method (verlet, euler)
--dn              : Draw quadtree nodes (Barnes-Hut only)
--b               : Run benchmarks
```

## 🎬 Media Generation
The simulation can automatically save frames during execution. These frames can be found in the `results/frames/` directory and can be converted to different video formats:

```bash
# The simulation generates numbered frames (frame_0000.bmp through frame_0239.bmp)
# Each frame is approximately 2.6MB in BMP format for maximum quality

# To enable frame capture during simulation, press the 'V' key
# Press 'V' again to stop recording and save the frames
# The frames will be automatically saved to the results/frames/ directory

# Generate GIF from saved frames (better for short sequences)
convert -delay 10 results/frames/frame_*.bmp simulation.gif

# Generate MP4 video (better for longer sequences)
ffmpeg -framerate 30 -pattern_type glob -i 'results/frames/frame_*.bmp' \
       -c:v libx264 -pix_fmt yuv420p simulation.mp4

# Clean up frames after video generation
rm results/frames/frame_*.bmp
```

Note: The default simulation generates 240 frames (0-239) at high resolution (800x800). 
The output MP4 video is approximately 20MB in size.

## 📊 Results Directory Structure
```
results/
├── frames/                    # Simulation frame exports (*.bmp)
│   ├── frame_0000.bmp        # First frame
│   ├── frame_0001.bmp        # Second frame
│   ├── ...                   # Additional frames
│   ├── frame_0239.bmp        # Last frame
│   └── output.mp4            # Generated video file
├── screenshot.bmp            # Latest screenshot
├── benchmark.ipynb           # Benchmark analysis notebook
├── barnes_hut_benchmark_results.csv    # Barnes-Hut performance data
└── direct_method_benchmark_results.csv  # Direct method performance data
```

## 📈 Performance
<!-- Performance graph placeholder - Create this from your benchmark data -->
![Performance Comparison](https://via.placeholder.com/800x400.png?text=Performance+Comparison+Graph)

The Barnes-Hut algorithm significantly improves performance for large-N simulations. Below is a table with actual benchmark data from our measurements:

| Number of Particles | Barnes-Hut Time (s) | Direct Method Time (s) | Speedup |
|---------------------|---------------------|------------------------|---------|
| 1,000               | 0.001538            | 0.003328              | 2.2x    |
| 10,000              | 0.011732            | 0.320299              | 27.3x   |
| 100,000             | 0.152524            | 32.540323             | 213.3x  |

The theoretical complexity is O(N log N) for Barnes-Hut compared to O(N²) for the direct method, which is clearly demonstrated by our benchmark results. At 100,000 particles, the Barnes-Hut algorithm is over 213 times faster than the direct method.

### 🔄 Parallelisation

The simulator utilises OpenMP for parallel computation of forces and integration steps. The performance measurements above were conducted using parallel execution with 8 threads. The parallelisation significantly improves performance for both algorithms, with nearly linear scaling for the Direct method and good scaling for the Barnes-Hut algorithm until communication overhead becomes significant.

To adjust the number of threads, modify the `THREAD_COUNT` constant in `Constants.h` or use the OpenMP environment variable:

```bash
export OMP_NUM_THREADS=16  # Use 16 threads
./n-body-simulator
```
The complete benchmark data can be found in:
- `results/barnes_hut_benchmark_results.csv`
- `results/direct_method_benchmark_results.csv`
- Analysis notebook: `results/benchmark.ipynb`

## 🔭 Physical Units
- Distance: Astronomical Units (AU)
- Mass: Solar Masses (M☉)
- Time: Years
- Gravitational Constant: 4π² AU³/(M☉·year²)

## 🆕 Recent Updates
- Unified implementation combining both Direct and Barnes-Hut methods (commit 9b7660d)
- Enhanced visualization with SDL2
- Added benchmark suite and results analysis
