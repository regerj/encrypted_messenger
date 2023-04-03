#include <iostream>

#include "client_socket.hpp"

int main()
{
    std::cout << "Beginning program." << std::endl;
    Client_Socket socket;
    std::cout << "Finished socket creation." << std::endl;
    socket.tempComms();
    Sleep(10000);
    return 0;
}