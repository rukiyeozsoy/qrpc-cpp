#include "ChatClient.h"
#include <iostream>
#include <thread>
#include <mutex>

static std::mutex io_mu;

static void erase_last_input_line() { // Terminal daha temiz görünmesi için mesaj yazılan satır silinir ve bir üst satıra taşınır
    std::cout << "\033[1A\r\033[2K" << std::flush;
}

// Server'a giden bağlantı hattı ile grpc stub oluşur böylece client tarafından rpc çağrıları yapılır
ChatClient::ChatClient(std::shared_ptr<grpc::Channel> channel, std::string username)
    : stub_(chat::ChatService::NewStub(channel)), username_(std::move(username)) {}

void ChatClient::Run() {
    grpc::ClientContext context;
    auto stream = stub_->ChatStream(&context);

    // Server'dan gelen mesajları dinleme
    std::thread reader([&]() {
        chat::MessageReply reply;
        while (stream->Read(&reply)) {
            std::lock_guard<std::mutex> lk(io_mu);
            std::cout << reply.username() << ": " << reply.message() << std::endl;
        }
    });

    // Kullanıcıdan gelen mesajları okuma ve gönderme
    std::string text;
    while (true) {
        if (!std::getline(std::cin, text)) break;
        if (text == "/quit") break;

        {
            std::lock_guard<std::mutex> lk(io_mu);
            erase_last_input_line();  
        }

        if (text.empty()) continue;

        chat::MessageRequest req;
        req.set_username(username_);
        req.set_message(text);
        stream->Write(req);
    }

    // Kapama
    stream->WritesDone(); // Clint artık mesaj göndermeyecek
    reader.join();

    auto status = stream->Finish();
    if (!status.ok()) {
        std::lock_guard<std::mutex> lk(io_mu);
        std::cout << "RPC failed: " << status.error_message() << std::endl;
    }
}

