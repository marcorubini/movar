# =================================================================================================
# Handle options
# =================================================================================================

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(PROJECT_IS_TOP_LEVEL TRUE)
  set(PROJECT_IS_NOT_TOP_LEVEL FALSE)
else()
  set(PROJECT_IS_TOP_LEVEL FALSE)
  set(PROJECT_IS_NOT_TOP_LEVEL TRUE)
endif()


option(ML_MOVAR_THROWING_CAST        "Throw on failed variant conversions" ON)
option(ML_MOVAR_THROWING_ACCESS      "Throw on failed variant access" ON)
option(ML_MOVAR_INCLUDES_WITH_SYSTEM "Disable all warnings in ml::movar headers" ${PROJECT_IS_NOT_TOP_LEVEL})
option(ML_MOVAR_BUILD_TEST           "Build unit test for ml::movar" ${PROJECT_IS_TOP_LEVEL})
option(ML_MOVAR_BUILD_DOCUMENTATION  "Compile doxygen documentation"  ${PROJECT_IS_TOP_LEVEL})

if (ML_MOVAR_THROWING_CAST)
  set(throwing_cast_status "ON")
else()
  set(throwing_cast_status "OFF")
endif()

if(ML_MOVAR_THROWING_ACCESS)
  set(throwing_access_status "ON")
else()
  set(throwing_access_status "OFF")
endif()

if(ML_MOVAR_INCLUDES_WITH_SYSTEM)
  set(includes_system_status "ON")
else()
  set(includes_system_status "OFF")
endif()

if (ML_MOVAR_BUILD_TEST)
  set(test_status "ON")
else()
  set(test_status "OFF ")
endif()

if (ML_MOVAR_BUILD_DOCUMENTATION)
  set(docs_status "ON")
else()
  set(docs_status "OFF ")
endif()

message(STATUS "[ml::movar] Building ${CMAKE_BUILD_TYPE} mode with: ${CMAKE_CXX_FLAGS}")
message(STATUS "[ml::movar] Throwing conversions : ${throwing_cast_status} (via ML_MOVAR_THROWING_CAST)")
message(STATUS "[ml::movar] Throwing access      : ${throwing_access_status} (via ML_MOVAR_THROWING_ACCESS)")
message(STATUS "[ml::movar] Includes as SYSTEM   : ${includes_system_status} (via ML_MOVAR_INCLUDES_WITH_SYSTEM)")
message(STATUS "[ml::movar] Unit tests           : ${test_status} (via ML_MOVAR_BUILD_TEST)")
message(STATUS "[ml::movar] Docs                 : ${docs_status} (via ML_MOVAR_BUILD_DOCUMENTATION)")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)