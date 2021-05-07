cmake_minimum_required(VERSION 3.7.2)

set(TARGET_NAME w3s)

# C and C++ sources are freely mixed.
set(SOURCES anchor.c buffer.c cookie.c display.c
    entity.c etc.c file.c form.c frame.c ftp.c func.c
    history.c istream.c keybind.c linein.c local.c mailcap.c
    main.c map.c menu.c mimehead.c parsetagx.c rc.c
    search.c symbol.c table.c terms.c url.c version.c
    tagtable.c)

set(INCLUDE_DIRS)
set(LIBRARIES indep ${OPENSSL_LIBRARIES} ${CURSES_LIBRARIES})

if (BSD_FOUND)
  add_definitions(-DUSE_LIBBSD)
  list(APPEND INCLUDE_DIRS ${BSD_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${BSD_LIBRARIES})
endif()

if (NEED_LINKING_AGAINST_LIBM)
  list(APPEND LIBRARIES m)
endif()


add_definitions(-DCONF_DIR="${CONF_DIR}")
add_definitions(-DETC_DIR="${ETC_DIR}")
add_definitions(-DRC_DIR="${RC_DIR}")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARIES})
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${INCLUDE_DIRS})
#configure_file(config.h.in config.h)

#if(MSVC)
#  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
#else()
#  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Werror)
#endif()

