#include "NetworkService.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <iostream>

NetworkService::NetworkService() : sock(-1) {}
NetworkService::~NetworkService() { closeSocket(); }

void NetworkService::openConnection(const std::string& ip, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) throw std::runtime_error("Ошибка создания сокета");
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) throw std::runtime_error("Неверный адрес");
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) throw std::runtime_error("Ошибка подключения");
}

void NetworkService::closeSocket() {
    if (sock >= 0) { close(sock); sock = -1; }
}

void NetworkService::sendText(const std::string& data) {
    if (send(sock, data.c_str(), data.size(), 0) < 0) throw std::runtime_error("Ошибка отправки");
}

std::string NetworkService::receiveText() {
    char buffer[1024] = {0};
    int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) throw std::runtime_error("Сервер разорвал соединение");
    return std::string(buffer, bytes);
}

void NetworkService::sendVectors(const std::vector<std::vector<double>>& vectors) {
    uint32_t count = vectors.size();
    if (send(sock, &count, sizeof(count), 0) < 0) throw std::runtime_error("Ошибка отправки");
    for (const auto& vec : vectors) {
        uint32_t size = vec.size();
        if (send(sock, &size, sizeof(size), 0) < 0) throw std::runtime_error("Ошибка отправки");
        if (send(sock, vec.data(), size * sizeof(double), 0) < 0) throw std::runtime_error("Ошибка отправки");
    }
}

std::vector<double> NetworkService::receiveResults(size_t count) {
    std::vector<double> results(count);
    for (size_t i = 0; i < count; ++i) {
        if (recv(sock, &results[i], sizeof(double), MSG_WAITALL) <= 0) throw std::runtime_error("Ошибка приема");
    }
    return results;
}
