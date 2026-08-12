add_executable(test_ecs test_ecs.cpp)
target_link_libraries(test_ecs PRIVATE BerkEngine)

add_test(NAME ECSTests COMMAND test_ecs)

add_executable(test_runtime test_runtime.cpp)
target_link_libraries(test_runtime PRIVATE BerkEngine)

add_test(NAME RuntimeTests COMMAND test_runtime)
