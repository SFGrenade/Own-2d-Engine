add_rules("mode.debug", "mode.release")

set_project("Own-2d-Engine")

if is_plat("windows") then
    set_languages("cxxlatest")

    -- bruh
    add_cxflags("/Zc:__cplusplus")
    add_cxflags("/Zc:preprocessor")
else
    set_languages("cxx20")
end

add_requires("fmt")
add_requires("libsdl")
add_requires("libsdl_image")
add_requires("libsdl_mixer")
add_requires("libsdl_ttf")
add_requires("spdlog")
add_requireconfs("spdlog", {configs = {fmt_external = true}})

set_warnings("allextra")

if is_mode("debug") then
    add_defines("DEBUG")

    set_symbols("debug")

    set_optimize("fastest")
end

if is_mode("release") then
    add_defines("NDEBUG")

    add_cxflags("-fomit-frame-pointer")

    set_optimize("fastest")
end

target("Own-2d-Engine")
    set_kind("binary")

    add_files("src/**.cpp")
    add_headerfiles("src/**.h")

    add_packages("fmt")
    add_packages("libsdl")
    add_packages("libsdl_image")
    add_packages("libsdl_mixer")
    add_packages("libsdl_ttf")
    add_packages("spdlog")

    after_build(function (target)
        import("core.project.config")
        os.cp("Resources", path.join(config.buildir(), config.plat(), config.arch(), config.mode()))
    end)
    after_install(function (target)
        import("core.project.config")
        os.cp("Resources", target.installdir())
    end)
