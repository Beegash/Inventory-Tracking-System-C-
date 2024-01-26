#ifndef FT_INVENTORY_H
#define FT_INVENTORY_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>

class Item;
class Inventory;
class User;
class Admin;
class UserFunctions;

const int MAX_ITEMS = 100;
const int MAX_NAME = 50;
const int MAX_USERS = 50;

#define CLEAR_SCREEN() system("clear");

class Item
{
private:
    int id;
    std::string name;
    float price;
    int quantity;
    std::string category;

public:
    Item(int id, const std::string &name, float price, int quantity, const std::string &category);

    int getId() const;
    const std::string &getName() const;
    float getPrice() const;
    int getQuantity() const;
    const std::string &getCategory() const;

    void setName(const std::string &name);
    void setPrice(float price);
    void setQuantity(int quantity);
    void setCategory(const std::string &category);
};

class Inventory
{
private:
public:
    std::vector<Item> items;
    int getItemCount() const;
    const Item &getItem(int index) const;
    Item *findItemById(int id);
    Item *findItemByName(const std::string &name);
    void addItem(const Item &item);
    bool removeItemById(int id);
    void sortInventoryById();
    void displayInventory() const;
};

class User
{
private:
    std::string username;
    std::string password;

public:
    Inventory inventory;
    User(const std::string &username, const std::string &password);

    const std::string &getUsername() const;
    const std::string &getPassword() const;
    Inventory &getInventory();

    void addItemToInventory(const Item &item);
    bool removeItemFromInventory(int itemId, std::vector<Item> &inventoryItems);
    void displayUserInventory() const;
    void showItemDetails(std::vector<Item> &inventoryItems) const;
    void saveToFile() const;
    void loadInventoryFromFile(const std::string &filename);
    virtual void calculateInventoryTotal() const;
    virtual void updateItem();
    virtual void removeUser();
    virtual void setUsername(const std::string &username);
    virtual void setPassword(const std::string &password);
    void updateItem(std::vector<Item> &inventoryItems);
    const Inventory &getProtectedInventory() const { return inventory; }
    friend class Admin;
};

class Admin : protected User
{
public:
    Admin(const std::string &username, const std::string &password);

    virtual void addItem(const Item &item);
    void addItemDoublePointer(struct InventoryData **data);
    void removeItem(int itemId);
    void updateItem(int itemId, const std::string &newName, float newPrice, int newQuantity, const std::string &newCategory);
    void showUserList(const UserFunctions &userFunctions) const;
    void removeUser(UserFunctions &userFunctions, const std::string username);
    void saveUsersToFile(const UserFunctions &userFunctions) const;
    const Inventory &getAdminInventory() const
    {
        return inventory;
    }
};

