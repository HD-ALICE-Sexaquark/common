#pragma once

#include <memory>
#include <vector>

namespace E2R {

struct InjectedSexa {
    std::shared_ptr<std::vector<int>> ReactionID;
    std::shared_ptr<std::vector<float>> Px;
    std::shared_ptr<std::vector<float>> Py;
    std::shared_ptr<std::vector<float>> Pz;
    std::shared_ptr<std::vector<float>> Nucleon_Px;
    std::shared_ptr<std::vector<float>> Nucleon_Py;
    std::shared_ptr<std::vector<float>> Nucleon_Pz;
};

}  // namespace E2R
