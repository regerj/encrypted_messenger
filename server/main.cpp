#include <iostream>

#include "server_socket.hpp"

int main()
{
    std::cout << "Beginning program." << std::endl;
    Server_Socket socket;
    std::cout << "Finished socket creation." << std::endl;
    socket.sockListen();
    socket.sockAccept();
    socket.echo();
    std::cout << "Post echo" << std::endl;
    return 0;
}