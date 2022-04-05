add_executable(driver unit/00-driver.cpp)
target_link_libraries(driver PRIVATE ml::movar doctest::doctest)
target_include_directories(driver PRIVATE unit)
target_compile_definitions(driver PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)