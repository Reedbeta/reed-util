reed-util
=========

Vector/matrix math and other basic utilities.

Instructions:
* `#include <util.h>` for everything
  * Or, `#include <util-basics.h>` for just the stuff in the "Basics" section below
* Link with `util.lib`, or just add `util.vcxproj` to your solution and reference it

Basics:
* `dim()`, `dim_field()`—size of static arrays
* `sizeof_field()`—size of a struct member
* `cassert()`—compile-time assert (like `static_assert` but no separate error message)
* `uint`, `byte`, sized integer types `i8`, `u8` through `i64`, `u64`
* Float constants: `pi`, `epsilon = 1e-6f`, `infinity`, `NaN`
* Generic `swap`, `min`, `max`, `clamp`, `saturate`, `lerp`, `square`
* Float utilities: `isnear` (comparisons with epsilon), `isfinite`, `exp2f`, `log2f`
* Integer utilities: `round`, `modPositive`, various power-of-2-related utilities
* `offset()`—do byte arithmetic on a pointer of any type
* `exit("error message", ...)`—printf a message to stderr, and exit with a nonzero code
* `LoadFile()`—just slurp an entire file into memory
* `tokenize()`—in-place destructive tokenization, à la `strtok` but stateless
* `makeLowercase()`—lowercase a string in-place
* `TextParsingHelper`—help parse a text file into lines and tokens, and issue warnings for syntax errors
* `SerializeHelper`, `DeserializeHelper`—help read/write common types from a byte stream
* Logging—to a file and/or a callback
* Asserts, checks, errors, and warnings—with log messages, callbacks, and debugbreaks

Math, based on design principles from [On Vector Math Libraries](http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/):
* Vectors and matrices in any number of dimensions
* Construction of common transformations
* Functionality for working with affine transformations stored as homogeneous matrices
* Boxes in any number of dimensions
* Quaternions
* Color space conversions
* SIMD math using AOSOA (not well tested)
* Half-float from OpenEXR

Random number generation, from [Quick and Easy GPU Random Numbers in D3D11](http://www.reedbeta.com/blog/2013/01/12/quick-and-easy-gpu-random-numbers-in-d3d11/) (although these are on the CPU):
* Xorshift PRNG (simple, fast 32-bit PRNG)
* Wang hash (simple, fast 32-bit hash for seeding Xorshift, etc.)

Todo list (in no particular order):
* Error context stack
* Allow choice of whether to break per individual error/warning/assert
* Range structs (one-dimensional boxes)?
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
* Dual numbers?
* Filename/path manipulation, à la Python's `os.path`
* Custom memory allocators with logging, leak detection, etc.
* Threading-related stuff?  Or does `std::async` provide enough support?
* Cross-platform, cross-compiler support
* Command-line parameter parsing
* Basic UTF-8 string processing and conversion to/from UTF-16 for Windows
* `rigid` transformation class, represented by quat and translation vector?
* Infinite-far-plane and z-reversed projection matrices
* Containers (arrays, hashtables, intrusive linked lists)
* Allocators (linear allocator, stack allocator, tracked heap allocator?)
