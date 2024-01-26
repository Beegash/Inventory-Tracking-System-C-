
#include "ft_inventory.h"

int Inventory::getItemCount() const { return items.size(); }

const Item &Inventory::getItem(int index) const { return items.at(index); }

Item *Inventory::findItemById(int id)
{
    auto it = std::find_if(items.begin(), items.end(), [id](const Item &item) { return item.getId() == id; });
    return it != items.end() ? &(*it) : nullptr;
}

Item *Inventory::findItemByName(const std::string &name)
{
    auto it = std::find_if(items.begin(), items.end(), [name](const Item &item) { return item.getName() == name; });
    return it != items.end() ? &(*it) : nullptr;
}

void Inventory::addItem(const Item &item) { items.push_back(item); }

bool Inventory::removeItemById(int id)
{
    auto it = std::remove_if(items.begin(), items.end(), [id](const Item &item) { return item.getId() == id; });
    if (it != items.end())
    {
        items.erase(it, items.end());
        return true;
    }
    return false;
}

void Inventory::sortInventoryById()
{
    std::sort(items.begin(), items.end(), [](const Item &a, const Item &b) { return a.getId() < b.getId(); });
}

void Inventory::displayInventory() const
{
    for (const auto &item : items)
    {
        std::cout << "ID: " << item.getId() << ", Name: " << item.getName()
                  << ", Price: " << item.getPrice() << ", Quantity: " << item.getQuantity()
                  << ", Category: " << item.getCategory() << std::endl;
    }
}
