#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context ioContext;
    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 61000));

    tcp::socket socket(ioContext);
    acceptor.accept(socket);

    while (true) {
        boost::asio::streambuf receiveBuffer;
        boost::system::error_code error;

        size_t bytesRead = boost::asio::read_until(socket, receiveBuffer, '\n', error);
        if (error) {
            std::cerr << "Error reading from client: " << error.message() << std::endl;
            break;
        }

        std::string message(boost::asio::buffers_begin(receiveBuffer.data()), boost::asio::buffers_begin(receiveBuffer.data()) + bytesRead);
        std::cout << "Client says: " << message;

        std::string response;
        std::cout << "Enter your response: ";
        std::getline(std::cin, response);

        boost::asio::write(socket, boost::asio::buffer(response + "\n"));
    }

    return 0;
}
