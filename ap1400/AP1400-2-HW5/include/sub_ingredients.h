#ifndef SUB_INGREDIENTS_H
#define SUB_INGREDIENTS_H

class Cinnamon
{
public:
    Cinnamon(size_t units) : Ingredient{5, units}
    {
        this->name = "Cinnamon";
    }

    virtual std::string get_name() {return this->name;}
    virtual Cinnamon* clone() override {
        return new Cinanmon(*this);
    }
};

class Chocolate
{
public:
    Chocolate(size_t units) : Ingredient{5, units}
    {
        this->name = "Chocolate";
    }

    virtual std::string get_name() {return this->name;}
    virtual Chocolate* clone() override {
        return new Cinanmon(*this);
    }
};

class Sugar
{
public:
    Sugar(size_t units) : Ingredient{1, units}
    {
        this->name = "Sugar";
    }

    virtual std::string get_name() {return this->name;}
    virtual Sugar* clone() override {
        return new Cinanmon(*this);
    }
};

class Cookie
{
public:
    Cookie(size_t units) : Ingredient{10, units}
    {
        this->name = "Cookie";
    }

    virtual std::string get_name() {return this->name;}
    virtual Cookie* clone() override {
        return new Cookie(*this);
    }
};

class Espresso
{
public:
    Espresso(size_t units) : Ingredient{15, units}
    {
        this->name = "Espresso";
    }

    virtual std::string get_name() {return this->name;}
    virtual Espresso* clone() override {
        return new Espresso(*this);
    }
};

class Milk
{
public:
    Milk(size_t units) : Ingredient{15, units}
    {
        this->name = "Milk";
    }

    virtual std::string get_name() {return this->name;}
    virtual Milk* clone() override {
        return new Milk(*this);
    }
};

class MilkFoam
{
public:
    MilkFoam(size_t units) : Ingredient{5, units}
    {
        this->name = "MilkFoam";
    }

    virtual std::string get_name() {return this->name;}
    virtual MilkFoam* clone() override {
        return new MilkFoam(*this);
    }
};

class Water
{
public:
    Water(size_t units) : Ingredient{1, units}
    {
        this->name = "Water";
    }

    virtual std::string get_name() {return this->name;}
    virtual Water* clone() override {
        return new Water(*this);
    }
};
#endif // SUB_INGREDIENTS_H