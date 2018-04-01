#ifndef EXCHANGABLETRANSPORTS_MEMORYREGION_H
#define EXCHANGABLETRANSPORTS_MEMORYREGION_H

#include <memory>
#include "libibverbscpp/libibverbscpp.h"

namespace rdma {
    template<typename T>
    struct RegisteredMemoryRegion {
        std::vector<T> underlying;
        std::unique_ptr<ibv::memoryregion::MemoryRegion> mr;

        RegisteredMemoryRegion(size_t size, rdma::Network &net, std::initializer_list<ibv::AccessFlag> flags) :
                underlying(size),
                mr(net.registerMr(underlying.data(), underlying.size() * sizeof(T), flags)) {}

        std::vector<T> &get() {
            return underlying;
        }

        T *data() {
            return underlying.data();
        }

        typename std::vector<T>::iterator begin() {
            return std::begin(underlying);
        }

        typename std::vector<T>::iterator end() {
            return std::end(underlying);
        }

        ibv::memoryregion::MemoryRegion &rdmaMr() {
            return *mr;
        }

        ibv::memoryregion::RemoteAddress getAddr() {
            return mr->getRemoteAddress();
        }

        ibv::memoryregion::Slice getSlice() {
            return mr->getSlice();
        }

        RegisteredMemoryRegion(const RegisteredMemoryRegion &) = delete;

        RegisteredMemoryRegion &operator=(const RegisteredMemoryRegion &) = delete;

        RegisteredMemoryRegion(RegisteredMemoryRegion &&) = delete;

        RegisteredMemoryRegion &operator=(RegisteredMemoryRegion &&) = delete;

        ~RegisteredMemoryRegion() = default;
    };
}

#endif //EXCHANGABLETRANSPORTS_MEMORYREGION_H
