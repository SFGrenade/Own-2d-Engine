add_requires( "libsdl" )
add_requires( "libsdl_image" )
add_requires( "libsdl_mixer" )
add_requires( "libsdl_ttf" )
add_requires( "networkinghelper" )
add_requires( "pugixml" )
add_requires( "spdlog" )
add_requires( "simpleini" )

add_requireconfs( "spdlog", { configs = { header_only = true, std_format = false, fmt_external = false, fmt_external_ho = true, noexcept = false } } )

target( "Own-2d-Engine" )
    set_kind( "binary" )

    set_default( true )
    set_group( "EXES" )

    if is_plat( "windows" ) then
        add_ldflags( "/subsystem:windows" )
    else
    end

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
