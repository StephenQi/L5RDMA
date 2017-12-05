#include <exchangeableTransports/util/domainSocketsWrapper.h>
#include <sys/un.h>
#include "DomainSocketsTransport.h"

DomainSocketsTransportServer::DomainSocketsTransportServer(std::string_view file) :
        initialSocket(domain_socket()),
        file(file) {
    domain_bind(initialSocket, file);
    domain_listen(initialSocket);
}

DomainSocketsTransportServer::~DomainSocketsTransportServer() {
    domain_close(initialSocket);

    if (communicationSocket != -1) {
        domain_close(communicationSocket);
    }
}

void DomainSocketsTransportServer::accept_impl() {
    sockaddr_un remote{};
    communicationSocket = domain_accept(initialSocket, remote);
}

void DomainSocketsTransportServer::write_impl(const uint8_t *data, size_t size) {
    domain_write(communicationSocket, data, size);
}

void DomainSocketsTransportServer::read_impl(uint8_t *buffer, size_t size) {
    domain_read(communicationSocket, buffer, size);
}

Buffer DomainSocketsTransportServer::getBuffer_impl(size_t) {
    throw std::runtime_error{"not implemented!"}; // TODO
}

void DomainSocketsTransportServer::write_impl(Buffer) {
    throw std::runtime_error{"not implemented!"}; // TODO
}

Buffer DomainSocketsTransportServer::read_impl(size_t) {
    throw std::runtime_error{"not implemented!"}; // TODO
}

void DomainSocketsTransportServer::markAsRead_impl(Buffer) {
    throw std::runtime_error{"not implemented!"}; // TODO
}

DomainSocketsTransportClient::DomainSocketsTransportClient() : socket(domain_socket()) {}

DomainSocketsTransportClient::~DomainSocketsTransportClient() {
    domain_close(socket);
}

void DomainSocketsTransportClient::connect_impl(std::string_view file) {
    domain_connect(socket, file);
    domain_unlink(file);
}

void DomainSocketsTransportClient::write_impl(const uint8_t *data, size_t size) {
    domain_write(socket, data, size);
}

void DomainSocketsTransportClient::read_impl(uint8_t *buffer, size_t size) {
    domain_read(socket, buffer, size);
}

Buffer DomainSocketsTransportClient::getBuffer_impl(size_t) {
    throw std::runtime_error{"not implemented!"}; // TODO
}

void DomainSocketsTransportClient::write_impl(Buffer) {
    throw std::runtime_error{"not implemented!"}; // TODO
}

Buffer DomainSocketsTransportClient::read_impl(size_t) {
    throw std::runtime_error{"not implemented!"}; // TODO
}

void DomainSocketsTransportClient::markAsRead_impl(Buffer) {
    throw std::runtime_error{"not implemented!"}; // TODO
}