class UserFunctions
{
private:
    std::vector<User> users;

public:
    void addUser(const std::string &username, const std::string &password, bool isAdmin);
    bool userExists(const std::string &username) const;
    void displayUserList() const;
    void removeUser(const std::string &username);
    int authenticateUser(const std::string &username, const std::string &password) const;
    void loadUsersFromFile();
    void saveUsersToFile() const;
    int getUserCount() const;
    int getAdminCount() const;
    void countUsersInFile() const;
    User getUserByUsername(const std::string &username) const;
};
void loadUserFromFile(UserFunctions &userFunctions)
{
    std::ifstream userFile("users.txt");
    if (!userFile.is_open())
    {
        std::cerr << "Error: Could not open the user file." << std::endl;
        return;
    }

    std::string username, password, isAdminStr;
    bool isAdmin;

    while (userFile >> username >> password >> isAdminStr)
    {
        isAdmin = (isAdminStr == "1");
        userFunctions.addUser(username, password, isAdmin);
    }
    std::cout << "Users loaded successfully." << std::endl;
    userFile.close();
}
void saveItemsToInventoryFile(const std::vector<Item> &inventoryItems);
std::vector<Item> loadItemsFromInventoryFile();
void addNewProduct(std::vector<Item> &inventoryItems)
{
    int newId;
    std::string newName, newCategory;
    float newPrice;
    int newQuantity;

    std::cout << "Enter new product ID: ";
    std::cin >> newId;
    std::cout << "Enter new product name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, newName);
    std::cout << "Enter new product price: ";
    std::cin >> newPrice;
    std::cout << "Enter new product quantity: ";
    std::cin >> newQuantity;
    std::cout << "Enter new product category: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, newCategory);

    Item newProduct(newId, newName, newPrice, newQuantity, newCategory);
    inventoryItems.push_back(newProduct);
    saveItemsToInventoryFile(inventoryItems);

    std::cout << "New product added successfully." << std::endl;
}
void saveItemsToInventoryFile(const std::vector<Item> &inventoryItems)
{
    std::ofstream outputFile("inventory.txt");

    if (!outputFile.is_open())
    {
        std::cerr << "Error: Could not open the inventory file." << std::endl;
        return;
    }

    for (const auto &item : inventoryItems)
    {
        outputFile << item.getId() << " " << item.getName() << " " << item.getPrice()
                   << " " << item.getQuantity() << " " << item.getCategory() << std::endl;
    }

    outputFile.close();
    std::cout << "Inventory file updated successfully." << std::endl;
}
void addNewItemToInventory(std::vector<Item> &inventoryItems, User &currentUser)
{
    int itemId, quantityToAdd;
    std::cout << "Enter the ID of the item to add to your inventory: ";
    std::cin >> itemId;
    std::cout << "Enter the quantity to add: ";
    std::cin >> quantityToAdd;

    auto foundItem = std::find_if(inventoryItems.begin(), inventoryItems.end(),
                                  [itemId](const Item &item)
                                  { return item.getId() == itemId; });

    if (foundItem != inventoryItems.end())
    {
        if (foundItem->getQuantity() >= quantityToAdd)
        {
            Item newItem = *foundItem;
            newItem.setQuantity(quantityToAdd);
            currentUser.addItemToInventory(newItem);

            foundItem->setQuantity(foundItem->getQuantity() - quantityToAdd);

            std::cout << "Items added to your inventory successfully." << std::endl;

            saveItemsToInventoryFile(inventoryItems);
        }
        else
        {
            std::cout << "Not enough quantity in the general items inventory." << std::endl;
        }
    }
    else
    {
        std::cout << "Item with ID " << itemId << " not found in the inventory." << std::endl;
    }
}
struct InventoryItem
{
    int id;
    char name[MAX_NAME];
    double price;
    int quantity;
    std::string category;
};

struct InventoryData
{
    InventoryItem inventory[MAX_ITEMS];
    int itemCount;
};

const char *INVENTORY_FILE = "inventory.txt";

