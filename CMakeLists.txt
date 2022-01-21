# CMakeLists.txt for DCTC

cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)

project(DCTC)

set(This dctc)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(HEADERS
    include/dctc/utils.h
    include/equations/quadratic_equation.h
    include/geometric_primitives/geom_common.h
    include/geometric_primitives/point2D.h
    include/geometric_primitives/geometric_2D_object.h
    include/geometric_primitives/vector2D.h
    include/geometric_primitives/line2D.h
    include/geometric_primitives/segment2D.h
    include/geometric_primitives/ray2D.h
    include/geometric_primitives/circle.h
    include/geometric_primitives/rotation.h
    include/geometric_primitives/geom_utils_1.h
    include/geometric_primitives/sector.h
    include/geometric_primitives/convex_hull.h
    include/geometric_primitives/geom2D.h

    include/random/generator.h
    
    include/data_structures/counter.h
    include/data_structures/union_find.h

    include/dctc/utils.h
    include/dctc/utils.tpp

    include/dctc/network_components/nodes/node.h
    include/dctc/network_components/nodes/DO_node.h
    include/dctc/network_components/nodes/DD_node.h
    include/dctc/network_components/nodes/MST_node.h
    include/dctc/network_components/nodes/node_factory.h

    include/dctc/network_components/edges/edge.h
    include/dctc/network_components/graphs/MST_graph.h

    include/dctc/instance/instance.h

    include/dctc/algorithms/coverage/coverage_alg.h
    include/dctc/algorithms/connectivity/utils.h
    include/dctc/algorithms/connectivity/communication_checker.h

    include/dctc/algorithms/connectivity/Tran/MST_node_Tran.h
    include/dctc/algorithms/connectivity/Tran/MST_graph_Tran.h
)

set(SOURCES
    src/utils.cpp
    src/equations/quadratic_equation.cpp
    src/geometric_primitives/geom_common.cpp
    src/geometric_primitives/point2D.cpp
    src/geometric_primitives/geometric_2D_object.cpp
    src/geometric_primitives/vector2D.cpp
    src/geometric_primitives/line2D.cpp
    src/geometric_primitives/segment2D.cpp
    src/geometric_primitives/ray2D.cpp
    src/geometric_primitives/circle.cpp
    src/geometric_primitives/rotation.cpp
    src/geometric_primitives/sector.cpp
    src/geometric_primitives/geom2D_utils.cpp
    src/geometric_primitives/convex_hull.cpp

    src/random/generator.cpp

    src/data_structures/counter.cpp
    src/data_structures/union_find.cpp

    src/dctc/network_components/nodes/node.cpp
    src/dctc/network_components/nodes/DO_node.cpp
    src/dctc/network_components/nodes/DD_node.cpp
    src/dctc/network_components/nodes/MST_node.cpp
    src/dctc/network_components/nodes/node_factory.cpp

    src/dctc/network_components/edges/edge.cpp
    src/dctc/network_components/graphs/MST_graph.cpp

    src/dctc/instance/instance.cpp

    src/dctc/algorithms/coverage/trivial_coverage_alg.cpp
    src/dctc/algorithms/coverage/greedy_coverage_alg.cpp
    src/dctc/algorithms/coverage/strip_coverage_alg.cpp
    src/dctc/algorithms/connectivity/utils.cpp
    src/dctc/algorithms/connectivity/convex_hull.cpp
    src/dctc/algorithms/connectivity/communication_checker.cpp

    src/dctc/algorithms/connectivity/Tran/MST_node_Tran.cpp
    src/dctc/algorithms/connectivity/Tran/MST_graph_Tran.cpp
)

add_library(${This} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${This} PUBLIC include)
target_link_libraries(${This} PRIVATE Threads::Threads)

add_executable(prog src/main.cpp ${SOURCES} ${HEADERS})
target_include_directories(prog PUBLIC include)
target_link_libraries(prog PRIVATE Threads::Threads)

add_subdirectory(third_party)
add_subdirectory(test)
