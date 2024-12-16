add_requires( "libsdl" )
add_requires( "libsdl_image" )
add_requires( "libsdl_mixer" )
add_requires( "libsdl_ttf" )
add_requires( "pugixml" )

add_requireconfs( "libsdl", { configs = { sdlmain = false } } )
--add_requireconfs( "libsdl_image", { configs = {} } )
--add_requireconfs( "libsdl_mixer", { configs = {} } )
--add_requireconfs( "libsdl_ttf", { configs = {} } )

target( "Engine" )
    set_kind( "static" )

    set_default( false )
    set_group( "LIBS" )

    add_deps( "Configuration", { public = true } )
    add_deps( "Logger", { public = true } )
    add_deps( "Network-Messages", { public = true } )
    add_deps( "Utils", { public = true } )

    add_packages( "libsdl", { public = true } )
    add_packages( "libsdl_image", { public = true } )
    add_packages( "libsdl_mixer", { public = true } )
    add_packages( "libsdl_ttf", { public = true } )
    add_packages( "pugixml", { public = true } )

    add_includedirs( "include", { public = true } )

    add_headerfiles( "include/(SFG/Own2dEngine/Engine/*.h)" )

    add_files( "src/*.cpp" )
    remove_files( "src/main.cpp" )

target( "Engine-Exe" )
    set_kind( "binary" )

    set_default( true )
    set_group( "EXES" )

    if is_plat( "windows" ) then
        add_ldflags( "/subsystem:windows" )
        add_ldflags( "/entry:mainCRTStartup", { force = true } )
    else
    end

    add_deps( "Engine", { public = true } )

    add_includedirs( "include", { public = true } )

    add_files( "src/main.cpp" )

    after_build( function ( target )
        import( "core.project.config" )
        os.cp( path.join( "$(scriptdir)", "config" ), path.join( "$(projectdir)", target.targetdir( target ) ) )
        os.cp( path.join( "$(scriptdir)", "Resources" ), path.join( "$(projectdir)", target.targetdir( target ) ) )
    end )
    after_install( function ( target )
        import( "core.project.config" )
        os.cp( path.join( "$(scriptdir)", "config" ), target.installdir( target ) )
        os.cp( path.join( "$(scriptdir)", "Resources" ), target.installdir( target ) )
    end )
    after_installcmd( function ( target )
        import( "core.project.config" )
        os.cp( path.join( "$(scriptdir)", "config" ), target.installdir( target ) )
        os.cp( path.join( "$(scriptdir)", "Resources" ), target.installdir( target ) )
    end )

target( "Engine-Test" )
    set_kind( "binary" )

    set_default( false )
    set_group( "TESTS" )

    add_deps( "Engine", { public = true } )
    add_deps( "Logger", { public = true } )
    add_packages( "gtest", { public = true } )

    add_files( "test/*.cpp" )
