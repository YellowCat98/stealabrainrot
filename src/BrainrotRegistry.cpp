#include <BrainrotRegistry.hpp>

BrainrotRegistry* BrainrotRegistry::get() {
    static BrainrotRegistry* instance;
    if (!instance) instance = new BrainrotRegistry();
    return instance;
}