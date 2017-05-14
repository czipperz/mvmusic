cmake_minimum_required(VERSION 2.8)
project(MVMUSIC)

find_package(Boost 1.40.0 COMPONENTS system filesystem REQUIRED)

set(files
  ${MVMUSIC_SOURCE_DIR}/src/conf.cc
  ${MVMUSIC_SOURCE_DIR}/src/consume_path.cc
  ${MVMUSIC_SOURCE_DIR}/src/consumer.cc
  ${MVMUSIC_SOURCE_DIR}/src/env.cc
  ${MVMUSIC_SOURCE_DIR}/src/globals.cc
  ${MVMUSIC_SOURCE_DIR}/src/mpd.cc
  ${MVMUSIC_SOURCE_DIR}/src/parse_args.cc
  ${MVMUSIC_SOURCE_DIR}/src/usage.cc
  )

add_executable(mvmusic ${files}
  ${MVMUSIC_SOURCE_DIR}/src/main.cc)

add_executable(mvmusic_tests ${files}
  ${MVMUSIC_SOURCE_DIR}/test/test_consume_path.cc
  ${MVMUSIC_SOURCE_DIR}/test/test_main.cc)

set(LIBS ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY}
         mpdclient tag)
target_link_libraries(mvmusic PRIVATE ${LIBS})
target_link_libraries(mvmusic_tests PRIVATE ${LIBS})

add_definitions("-std=c++11" "-Wpedantic" "-Wall" "-Wextra"
  "-Wno-variadic-macros" "-Wno-long-long" "-pthread"
  "-Werror=implicit-function-declaration"
  "-Werror=incompatible-pointer-types")
