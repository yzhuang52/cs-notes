#include "server.h"
#include "client.h"

std::vector<std::string> pending_trxs;

Server::Server() {
    
}

std::shared_ptr<Client> Server::add_client(std::string id) {
    std::shared_ptr<Client> client = std::make_shared<Client>(id, *this);
    for (auto it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it->first).get_id() == id)
        {
            // id in clients, add 4 random number
            int number = rand() % 9000 + 1000;
            id += std::to_string(number);
            clients.insert({client, 5});
            return client;
        }
    }
    clients.insert({client, 5});
    return client;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
    for (auto it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it->first).get_id() == id)
        {
            return it->first;
        }
    }
    return nullptr;
}

double Server::get_wallet(std::string id) const {
    for (auto it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it->first).get_id() == id)
        {
            return it->second;
        }
    }
    return 0;
}

bool Server::parse_trx(const std::string& trx, std::string& sender, std::string& receiver, double& value) {
    return false;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const {
    return false;
}

size_t Server::mine() {
    return 1;
}


void show_wallets(const Server& server)
{
std::map<std::shared_ptr<Client>, double>* clients = (std::map<std::shared_ptr<Client>, double>*)&server;
std::cout << std::string(20, '*') << std::endl;
for(const auto& client: *clients)
    std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
std::cout << std::string(20, '*') << std::endl;
}