void addItem(InventoryData &data)
{
    CLEAR_SCREEN();
    std::cout << "\nEnter item details:\n";

    if (data.itemCount >= MAX_ITEMS)
    {
        std::cout << "Error: Maximum number of items exceeded.\n";
        return;
    }

    std::cout << "ID: ";
    int newId;
    while (!(std::cin >> newId) || newId < 0)
    {
        CLEAR_SCREEN();

        std::cout << "Invalid input. Please enter a non-negative number for ID.\n\n";
        std::cout << "Enter item details:\n";
        std::cout << "ID: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::ifstream inputFile(INVENTORY_FILE);
    bool idExists = false;
    if (inputFile.is_open())
    {
        int idFromFile;
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            if (iss >> idFromFile)
            {
                if (idFromFile == newId)
                {
                    idExists = true;
                    break;
                }
            }
        }

        inputFile.close();
    }

    if (idExists)
    {
        CLEAR_SCREEN();
        std::cout << "Error: An item with the same ID already exists. Item not added.\n";
        return;
    }
    std::cout << "Name: ";
    char newName[MAX_NAME];
    std::cin >> newName;
    for (int i = 0; i < data.itemCount; i++)
    {
        if (strcmp(data.inventory[i].name, newName) == 0 || strcmp(data.inventory[i].name, newName) == 32 || strcmp(data.inventory[i].name, newName) == -32)
        {
            CLEAR_SCREEN();

            std::cout << "Error: An item with the same name already exists or Item started with lower case. Item not added.\n";
            return;
        }
    }
    inputFile.open(INVENTORY_FILE);
    bool nameExists = false;
    if (inputFile.is_open())
    {
        std::string nameFromFile;
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            if (iss >> nameFromFile)
            {
                if (nameFromFile == newName)
                {
                    nameExists = true;
                    break;
                }
            }
        }

        inputFile.close();
    }

    if (nameExists)
    {
        CLEAR_SCREEN();
        std::cout << "Error: An item with the same name already exists. Item not added.\n";
        return;
    }

    std::cout << "Price: ";
    while (!(std::cin >> data.inventory[data.itemCount].price) || data.inventory[data.itemCount].price < 0)
    {
        CLEAR_SCREEN();

        std::cout << "Invalid input. Please enter a non-negative number for Price.\n\n";
        std::cout << "Enter item details:\n";
        std::cout << "ID: " << newId << "\n";
        std::cout << "Name: " << newName << "\n";
        std::cout << "Price: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Quantity: ";
    while (!(std::cin >> data.inventory[data.itemCount].quantity) || data.inventory[data.itemCount].quantity < 0)
    {
        CLEAR_SCREEN();

        std::cout << "Invalid input. Please enter a non-negative number for Quantity.\n\n";
        std::cout << "Enter item details:\n";
        std::cout << "ID: " << newId << "\n";
        std::cout << "Name: " << newName << "\n";
        std::cout << "Price: " << data.inventory[data.itemCount].price << "\n";
        std::cout << "Quantity: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Category: ";
    std::cin >> data.inventory[data.itemCount].category;

    data.inventory[data.itemCount].id = newId;
    strcpy(data.inventory[data.itemCount].name, newName);

    std::ofstream outputFile(INVENTORY_FILE, std::ios::app);
    if (outputFile.is_open())
    {
        outputFile << data.inventory[data.itemCount].id << " "
                   << data.inventory[data.itemCount].name << " "
                   << data.inventory[data.itemCount].price << " "
                   << data.inventory[data.itemCount].quantity << " "
                   << data.inventory[data.itemCount].category << std::endl;

        outputFile.close();
        CLEAR_SCREEN();

        std::cout << "Item added successfully!\n";
        (data.itemCount)++;
    }
    else
    {
        CLEAR_SCREEN();

        std::cerr << "Error: Could not open the file for writing.\n";
    }
}

void removeItem(std::vector<Item> &inventoryItems)
{
    CLEAR_SCREEN();

    if (inventoryItems.empty())
    {
        std::cout << "Inventory is empty. Nothing to remove.\n";
        return;
    }

    std::cout << "\nEnter the ID of the item to remove: ";
    int idToRemove;
    while (!(std::cin >> idToRemove))
    {
        CLEAR_SCREEN();
        std::cout << "Invalid input. Please enter a number for ID.\n\n";
        std::cout << "Enter the ID of the item to remove: ";
        while (std::cin.get() != '\n')
            ;
    }

    auto foundItem = std::find_if(inventoryItems.begin(), inventoryItems.end(),
                                  [idToRemove](const Item &item)
                                  { return item.getId() == idToRemove; });

    if (foundItem != inventoryItems.end())
    {
        inventoryItems.erase(foundItem);
        saveItemsToInventoryFile(inventoryItems);
        CLEAR_SCREEN();
        std::cout << "Item removed from the system successfully!\n";
    }
    else
    {
        CLEAR_SCREEN();
        std::cout << "Item not found in the system.\n";
    }
}

void displayInventory(const std::vector<Item> &inventoryItems)
{
    CLEAR_SCREEN();

    if (inventoryItems.empty())
    {
        std::cout << "Inventory is empty. Nothing to display.\n";
        return;
    }

    std::cout << "\nInventory items:\n\n";
    std::cout << "------------------------------------------------------------\n";
    if (!inventoryItems.empty())
    {
        std::cout << "Items loaded from the inventory file:" << std::endl;
        for (const auto &item : inventoryItems)
        {
            std::cout << "ID: " << item.getId() << ", Name: " << item.getName()
                      << ", Price: " << item.getPrice() << ", Quantity: " << item.getQuantity()
                      << ", Category: " << item.getCategory() << std::endl;
        }
    }
    std::cout << "------------------------------------------------------------\n";
}

void calculateInventoryTotalValue(const std::vector<Item> &inventoryItems);

User authenticateUser(UserFunctions &userFunctions)
{
    std::string username, password;
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;

    int userId = userFunctions.authenticateUser(username, password);

    if (userId != -1)
    {
        User foundUser = userFunctions.getUserByUsername(username);
        return foundUser;
    }
    else
    {
        return User("", "");
    }
}

#endif
