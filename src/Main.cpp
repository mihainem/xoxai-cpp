#include <websocketapp/config/core.hpp>
#include "websocketpp/config/asio_no_tls.hpp"
#include <websocketpp/server.hpp>
#include <iostream>

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::string response = "Hello, World!";
    try {
        s->send(hdl, response, msg->get_opcode());
    } catch (const websocketpp::lib::error_code& e) {
        std::cout << "Error sending message: " << e.message() << std::endl;
    }
}

int main() {
    // Create a websocket server endpoint
    server endpoint;

    // Set logging settings
    endpoint.set_access_channels(websocketpp::log::alevel::all);
    endpoint.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Set handlers
    endpoint.set_message_handler(std::bind(&on_message, &endpoint, std::placeholders::_1, std::placeholders::_2));

    // Start the server
    try {
        endpoint.listen(9002);
        endpoint.start_accept();
        endpoint.run();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
