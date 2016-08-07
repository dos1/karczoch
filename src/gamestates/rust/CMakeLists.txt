project(libsuperderpy-gamestate-rust NONE)

find_package(rustc)
find_package(rustdoc)
find_package(cargo)
include(Rust)

#cargo_dependency(cargo
#                 PACKAGE_NAMES url
#		 PACKAGE_VERSIONS =0.2.31)

#set(RUSTC_FLAGS -L ${CMAKE_BINARY_DIR}/lib -L ${CMAKE_BINARY_DIR}/cargo/target/debug/deps)
#set(RUSTDOC_FLAGS -L ${CMAKE_BINARY_DIR}/lib -L ${CMAKE_BINARY_DIR}/cargo/target/debug/deps)

# Get the dependencies of all the crates
#get_rust_deps(src/lib.rs TESTLIB_DEPS)
#get_rust_deps(examples/example1.rs EXAMPLE1_DEPS)
#get_rust_deps(examples/example2.rs EXAMPLE2_DEPS)


#register_gamestate("rust")
