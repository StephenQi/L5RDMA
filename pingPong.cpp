#include <iostream>
#include <algorithm>
#include <chrono>
#include <array>
#include <memory>
#include <exchangeableTransports/transports/Transport.h>
#include <exchangeableTransports/transports/TcpTransport.h>
#include <exchangeableTransports/transports/DomainSocketsTransport.h>
#include <exchangeableTransports/transports/SharedMemoryTransport.h>
#include <exchangeableTransports/apps/PingPong.h>
#include <exchangeableTransports/util/bench.h>
#include <fstream>

using namespace std;
using namespace std::string_view_literals;

static const size_t MESSAGES = 1024 * 128; // approx 1s

int main(int argc, char **argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <client / server> <connection string (e.g. 127.0.0.1:1234)>" << endl;
        return -1;
    }
    const auto isClient = argv[1][0] == 'c';
    const auto port = argv[2];

    if (isClient) {
        auto client = Ping(make_transportClient<TcpTransportClient>(), port);
        bench([&]() {
            const auto start = chrono::steady_clock::now();
            for (size_t i = 0; i < MESSAGES; ++i) {
                client.ping();
            }
            const auto end = chrono::steady_clock::now();
            const auto msTaken = chrono::duration<double, milli>(end - start).count();
            const auto sTaken = msTaken / 1000;
            cout << MESSAGES / sTaken << " msg/s" << endl;
        });
    } else {
        auto server = Pong(make_transportServer<TcpTransportServer>(port));
        server.start();
        bench([&]() {
            for (size_t i = 0; i < MESSAGES; ++i) {
                server.pong();
            }
        });
    }

    return 0;
}

