cmake_minimum_required(VERSION 3.7.2)

set(TARGET_NAME bookmark)

# C and C++ sources are freely mixed.
set(SOURCES bookmark.c ../../src/entity.c)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE indep ${GC_LIBRARIES})
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${GC_INCLUDE_DIRS})
target_compile_definitions(${TARGET_NAME} PRIVATE DUMMY)

#if(MSVC)
#  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
#else()
#  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Werror)
#endif()

install(TARGETS ${TARGET_NAME} DESTINATION libexec/w3s/cgi-bin)
