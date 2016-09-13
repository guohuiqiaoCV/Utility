# Utility
Share some useful data structures and functions.   
Language: C++ 11
Platform: Normally the code is tested under Linux/Mac, but Windows is supposed
be compatible.

# Compiler
Any compiler which supports C++ 11 should work. `GCC` (>=4.8) on Linux and `Microsoft
Visual Studio` on Windows is recommended.  

# Current data structures
- `ConcurrentQueue`: A thread-safe queue, enabling `pop` block when it is empty.
- `MaxConcurrentQueue`: A thread-safe queue with maximum size, enabling both blocking
`push` (when it is full) and `pop` (when it is empty).
