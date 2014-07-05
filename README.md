reed-util
=========

Vector/matrix math and other basic utilities.

Instructions:
* `#include <util-all.h>` for everything
  * Or, `#include <util.h>` for just the stuff in the "Basics" section below
* Link with `util.lib`, or just add `util.vcxproj` to your solution and reference it

Basics:
* `dim()` and `dim_field()`—size of static arrays
* `cassert()`
* `uint`, `byte`, sized integer types `i8`, `u8` through `i64`, `u64`
* Float constants: `pi`, `epsilon = 1e-6f`, `infinity`, `NaN`
* Generic `swap`, `min`, `max`, `clamp`, `saturate`, `lerp`, `square`
* Float utilities: `isnear` (comparisons with epsilon), `isfinite`, `exp2f`, `log2f`
* Integer utilities: `round`, `modPositive`, various power-of-2-related utilities
* `advanceBytes`—do byte arithmetic on a pointer of any type
* `exit("error message", ...)`—printf a message to stderr, and exit with a nonzero code
* `loadFile`—just slurp an entire file into memory
* `tokenize`—in-place destructive tokenization, à la `strtok` but stateless
* Logging—to a file, to the console, etc.
* Custom asserts, errors, and warnings

Math, based on design principles from [On Vector Math Libraries](http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/):
* Vectors and matrices
* Affine math (points and affine transforms)
* Rectangles and bounding boxes
* SIMD math using AoSoA
* Half-float from OpenEXR
* Quaternions
* Color space conversions

Random number generation, from [Quick and Easy GPU Random Numbers in D3D11](http://www.reedbeta.com/blog/2013/01/12/quick-and-easy-gpu-random-numbers-in-d3d11/) (although these are on the CPU):
* Xorshift PRNG (simple, fast 32-bit PRNG)
* Wang hash (simple, fast 32-bit hash for seeding Xorshift, etc.)

Todo list (in no particular order):
* Error context stack
* Allow choice of whether to break per individual error/warning/assert
* Ray/line/plane structs
* Primitive intersection tests (e.g. box vs ray, box vs frustum, etc.)
* Primitive clipping (e.g. line vs box, triangle vs box, etc.)
* Splines
* Spherical harmonics
* Perlin noise
* Quadratic equation solver
* Convex hull solver
* Matrix diagonalization and decomposition?
* Complex numbers?
* Projective math?
* Grassmann algebra?
* Filename/path manipulation, à la Python's `os.path`
* Custom memory allocators with logging, leak detection, etc.
* Threading-related stuff?  Or does `std::async` provide enough support?
* Cross-platform, cross-compiler support
