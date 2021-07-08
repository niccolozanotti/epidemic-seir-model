
## adding a mutual main for the tests
add_library(test_main OBJECT main.cpp)
target_include_directories(test_main PUBLIC ../extern/doctest/doctest)


add_executable(seir.t test1.cpp $<TARGET_OBJECTS:test_main>)

target_include_directories(seir.t PUBLIC ../include/seir
                                          ../extern/doctest/doctest)
target_compile_features(seir.t PRIVATE cxx_std_17)
target_compile_options(seir.t PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(seir.t PRIVATE -fsanitize=address)
target_link_libraries(seir.t PRIVATE numeric)
add_test(NAME seir.t COMMAND seir.t)

add_executable(sim.t test2.cpp $<TARGET_OBJECTS:test_main>)

target_include_directories(sim.t PUBLIC ../include/simulation
                                        ../extern/doctest/doctest)
target_compile_features(sim.t PRIVATE cxx_std_17)
target_compile_options(sim.t PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(sim.t PRIVATE -fsanitize=address)
target_link_libraries(sim.t PRIVATE     simulation
                                        mobility_model
                                        person
                                        world
                                        cluster
                                        group
                                        rectangle
                                        location
                                        position
                                        random)
add_test(NAME sim.t COMMAND sim.t )

add_executable(test3 test3.cpp $<TARGET_OBJECTS:test_main>)

target_include_directories(test3 PUBLIC ../include/simulation
        ../extern/doctest/doctest)
target_compile_features(test3 PRIVATE cxx_std_17)
target_compile_options(test3 PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(test3 PRIVATE -fsanitize=address)
target_link_libraries(test3 PRIVATE     simulation
                                        mobility_model
                                        person
                                        world
                                        cluster
                                        group
                                        rectangle
                                        location
                                        position
                                        random)
add_test(NAME test3 COMMAND test3 )

