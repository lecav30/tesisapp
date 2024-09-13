// #include <mysqlx/xdevapi.h>
//
// #include <boost/algorithm/string.hpp>
// #include <fstream>
// #include <inja/inja.hpp>
// #include <iostream>
// #include <nlohmann/json.hpp>
// #include <string>
//
// // Structure to hold credentials
// struct Credentials {
//     std::string user;
//     std::string password;
//     std::string dbname;
// };
// // Structure to hold column information
// struct Column {
//     std::string name;
//     std::string type;
//     std::string null;
//     bool isForeignKey;
//     std::string foreignKeyTable;
// };
//
// // Structure to hold table information
// struct Table {
//     std::string name;
//     std::vector<Column> columns;
// };
//
// // Function to read credentials from a text file
// bool readCredentials(const std::string &filename, Credentials &credentials) {
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         std::cerr << "Unable to open file: " << filename << std::endl;
//         return false;
//     }
//
//     std::getline(file, credentials.user);
//     std::getline(file, credentials.password);
//     std::getline(file, credentials.dbname);
//
//     file.close();
//     return true;
// }
//
// // Function to connect to the database using X DevAPI
// bool connectToDatabase(const Credentials &credentials) {
//     try {
//         mysqlx::Session session("localhost", 33060, credentials.user,
//                                 credentials.password);
//         mysqlx::Schema schema = session.getSchema(credentials.dbname);
//
//         std::cout << "Connected to database successfully!" << std::endl;
//         return true;
//     } catch (const mysqlx::Error &err) {
//         std::cerr << "Error: " << err.what() << std::endl;
//         return false;
//     } catch (std::exception &ex) {
//         std::cerr << "STD Exception: " << ex.what() << std::endl;
//         return false;
//     } catch (const char *ex) {
//         std::cerr << "Exception: " << ex << std::endl;
//         return false;
//     }
// }
//
// // Function to read the schema of the database
// bool readDatabaseSchema(const std::string &filename,
//                         std::vector<Table> &tables) {
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         std::cerr << "Unable to open file: " << filename << std::endl;
//         return false;
//     }
//
//     std::string amountOfTables;
//     std::getline(file, amountOfTables);
//
//     for (int i = 0; i < std::stoi(amountOfTables); i++) {
//         Table table;
//         std::getline(file, table.name);
//
//         std::string amountOfColumns;
//         std::getline(file, amountOfColumns);
//
//         for (int j = 0; j < std::stoi(amountOfColumns); j++) {
//             Column column;
//             std::getline(file, column.name);
//             std::getline(file, column.type);
//             std::getline(file, column.null);
//             std::string temp;
//             std::getline(file, temp);
//             column.isForeignKey = temp == "true";
//             if (column.isForeignKey) {
//                 std::getline(file, column.foreignKeyTable);
//             } else {
//                 column.foreignKeyTable = "";
//             }
//             table.columns.push_back(column);
//         }
//
//         tables.push_back(table);
//     }
//
//     // Print the tables and their columns
//     for (const auto &table : tables) {
//         std::cout << "Table: " << table.name << std::endl;
//         for (const auto &column : table.columns) {
//             std::cout << "  Column: " << column.name << " (" << column.type
//                       << ", " << column.null << ")" << std::endl;
//             if (column.isForeignKey) {
//                 std::cout << "    Foreign Key: " << column.foreignKeyTable
//                           << std::endl;
//             }
//         }
//     }
//
//     file.close();
//     return true;
// }
//
// // Función para dividir las sentencias SQL en partes individuales
// std::vector<std::string> splitSqlStatements(const std::string &sqlStatements) {
//     std::vector<std::string> statements;
//     std::istringstream stream(sqlStatements);
//     std::string statement;
//
//     while (std::getline(stream, statement, ';')) {
//         boost::trim(statement);
//         if (!statement.empty()) {
//             statements.push_back(statement + ";");
//         }
//     }
//
//     return statements;
// }
//
// // Function to create the database and tables
// bool createDatabase(const Credentials &credentials,
//                     const std::vector<Table> &tables) {
//     try {
//         mysqlx::Session session("localhost", 33060, credentials.user,
//                                 credentials.password, credentials.dbname);
//
//         inja::Environment env;
//         env.set_trim_blocks(true);
//         env.set_lstrip_blocks(true);
//
//         // Leer la plantilla desde el archivo
//         std::ifstream template_file("./sql_script.sql");
//         if (!template_file.is_open()) {
//             std::cerr << "Error: No se pudo abrir el archivo de plantilla "
//                          "'slq_script.sql'"
//                       << std::endl;
//         }
//         std::string templateString(
//             (std::istreambuf_iterator<char>(template_file)),
//             std::istreambuf_iterator<char>());
//         if (templateString.empty()) {
//             std::cerr << "Error: La plantilla está vacía o no se pudo leer "
//                          "correctamente"
//                       << std::endl;
//         }
//
//         std::string sqlStatements = "";
//
//         auto tablesToJson = [&tables]() {
//             inja::json tablesJson = inja::json::array();
//             for (const Table &table : tables) {
//                 inja::json tableJson;
//                 tableJson["name"] = table.name;
//                 std::cout << "Table: " << table.name << std::endl;
//                 inja::json columnsJson = inja::json::array();
//                 inja::json foreignKeysJson = inja::json::array();
//                 for (const Column &column : table.columns) {
//                     inja::json columnJson;
//                     columnJson["name"] = column.name;
//                     columnJson["type"] = column.type;
//                     if (column.isForeignKey) {
//                         columnJson["null"] = "";
//                         inja::json foreignColumnJson;
//                         foreignColumnJson["name"] = column.name;
//                         foreignColumnJson["table"] = column.foreignKeyTable;
//                         foreignKeysJson.push_back(foreignColumnJson);
//                     } else {
//                         columnJson["null"] = column.null;
//                     }
//                     columnsJson.push_back(columnJson);
//                 }
//                 tableJson["columns"] = columnsJson;
//                 tableJson["foreign_keys"] = foreignKeysJson;
//                 tablesJson.push_back(tableJson);
//             }
//             return tablesJson;
//         };
//
//         inja::json tablesJson = tablesToJson();
//
//         for (const auto &tableJson : tablesJson) {
//             sqlStatements += env.render(templateString, tableJson);
//         }
//
//         // std::cout << sqlStatements << std::endl;
//
//         // Dividir las sentencias SQL en partes individuales
//         std::vector<std::string> statements = splitSqlStatements(sqlStatements);
//
//         // Execute SQL statements
//         try {
//             // Ejecutar cada sentencia individualmente
//             for (const auto &statement : statements) {
//                 session.sql(statement).execute();
//                 // std::cout << statement << std::endl;
//             }
//             std::cout << "Tables created successfully!" << std::endl;
//             return true;
//         } catch (const mysqlx::Error &err) {
//             std::cerr << "Error: " << err.what() << std::endl;
//             return false;
//         } catch (std::exception &ex) {
//             std::cerr << "STD Exception: " << ex.what() << std::endl;
//             return false;
//         } catch (const char *ex) {
//             std::cerr << "Exception: " << ex << std::endl;
//             return false;
//         }
//
//         return true;
//     } catch (const mysqlx::Error &err) {
//         std::cerr << "Error: " << err.what() << std::endl;
//         return false;
//     } catch (std::exception &ex) {
//         std::cerr << "STD Exception: " << ex.what() << std::endl;
//         return false;
//     } catch (const char *ex) {
//         std::cerr << "Exception: " << ex << std::endl;
//         return false;
//     }
// }

// int main() {
//     // std::string user, password, dbname;
//     std::string credentialsFile = "credentials.txt";
//     std::string schemaFile = "db_data.txt";
//     Credentials credentials;
//     std::vector<Table> tables = std::vector<Table>();
//
//     // Request credentials
//     if (!readCredentials(credentialsFile, credentials)) {
//         return 1;
//     }
//
//     // Test credentials
//     if (!connectToDatabase(credentials)) {
//         return 1;
//     }
//
//     // Read database schema from file
//     if (!readDatabaseSchema(schemaFile, tables)) {
//         return 1;
//     }
//
//     // Create database and tables
//     if (!createDatabase(credentials, tables)) {
//         return 1;
//     }
//
//     // Create models to ExpressJS
//
//
//     return 0;
// }
