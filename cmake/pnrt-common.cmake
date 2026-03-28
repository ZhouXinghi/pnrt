function(pnrt_add_test TARGET_NAME)
  set(options "")
  set(oneValueArgs "")
  set(multiValueArgs SRCS DEPS)
  cmake_parse_arguments(PARSED_ARGS "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})
  add_executable(${TARGET_NAME} ${PARSED_ARGS_SRCS})
  target_link_libraries(${TARGET_NAME} PRIVATE ${PARSED_ARGS_DEPS} GTest::GTest
                                               GTest::Main)
endfunction(pnrt_add_test TARGET_NAME)

function(pnrt_add_library TARGET_NAME)
  set(options SHARED STATIC)
  set(oneValueArgs "")
  set(multiValueArgs SRCS DEPS INC)
  cmake_parse_arguments(PARSED_ARGS "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  if(${PARSED_ARGS_SHARED})
    set(LIB_TYPE SHARED)
  elseif(${PARSED_ARGS_STATIC})
    set(LIB_TYPE STATIC)
  endif()
  add_library(${TARGET_NAME} ${LIB_TYPE} ${PARSED_ARGS_SRCS})
  target_link_libraries(${TARGET_NAME} PRIVATE ${PARSED_ARGS_DEPS})
endfunction(pnrt_add_library TARGET_NAME)
