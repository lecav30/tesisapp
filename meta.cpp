#include <iostream>
#include <string>
#include <vector>

// Estructura para almacenar la información de una ruta
struct Route {
     std::string method;
     std::string path;
     std::string response;
};

// Plantilla para generar el código de una ruta
template <typename T>
struct RouteTemplate {
     static std::string generate(const T& route) {
          return "app." + route.method + "('" + route.path +
                 "', (req, res) => {\n"
                 "    res.send('" +
                 route.response +
                 "');\n"
                 "});\n";
     }
};

// Plantilla para generar el código del servidor
template <typename T>
struct ServerTemplate {
     static std::string generate(const std::string& port, const std::vector<T>& routes) {
          std::string serverCode =
              "const express = require('express');\n"
              "const app = express();\n"
              "const port = " +
              port +
              ";\n\n"
              "app.use(express.json());\n\n";

          // Generar código para cada ruta
          for (const auto& route : routes) {
               serverCode += RouteTemplate<T>::generate(route);
          }

          serverCode +=
              "\napp.listen(port, () => {\n"
              "    console.log(`Server is running on port ${port}`);\n"
              "});\n";

          return serverCode;
     }
};

int main() {
     std::string port;
     std::vector<Route> routes;
     int numRoutes;

     // Capturar el puerto del usuario
     std::cout << "Ingrese el puerto del servidor: ";
     std::cin >> port;

     // Capturar el número de rutas
     std::cout << "Ingrese el número de rutas: ";
     std::cin >> numRoutes;

     // Capturar los detalles de cada ruta
     for (int i = 0; i < numRoutes; ++i) {
          Route route;
          std::cout << "Ingrese el método de la ruta (get, post, etc.): ";
          std::cin >> route.method;
          std::cout << "Ingrese el path de la ruta: ";
          std::cin >> route.path;
          std::cout << "Ingrese la respuesta de la ruta: ";
          std::cin.ignore();  // Ignorar el salto de línea pendiente
          std::getline(std::cin, route.response);
          routes.push_back(route);
     }

     // Generar el código del servidor
     std::string serverCode = ServerTemplate<Route>::generate(port, routes);

     // Imprimir el código generado
     std::cout << "\nCódigo generado:\n"
               << serverCode << std::endl;

     return 0;
}