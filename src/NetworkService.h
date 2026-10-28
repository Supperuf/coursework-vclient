#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H
#include <string>
#include <vector>
class NetworkService {
private:
    int sock;
public:
    NetworkService();
    ~NetworkService();
    void openConnection(const std::string& ip, int port);
    void closeSocket();
    void sendText(const std::string& data);
    std::string receiveText();
    void sendVectors(const std::vector<std::vector<double>>& vectors);
    std::vector<double> receiveResults(size_t count);
};
#endif
