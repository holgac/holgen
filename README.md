# HolGen : Generate your Json/Lua C++ Bindings!

HolGen (pronounced whole-gen) is an MIT licensed code generation tool developed
alongside [a game](https://x.com/dj_revani) that I'm working
on, that generates json loaders (using rapidjson) and Lua exporters for your data structures in C++. It has a C-like DSL
where you can express your data structure along with your usage intent, and it will generate the C++ classes and CMake
files.

PS: The project is not yet ready for its prime time; please don't use it in production unless you're OK with frqeuent
changes. A Long Term Support version will be published eventually.

## Embedding in your project

Compile holgen cli application and put it in your PATH. Holgen is smart enough that it won't rewrite files if the
contents so you can integrate it with your build system. Or if you're lazy, just create a bash script like this:

```
#!/usr/bin/env bash

holgen holgen/schemas libs/schemas project_r::schemas project_r_schemas ../ProjectRSchemas.h
```

CLI will definitely change over time (getopts support incoming) but currently the usage is:

holgen [schema dir] [output dir] [namespace] [cmake project name] [common header file to include where you can configure error handling]

In the examples directory, you can see the schemas under [example id]/schemas and the generated output in [example id]
/output. There's a main project that includes all the projects in order to showcase its usage. The project is under
[examples/usage](examples/usage), and each example has an examples/usage/[example id].cpp file making use of the
generated code.

## Example 1: Classes and enums

[Check out this example](examples/1/schemas/character.hsc) to see the basic syntax, structs, enums and fields. I used a
single file for simplicity.

## Example 2: Extended class/enum usage

[This](examples/2/schemas/character.hsc) shows some field annotations to customize your getters/setters along with
defining functions that can be called from C++ or Lua.

# Example 3: Variants (type unions)

If you need to determine the type of a variable in runtime and store different types of objects in the same c++
container, [variants](examples/3/schemas/character.hsc) will be useful! They're more or less like std::variant.

# Example 4: Data Manager for data oriented projects

If you're building an RPG, you'll have many weapons, armors, characters, and you'll want to be systematic in your usage.
Then [DataManager](examples/4/schemas/dataManager.hsc) is for you! It recursively loads all your data from a directory,
assigns IDs, creates field based indices so that you can reference your resources using their name in json.
