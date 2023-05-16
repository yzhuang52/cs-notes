#include "client.h"
#include "server.h"


Client::Client(std::string id, const Server& server) {
    this.id = id;
    this.server = server;
    std::string public_key{}, private_key{};
    crypto::generate_key(public_key, private_key);
    this.public_key = public_key;
    this.private_key = private_key;
}

std::string Client::get_id() {
    return this.id;
}

std::string Client::get_publickey() {
    return this.public_key;
}

double Client::get_wallet() {
    return server.get_wallet(id);
}

std::string Client::sign(std::string txt) {
    return txt;
}

bool Client::transfer_money(std::string receiver, double value) {
    return nullptr;
}

size_t Client::generate_nonce() {
    return 1;
}

