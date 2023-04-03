#include "client_socket.hpp"

Client_Socket::Client_Socket()
{
    SOCK_STATUS status = SOCK_GENERAL_FAIL;

    status = initAddr();
    if(status != SOCK_SUCCESS)
    {
        WSACleanup();
        throw std::runtime_error("Failed to init address.");
    }

    status = initAndConnectSocket();
    if(status != SOCK_SUCCESS)
    {
        WSACleanup();
        freeaddrinfo(address);
        throw std::runtime_error("Failed to connect to socket.");
    }
}

Client_Socket::~Client_Socket()
{

}

SOCK_STATUS Client_Socket::initAddr()
{
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    int status = getaddrinfo("192.168.1.104", DEFUALT_PORT, &hints, &address);
    if(status)
    {
        log(LOG_ERROR, "Failed to get address info.");
        log(LOG_ERROR, std::to_string(status));
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Got address info.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Client_Socket::initAndConnectSocket()
{
    int status;
    socketHandle = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if(socketHandle == INVALID_SOCKET)
    {
        log(LOG_ERROR, "Failed to create socket.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Created socket.");

    status = connect(socketHandle, address->ai_addr, address->ai_addrlen);
    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to connect to socket.");
        closesocket(socketHandle);
        socketHandle = INVALID_SOCKET;
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Connected to socket.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Client_Socket::sockReceive(std::string &buffer, int &size)
{
    char cStringBuffer[BUFFER_LEN];
    size = recv(socketHandle, cStringBuffer, BUFFER_LEN, 0);

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
SOCK_STATUS Client_Socket::sockSend(std::string buffer, int & size)
{
    const char * cStringBuffer = buffer.c_str();
    int status = send(socketHandle, cStringBuffer, size, 0);
    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to send.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Sent bytes.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Client_Socket::sockShutdown(int type)
{
    int status = shutdown(socketHandle, type);
    if(status == SOCKET_ERROR)
    {
        log(LOG_ERROR, "Failed to shutdown socket.");
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Shutdown socket.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Client_Socket::tempComms()
{
    std::string sendBuff;
    std::string recBuff;
    // char sendBuff[BUFFER_LEN];
    // char recBuff[BUFFER_LEN];
    int recLen = BUFFER_LEN;
    int sendLen = BUFFER_LEN;
    SOCK_STATUS status = SOCK_GENERAL_FAIL;

    std::cout << "Enter your message here: ";
    getline(std::cin, sendBuff);
    sendLen = sendBuff.length();

    status = sockSend(sendBuff, sendLen);
    if(status != SOCK_SUCCESS) return status;

    status = sockReceive(recBuff, recLen);
    std::cout << "Jacob: " << recBuff << std::endl;
    return status;
}