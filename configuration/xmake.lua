add_requires( "inifile-cpp" )

target( "Configuration" )
    set_kind( "$(kind)" )

    set_default( false )
    set_group( "LIBS" )

    add_packages( "hedley", { public = true } )
    add_packages( "inifile-cpp", { public = true } )

    add_includedirs( "include", { public = true } )

    add_headerfiles( "include/(SFG/Own2dEngine/Configuration/*.h)" )

    add_files( "src/*.cpp" )
    remove_files( "src/main.cpp" )

    after_build( function ( target )
        import( "core.project.config" )
        os.cp( path.join( "$(scriptdir)", "config" ), path.join( "$(projectdir)", target.targetdir( target ) ) )
    end )
    after_install( function ( target )
        import( "core.project.config" )
        os.cp( path.join( "$(scriptdir)", "config" ), target.installdir( target ) )
    end )

target( "Configuration-Test" )
    set_kind( "binary" )

    set_default( false )
    set_group( "TESTS" )

    add_deps( "Configuration", { public = true } )
    add_deps( "Logger", { public = true } )
    add_packages( "gtest", { public = true } )

    add_files( "test/*.cpp" )
