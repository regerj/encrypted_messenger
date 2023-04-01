#include "socket_handler.hpp"

#pragma comment(lib, "Ws2_32.lib")

Socket_Handler::Socket_Handler()
{
    SOCK_STATUS status = SOCK_GENERAL_FAIL;
    status = initWSA();
    if(status != SOCK_SUCCESS) throw std::runtime_error("Failed to init WSA");

    status = initLog();
    if(status != SOCK_SUCCESS) 
    {
        WSACleanup(); 
        throw std::runtime_error("Failed to init log.");
    }

    status = initAddr();
    if(status != SOCK_SUCCESS)
    {
        WSACleanup();
        throw std::runtime_error("Failed to init log.");
    }

    status = initAndBindSocket();
    if(status != SOCK_SUCCESS)
    {
        WSACleanup();
        freeaddrinfo(address);
        throw std::runtime_error("Failed to init log.");
    }
}

inline void Socket_Handler::log(std::string type, std::string message)
{
    logFile << type << ": " << message << std::endl;
}

Socket_Handler::~Socket_Handler()
{
    log(LOG_INFO, "Cleaning up.");
    WSACleanup();
    freeaddrinfo(address);
    closesocket(socketHandle);
}

SOCK_STATUS Socket_Handler::sockListen()
{
    int status;
    status = listen(socketHandle, SOMAXCONN);

    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to listen to socket.");
        return SOCK_GENERAL_FAIL;
    }
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::initWSA()
{
    int status = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(status)
    {
        log(LOG_ERROR, "Failed to startup WSA.");
        return SOCK_GENERAL_FAIL;
    }
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::initLog()
{
    logFile.open(".\\server_out.log", std::ofstream::out | std::ofstream::trunc);
    if(!logFile.is_open())
    {
        std::cout << "Failed to create log file." << std::endl;
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Created log file.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::initAddr()
{
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    
    int status = getaddrinfo(NULL, DEFUALT_PORT, &hints, &address);
    if(status)
    {
        log(LOG_ERROR, "Failed to get address info.");
        log(LOG_ERROR, std::to_string(status));
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Got address info.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::initAndBindSocket()
{
    socketHandle = socket(address->ai_family, address->ai_socktype, address->ai_protocol);

    if(socketHandle == INVALID_SOCKET)
    {
        log(LOG_ERROR, "Failed to create socket.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Created socket.");

    int status = bind(socketHandle, address->ai_addr, address->ai_addrlen);

    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to bind socket.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Binded socket.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::sockAccept()
{
    clientHandle = accept(socketHandle, NULL, NULL);

    if(clientHandle == INVALID_SOCKET)
    {
        log(LOG_ERROR, "Failed to accept connection.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Accepted connection from client.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::echo()
{
    SOCK_STATUS status = SOCK_GENERAL_FAIL;
    int numBytesReceived = 0;
    char receiveBuffer[BUFFER_LEN];
    
    while(true)
    {
        status = sockReceive(receiveBuffer, numBytesReceived);
        if(status != SOCK_SUCCESS) break;

        status = sockSend(receiveBuffer, numBytesReceived);
        if(status != SOCK_SUCCESS) break;
    }
    return status;
}

SOCK_STATUS Socket_Handler::sockReceive(char * buffer, int &size)
{
    size = recv(clientHandle, buffer, BUFFER_LEN, 0);

    if(size == 0)
    {
        log(LOG_WARNING, "Connection closed.");
        return SOCK_CONNECTION_CLOSED;
    }

    if(size < 0)
    {
        log(LOG_ERROR, "Failed to receive.");
        return SOCK_GENERAL_FAIL;
    }

    log(LOG_INFO, "Received bytes.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::sockSend(char * buffer, int &size)
{
    int status = send(clientHandle, buffer, size, 0);
    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to send.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Sent bytes.");
    return SOCK_SUCCESS;
}