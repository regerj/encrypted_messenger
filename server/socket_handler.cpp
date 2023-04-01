#include "socket_handler.hpp"

#pragma comment(lib, "Ws2_32.lib")

Socket_Handler::Socket_Handler()
{
    initWSA();
    initLog();
    initAddr();
    initAndBindSocket();
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
    logFile.close();
    closesocket(socketHandle);
}

void Socket_Handler::sockListen()
{
    int status = STATUS_FAIL;
    status = listen(socketHandle, SOMAXCONN);

    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to listen to socket.");
        throw std::runtime_error("Error: Could not create socket.");
    }
}

void Socket_Handler::initWSA()
{
    int status = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(status)
    {
        log(LOG_ERROR, "Failed to startup WSA.");
        throw std::runtime_error("Error: Could not startup WSA.");
    }
}

void Socket_Handler::initLog()
{
    logFile.open(".\\server_out.log", std::ofstream::out | std::ofstream::trunc);
    if(!logFile.is_open())
    {
        std::cout << "Failed to create log file." << std::endl;
        throw std::runtime_error("Error: Could not create log file.");
    }
    log(LOG_INFO, "Created log file.");
}

void Socket_Handler::initAddr()
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
        throw std::runtime_error("Error: Could not get address info.");
    }
    log(LOG_INFO, "Got address info.");
}

void Socket_Handler::initAndBindSocket()
{
    socketHandle = socket(address->ai_family, address->ai_socktype, address->ai_protocol);

    if(socketHandle == INVALID_SOCKET)
    {
        log(LOG_ERROR, "Failed to create socket.");
        throw std::runtime_error("Error: Could not create socket.");
    }
    log(LOG_INFO, "Created socket.");

    int status = bind(socketHandle, address->ai_addr, address->ai_addrlen);

    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to bind socket.");
        throw std::runtime_error("Error: Could not bind socket.");
    }
    log(LOG_INFO, "Binded socket.");
}

void Socket_Handler::sockAccept()
{
    clientHandle = accept(socketHandle, NULL, NULL);

    if(clientHandle == INVALID_SOCKET)
    {
        log(LOG_ERROR, "Failed to accept connection.");
        throw std::runtime_error("Error: Could not accept connection.");
    }
    log(LOG_INFO, "Accepted connection from client.");
}

void Socket_Handler::echo()
{
    int status;
    int numBytesReceived = 0;
    char receiveBuffer[BUFFER_LEN];
    
    do
    {
        status = recv(clientHandle, receiveBuffer, BUFFER_LEN, 0);

        if(!status)
        {
            log(LOG_WARNING, "Connection closed.");
            break;
        }

        if(status < 0)
        {
            log(LOG_ERROR, "Failed to receive.");
            break;
        }

        log(LOG_INFO, "Received bytes.");
        numBytesReceived = status;

        status = send(clientHandle, receiveBuffer, numBytesReceived, 0);
        if(status == SOCKET_ERROR)
        {
            log(LOG_ERROR, "Failed to send.");
            break;
        }
    } 
    while(1);
}