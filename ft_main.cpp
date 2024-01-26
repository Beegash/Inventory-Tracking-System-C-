#include <iostream>
#include <fstream>
#include <vector>
#include "ft_inventory.h"
#include "cl_admin.cpp"
#include "cl_inventory.cpp"
#include "cl_item.cpp"
#include "cl_user.cpp"
#include "cl_userfunctions.cpp"
#include "cl_utility.cpp"

int main()
{
    CLEAR_SCREEN();
    UserFunctions userFunctions;
    loadUserFromFile(userFunctions);
    std::vector<Item> inventoryItems = loadItemsFromInventoryFile();
    User currentUser = authenticateUser(userFunctions);
    InventoryData inventoryData;
    inventoryData.itemCount = 0;

    CLEAR_SCREEN();
    if (currentUser.getUsername() != "")
    {
        currentUser.loadInventoryFromFile(currentUser.getUsername() + ".txt");
        while (true)
        {
            std::cout << "=========== MENU ============" << std::endl;
            std::cout << "1. Display All Items" << std::endl;
            std::cout << "2. Add New Item to System" << std::endl;
            std::cout << "3. Display User's Inventory" << std::endl;
            std::cout << "4. Add Item to User's Inventory" << std::endl;
            std::cout << "5. Remove Item from User's Inventory" << std::endl;
            std::cout << "6. Show Item Details" << std::endl;
            std::cout << "7. Save User Inventory to File" << std::endl;
            std::cout << "8. Calculate Inventory Total Value" << std::endl;
            std::cout << "9. Update Item" << std::endl;
            std::cout << "10. Add Item to System" << std::endl;
            std::cout << "11. Remove Item from System" << std::endl;
            std::cout << "12. Calculate Total Value Of System" << std::endl;
            std::cout << "13. Exit" << std::endl;
            std::cout << "=============================" << std::endl;

            int choice;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
            {
                CLEAR_SCREEN();
                displayInventory(inventoryItems);
                inventoryItems = loadItemsFromInventoryFile();
                break;
            }
            case 2:
                CLEAR_SCREEN();
                addNewProduct(inventoryItems);
                break;
            case 3:
                currentUser.displayUserInventory();
                break;
            case 4:
                CLEAR_SCREEN();
                addNewItemToInventory(inventoryItems, currentUser);
                break;
            case 5:
            {
                int itemId;
                std::cout << "Enter Item ID to remove: ";
                std::cin >> itemId;

                bool removed = currentUser.removeItemFromInventory(itemId, inventoryItems);

                if (removed)
                {
                    std::cout << "Item removed from inventory successfully." << std::endl;

                    auto foundItem = std::find_if(inventoryItems.begin(), inventoryItems.end(),
                                                  [itemId](const Item &item)
                                                  { return item.getId() == itemId; });

                    if (foundItem != inventoryItems.end())
                    {
                        foundItem->setQuantity(foundItem->getQuantity() + 1);
                    }
                }
                else
                {
                    std::cout << "Item not found in the inventory." << std::endl;
                }
                break;
            }
            case 6:
                CLEAR_SCREEN();
                currentUser.showItemDetails(inventoryItems);
                break;
            case 7:
                CLEAR_SCREEN();
                currentUser.saveToFile();
                break;
            case 8:
                CLEAR_SCREEN();
                currentUser.calculateInventoryTotal();
                break;
            case 9:
                CLEAR_SCREEN();
                currentUser.updateItem(inventoryItems);
                break;
            case 10:
                CLEAR_SCREEN();
                addItem(inventoryData);
                inventoryItems = loadItemsFromInventoryFile();

                break;
            case 11:
                CLEAR_SCREEN();
                removeItem(inventoryItems);
                inventoryItems = loadItemsFromInventoryFile();
                break;
            case 12:
                CLEAR_SCREEN();
                calculateInventoryTotalValue(inventoryItems);
                break;
            case 13:
                CLEAR_SCREEN();
                std::cout << "Exiting the program." << std::endl;
                return 0;
            default:
                CLEAR_SCREEN();
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
    }
    else
    {
        std::cout << "Authentication failed. Exiting the program." << std::endl;
    }

    return 0;
}
