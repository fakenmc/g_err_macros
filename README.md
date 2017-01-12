### Summary

A [header file](_g_err_macros.h) containing a set of error handling macros for
C programs using Glib.

### What projects are using these macros?

* [cf4ocl][]
* [cl_ops][]
* Is your project using these macros? Tell us.

### Tests

##### Building the tests executable

This procedure requires [CMake][], [Glib][] and a C compiler.

1. Create a build folder inside the [tests](tests/) folder (e.g. `mkdir build`)
2. CD into the build folder (e.g. `cd build`) and run CMake (e.g. `cmake ..`)
3. Compile the tests file (e.g. `make`)

##### Performing the tests

The tests can be executed in two ways:

1. Run the tests executable directly (e.g. `./tests`)
2. Run the tests with [gtester][] (e.g. `gtester tests`)

### License

[MIT License](LICENSE)

[cf4ocl]: http://fakenmc.github.io/cf4ocl/
[cl_ops]: https://github.com/fakenmc/cl_ops
[CMake]: https://cmake.org/
[Glib]: https://developer.gnome.org/glib/
[gtester]: https://developer.gnome.org/glib/stable/gtester.html
