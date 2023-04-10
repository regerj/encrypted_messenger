////////////////////////////////////////////////////////////////////////////////
//! \file client_socket.hpp
//! \author Jacob Reger (regerjacob@gmail.com)
//! \brief Header file for client socket class.
//! \version 0.1
//! \date 2023-04-08
//! 
//! \copyright Copyright (c) 2023
//! 
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <thread>
#include <socket_handler.hpp>

////////////////////////////////////////////////////////////////////////////////
//! \brief A class for interacting with the client's socket connection to the
//! server.
////////////////////////////////////////////////////////////////////////////////
class Client_Socket : public Socket_Handler
{
private:
    ////////////////////////////////////////////////////////////////////////////////
    //! \brief This function wraps the raw recv socket function for easier error
    //! handling and use with C++ style strings, abstracting away the C strings.
    //! 
    //! \param buffer C++ string for the buffer, will be populated with the received
    //! message.
    //! \param size Returns the size of the message sent, will be removed later.
    //! \return SOCK_STATUS representing if the operation succeeded or failed.
    ////////////////////////////////////////////////////////////////////////////////
    SOCK_STATUS sockReceive(std::string& buffer);

    ////////////////////////////////////////////////////////////////////////////////
    //! \brief This function wraps the raw send socket function for easier error
    //! handling and use with C++ style strings, abstracting away the C Strings.
    //! 
    //! \param buffer C++ string for the buffer, contains the message to send.
    //! \param size The size of the buffer, will be removed later.
    //! \return SOCK_STATUS representing if the operation succeeded or failed.
    ////////////////////////////////////////////////////////////////////////////////
    SOCK_STATUS sockSend(std::string buffer);

    ////////////////////////////////////////////////////////////////////////////////
    //! \brief This function encapsulates the address aquisition and error handling
    //! for socket communication.
    //! 
    //! \return SOCK_STATUS representing if the operation succeeded or failed.
    ////////////////////////////////////////////////////////////////////////////////
    SOCK_STATUS initAddr();

    ////////////////////////////////////////////////////////////////////////////////
    //! \brief This function initializes and connects the socket. This must be
    //! called only after previous setup has been completed.
    //! 
    //! \return SOCK_STATUS representing if the operation succeeded or failed.
    ////////////////////////////////////////////////////////////////////////////////
    SOCK_STATUS initAndConnectSocket();
protected:
public:
    ////////////////////////////////////////////////////////////////////////////////
    //! \brief Construct a new Client_Socket object.
    //! 
    ////////////////////////////////////////////////////////////////////////////////
    Client_Socket();

    ////////////////////////////////////////////////////////////////////////////////
    //! \brief Destroy the Client_Socket object.
    //! 
    ////////////////////////////////////////////////////////////////////////////////
    ~Client_Socket();

    ////////////////////////////////////////////////////////////////////////////////
    //! \brief This function shuts down the client's socket connection to the
    //! server.
    //! 
    //! \param type Integer representing shutting down receive (0), send (1), or
    //! both (2).
    //! \return SOCK_STATUS representing if the operation succeeded or failed.
    ////////////////////////////////////////////////////////////////////////////////
    SOCK_STATUS sockShutdown(int type);

    ////////////////////////////////////////////////////////////////////////////////
    //! \brief This function is a temporary implementation of talking back and forth
    //! with server for testing purposes. Will likely be deprecated and removed.
    //! 
    //! \return SOCK_STATUS representing if the operation succeeded or failed.
    ////////////////////////////////////////////////////////////////////////////////
    SOCK_STATUS tempComms();
};