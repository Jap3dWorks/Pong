
set(Boost_LIBRARY_DIRS C:/msys64/mingw64/lib)
set(Boost_INCLUDE_DIR C:/msys64/mingw64/include)

file(GLOB Boost_LIBRARIES
        LIST_DIRECTORIES false RELATIVE ${Boost_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}/libboost_*.dll.a)

message("Link libraries: ${Boost_LIBRARIES}")

set(Boost_FOUND 1)
