#pragma once
#include <mutex>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

class ChatServiceImpl final : public chat::ChatService::Service {
public:
    // Client server'a bağlanıp ChatStream(chat.proto) rpc çağrıldığında bu fonksiyon çağrılır. Her client için ayrı çağrı.
    grpc::Status ChatStream(
        grpc::ServerContext* context,
        grpc::ServerReaderWriter<chat::MessageReply, chat::MessageRequest>* stream
    ) override;

private:
    std::mutex mu_;
    std::vector<grpc::ServerReaderWriter<chat::MessageReply, chat::MessageRequest>*> clients_;
};
