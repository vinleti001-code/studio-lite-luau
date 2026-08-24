#ifndef LUAU_REGISTER_TYPES_H
#define LUAU_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

void initialize_luau_module(godot::ModuleInitializationLevel p_level);
void uninitialize_luau_module(godot::ModuleInitializationLevel p_level);

#endif
