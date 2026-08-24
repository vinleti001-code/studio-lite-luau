#include "luau_vm.h"

#include <godot_cpp/core/class_db.hpp>

#include "lua.h"
#include "lualib.h"
#include "luacode.h"

using namespace godot;

void LuauVM::_bind_methods() {
    ClassDB::bind_method(D_METHOD("run_script", "source", "chunk_name"), &LuauVM::run_script, DEFVAL("=script"));
    ClassDB::bind_method(D_METHOD("get_output"), &LuauVM::get_output);
    ClassDB::bind_method(D_METHOD("clear_output"), &LuauVM::clear_output);
    ClassDB::bind_method(D_METHOD("get_last_error"), &LuauVM::get_last_error);
    ClassDB::bind_method(D_METHOD("reset_state"), &LuauVM::reset_state);
}

int LuauVM::gd_print(lua_State *L) {
    LuauVM *self = static_cast<LuauVM *>(lua_touserdata(L, lua_upvalueindex(1)));
    int n = lua_gettop(L);
    String line;
    for (int i = 1; i <= n; i++) {
        size_t len = 0;
        const char *s = luaL_tolstring(L, i, &len);
        line += String::utf8(s, (int)len);
        lua_pop(L, 1);
        if (i < n) line += "\t";
    }
    self->output_buffer += line + "\n";
    return 0;
}

LuauVM::LuauVM() {
    reset_state();
}

LuauVM::~LuauVM() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void LuauVM::reset_state() {
    if (L) {
        lua_close(L);
    }
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, gd_print, "print", 1);
    lua_setglobal(L, "print");

    output_buffer = "";
    last_error = "";
}

bool LuauVM::run_script(const String &p_source, const String &p_chunk_name) {
    CharString src_utf8 = p_source.utf8();
    CharString name_utf8 = p_chunk_name.utf8();

    size_t bytecode_size = 0;
    char *bytecode = luau_compile(src_utf8.get_data(), src_utf8.length(), nullptr, &bytecode_size);

    if (!bytecode) {
        last_error = "Compile gagal: bytecode kosong.";
        return false;
    }

    int load_result = luau_load(L, name_utf8.get_data(), bytecode, bytecode_size, 0);
    free(bytecode);

    if (load_result != 0) {
        const char *err = lua_tostring(L, -1);
        last_error = err ? String::utf8(err) : String("Load gagal (unknown).");
        lua_pop(L, 1);
        return false;
    }

    int call_result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (call_result != LUA_OK) {
        const char *err = lua_tostring(L, -1);
        last_error = err ? String::utf8(err) : String("Runtime error (unknown).");
        lua_pop(L, 1);
        return false;
    }

    last_error = "";
    return true;
}

String LuauVM::get_output() const {
    return output_buffer;
}

void LuauVM::clear_output() {
    output_buffer = "";
}

String LuauVM::get_last_error() const {
    return last_error;
}
