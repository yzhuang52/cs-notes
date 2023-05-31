#ifndef ESPRESSO_BASED_H
#define ESPRESSO_BASED_H
#include <vector>
class EspressoBased
{
public:
    virtual std::string get_name() = 0;
    virtual double price() = 0;

    virtual void brew() = 0;

    std::vector<Ingredient*>& get_ingredients();
    ~EspressoBased() {
        for(const auto& i : ingredients)
            delete i;
        ingredients.clear();
    }

protected:
    EspressoBased() = default;
    EspressoBased(const EspressoBased& esp): name(esp.name) {
        for (const auto& i: esp.ingredients)
        {
            ingredients.push_back(i->clone());
        }
    }
    EspressoBased& operator=(const EspressoBased& esp) = delete;

    std::vector<Ingredient*> ingredients;
    std::string name;

};
#endif // ESPRESSO_BASED_H