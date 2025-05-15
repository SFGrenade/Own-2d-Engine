add_requires( "spdlog" )

add_requireconfs( "spdlog", { configs = { header_only = true, std_format = false, fmt_external = false, fmt_external_ho = true, noexcept = false } } )

target( "Logger" )
  set_kind( "static" )

  set_default( false )
  set_group( "LIBS" )

  add_packages( "hedley", { public = true } )
  add_packages( "spdlog", { public = true } )
  -- probably a good idea to have this part of the fmt package
  if is_plat( "windows" ) then
    add_cxxflags( "/utf-8", { public = true } )
  end

  add_includedirs( "include", { public = true } )

  add_headerfiles( "include/(SFG/Own2dEngine/Logger/*.h)" )

  add_files( "src/*.cpp" )
  remove_files( "src/main.cpp" )

target( "Logger-Test" )
  set_kind( "binary" )

  set_default( false )
  set_group( "TESTS" )

  add_deps( "Logger", { public = true } )
  add_packages( "gtest", { public = true } )

  add_files( "test/*.cpp" )
