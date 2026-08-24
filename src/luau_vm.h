#ifndef LUAU_VM_GD_H
#define LUAU_VM_GD_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>

struct lua_State;

namespace godot {

class LuauVM : public RefCounted {
    GDCLASS(LuauVM, RefCounted)

private:
    lua_State *L = nullptr;
    String output_buffer;
    String last_error;

    static int gd_print(lua_State *L);

protected:
    static void _bind_methods();

public:
    LuauVM();
    ~LuauVM();

    void reset_state();
    bool run_script(const String &p_source, const String &p_chunk_name);
    String get_output() const;
    void clear_output();
    String get_last_error() const;
};

} // namespace godot

#endif
