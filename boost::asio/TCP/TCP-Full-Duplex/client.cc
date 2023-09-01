#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context ioContext;
    tcp::socket socket(ioContext);
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 61000));

    while (true) {
        std::string message;
        std::cout << "Enter your message: ";
        std::getline(std::cin, message);

        boost::asio::write(socket, boost::asio::buffer(message + "\n"));

        boost::asio::streambuf receiveBuffer;
        boost::system::error_code error;

        size_t bytesRead = boost::asio::read_until(socket, receiveBuffer, '\n', error);
        if (error) {
            std::cerr << "Error reading from server: " << error.message() << std::endl;
            break;
        }

        std::string response(boost::asio::buffers_begin(receiveBuffer.data()), boost::asio::buffers_begin(receiveBuffer.data()) + bytesRead);
        std::cout << "Server says: " << response;
    }

    return 0;
}
