add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_project("Own-2d-Engine")

if is_plat("windows") then
    set_languages("cxxlatest")

    -- bruh
    add_defines("_WIN32_WINNT=0x0601")

    add_cxflags("/Zc:__cplusplus")
    add_cxflags("/Zc:preprocessor")

    add_cxflags("/permissive-")
else
    set_languages("cxx20")
end

add_requires("cppzmq")
add_requires("fmt")
add_requires("libsdl")
add_requires("libsdl_image")
add_requires("libsdl_mixer")
add_requires("libsdl_ttf")
add_requires("protobuf-cpp")
add_requires("pugixml")
add_requires("spdlog")
add_requireconfs("spdlog", {configs = {fmt_external = true}})
add_defines("SPDLOG_FMT_EXTERNAL")
add_requires("simpleini")
add_requires("zmqpb")

set_warnings("allextra")

if is_mode("debug") then
    add_defines("DEBUG")

    set_symbols("debug")
    if is_plat("windows") then
        add_cxflags("/Zi")
        add_cxflags("/ZI")
    else
    end

    set_optimize("fastest")
end

if is_mode("release") then
    add_defines("NDEBUG")

    if is_plat("windows") then
        add_cxflags("/Qpar")
    else
        add_cxflags("-fomit-frame-pointer")
    end

    set_optimize("fastest")
end

target("Own-2d-Engine")
    set_kind("binary")

    add_packages("cppzmq")
    add_packages("fmt")
    add_packages("libsdl")
    add_packages("libsdl_image")
    add_packages("libsdl_mixer")
    add_packages("libsdl_ttf")
    add_packages("protobuf-cpp", {public = true})
    add_packages("pugixml")
    add_packages("spdlog")
    add_packages("simpleini")
    add_packages("zmqpb")

    add_rules("protobuf.cpp")

    add_includedirs("include")

    add_headerfiles("include/_globals/*.h")
    add_headerfiles("include/_inproc/*.h")
    add_headerfiles("include/graphics/*.h")
    add_headerfiles("include/input/*.h")
    add_headerfiles("include/logic/*.h")
    add_headerfiles("include/logic/CustomScripts/*.h")
    add_headerfiles("include/network/*.h")

    add_files("proto/sfg/*.proto", {proto_public = true})

    add_files("src/*.cpp")
    add_files("src/_globals/*.cpp")
    add_files("src/_inproc/*.cpp")
    add_files("src/graphics/*.cpp")
    add_files("src/input/*.cpp")
    add_files("src/logic/*.cpp")
    add_files("src/logic/CustomScripts/*.cpp")
    add_files("src/network/*.cpp")

    after_build(function (target)
        import("core.project.config")
        os.cp("config", path.join(config.buildir(), config.plat(), config.arch(), config.mode()))
        os.cp("Resources", path.join(config.buildir(), config.plat(), config.arch(), config.mode()))
    end)
    after_install(function (target)
        import("core.project.config")
        os.cp("config", target.installdir())
        os.cp("Resources", target.installdir())
    end)

target("Own-2d-Engine-Server")
    set_kind("binary")

    add_packages("cppzmq")
    add_packages("fmt")
    add_packages("libsdl")
    add_packages("libsdl_image")
    add_packages("libsdl_mixer")
    add_packages("libsdl_ttf")
    add_packages("protobuf-cpp", {public = true})
    add_packages("pugixml")
    add_packages("spdlog")
    add_packages("simpleini")
    add_packages("zmqpb")

    add_rules("protobuf.cpp")

    add_includedirs("include")

    add_headerfiles("include/_server/*.h")
    add_headerfiles("include/_globals/*.h")
    add_headerfiles("include/_inproc/*.h")
    add_headerfiles("include/graphics/*.h")
    add_headerfiles("include/input/*.h")
    add_headerfiles("include/logic/*.h")
    add_headerfiles("include/logic/CustomScripts/*.h")
    add_headerfiles("include/network/*.h")

    add_files("proto/sfg/*.proto", {proto_public = true})

    add_files("src/_server/*.cpp")
    add_files("src/_globals/*.cpp")
    add_files("src/_inproc/*.cpp")
    add_files("src/graphics/*.cpp")
    add_files("src/input/*.cpp")
    add_files("src/logic/*.cpp")
    add_files("src/logic/CustomScripts/*.cpp")
    add_files("src/network/*.cpp")

target("Own-2d-Engine-Test")
    set_kind("binary")

    add_packages("cppzmq")
    add_packages("fmt")
    add_packages("libsdl")
    add_packages("libsdl_image")
    add_packages("libsdl_mixer")
    add_packages("libsdl_ttf")
    add_packages("protobuf-cpp", {public = true})
    add_packages("pugixml")
    add_packages("spdlog")
    add_packages("simpleini")
    add_packages("zmqpb")

    add_rules("protobuf.cpp")

    add_includedirs("include")

    add_headerfiles("include/_globals/*.h")
    add_headerfiles("include/_inproc/*.h")
    add_headerfiles("include/graphics/*.h")
    add_headerfiles("include/input/*.h")
    add_headerfiles("include/logic/*.h")
    add_headerfiles("include/logic/CustomScripts/*.h")
    add_headerfiles("include/network/*.h")

    add_files("proto/sfg/*.proto", {proto_public = true})

    add_files("src/_test/*.cpp")
    add_files("src/_globals/*.cpp")
    add_files("src/_inproc/*.cpp")
    add_files("src/graphics/*.cpp")
    add_files("src/input/*.cpp")
    add_files("src/logic/*.cpp")
    add_files("src/logic/CustomScripts/*.cpp")
    add_files("src/network/*.cpp")
