reed-util
=========

Vector/matrix math and other basic utilities.

Basics:
* `dim()` and `dim_field()` - size of static arrays
* `cassert()`
* `exit("error message")`
* `uint`, `byte`, sized integer types
* `min`, `max`, `clamp`, `saturate`, `lerp`
* `isnear` - comparisons with epsilon
* Logging - to a file, to the console, etc.
* Custom asserts/errors/warnings

Math, based on design ideas from [On Vector Math Libraries](http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/):
* Vectors and matrices
* Affine math (points and affine transforms)
* Rectangles and bounding boxes
* SIMD math using AoSoA
* Half-float from OpenEXR
* Quaternions
* Color space conversions

Random number generation (see [Quick and Easy GPU Random Numbers in D3D11](http://www.reedbeta.com/blog/2013/01/12/quick-and-easy-gpu-random-numbers-in-d3d11/)):
* Xorshift PRNG (simple, fast 32-bit PRNG)
* Wang hash (simple, fast 32-bit hash for seeding Xorshift, etc.)

Todo list:
* Error context stack
* Allow choice of whether to break per individual error/warning/assert
* Splines
* Spherical harmonics
* Perlin noise

Potential future stuff
* Custom memory allocators with logging, leak detection, etc.
* Threading-related stuff?  Or does `std::async` provide enough support?
* Ray/line/plane structs
* Primitive intersection tests
* Matrix diagonalization and decomposition?
* Complex numbers?
* Projective math?
* Grassmann algebra?
