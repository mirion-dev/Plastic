add_rules("mode.debug", "mode.release")

set_languages("c++latest")
set_warnings("all", "error")

add_requires("vcpkg::doctest", {debug = is_mode("debug"), configs = {shared = true}})

add_defines("NOMINMAX", "UNICODE", "_UNICODE", "_CONSOLE")

add_cxxflags("/JMC", "/sdl", "/permissive-", "/Zc:preprocessor", {tools = "cl", force = true})

if is_mode("release") then
    set_symbols("debug")
    set_policy("build.optimization.lto", true)
    add_cxxflags("/Oi", "/Gy", {tools = "cl"})
end

target("plastic")
    set_kind("shared")
    add_files("src/*.ixx", {public = true})

    set_targetdir(is_mode("debug") and "build/debug" or "build/release")

target("plastic_test")
    set_kind("binary")
    add_files("tests/*.cpp", "tests/*.ixx")
    add_deps("plastic", {links = false})
    add_packages("vcpkg::doctest")

    set_targetdir(is_mode("debug") and "build/tests/debug" or "build/tests/release")
