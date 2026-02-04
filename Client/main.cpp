#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "ChatClient.h"

int main() {
    std::string username;
    std::cout << "Enter your username: "; //Client için kullanıcı adı isteme
    std::getline(std::cin, username);

    ChatClient client( // Server'a bağlanma
        grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()),
        username
    );

    client.Run();
    return 0;
}

