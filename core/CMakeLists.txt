project(tina_core)

set(TINA_CORE_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TINA_CORE_SOVERSION 1)

add_subdirectory(third_party)
add_subdirectory(src)
