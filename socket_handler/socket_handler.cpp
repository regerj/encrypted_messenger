#include "socket_handler.hpp"

#pragma comment(lib, "Ws2_32.lib")

Socket_Handler::Socket_Handler()
{
    SOCK_STATUS status = SOCK_GENERAL_FAIL;
    status = initWSA();
    if (status != SOCK_SUCCESS) throw std::runtime_error("Failed to init WSA");

    status = initLog();
    if (status != SOCK_SUCCESS)
    {
        WSACleanup();
        throw std::runtime_error("Failed to init log.");
    }
}

Socket_Handler::~Socket_Handler()
{
    log(LOG_INFO, "Cleaning up.");
    WSACleanup();
    freeaddrinfo(address);
    closesocket(socketHandle);
}

SOCK_STATUS Socket_Handler::initWSA()
{
    int status = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (status)
    {
        log(LOG_ERROR, "Failed to startup WSA.");
        return SOCK_GENERAL_FAIL;
    }
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::initLog()
{
    logFile.open(".\\lovelace_out.log", std::ofstream::out | std::ofstream::trunc);
    if (!logFile.is_open())
    {
        std::cout << "Failed to create log file." << std::endl;
        return SOCK_GENERAL_FAIL;
    }
    log(LOG_INFO, "Created log file.");
    return SOCK_SUCCESS;
}

SOCK_STATUS Socket_Handler::sockReceive(std::string& buffer)
{
    return SOCK_NOT_IMPLEMENTED;
}

SOCK_STATUS Socket_Handler::sockSend(std::string buffer)
{
    return SOCK_NOT_IMPLEMENTED;
}

SOCK_STATUS Socket_Handler::initAddr()
{
    return SOCK_NOT_IMPLEMENTED;
}

void Socket_Handler::threadControlReceive()
{
    SOCK_STATUS status = SOCK_GENERAL_FAIL;
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
    std::string recBuff;

    while (true)
    {
        status = sockReceive(recBuff);
        if (status != SOCK_SUCCESS)
        {
            log(LOG_ERROR, "Failed to read from socket.");
            break;
        }
        GetConsoleScreenBufferInfo(outputHandle, &consoleBufferInfo);
        COORD newPos;
        newPos.X = 0;
        newPos.Y = consoleBufferInfo.dwCursorPosition.Y;
        SetConsoleCursorPosition(outputHandle, newPos);
        std::cout << "Server: " << recBuff << std::endl;
        SetConsoleCursorPosition(outputHandle, consoleBufferInfo.dwCursorPosition);
    }
}