#include <mysqlx/xdevapi.h>
#include <fstream>
#include <iostream>
#include <string>

// Function to read credentials from a text file
bool readCredentials(const std::string& filename, std::string& user, std::string& password, std::string& dbname) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    std::getline(file, user);
    std::getline(file, password);
    std::getline(file, dbname);

    file.close();
    return true;
}

// Function to connect to the database using X DevAPI
bool connectToDatabase(const std::string& user, const std::string& password, const std::string& dbname) {
    try {
        mysqlx::Session session("localhost", 33060, user, password);
        mysqlx::Schema schema = session.getSchema(dbname);

        std::cout << "Connected to database successfully!" << std::endl;
        return true;
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return false;
    } catch (std::exception& ex) {
        std::cerr << "STD Exception: " << ex.what() << std::endl;
        return false;
    } catch (const char* ex) {
        std::cerr << "Exception: " << ex << std::endl;
        return false;
    }
}

int main() {
    std::string user, password, dbname;
    std::string filename = "credentials.txt";

    if (!readCredentials(filename, user, password, dbname)) {
        return 1;
    }

    if (!connectToDatabase(user, password, dbname)) {
        return 1;
    }

    return 0;
}