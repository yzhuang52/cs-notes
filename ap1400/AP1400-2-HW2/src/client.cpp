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
    std::string signature = crypto::signMessage(this->private_key, txt);
    return signature;
}

bool Client::transfer_money(std::string receiver, double value) const {
    std::string transaction = this->id + "-" + receiver + "-" + std::to_string(value);
    std::string signature{this->sign(transaction)};
    return this->server->add_pending_trx(transaction, signature);
}

size_t Client::generate_nonce() {
    return rand();
}

