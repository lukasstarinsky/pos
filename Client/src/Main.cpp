#include <iostream>
#include "Socket/Socket.hpp"

int main()
{
    try
    {
        auto socket = Socket::CreateConnection("frios2.fri.uniza.sk", 16912);

        socket->SendData("Negri su zli velmi moc.");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
    }

    return 0;
}