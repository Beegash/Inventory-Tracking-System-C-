
#include "ft_inventory.h"

Admin::Admin(const std::string &username, const std::string &password)
    : User(username, password) {}

void Admin::addItem(const Item &item)
{

    inventory.addItem(item);
}

void addItemDoublePointer(struct InventoryData **data)
{
	CLEAR_SCREEN();
	std::cout << "\nEnter item details:\n";

	if ((*data)->itemCount >= MAX_ITEMS)
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
		while (std::cin.get() != '\n')
			;
	}

	for (int i = 0; i < (*data)->itemCount; i++)
	{
		if ((*data)->inventory[i].id == newId)
		{
			CLEAR_SCREEN();
			std::cout << "Error: An item with the same ID already exists. Item not added.\n";
			return;
		}
	}

	std::cout << "Name: ";
	char newName[MAX_NAME];
	std::cin >> newName;
	for (int i = 0; i < (*data)->itemCount; i++)
	{
		if (strcmp((*data)->inventory[i].name, newName) == 0 || strcmp((*data)->inventory[i].name, newName) == 32 || strcmp((*data)->inventory[i].name, newName) == -32)
		{
			CLEAR_SCREEN();
			std::cout << "Error: An item with the same name already exists. Item not added.\n";
			return;
		}
	}

	std::cout << "Price: ";
	while (!(std::cin >> (*data)->inventory[(*data)->itemCount].price) || (*data)->inventory[(*data)->itemCount].price < 0)
	{
		CLEAR_SCREEN();
		std::cout << "Invalid input. Please enter a non-negative number for Price.\n\n";
		std::cout << "Enter item details:\n";
		std::cout << "ID: " << newId << "\n";
		std::cout << "Name: " << newName << "\n";
		std::cout << "Price: ";
		while (std::cin.get() != '\n')
			;
	}

	std::cout << "Quantity: ";
	while (!(std::cin >> (*data)->inventory[(*data)->itemCount].quantity) || (*data)->inventory[(*data)->itemCount].quantity < 0)
	{
		CLEAR_SCREEN();
		std::cout << "Invalid input. Please enter a non-negative number for Quantity.\n\n";
		std::cout << "Enter item details:\n";
		std::cout << "ID: " << newId << "\n";
		std::cout << "Name: " << newName << "\n";
		std::cout << "Price: " << (*data)->inventory[(*data)->itemCount].price << "\n";
		std::cout << "Quantity: ";
		while (std::cin.get() != '\n')
			;
	}

	std::cout << "Category: ";
	std::cin >> (*data)->inventory[(*data)->itemCount].category;

	(*data)->inventory[(*data)->itemCount].id = newId;
	strcpy((*data)->inventory[(*data)->itemCount].name, newName);
	((*data)->itemCount)++;

	CLEAR_SCREEN();
	std::cout << "Item added successfully!\n";
}

void removeItem(struct InventoryData *data)
{
	CLEAR_SCREEN();
	if (data->itemCount == 0)
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

	int foundIndex = -1;

	for (int i = 0; i < data->itemCount; i++)
	{
		if (data->inventory[i].id == idToRemove)
		{
			foundIndex = i;
			break;
		}
	}

	if (foundIndex != -1)
	{
		for (int i = foundIndex; i < data->itemCount - 1; i++)
		{
			data->inventory[i] = data->inventory[i + 1];
		}

		(data->itemCount)--;
		CLEAR_SCREEN();
		std::cout << "Item removed successfully!\n";
	}
	else
	{
		CLEAR_SCREEN();
		std::cout << "Item not found.\n";
	}
}

void Admin::removeItem(int itemId)
{

    inventory.removeItemById(itemId);
}

void Admin::updateItem(int itemId, const std::string &newName, float newPrice, int newQuantity, const std::string &newCategory)
{

    Item *itemToUpdate = inventory.findItemById(itemId);
    if (itemToUpdate != nullptr)
    {

        itemToUpdate->setName(newName);
        itemToUpdate->setPrice(newPrice);
        itemToUpdate->setQuantity(newQuantity);
        itemToUpdate->setCategory(newCategory);
        std::cout << "Item updated successfully." << std::endl;
    }
    else
    {
        std::cout << "Item not found in the inventory." << std::endl;
    }
}

void Admin::showUserList(const UserFunctions &userFunctions) const
{

    userFunctions.displayUserList();
}

void Admin::removeUser(UserFunctions &userFunctions, const std::string username)
{

    if (userFunctions.userExists(username))
    {
        userFunctions.removeUser(username);
        std::cout << "User removed successfully." << std::endl;
    }
    else
    {
        std::cout << "User not found." << std::endl;
    }
}

void Admin::saveUsersToFile(const UserFunctions &userFunctions) const
{

    userFunctions.saveUsersToFile();
    std::cout << "Users saved to file successfully." << std::endl;
}
