#include <fmt/core.h>
#include <mysqlx/xdevapi.h>

#include <fstream>
#include <inja/inja.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

// Structure to hold credentials
struct Credentials {
    std::string user;
    std::string password;
    std::string dbname;
};
// Structure to hold column information
struct Column {
    std::string name;
    std::string type;
    bool isNull;
    bool isUnique;
};

// Structure to hold table information
struct Table {
    std::string name;
    std::vector<Column> columns;
};

// Function to read credentials from a text file
bool readCredentials(const std::string &filename, Credentials &credentials) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    std::getline(file, credentials.user);
    std::getline(file, credentials.password);
    std::getline(file, credentials.dbname);

    file.close();
    return true;
}

// Function to connect to the database using X DevAPI
bool connectToDatabase(const Credentials &credentials) {
    try {
        mysqlx::Session session("localhost", 33060, credentials.user,
                                credentials.password);
        mysqlx::Schema schema = session.getSchema(credentials.dbname);

        std::cout << "Connected to database successfully!" << std::endl;
        return true;
    } catch (const mysqlx::Error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return false;
    } catch (std::exception &ex) {
        std::cerr << "STD Exception: " << ex.what() << std::endl;
        return false;
    } catch (const char *ex) {
        std::cerr << "Exception: " << ex << std::endl;
        return false;
    }
}

// Function to read the schema of the database
bool readDatabaseSchema(const std::string &filename,
                        std::vector<Table> &tables) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    std::string amountOfTables;
    std::getline(file, amountOfTables);

    for (int i = 0; i < std::stoi(amountOfTables); i++) {
        Table table;
        std::getline(file, table.name);

        std::string amountOfColumns;
        std::getline(file, amountOfColumns);

        for (int j = 0; j < std::stoi(amountOfColumns); j++) {
            Column column;
            std::string tempStr;
            std::getline(file, column.name);
            std::getline(file, column.type);
            std::getline(file, tempStr);
            column.isNull = tempStr == "true";
            std::getline(file, tempStr);
            column.isUnique = tempStr == "true";
            table.columns.push_back(column);
        }

        tables.push_back(table);
    }

    // Print the tables and their columns
    for (const auto &table : tables) {
        std::cout << "Table: " << table.name << std::endl;
        for (const auto &column : table.columns) {
            std::cout << "  Column: " << column.name << " - " << column.type
                      << ", " << (column.isNull ? "NULL" : "NOT NULL") << ", "
                      << (column.isUnique ? "UNIQUE" : "NOT UNIQUE")
                      << std::endl;
        }
    }

    file.close();
    return true;
}

bool createModel(const std::string &filename, Credentials &credentials,
                 std::vector<Table> &tables) {
    try {
        mysqlx::Session session("localhost", 33060, credentials.user,
                                credentials.password, credentials.dbname);

        inja::Environment env;
        env.set_trim_blocks(true);
        env.set_lstrip_blocks(true);

        // Leer la plantilla desde el archivo
        std::ifstream template_file("./inja_templates/models.inja");
        if (!template_file.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo de plantilla "
                         "'models.inja'"
                      << std::endl;
        }
        std::string templateString(
            (std::istreambuf_iterator<char>(template_file)),
            std::istreambuf_iterator<char>());
        if (templateString.empty()) {
            std::cerr << "Error: La plantilla está vacía o no se pudo leer "
                         "correctamente"
                      << std::endl;
        }

        auto tablesToJson = [&tables]() {
            inja::json tablesJson = inja::json::array();
            for (const Table &table : tables) {
                inja::json tableJson;
                tableJson["name"] = table.name;
                std::cout << "Table: " << table.name << std::endl;
                inja::json columnsJson = inja::json::array();
                for (const Column &column : table.columns) {
                    inja::json colJson;
                    colJson["name"] = column.name;
                    colJson["type"] = column.type;
                    colJson["isNull"] = column.isNull;
                    colJson["isUnique"] = column.isUnique;
                    columnsJson.push_back(colJson);
                }
                tableJson["fields"] = columnsJson;
                tablesJson.push_back(tableJson);
            }
            return tablesJson;
        };

        inja::json tablesJson = tablesToJson();

        // Create files of models for each table
        for (const auto &tableJson : tablesJson) {
            std::string result = env.render(templateString, tableJson);

            // fmt::print("{}", result);

            std::ofstream file("./output/"
                               + tableJson["name"].get<std::string>() + ".js");
            if (!file.is_open()) {
                std::cerr << "Error: No se pudo abrir el archivo de salida "
                             "'output/"
                                 + tableJson["name"].get<std::string>() + ".js'"
                          << std::endl;
            }
            file << result;
            file.close();

            fmt::print("Model for table {} created successfully!\n",
                       tableJson["name"].get<std::string>());
        }

    } catch (const mysqlx::Error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return false;
    } catch (std::exception &ex) {
        std::cerr << "STD Exception: " << ex.what() << std::endl;
        return false;
    } catch (const char *ex) {
        std::cerr << "Exception: " << ex << std::endl;
        return false;
    }

    return true;
}

int main() {
    // std::string user, password, dbname;
    std::string credentialsFile = "input/credentials.txt";
    std::string schemaFile = "input/models.txt";
    Credentials credentials;
    std::vector<Table> tables = std::vector<Table>();

    // Request credentials
    if (!readCredentials(credentialsFile, credentials)) {
        return 1;
    }

    // Test credentials
    if (!connectToDatabase(credentials)) {
        return 1;
    }

    // Read database schema from file
    if (!readDatabaseSchema(schemaFile, tables)) {
        return 1;
    }

    // Create models to ExpressJS
    if (!createModel(schemaFile, credentials, tables)) {
        return 1;
    }

    return 0;
}
