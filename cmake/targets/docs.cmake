## ======================================================================================
## Collect headers to document
## ======================================================================================
get_target_property(public_include_dir ml::movar INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${public_include_dir}/ml/movar/internal/pipe/*.hpp"
  "${public_include_dir}/ml/movar/internal/type/*.hpp"
  "${public_include_dir}/ml/movar/movar.hpp")
string(REPLACE ";" " " headers_space_separated "${headers}")

## ======================================================================================
## Configure doxygen
## ======================================================================================
set(DOXYFILE_PROJECT_NAME           "MOVAR")
set(DOXYFILE_PROJECT_VERSION        "${PROJECT_VERSION}")
set(DOXYFILE_PROJECT_DESCRIPTION    "${PROJECT_DESCRIPTION}")
set(DOXYFILE_PROJECT_LOGO           "")
set(DOXYFILE_OUTPUT_DIRECTORY       "${CMAKE_CURRENT_BINARY_DIR}/docs")
set(DOXYFILE_STRIP_FROM_PATH        "${public_include_dir}")
set(DOXYFILE_STRIP_FROM_INC_PATH    "${public_include_dir}")
set(DOXYFILE_INPUT                  "${headers_space_separated} pipelines.md lazy-pipelines.md")
# set(DOXYFILE_HTML_HEADER            "${CMAKE_CURRENT_BINARY_DIR}/docs/header.html")
set(DOXYFILE_HTML_FOOTER            "")
set(DOXYFILE_HTML_STYLESHEET        "")
# set(DOXYFILE_HTML_EXTRA_STYLESHEET  
#  "${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css \
#  ${CMAKE_CURRENT_BINARY_DIR}/docs/custom.css")
set(DOXYFILE_HTML_EXTRA_FILES       "")
set(DOXYFILE_TREEVIEW_WIDTH         "335")
set(DOXYFILE_COLORSTYLE_HUE         "209")
set(DOXYFILE_COLORSTYLE_SAT         "255")
set(DOXYFILE_COLORSTYLE_GAMMA       "113")
file(MAKE_DIRECTORY ${DOXYFILE_OUTPUT_DIRECTORY})
configure_file(docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile @ONLY)
configure_file(docs/pipelines.md ${CMAKE_CURRENT_BINARY_DIR}/docs/pipelines.md @ONLY)
configure_file(docs/lazy-pipelines.md ${CMAKE_CURRENT_BINARY_DIR}/docs/lazy-pipelines.md @ONLY)

## ======================================================================================
## Doxygen target
## ======================================================================================
add_custom_target(docs ALL
  COMMAND Doxygen::doxygen ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile
  COMMAND ${mcss_SOURCE_DIR}/documentation/doxygen.py ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile --no-doxygen
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
  DEPENDS ${headers}
  COMMENT "[ml::movar] Generating documentation with Doxygen"
  VERBATIM)