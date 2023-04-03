#pragma once

#include "..\inc\socket_handler.hpp"

class Server_Socket: public Socket_Handler
{
    private:
        SOCKET clientHandle = INVALID_SOCKET;

        SOCK_STATUS initAndBindSocket();
        SOCK_STATUS initAddr();
        SOCK_STATUS sockReceive(char *, int &);
        SOCK_STATUS sockSend(char *, int &);
    protected:
    public:
        Server_Socket();
        ~Server_Socket();
        SOCK_STATUS sockListen();
        SOCK_STATUS sockAccept();
        SOCK_STATUS echo();
};