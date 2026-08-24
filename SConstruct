#!/usr/bin/env python
import os

env = SConscript("godot-cpp/SConstruct")

luau_dirs = [
    "luau/Ast/include",
    "luau/Common/include",
    "luau/Bytecode/include",
    "luau/Compiler/include",
    "luau/VM/include",
]

env.Append(CPPPATH=[d for d in luau_dirs if os.path.isdir(d)])
env.Append(CPPPATH=["src/"])

if env.get("is_msvc", False):
    env.Append(CXXFLAGS=["/std:c++17"])
else:
    env.Append(CXXFLAGS=["-std=c++17"])

sources = Glob("src/*.cpp")

luau_src_dirs = ["luau/Ast/src", "luau/Common/src", "luau/Bytecode/src", "luau/Compiler/src", "luau/VM/src"]
for d in luau_src_dirs:
    if os.path.isdir(d):
        sources += Glob(d + "/*.cpp")

if not any(os.path.isdir(d) for d in luau_src_dirs):
    print("!! Folder luau/ belum diisi source Luau.")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "bin/lib/libluauext.{}.{}.framework/libluauext.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "bin/lib/libluauext{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
