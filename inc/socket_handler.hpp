////////////////////////////////////////////////////////////////////////////////
//! \file socket_handler.hpp
//! \author Jacob Reger (regerjacob@gmail.com)
//! \brief This is the header file for the base class Socket_Handler from which
//! Client_Socket and Server_Socket inherit.
//! \version 0.1
//! \date 2023-04-08
//! 
//! \copyright Copyright (c) 2023
//! 
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <sdkddkver.h>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <cstdlib>
#include <sys/types.h>
#include <fstream>
#include <string>

////////////////////////////////////////////////////////////////////////////////
//! \def INTERNET_PROTOCOL
//! \brief Basic macro to represent the selected protocol
//! 
////////////////////////////////////////////////////////////////////////////////
#define INTERNET_PROTOCOL 0

////////////////////////////////////////////////////////////////////////////////
//! \def LOG_ERROR
//! \brief Macro for logging an error, always passed into the log function
//! defined below.
//! 
////////////////////////////////////////////////////////////////////////////////
#define LOG_ERROR "ERROR"

////////////////////////////////////////////////////////////////////////////////
//! \def LOG_INFO
//! \brief Macro for logging info, always passed into the log function defined
//! below.
//! 
////////////////////////////////////////////////////////////////////////////////
#define LOG_INFO "INFO"

////////////////////////////////////////////////////////////////////////////////
//! \def LOG_WARNING
//! \brief Macro for logging warnings, always passed into the log function
//! defined below.
//! 
////////////////////////////////////////////////////////////////////////////////
#define LOG_WARNING "WARNING"

////////////////////////////////////////////////////////////////////////////////
//! \def DEFAULT_PORT
//! \brief Macro for the default port to select. Feel free to change if you want
//! to connect using a different port. Server and client must match.
//! 
////////////////////////////////////////////////////////////////////////////////
#define DEFUALT_PORT "27015"

////////////////////////////////////////////////////////////////////////////////
//! \def BUFFER_LEN
//! \brief Macro for the set buffer length for socket communication. Change to
//! increase or decrease the maximum buffer size.
//! 
////////////////////////////////////////////////////////////////////////////////
#define BUFFER_LEN 1024

// Forward declarations becuase windows is a piece of shit sometimes
#ifdef __cplusplus
extern "C" {
#endif
   void WSAAPI freeaddrinfo( struct addrinfo* );

   int WSAAPI getaddrinfo( const char*, const char*, const struct addrinfo*,
                 struct addrinfo** );

   int WSAAPI getnameinfo( const struct sockaddr*, socklen_t, char*, DWORD,
                char*, DWORD, int );
#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////
//! \enum SOCK_STATUS
//! \brief Enum representing possible socket statuses.
//! 
////////////////////////////////////////////////////////////////////////////////
enum SOCK_STATUS
{
    SOCK_SUCCESS = 0,
    SOCK_GENERAL_FAIL = 1,
    SOCK_CONNECTION_CLOSED = 2,
    SOCK_NOT_IMPLEMENTED = 3
};

////////////////////////////////////////////////////////////////////////////////
//! \class Socket_Handler
//! \brief A base class for the other sockets. Should not be used alone, needs
//! to be built off of with a child class.
//! 
////////////////////////////////////////////////////////////////////////////////
class Socket_Handler
{
    private:
    protected:
        ////////////////////////////////////////////////////////////////////////////////
        //! \var socketHandle
        //! \brief Handle to the socket being used for base communication.
        //! 
        ////////////////////////////////////////////////////////////////////////////////
        SOCKET socketHandle = INVALID_SOCKET;

        
        struct addrinfo * address = NULL;
        struct addrinfo * ptr = NULL;
        std::ofstream logFile;
        WSADATA wsaData;
        struct addrinfo hints;

        inline void log(std::string type, std::string message) { logFile << type << ": " << message << std::endl;}
        SOCK_STATUS initWSA();
        SOCK_STATUS initLog();
        virtual SOCK_STATUS sockReceive(std::string &, int &);
        virtual SOCK_STATUS sockSend(std::string, int &);
        virtual SOCK_STATUS initAddr();
    public:
        Socket_Handler();
        ~Socket_Handler();
};