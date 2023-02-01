#ifndef LV_SWAP_CHAIN_H
#define LV_SWAP_CHAIN_H

#ifdef LV_BACKEND_VULKAN

#include "vulkan/lvcore/core/swap_chain.hpp"

#define g_swapChain g_vulkan_swapChain

namespace lv {

typedef Vulkan_SwapChainCreateInfo SwapChainCreateInfo;
typedef Vulkan_SwapChain SwapChain;

} //namespace lv

#endif

#ifdef LV_BACKEND_METAL

#include "metal/lvcore/core/swap_chain.hpp"

#define g_swapChain g_metal_swapChain

namespace lv {

typedef Metal_SwapChainCreateInfo SwapChainCreateInfo;
typedef Metal_SwapChain SwapChain;

} //namespace lv

#endif

#endif
