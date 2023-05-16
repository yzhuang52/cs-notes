#include "server.h"
#include "client.h"


Server::Server() {
    
}

std::shared_ptr<Client> Server::add_client(std::string id) {
    std::map<std::shared_ptr<Client>, double>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (*(it->first).get_id() == id)
        {
            // id in clients, add 4 random number
            int number = rand() % 9000 + 1000;
            id += std::to_string(number);
            clients.insert(Client(id, this), 5);
            return clients;
        }
    }
    clients.insert(Client(id, this), 5);
    return clients;
}

std::shared_ptr<Client> Server::get_client(std::string id) {
    std::map<std::shared_ptr<Client>, double>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (*(it->first).get_id() == id)
        {
            return it->first;
        }
    }
    return nullptr;
}

double Server::get_wallet(std::string id) {
    std::map<std::shared_ptr<Client> double>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (*(it->first).get_id() == id)
        {
            return it->second;
        }
    }
    return NULL;
}

bool Server::parse_trx(std::string trx, std::string sender, std::string receiver, double value) {
    return nullptr;
}

bool Server::add_pending_trx(std::string trx, std::string signature) {
    return nullptr;
}

size_t Server::mine() {

}
