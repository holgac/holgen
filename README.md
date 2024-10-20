# HolGen : Generate your Json/Lua C++ Bindings!

HolGen (pronounced whole-gen) is an MIT licensed code generation tool developed
alongside [a game](https://x.com/armonium_rpg) that I'm working
on, that generates json loaders (using rapidjson) and Lua exporters for your data structures in C++. It has a C-like DSL
allowing you to express your data structure along with your usage intent, and it generates C++ classes and CMake
files.

PS: The project is not yet ready for its prime time; please don't use it in production unless you're OK with frequent
changes. Once the project reaches 1.0, I'll make sure not to make backwards incompatible changes between minor versions.

## Embedding in your project

Compile holgen cli application and put it in your PATH. Holgen is smart enough that it won't overwrite your files if the
contents are identical, so you can integrate it with your build system and regenerate without triggering unnecessary
rebuilds. Or if you're lazy, just create a bash script like this that you execute manually:

```
#!/usr/bin/env bash

holgen --input holgen/schemas --cpp_output libs/schemas --namespace project_r::schemas --cmake project_r_schemas --header ../ProjectRSchemas.h
```

CLI options:

```
-i, --input=DIR           Directory containing holgen schema files
-o, --cpp_output=DIR      Directory to put the C++ output files
-n, --namespace=VALUE     C++ namespace to use for all data structures
-c, --cmake=VALUE         CMake target name to use for the generated static lib
-h, --header=VALUE        Config header to include in all generated headers
```

# Usage

In the examples directory, you can see the schemas under < id >/schemas and the generated output in [example id]
/output. There's a main project that includes all the projects in order to showcase its usage. The project is under
[examples/usage](examples/usage), and each example has an examples/usage/example< id >.cpp file making use of the
generated code.

## Example 1: Classes and enums

Check out [this schema](examples/1/schemas/character.hsc) and its [usage](examples/usage/example1.cpp) to see the basic
syntax, structs, enums and fields. This example uses a single schema file for simplicity, but holgen cli will read all
schema files under the provided directory.

## Example 2: Extended class/enum usage

[This schema](examples/2/schemas/character.hsc) and its [usage](examples/usage/example2.cpp) shows some field
annotations to customize your getters/setters along with defining functions that can be called from C++ or Lua.

## Example 3: Variants (type unions)

If you need to determine the type of a variable in runtime and store different types of objects in the same c++
container, [variants](examples/3/schemas/character.hsc) will be useful! They're more or less like std::variant.
[See how to use it here](examples/usage/example3.cpp).

## Example 4: Data Manager for data oriented projects

If you're building an RPG, you'll have many weapons, armors, characters, and you'll want to be systematic in your usage.
In that case [DataManager](examples/4/schemas/dataManager.hsc) is for you! It recursively loads all your data from a
directory, assigns IDs, creates field based indices so that you can reference your resources using their name in json.
[Check out how it's used](examples/usage/example4.cpp).

## Interested in a feature not yet supported? Let me know!
