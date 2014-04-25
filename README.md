reed-util
=========

Vector/matrix math and other basic utilities.

Basics:
* `dim()` and `dim_field()` - size of static arrays
* `cassert()`
* `exit("error message")`
* `uint`, `byte`, sized integer types
* `min`, `max`, `clamp`, `saturate`, `lerp`
* `isnear` - comparisons with epsilon - DONE
* Logging - to a file, to the console, etc.
* Custom asserts/errors/warnings

Math, based on design ideas in [On Vector Math Libraries](http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/):
* Vectors and matrices
* Affine math (points and affine transforms)
* Rectangles and bounding boxes
* SIMD math using AoSoA
* Half-float from OpenEXR
* Quaternions
* Color space conversions

Random number generation:
* Xorshift PRNG (simple, fast 32-bit PRNG)
* Wang hash (simple, fast 32-bit hash for seeding Xorshift, etc.)

Todo:
* Matrix diagonalization and decomposition
* Error context stack
* Allow choice of whether to break per individual error/warning/assert
* Custom memory allocators with logging, leak detection, etc.
* Threading-related stuff?  Or does `std::async` provide enough support?
* Splines
* Spherical harmonics
* Perlin noise
* Complex numbers?
* Projective math?
* Grassmann algebra?
* Ray/line/plane structs?
* Primitive intersection tests?
