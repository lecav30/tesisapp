#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "backend_creator.hpp"
#include "backend_generator.hpp"

// int main() {
// BackendCreator creator = BackendCreator();
// BackendGenerator generator = BackendGenerator();

// creator.createBackend();
// generator.generateBackend();

//     return 0;
// }

struct Column {
    std::string name;
    std::string type;
    bool primaryKey;
    bool autoIncrement;
};

struct Table {
    std::string name;
    std::vector<Column> columns;
};

std::vector<Table> read_tables_from_file(const std::string& file_path) {
    std::ifstream file(file_path);
    std::vector<Table> tables;
    std::string line;
    while (std::getline(file, line)) {
        Table table;
        table.name = line;

        std::getline(file, line);
        int num_columns = std::stoi(line);

        for (int i = 0; i < num_columns; ++i) {
            std::getline(file, line);
            std::istringstream iss(line);
            Column column;
            std::string primaryKey, autoIncrement;
            iss >> column.name >> column.type >> primaryKey >> autoIncrement;
            column.primaryKey = (primaryKey == "true");
            column.autoIncrement = (autoIncrement == "true");
            table.columns.push_back(column);
        }

        tables.push_back(table);
    }
    return tables;
}

void generate_proto(const std::vector<Table>& tables, const std::string& file_path) {
    std::ofstream proto_file(file_path);
    proto_file << "syntax = \"proto3\";\n\n";

    proto_file << "message Column {\n";
    proto_file << "  string name = 1;\n";
    proto_file << "  string type = 2;\n";
    proto_file << "  bool primaryKey = 3;\n";
    proto_file << "  bool autoIncrement = 4;\n";
    proto_file << "}\n\n";

    proto_file << "message Table {\n";
    proto_file << "  string name = 1;\n";
    proto_file << "  repeated Column columns = 2;\n";
    proto_file << "}\n\n";

    proto_file << "message Database {\n";
    proto_file << "  repeated Table tables = 1;\n";
    proto_file << "}\n\n";

    proto_file << "message Database {\n";
    proto_file << "  repeated Table tables = 1;\n";
    proto_file << "}\n\n";

    proto_file.close();
}

int main() {
    std::string file_path = "tables_input.txt";
    std::vector<Table> tables = read_tables_from_file(file_path);

    // Imprimir para verificar
    for (const auto& table : tables) {
        std::cout << "Table: " << table.name << std::endl;
        for (const auto& column : table.columns) {
            std::cout << "  Column: " << column.name << ", Type: " << column.type
                      << ", Primary Key: " << (column.primaryKey ? "true" : "false")
                      << ", Auto Increment: " << (column.autoIncrement ? "true" : "false") << std::endl;
        }
    }

    // Generar archivo .proto
    generate_proto(tables, "db_structure.proto");

    return 0;
}