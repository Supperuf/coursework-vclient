#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstring>
#include "VectorLoader.h"
#include "NetworkService.h"
#include "Sha1Auth.h"

void printHelp() {
    std::cout << "Использование: vclient -a <IP> [-p <PORT>] -i <input.txt> -o <output.txt> [-c <config>]\n";
}

int main(int argc, char** argv) {
    std::string address, inFile, outFile, configFile = "~/.config/vclient.conf";
    int port = 33333;

    if (argc == 1) { printHelp(); return 0; }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) { printHelp(); return 0; }
        else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) address = argv[++i];
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) port = std::stoi(argv[++i]);
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) inFile = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) outFile = argv[++i];
    }

    if (address.empty() || inFile.empty() || outFile.empty()) {
        std::cerr << "Ошибка: не указаны обязательные параметры.\n";
        return 1;
    }

    try {
        auto vectors = VectorLoader::load(inFile);
        NetworkService net;
        net.openConnection(address, port);

        net.sendText("LOGIN\n");
        std::string salt = net.receiveText();
        std::string password = "mypassword"; 
        std::string hash = Sha1Auth::computeSHA1(salt, password);
        net.sendText(hash + "\n");

        std::string response = net.receiveText();
        if (response.find("OK") == std::string::npos) throw std::runtime_error("Ошибка аутентификации");

        net.sendVectors(vectors);
        auto results = net.receiveResults(vectors.size());

        std::ofstream out(outFile);
        if (!out.is_open()) throw std::runtime_error("Не удалось открыть файл результатов");
        out << results.size() << "\n";
        for (double res : results) out << res << " ";
        out << "\n";
        net.closeSocket();
        std::cout << "Успешно завершено.\n";
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
