
set(TARGET_NAME indep)

set(SOURCES hash.c indep.c parsetag.c regex.c Str.c textlist.c)

add_library(${TARGET_NAME} STATIC ${SOURCES})

set(INCLUDE_DIRS ${CURSES_INCLUDE_DIRS} ${GC_INCLUDE_DIRS})
set(LIBRARIES ${CURSES_LIBRARIES} ${GC_LIBRARIES})

if (BSD_FOUND)
  add_definitions(-DUSE_LIBBSD)
  list(APPEND INCLUDE_DIRS ${BSD_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${BSD_LIBRARIES})
endif()

add_definitions(-DAUXBIN_DIR="${AUXBIN_DIR}")
add_definitions(-DCGIBIN_DIR="${CGIBIN_DIR}")
add_definitions(-DCONF_DIR="${CONF_DIR}")
add_definitions(-DETC_DIR="${ETC_DIR}")
add_definitions(-DHELP_DIR="${HELP_DIR}")

target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PUBLIC ${LIBRARIES})

