#include "ft_inventory.h"

User::User(const std::string &username, const std::string &password)
	: username(username), password(password) {}

const std::string &User::getUsername() const
{
	return username;
}

const std::string &User::getPassword() const
{
	return password;
}

Inventory &User::getInventory()
{
	return inventory;
}

void User::addItemToInventory(const Item &item)
{
	inventory.addItem(item);
}

bool User::removeItemFromInventory(int itemId, std::vector<Item> &inventoryItems)
{
	auto it = std::find_if(inventory.items.begin(), inventory.items.end(),
						   [itemId](const Item &item)
						   { return item.getId() == itemId; });

	if (it != inventory.items.end())
	{
		auto foundItem = std::find_if(inventoryItems.begin(), inventoryItems.end(),
									  [itemId](const Item &item)
									  { return item.getId() == itemId; });

		if (foundItem != inventoryItems.end())
		{
			foundItem->setQuantity(foundItem->getQuantity() + 1);
		}

		inventory.items.erase(it);
		saveItemsToInventoryFile(inventoryItems);
		return 1;
	}
	else
	{
		std::cout << "Item with ID " << itemId << " not found in your inventory." << std::endl;
		return 0;
	}
	return 0;
}
void User::displayUserInventory() const
{
	inventory.displayInventory();
}

void User::showItemDetails(std::vector<Item> &inventoryItems) const
{

	if (inventoryItems.empty())
	{
		std::cout << "Inventory is empty. Nothing to update.\n";
		return;
	}

	std::cout << "\nEnter the ID of the item to update: ";
	int idToUpdate;
	while (!(std::cin >> idToUpdate))
	{
		CLEAR_SCREEN();
		std::cout << "Invalid input. Please enter a number for ID.\n\n";
		std::cout << "Enter the ID of the item to update: ";
		while (std::cin.get() != '\n')
			;
	}

	auto foundItem = std::find_if(inventoryItems.begin(), inventoryItems.end(),
								  [idToUpdate](const Item &item)
								  { return item.getId() == idToUpdate; });

	if (foundItem != inventoryItems.end())
	{
		std::cout << "\nCurrent Item Details:\n";
		std::cout << "ID: " << foundItem->getId() << ", Name: " << foundItem->getName()
				  << ", Price: " << foundItem->getPrice() << ", Quantity: " << foundItem->getQuantity()
				  << ", Category: " << foundItem->getCategory() << std::endl;
	}
}

void User::saveToFile() const
{
	std::ofstream outputFile(username + ".txt");

	if (!outputFile.is_open())
	{
		std::cerr << "Error: Could not create/open the file." << std::endl;
		return;
	}

	outputFile << "Username: " << username << std::endl;
	outputFile << "Password: " << password << std::endl;

	outputFile << "Inventory:" << std::endl;
	for (int i = 0; i < inventory.getItemCount(); ++i)
	{
		const Item &item = inventory.getItem(i);
		outputFile << "ID: " << item.getId() << ", Name: " << item.getName()
				   << ", Price: " << item.getPrice() << ", Quantity: " << item.getQuantity()
				   << ", Category: " << item.getCategory() << std::endl;
	}

	std::cout << "User data and inventory saved to file successfully." << std::endl;
	outputFile.close();
}
void User::loadInventoryFromFile(const std::string &fileName)
{
	std::vector<Item> itemsFromFile = readInventoryFromFile(fileName);

	for (const auto &item : itemsFromFile)
	{
		inventory.addItem(item);
	}
}

void User::calculateInventoryTotal() const
{

	float totalValue = 0.0;

	for (int i = 0; i < inventory.getItemCount(); ++i)
	{
		const Item &item = inventory.getItem(i);
		totalValue += item.getPrice() * item.getQuantity();
	}

	std::cout << "Total value of the inventory: " << totalValue << std::endl;
}
void User::updateItem(std::vector<Item> &inventoryItems)
{
	CLEAR_SCREEN();

	if (inventoryItems.empty())
	{
		std::cout << "Inventory is empty. Nothing to update.\n";
		return;
	}

	std::cout << "\nEnter the ID of the item to update: ";
	int idToUpdate;
	while (!(std::cin >> idToUpdate))
	{
		CLEAR_SCREEN();
		std::cout << "Invalid input. Please enter a number for ID.\n\n";
		std::cout << "Enter the ID of the item to update: ";
		while (std::cin.get() != '\n')
			;
	}

	auto foundItem = std::find_if(inventoryItems.begin(), inventoryItems.end(),
								  [idToUpdate](const Item &item)
								  { return item.getId() == idToUpdate; });

	if (foundItem != inventoryItems.end())
	{
		std::cout << "\nCurrent Item Details:\n";
		std::cout << "ID: " << foundItem->getId() << ", Name: " << foundItem->getName()
				  << ", Price: " << foundItem->getPrice() << ", Quantity: " << foundItem->getQuantity()
				  << ", Category: " << foundItem->getCategory() << std::endl;

		std::cout << "\nEnter new details:\n";

		std::string newName, newCategory;
		float newPrice;
		int newQuantity;

		std::cout << "Enter new name: ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, newName);

		std::cout << "Enter new price: ";
		while (!(std::cin >> newPrice) || newPrice < 0)
		{
			CLEAR_SCREEN();
			std::cout << "Invalid input. Please enter a non-negative number for Price.\n\n";
			std::cout << "Enter new price: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		std::cout << "Enter new quantity: ";
		while (!(std::cin >> newQuantity) || newQuantity < 0)
		{
			CLEAR_SCREEN();
			std::cout << "Invalid input. Please enter a non-negative number for Quantity.\n\n";
			std::cout << "Enter new quantity: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		std::cout << "Enter new category: ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, newCategory);

		foundItem->setName(newName);
		foundItem->setPrice(newPrice);
		foundItem->setQuantity(newQuantity);
		foundItem->setCategory(newCategory);

		saveItemsToInventoryFile(inventoryItems);

		std::cout << "Item updated successfully.\n";
	}
	else
	{
		CLEAR_SCREEN();
		std::cout << "Item not found in the system.\n";
	}
}

void User::updateItem()
{

	std::cout << "Updating an item is not allowed for regular users." << std::endl;
}

void User::removeUser()
{

	std::cout << "Removing a user is not allowed for regular users." << std::endl;
}

void User::setUsername(const std::string &newUsername)
{
	username = newUsername;
}

void User::setPassword(const std::string &newPassword)
{
	password = newPassword;
}

void removeUser(UserFunctions &userFunctions, const std::string username)
{
    Admin admin("admin", "admin"); 
    admin.removeUser(userFunctions, username);
}

void saveUsersToFile(const UserFunctions &userFunctions)
{
    Admin admin("admin", "admin");
    admin.saveUsersToFile(userFunctions);
}

void showUserList(const UserFunctions &userFunctions)
{
    Admin admin("admin", "admin");
    admin.showUserList(userFunctions);
} 