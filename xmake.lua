set_project( "Own-2d-Engine" )

set_version( "0.0.1", { build = "%Y%m%d", soname = true } )

add_rules( "mode.debug", "mode.release", "mode.releasedbg", "mode.minsizerel" )
add_rules( "plugin.compile_commands.autoupdate", { outputdir = ".vscode" } )

if is_plat( "windows" ) then
    set_languages( "cxx20" )

    add_cxflags( "/Zc:__cplusplus" )
    add_cxflags( "/Zc:preprocessor" )

    add_cxflags( "/permissive-" )
else
    set_languages( "c++20" )
end

set_warnings( "allextra" )

-- maybe this helps for the ci?
set_policy( "build.across_targets_in_parallel", false )

add_repositories( "testing-repo https://github.com/SFGrenade/NetworkingHelpers-xmake-repo.git" )

-- preprocessor shortcuts
add_requires( "hedley" )
-- test framework, gonna be added to each test
add_requires( "gtest" )

add_requireconfs( "**", "*.**", { system = false } )
add_requireconfs( "*", { configs = { shared = get_config( "kind" ) == "shared" } } )

--includes( "configuration" )
includes( "engine" )
--includes( "logger" )
--includes( "network_messages" )
--includes( "sdl_wrapper" )
