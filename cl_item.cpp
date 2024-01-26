#include "ft_inventory.h"

Item::Item(int id, const std::string &name, float price, int quantity, const std::string &category)
    : id(id), name(name), price(price), quantity(quantity), category(category) {}

int Item::getId() const { return id; }
const std::string &Item::getName() const { return name; }
float Item::getPrice() const { return price; }
int Item::getQuantity() const { return quantity; }
const std::string &Item::getCategory() const { return category; }

void Item::setName(const std::string &newName) { name = newName; }
void Item::setPrice(float newPrice) { price = newPrice; }
void Item::setQuantity(int newQuantity) { quantity = newQuantity; }
void Item::setCategory(const std::string &newCategory) { category = newCategory; }
std::vector<Item> readInventoryFromFile(const std::string &fileName)
{
    std::vector<Item> items;

    std::ifstream file(fileName);

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("ID:") != std::string::npos)
            {
                int itemId, quantity, price;
                std::string name, category;

                std::istringstream iss(line);
                std::string dummy;

                iss >> dummy;
                iss >> itemId >> dummy;
                iss >> dummy;                 
                std::getline(iss >> std::ws, name, ',');
                iss >> dummy >> price >> dummy >> quantity >> dummy;
                std::getline(iss >> std::ws, category);

                Item item(itemId, name, price, quantity, category);
                items.push_back(item);
            }
        }
        file.close();
    }

    return items;
}

std::ostream& operator<<(std::ostream& os, const Item& item)
{
    os << "ID: " << item.getId() << ", Name: " << item.getName()
       << ", Price: " << item.getPrice() << ", Quantity: " << item.getQuantity()
       << ", Category: " << item.getCategory();
    return os;
}
