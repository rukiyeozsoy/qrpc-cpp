#include "ChatServiceImpl.h"
#include <algorithm>

grpc::Status ChatServiceImpl::ChatStream(
    grpc::ServerContext*,
    grpc::ServerReaderWriter<chat::MessageReply, chat::MessageRequest>* stream) {

        {
            std::lock_guard<std::mutex> lk(mu_);
            clients_.push_back(stream); // Gelen mesajı tüm Client'lara göndermek için listeye ekleme yapıldı
        }

        //Dinleme
        chat::MessageRequest req;
        while (stream->Read(&req)) {
            chat::MessageReply msg;
            msg.set_username(req.username());
            msg.set_message(req.message());

            std::lock_guard<std::mutex> lk(mu_); // Aynı anda tek thread erişebilir
            for (auto* s : clients_) {
                s->Write(msg);
            }
        }

        {
            std::lock_guard<std::mutex> lk(mu_);
            clients_.erase(std::remove(clients_.begin(), clients_.end(), stream), clients_.end());
        }

        return grpc::Status::OK;
}
