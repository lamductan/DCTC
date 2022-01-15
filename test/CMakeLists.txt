set(TESTS
    #"src/equations/test_quadratic_equation.cpp"
    #"src/geometric_primitives/test_geom_common.cpp"
    #"src/geometric_primitives/test_point2D.cpp"
    #"src/geometric_primitives/test_vector2D.cpp"
    #"src/geometric_primitives/test_line2D.cpp"
    #"src/geometric_primitives/test_segment2D.cpp"
    #"src/geometric_primitives/test_ray2D.cpp"
    #"src/geometric_primitives/test_circle.cpp"
    #"src/geometric_primitives/test_rotation.cpp"
    #"src/geometric_primitives/test_sector.cpp"
    #"src/geometric_primitives/test_geom_utils.cpp"

    "src/random/test_generator.cpp"
)

add_executable(tests ${TESTS})
set_target_properties(${This} PROPERTIES
    FOLDER Tests
)

target_link_libraries(tests gtest_main dctc)

add_test(
    NAME ${This}
    COMMAND ${This}
)
