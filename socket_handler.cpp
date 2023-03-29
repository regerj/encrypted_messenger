#include "socket_handler.hpp"

Socket_Handler::Socket_Handler()
{
    int status = STATUS_FAIL;
    WSADATA wsaData;

    status = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(status)
    {
        log(LOG_ERROR, "Failed to startup WSA.");
    }

    logFile.open(".\\server_out.log", std::ofstream::out | std::ofstream::trunc);
    if(!logFile.is_open())
        std::cout << "Failed to create log file." << std::endl;
    log(LOG_INFO, "Created log file.");

    socketHandle = socket(AF_INET, SOCK_STREAM, INTERNET_PROTOCOL);

    if(socketHandle == INVALID_SOCKET)
    {
        log(LOG_ERROR, "Failed to create socket.");
        exit(EXIT_FAILURE);
    }
    log(LOG_INFO, "Created socket.");

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    status = bind(socketHandle, (struct sockaddr *)&address, sizeof(address));

    if(status < 0)
    {
        log(LOG_ERROR, "Failed to bind socket.");
        exit(EXIT_FAILURE);
    }
    log(LOG_INFO, "Binded socket.");
}

inline void Socket_Handler::log(std::string type, std::string message)
{
    logFile << type << ": " << message << std::endl;
}

Socket_Handler::~Socket_Handler()
{
    logFile.close();
    closesocket(socketHandle);
}