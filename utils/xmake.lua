target( "Utils" )
  set_kind( "static" )

  set_default( false )
  set_group( "LIBS" )

  add_includedirs( "include", { public = true } )

  add_headerfiles( "include/(SFG/Own2dEngine/Utils/*.h)" )

  add_files( "src/*.cpp" )
  remove_files( "src/main.cpp" )

target( "Utils-Test" )
  set_kind( "binary" )

  set_default( false )
  set_group( "TESTS" )

  add_deps( "Utils", { public = true } )
  add_deps( "Logger", { public = true } )
  add_packages( "gtest", { public = true } )

  add_files( "test/*.cpp" )
