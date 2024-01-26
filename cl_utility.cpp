#include "ft_inventory.h"

std::vector<Item> loadItemsFromInventoryFile()
{
    std::ifstream inputFile("inventory.txt");
    std::vector<Item> items;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open the inventory file." << std::endl;
        return items;
    }

    int itemId;
    std::string itemName, itemCategory;
    float itemPrice;
    int itemQuantity;

    while (inputFile >> itemId >> itemName >> itemPrice >> itemQuantity >> itemCategory)
    {
        Item newItem(itemId, itemName, itemPrice, itemQuantity, itemCategory);
        items.push_back(newItem);
    }

    inputFile.close();
    return items;
}
void calculateInventoryTotalValue(const std::vector<Item> &inventoryItems)
{
    float totalValue = 0.0;

    for (const auto &item : inventoryItems)
    {
        totalValue += item.getPrice() * item.getQuantity();
    }

    std::cout << "Total value of the inventory: " << totalValue << "$" << std::endl;
}
