#include <vector>
#include <string>
#include <iostream>

#include "graph.h"
#include "instance.h"


const std::string DATA_DIR = "samples/";

void test2() {
    std::string instance_path = DATA_DIR + "instance.txt";
    std::string mst_graph_path = DATA_DIR + "mst_graph.txt";
    std::string relays_mst_graph_Aschner_path = DATA_DIR + "relays_mst_graph_Aschner.txt";
    std::string relays_mst_graph_Tran_path = DATA_DIR + "relays_mst_graph_Tran.txt";
    std::string relays_mst_graph_Lam_LEF_path = DATA_DIR + "relays_mst_graph_Lam_LEF.txt";

    std::string mst_graph_img_path = DATA_DIR + "mst_graph.png";
    std::string relays_mst_graph_Aschner_img_path = DATA_DIR + "relays_mst_graph_Aschner.png";
    std::string relays_mst_graph_Tran_img_path = DATA_DIR + "relays_mst_graph_Tran.png";
    std::string relays_mst_graph_Lam_LEF_img_path = DATA_DIR + "relays_mst_graph_Lam_LEF.png";

    Instance instance = Instance::load(instance_path);
    std::vector<std::pair<long double, long double>> targets = instance.getTargets();

    /* draw mst graph */
    Graph mst_graph = Graph::load(mst_graph_path);
    mst_graph.draw_targets(targets);
    mst_graph.draw(mst_graph_img_path);

    /* draw relays mst graph Aschner */
    Graph relays_mst_graph_Aschner = Graph::load(relays_mst_graph_Aschner_path);
    relays_mst_graph_Aschner.draw_targets(targets);
    relays_mst_graph_Aschner.draw(relays_mst_graph_Aschner_img_path);

    /* draw relays mst graph Tran */
    Graph relays_mst_graph_Tran = Graph::load(relays_mst_graph_Tran_path);
    relays_mst_graph_Tran.draw_targets(targets);
    relays_mst_graph_Tran.draw(relays_mst_graph_Tran_img_path);

    /* draw relays mst graph Lam_LEF */
    Graph relays_mst_graph_Lam_LEF = Graph::load(relays_mst_graph_Lam_LEF_path);
    relays_mst_graph_Lam_LEF.draw_targets(targets);
    relays_mst_graph_Lam_LEF.draw(relays_mst_graph_Lam_LEF_img_path);
}

int main() {
    test2();
}
