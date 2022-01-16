#ifndef DCTC_ALGORITHMS_COVERAGE_COVERAGE_ALG_H_
#define DCTC_ALGORITHMS_COVERAGE_COVERAGE_ALG_H_

#include <vector>

#include "dctc/instance/instance.h"
#include "dctc/network_components/nodes/node.h"

std::vector<Node*> trivialCoverageAlg(const Instance* instance);
std::vector<Node*> greedyCoverageAlg(const Instance* instance);
std::vector<Node*> stripCoverageAlg(const Instance* instance);

#endif //DCTC_ALGORITHMS_COVERAGE_COVERAGE_ALG_H_