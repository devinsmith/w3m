cmake_minimum_required(VERSION 3.7.2)

set(TARGET_NAME imgdisplay)

find_package(X11 REQUIRED)
pkg_check_modules(IMLIB2 REQUIRED imlib2)

# C and C++ sources are freely mixed.
set(SOURCES img-display.c x11-imlib2.c)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE ${IMLIB2_LIBRARIES} ${X11_X11_LIB})
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${IMLIB2_INCLUDE_DIRS} ${X11_INCLUDE_DIR})

#if(MSVC)
#  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
#else()
#  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Werror)
#endif()

install(TARGETS ${TARGET_NAME} DESTINATION libexec/w3s)
