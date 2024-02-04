add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_project("Own-2d-Engine")

if is_plat("windows") then
    set_languages("cxxlatest")

    -- bruh
    --add_defines("_WIN32_WINNT=0x0601")

    add_cxflags("/Zc:__cplusplus")
    add_cxflags("/Zc:preprocessor")

    add_cxflags("/permissive-")
    add_ldflags("/subsystem:windows")
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
add_requires("zmqpb master")

set_warnings("allextra")

if is_mode("debug") then
    add_defines("DEBUG")
end
if is_mode("release") then
    add_defines("NDEBUG")

    if is_plat("windows") then
        --add_cxflags("/favor:AMD64")
        --add_cxflags("/O2")
        --add_cxflags("/arch:AVX2")
        --add_cxflags("/fp:fast")
        --add_cxflags("/GL")
        --add_cxflags("/Qfast_transcendentals")
        --add_cxflags("/Qpar")
    else
    end
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
    add_headerfiles("include/engine/*.h")
    add_headerfiles("include/content/scripts/*.h")

    add_files("proto/sfg/*.proto", {proto_public = true})

    add_files("src/*.cpp")
    add_files("src/_globals/*.cpp")
    add_files("src/engine/*.cpp")
    add_files("src/content/scripts/*.cpp")

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
