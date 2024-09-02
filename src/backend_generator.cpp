#include "backend_generator.hpp"
#include <fstream>
#include <inja/inja.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

BackendGenerator::BackendGenerator() {}

BackendGenerator::~BackendGenerator() {}

// Definir la estructura Route
struct Route {
    std::string method;
    std::string path;
    std::string response;
};

// Definir la función de conversión para la estructura Route
void to_json(nlohmann::json& j, const Route& r) {
    j = nlohmann::json{{"method", r.method}, {"path", r.path}, {"response", r.response}};
}

void BackendGenerator::generateBackend() {
    std::string port;
    std::vector<Route> routes;
    int numRoutes;

    // Capturar el puerto del servidor
    // std::cout << "Ingrese el puerto del servidor: ";
    // std::cin >> port;

    // Capturar el número de rutas
    // std::cout << "Ingrese el número de rutas: ";
    // std::cin >> numRoutes;

    // Capturar los detalles de cada ruta
    // for (int i = 0; i < numRoutes; ++i) {
    //     Route route;
    //     std::cout << "Ingrese el método de la ruta (get, post, etc.): ";
    //     std::cin >> route.method;
    //     std::cout << "Ingrese el path de la ruta: ";
    //     std::cin >> route.path;
    //     std::cout << "Ingrese la respuesta de la ruta: ";
    //     std::cin.ignore();  // Ignorar el salto de línea pendiente
    //     std::getline(std::cin, route.response);
    //     routes.push_back(route);
    // }

    // Crear el entorno de inja
    inja::Environment env;

    // Leer la plantilla desde el archivo .inja
    std::ifstream template_file("./template.inja");
    if (!template_file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de plantilla 'template.inja'" << std::endl;
    }
    std::string template_text((std::istreambuf_iterator<char>(template_file)),
                              std::istreambuf_iterator<char>());
    if (template_text.empty()) {
        std::cerr << "Error: La plantilla está vacía o no se pudo leer correctamente" << std::endl;
    }

    // Crear datos JSON
    nlohmann::json data;
    data["port"] = port;
    data["routes"] = routes;

    // Renderizar la plantilla con los datos
    std::string result;
    try {
        result = env.render(template_text, data);
    } catch (const std::exception& e) {
        std::cerr << "Error al renderizar la plantilla: " << e.what() << std::endl;
    }

    // Imprimir el código generado
    std::cout << "\nCódigo generado:\n"
              << result << std::endl;
}