set_project( "Own-2d-Engine" )

set_version( "0.0.1", { build = "%Y%m%d", soname = true } )

add_rules( "mode.debug", "mode.release", "mode.releasedbg", "mode.minsizerel" )
add_rules( "plugin.compile_commands.autoupdate", { outputdir = ".vscode" } )

if is_plat( "windows" ) then
    set_languages( "cxx20" )

    add_cxflags( "/Zc:__cplusplus" )
    add_cxflags( "/Zc:preprocessor" )

    add_cxflags( "/permissive-" )
    add_ldflags( "/subsystem:windows" )
else
    set_languages( "c++20" )
end

set_warnings( "allextra" )

add_repositories( "testing-repo https://github.com/SFGrenade/NetworkingHelpers-xmake-repo.git" )

-- test framework, gonna be added to each test
add_requires( "gtest" )

add_requireconfs( "*", { configs = { shared = get_config( "kind" ) == "shared" } } )
add_requireconfs( "spdlog", { configs = { fmt_external = true } } )

add_requires( "libsdl" )
add_requires( "libsdl_image" )
add_requires( "libsdl_mixer" )
add_requires( "libsdl_ttf" )
add_requires( "networkinghelper" )
add_requires( "pugixml" )
add_requires( "spdlog" )
add_requires( "simpleini" )

target( "Own-2d-Engine" )
    set_kind( "binary" )

    add_packages( "libsdl", { public = true } )
    add_packages( "libsdl_image", { public = true } )
    add_packages( "libsdl_mixer", { public = true } )
    add_packages( "libsdl_ttf", { public = true } )
    add_packages( "networkinghelper", { public = true } )
    add_packages( "pugixml", { public = true } )
    add_packages( "spdlog", { public = true } )
    add_packages( "simpleini", { public = true } )

    add_includedirs( "include", { public = true } )

    add_headerfiles( "include/(_globals/*.h)" )
    add_headerfiles( "include/(engine/*.h)" )
    add_headerfiles( "include/(content/scripts/*.h)" )
    add_headerfiles( "include/(messages/*.h)" )

    add_files( "src/*.cpp" )
    add_files( "src/_globals/*.cpp" )
    add_files( "src/engine/*.cpp" )
    add_files( "src/content/scripts/*.cpp" )
    add_files( "src/messages/*.cpp" )

    after_build( function ( target )
        import( "core.project.config" )
        os.cp( "config", path.join( config.buildir(), config.plat(), config.arch(), config.mode() ) )
        os.cp( "Resources", path.join( config.buildir(), config.plat(), config.arch(), config.mode() ) )
    end )
    after_install( function ( target )
        import( "core.project.config" )
        os.cp( "config", target.installdir() )
        os.cp( "Resources", target.installdir() )
    end )
