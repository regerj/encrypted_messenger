#include <iostream>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <cstdlib>
#include <sys/types.h>
#include <winsock2.h>
#include <fstream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define INTERNET_PROTOCOL 0
#define LOG_ERROR "ERROR"
#define LOG_INFO "INFO"
#define PORT 8082
#define STATUS_FAIL -1

class Socket_Handler
{
    private:
        SOCKET socketHandle = INVALID_SOCKET;
        struct addrinfo * address = NULL;
        struct addrinfo * ptr = NULL;
        struct addrinfo hints;
        std::ofstream logFile;

        inline void log(std::string, std::string);
    protected:
    public:
        Socket_Handler();
        ~Socket_Handler();
};