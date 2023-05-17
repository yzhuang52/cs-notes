#include "client.h"
#include "server.h"
#include "crypto.h"

Client::Client(std::string id, const Server& server): id(id), server(&server) {
    std::string public_key{}, private_key{};
    crypto::generate_key(public_key, private_key);
    this->public_key = public_key;
    this->private_key = private_key;
}

std::string Client::get_id() {
    return this->id;
}

std::string Client::get_publickey() const {
    return this->public_key;
}

double Client::get_wallet() const {
    return server->get_wallet(this->id);
}

std::string Client::sign(std::string txt) const {
    return txt;
}

bool Client::transfer_money(std::string receiver, double value) const {
    return false;
}

size_t Client::generate_nonce() {
    return 1;
}

