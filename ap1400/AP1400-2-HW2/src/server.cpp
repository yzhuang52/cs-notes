#include "server.h"
#include "client.h"

std::vector<std::string> pending_trxs;

Server::Server() {
    
}

std::shared_ptr<Client> Server::add_client(std::string id) {
    for (auto it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it->first).get_id() == id)
        {
            // id in clients, add 4 random number
            int number = rand() % 9000 + 1000;
            id += std::to_string(number);
            std::shared_ptr<Client> client = std::make_shared<Client>(id, *this);
            clients.insert({client, 5});
            return client;
        }
    }
    std::shared_ptr<Client> client = std::make_shared<Client>(id, *this);
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
    std::string delimiter = "-";
    size_t index = 0;
    size_t pos = 0;
    pos = trx.find(delimiter, index);
    if (pos != std::string::npos)
    {
        sender = trx.substr(index, pos-index);
        index = pos + 1;
    } else 
    {
        throw std::runtime_error("non standard trx\n");
    }
    
    pos = trx.find(delimiter, index);
    if (pos != std::string::npos)
    {
        receiver = trx.substr(index, pos-index);
        index = pos + 1;
    } else 
    {
        throw std::runtime_error("non standard trx\n");
    }
    value = std::stod(trx.substr(index, trx.length() - 1));
    return true;

}

bool Server::add_pending_trx(std::string trx, std::string signature) const {
    
    std::string sender{}, receiver{};
    double value;
    Server::parse_trx(trx, sender, receiver, value);
    std::cout << value << std::endl;
    bool authentic = crypto::verifySignature((*this->get_client(sender)).get_publickey(), trx, signature);
    if(authentic && this->get_wallet(sender) >= value && this->get_client(receiver))
    {
        pending_trxs.push_back(trx);
        return true;
    }
    return false;
}

size_t Server::mine() {
    std::string mempool{};
    for(const auto& trx: pending_trxs)
    {
        mempool += trx;
    }
    size_t nonce = 0;
    bool flag = true;
    while (flag)
    {
        for (auto it = this->clients.begin(); it != this->clients.end(); it++)
            {
                nonce = (*it->first).generate_nonce();
                if (crypto::sha256(mempool + std::to_string(nonce)).substr(0, 10).find("000") != std::string::npos)
                {   
                    std::cout << (*it->first).get_id() << std::endl;
                    flag = false;
                    it->second += 6.25;
                    for (const auto& trx : pending_trxs)
                    {
                        std::string sender{}, receiver{};
                        double value;
                        Server::parse_trx(trx, sender, receiver, value);
                        this->clients[this->get_client(sender)] -= value;
                        this->clients[this->get_client(receiver)] += value;
                    }
                    pending_trxs.clear();
                    return nonce;
                }
                
            }
    }
    return nonce;
}


void show_wallets(const Server& server)
{
std::map<std::shared_ptr<Client>, double>* clients = (std::map<std::shared_ptr<Client>, double>*)&server;
std::cout << std::string(20, '*') << std::endl;
for(const auto& client: *clients)
    std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
std::cout << std::string(20, '*') << std::endl;
}

