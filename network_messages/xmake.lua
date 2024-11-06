add_requires( "protobuf-cpp" )
add_requires( "zmqpb" )

add_requireconfs( "protobuf-cpp", { configs = { shared = false } } )
add_requireconfs( "zmqpb", { debug = true, configs = { shared = false } } )

target( "Network-Messages" )
    set_kind( "static" )

    set_default( false )
    set_group( "LIBS" )

    add_packages( "hedley", { public = true } )
    add_packages( "protobuf-cpp", { public = true } )
    add_packages( "zmqpb", { public = true } )

    add_rules( "protobuf.cpp" )
    add_files( "messages/**.proto", { proto_public = true, proto_rootdir = path.join( "network_messages", "messages" ) } )

target( "Network-Messages-Test" )
    set_kind( "binary" )

    set_default( false )
    set_group( "TESTS" )

    add_deps( "Network-Messages", { public = true } )
    add_deps( "Logger", { public = true } )
    add_packages( "gtest", { public = true } )

    add_files( "test/*.cpp" )
