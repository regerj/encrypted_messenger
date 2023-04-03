#pragma once

#include <iostream>
#include "lib\socket_handler.hpp"

class Client_Socket: public Socket_Handler
{
    private:
        SOCK_STATUS sockReceive(std::string &, int &);
        SOCK_STATUS sockSend(std::string, int &);
        SOCK_STATUS initAddr();
        SOCK_STATUS initAndConnectSocket();
    protected:
    public:
        Client_Socket();
        ~Client_Socket();
        SOCK_STATUS sockShutdown(int);
        SOCK_STATUS tempComms();
};