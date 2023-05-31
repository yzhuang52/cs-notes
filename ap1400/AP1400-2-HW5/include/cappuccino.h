#ifndef CAPPUCCINO
#define CAPPUCCINO
#include <vector>

class Cappuccino
{
public:
    Cappuccino() = default;
    Cappuccino(const Cappuccino& cap);
	~Cappuccino() 
    {
        for(const auto& i : side_items)
            delete i;
        side_items.clear();
    }
    void operator=(const Cappuccino& cap);

    virtual std::string get_name();
    virtual double price();

    void add_side_item(Ingredient* side);
    std::vector<Ingredient*>& get_side_items();

private:
    std::vector<Ingredient*> side_items;

};

#endif // CAPPUCCINO