#pragma once
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

// Server ile gRPC üzerinden bağlantı kurar
class ChatClient {
public:
    ChatClient(std::shared_ptr<grpc::Channel> channel, std::string username);
    void Run();

private:
    std::unique_ptr<chat::ChatService::Stub> stub_;
    std::string username_;
};

