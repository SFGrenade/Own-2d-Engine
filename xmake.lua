set_project( "Own-2d-Engine" )

set_version( "0.0.1", { build = "%Y%m%d", soname = true } )

set_warnings( "allextra" )

add_rules( "mode.debug", "mode.release", "mode.releasedbg", "mode.minsizerel" )

set_languages( "c++20" )

if is_plat( "windows" ) then
  add_cxflags( "/Zc:__cplusplus" )
  add_cxflags( "/Zc:preprocessor" )

  add_cxflags( "/permissive-" )
else
end

-- maybe this helps for the ci?
set_policy( "build.across_targets_in_parallel", false )

-- preprocessor shortcuts
add_requires( "hedley" )
-- test framework, gonna be added to each test
add_requires( "gtest" )

--add_requireconfs( "**", "*.**", { system = false } )
add_requireconfs( "*", { configs = { shared = get_config( "kind" ) == "shared" } } )

includes( "configuration" )
includes( "engine" )
includes( "game" )
includes( "logger" )
includes( "network_messages" )
includes( "resources" )
includes( "utils" )
