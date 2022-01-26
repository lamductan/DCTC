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

    #"src/random/test_generator.cpp"

    #"src/dctc/test_utils.cpp"

    #"src/dctc/network_components/nodes/test_node.cpp"
    #"src/dctc/network_components/nodes/test_DD_node.cpp"
    #"src/dctc/network_components/nodes/test_DO_node.cpp"
    #"src/dctc/network_components/edges/test_edge.cpp"

    #"src/dctc/instance/test_instance.cpp"

    #"src/dctc/algorithms/connectivity/test_convex_hull.cpp"
    #"src/dctc/algorithms/connectivity/test_utils.cpp"
    #"src/dctc/algorithms/connectivity/Tran/test_MST_graph_Tran.cpp"
    #"src/dctc/algorithms/connectivity/Aschner/test_MST_graph_Aschner.cpp"

    "src/dctc/algorithms/connectivity/relays/test_relays_utils.cpp"
    #"src/dctc/algorithms/connectivity/relays/test_simple_relays_alg.cpp"
    #"src/dctc/algorithms/connectivity/relays/test_relays_MST_graph_Tran.cpp"
    #"src/dctc/algorithms/connectivity/relays/test_relays_MST_graph_Aschner.cpp"
    #"src/dctc/algorithms/connectivity/relays/test_relays_MST_graph_Aschner_and_Tran.cpp"
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
