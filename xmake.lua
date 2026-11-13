set_languages("c++latest")

add_rules("mode.debug", "mode.release")

add_requires("vcpkg::doctest", { debug = is_mode("debug"), configs = { shared = true } })

add_defines("_UNICODE", "NOMINMAX", "UNICODE")
add_defines(is_mode("debug") and "_DEBUG" or "NDEBUG")

set_warnings("all", "error")
if is_mode("release") then
	set_symbols("debug")
	set_policy("build.optimization.lto", true)
end

add_cxxflags(
	"/diagnostics:caret",
	"/sdl",
	"/Zc:__cplusplus,checkGwOdr,cmath,enumTypes,inline,preprocessor,templateScope,throwingNew",
	{ tools = "cl" }
)
if is_mode("debug") then
	add_cxxflags("/JMC", "/RTC1", { tools = "cl", force = true })
end

target("plastic")
do
	set_kind("shared")
	add_files("src/*.ixx", { public = true })

	set_targetdir(is_mode("debug") and "build/debug" or "build/release")
end

target("plastic_test")
do
	set_kind("binary")
	add_files("tests/*.cpp", "tests/*.ixx")

	add_deps("plastic", { links = false })
	add_packages("vcpkg::doctest")
end
