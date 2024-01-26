#include "ft_inventory.h"

void UserFunctions::addUser(const std::string &username, const std::string &password, bool isAdmin)
{

    User newUser(username, password);
    if (isAdmin)
    {

        Admin newAdmin(username, password);
    }

    users.push_back(newUser);
}

bool UserFunctions::userExists(const std::string &username) const
{

    for (const User &user : users)
    {
        if (user.getUsername() == username)
        {
            return true;
        }
    }
    return false;
}

void UserFunctions::displayUserList() const
{

    std::cout << "User List:" << std::endl;
    for (const User &user : users)
    {
        std::cout << "- " << user.getUsername() << std::endl;
    }
}

void UserFunctions::removeUser(const std::string &username)
{

    auto it = std::remove_if(users.begin(), users.end(), [&](const User &user)
                             { return user.getUsername() == username; });
    users.erase(it, users.end());

    std::cout << "User removed successfully." << std::endl;
}

int UserFunctions::authenticateUser(const std::string &username, const std::string &password) const
{

    for (size_t i = 0; i < users.size(); ++i)
    {
        if (users[i].getUsername() == username && users[i].getPassword() == password)
        {
            return i;
        }
    }
    return -1;
}

void UserFunctions::loadUsersFromFile()
{
    std::ifstream inputFile("users.txt");
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    users.clear();

    std::string username, password, isAdminStr;
    bool isAdmin;

    while (inputFile >> username >> password >> isAdminStr)
    {
        isAdmin = (isAdminStr == "1");

        addUser(username, password, isAdmin);
    }

    std::cout << "Users loaded from file successfully." << std::endl;
    inputFile.close();
}

void UserFunctions::saveUsersToFile() const
{
    std::ofstream outputFile("users.txt");
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Could not create/open the file." << std::endl;
        return;
    }

    for (const User &user : users)
    {

        outputFile << user.getUsername() << " "
                   << user.getPassword() << " "
                   << ((dynamic_cast<const Admin *>(&user) != nullptr) ? "1" : "0")
                   << std::endl;
    }

    std::cout << "Users saved to file successfully." << std::endl;
    outputFile.close();
}

int UserFunctions::getUserCount() const
{
    return users.size();
}

int UserFunctions::getAdminCount() const
{

    int adminCount = 0;
    for (const User &user : users)
    {
        const Admin *adminUser = dynamic_cast<const Admin *>(&user);
        if (adminUser)
        {
            adminCount++;
        }
    }
    return adminCount;
}

void UserFunctions::countUsersInFile() const
{
    std::ifstream inputFile("users.txt");
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    int userCount = 0;
    std::string line;

    while (std::getline(inputFile, line))
    {
        userCount++;
    }

    std::cout << "Number of users in the file: " << userCount << std::endl;
    inputFile.close();
}

User UserFunctions::getUserByUsername(const std::string &username) const
{
    for (const User &user : users)
    {
        if (user.getUsername() == username)
        {
            return user;
        }
    }
    return User("", "");
}

