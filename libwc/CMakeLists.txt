
set(TARGET_NAME wc)

set(HEADERS
  big5.h ccs.h ces.h gb18030.h gbk.h hkscs.h hz.h
  iso2022.h jis.h priv.h search.h sjis.h ucs.h
  uhc.h utf8.h viet.h wc.h wc_types.h wtf.h)

set(SOURCES
  big5.c ces.c char_conv.c charset.c combining.c
  conv.c detect.c gb18030.c gbk.c hkscs.c hz.c
  iso2022.c jis.c priv.c putc.c search.c sjis.c
  status.c ucs.c uhc.c utf8.c viet.c wtf.c)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC . ${PROJECT_SOURCE_DIR}/include ${CURSES_INCLUDE_DIRS} ${GC_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PUBLIC ${CURSES_LIBRARIES} ${GC_LIBRARIES})

