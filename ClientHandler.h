#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H


using namespace std;
class ClientHandler {

public:
    virtual void handleClient(int socketId) = 0;
    virtual ~ClientHandler() = default;

};
#endif 