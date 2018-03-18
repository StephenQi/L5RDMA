#include <iostream>
#include "RDMANetworking.h"
#include "tcpWrapper.h"

static void exchangeQPNAndConnect(int sock, rdma::Network &network, rdma::QueuePair &queuePair) {
    rdma::Address addr{};
    addr.lid = network.getLID();
    addr.qpn = queuePair.getQPN();
    tcp_write(sock, &addr, sizeof(addr)); // Send own qpn to server
    tcp_read(sock, &addr, sizeof(addr)); // receive qpn
    queuePair.connect(addr);
}

RDMANetworking::RDMANetworking(int sock) :
        completionQueue(network.newCompletionQueuePair()),
        queuePair(network, completionQueue) {
    tcp_setBlocking(sock); // just set the socket to block for our setup.
    exchangeQPNAndConnect(sock, network, queuePair);
}

void receiveAndSetupRmr(int sock, ibv::memoryregion::RemoteAddress &buffer, ibv::memoryregion::RemoteAddress &readPos) {
    RmrInfo rmrInfo{};
    tcp_read(sock, &rmrInfo, sizeof(rmrInfo));
    buffer.rkey = rmrInfo.bufferKey;
    buffer.address = rmrInfo.bufferAddress;
    readPos.rkey = rmrInfo.readPosKey;
    readPos.address = rmrInfo.readPosAddress;
}

void sendRmrInfo(int sock, const ibv::memoryregion::MemoryRegion &buffer, const ibv::memoryregion::MemoryRegion &readPos) {
    RmrInfo rmrInfo{};
    rmrInfo.bufferKey = buffer.getRkey();
    rmrInfo.bufferAddress = reinterpret_cast<uintptr_t>(buffer.getAddr());
    rmrInfo.readPosKey = readPos.getRkey();
    rmrInfo.readPosAddress = reinterpret_cast<uintptr_t>(readPos.getAddr());
    tcp_write(sock, &rmrInfo, sizeof(rmrInfo));
}
