#include "server_socket.hpp"

Server_Socket::Server_Socket()
{
    SOCK_STATUS status = SOCK_GENERAL_FAIL;
    status = initAddr();
    if(status != SOCK_SUCCESS)
    {
        WSACleanup();
        throw std::runtime_error("Failed to init address.");
    }

    status = initAndBindSocket();
    if(status != SOCK_SUCCESS)
    {
        WSACleanup();
        freeaddrinfo(address);
        throw std::runtime_error("Failed to init and bind to socket.");
    }
}

Server_Socket::~Server_Socket()
{

}

SOCK_STATUS Server_Socket::initAndBindSocket()
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
        closesocket(socketHandle);
        socketHandle = INVALID_SOCKET;
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Binded socket.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Server_Socket::initAddr()
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

SOCK_STATUS Server_Socket::sockListen()
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

SOCK_STATUS Server_Socket::sockAccept()
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

SOCK_STATUS Server_Socket::echo()
{
    SOCK_STATUS status = SOCK_GENERAL_FAIL;
    std::string receiveBuffer;
    
    while(true)
    {
        status = sockReceive(receiveBuffer);
        if(status != SOCK_SUCCESS) break;

        status = sockSend(receiveBuffer);
        if(status != SOCK_SUCCESS) break;
    }
    return status;
}

SOCK_STATUS Server_Socket::sockReceive(std::string &buffer)
{
    int size = 0;
    char cStringBuffer[BUFFER_LEN];

    size = recv(clientHandle, cStringBuffer, BUFFER_LEN, 0);

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

    cStringBuffer[size] = '\0';
    buffer = std::string(cStringBuffer);
    log(LOG_INFO, "Received bytes.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Server_Socket::sockSend(std::string buffer)
{
    const char * cStringBuffer = buffer.c_str();
    int status = send(clientHandle, cStringBuffer, buffer.length(), 0);
    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to send.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Sent bytes.");
    return SOCK_SUCCESS;
}