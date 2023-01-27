#include "lvcore/core/shader_module.hpp"

#include <string>

namespace lv {

dispatch_data_t convertStringToDispatchData(std::string str) {
    return dispatch_data_create((void*)str.c_str(), str.size() * sizeof(char), nullptr, nullptr);
}

} //namespace lv
