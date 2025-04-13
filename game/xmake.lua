add_requires( "dr_wav" )
add_requires( "libsamplerate" )
add_requires( "libsdl2" )
add_requires( "libsdl2_image" )
add_requires( "libsdl2_mixer" )
add_requires( "libsdl2_ttf" )
add_requires( "portaudio" )

--add_requireconfs( "dr_wav", { configs = {} } )
--add_requireconfs( "libsamplerate", { configs = {} } )
add_requireconfs( "libsdl2", { configs = { sdlmain = false } } )
--add_requireconfs( "libsdl2_image", { configs = {} } )
--add_requireconfs( "libsdl2_mixer", { configs = {} } )
--add_requireconfs( "libsdl2_ttf", { configs = {} } )
--add_requireconfs( "portaudio", { configs = {} } )

target( "Game" )
    set_kind( "binary" )

    set_default( true )
    set_group( "EXES" )

    if is_plat( "windows" ) then
        --add_ldflags( "/subsystem:windows" )
        --add_ldflags( "/entry:mainCRTStartup", { force = true } )
    else
    end

    add_deps( "Configuration", { public = true } )
    add_deps( "Logger", { public = true } )
    add_deps( "Network-Messages", { public = true } )
    add_deps( "Utils", { public = true } )

    add_packages( "dr_wav", { public = true } )
    add_packages( "libsamplerate", { public = true } )
    add_packages( "libsdl2", { public = true } )
    add_packages( "libsdl2_image", { public = true } )
    add_packages( "libsdl2_mixer", { public = true } )
    add_packages( "libsdl2_ttf", { public = true } )
    add_packages( "portaudio", { public = true } )

    add_includedirs( "include", { public = true } )

    add_headerfiles( "include/(SFG/Own2dEngine/Game/ui/*.h)" )
    add_headerfiles( "include/(SFG/Own2dEngine/Game/*.h)" )

    add_files( "src/ui/*.cpp" )
    add_files( "src/*.cpp" )

    after_build( function ( target )
        import( "core.project.config" )
        --os.cp( path.join( "$(scriptdir)", "config" ), path.join( "$(projectdir)", target.targetdir( target ) ) )
        os.cp( path.join( "$(scriptdir)", "Resources" ), path.join( "$(projectdir)", target.targetdir( target ) ) )
    end )
    after_install( function ( target )
        import( "core.project.config" )
        --os.cp( path.join( "$(scriptdir)", "config" ), target.installdir( target ) )
        os.cp( path.join( "$(scriptdir)", "Resources" ), target.installdir( target ) )
    end )
    after_installcmd( function ( target )
        import( "core.project.config" )
        --os.cp( path.join( "$(scriptdir)", "config" ), target.installdir( target ) )
        os.cp( path.join( "$(scriptdir)", "Resources" ), target.installdir( target ) )
    end )
