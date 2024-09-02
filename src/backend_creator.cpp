#include "backend_creator.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

BackendCreator::BackendCreator() {}

BackendCreator::~BackendCreator() {}

void BackendCreator::createBackend() {
    const std::string directory = "/Users/lecav/Programs/Test";
    std::string projectName = "myProject";

    std::cout << "Ingrese el nombre del proyecto: ";
    std::cin >> projectName;

    const std::string projectPath = directory + "/" + projectName;

    // Ensure the directory exists
    std::filesystem::create_directories(directory + "/" + projectName);

    // Construct the command to create the file
    const std::string command = "cd " + projectPath + " && npm init -y && npm install express";

    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Comando ejecutado exitosamente en " << projectPath << std::endl;
    } else {
        std::cout << "Error al ejecutar el comando en " << projectPath << std::endl;
    }
}