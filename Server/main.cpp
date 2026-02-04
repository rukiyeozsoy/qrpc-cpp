#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "ChatServiceImpl.h"

int main() {
    const std::string addr = "0.0.0.0:50051"; // Server'ın dinleyeceği ip:port
    ChatServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service); //Servis geldiğinde çalışacak alan (ChatServiceImpl)

    auto server = builder.BuildAndStart(); // Start server

    if (!server) {
        std::cerr << "Server failed to start!" << std::endl;
        return 1;
    }

    std::cout << "Server listening on " << addr << std::endl;
    server->Wait(); // Server kapanana kadar devam et
    return 0;
}
