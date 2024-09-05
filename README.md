# Test

## Instalación de vcpkg

Para poder compilar este proyecto, necesitas instalar `vcpkg`. Sigue estos pasos:

1. Clona el repositorio de `vcpkg`:

   ```sh
   git clone https://github.com/microsoft/vcpkg.git
   ```

2. Navega al directorio de `vcpkg`:

   ```sh
   cd vcpkg
   ```

3. Ejecuta el script de bootstrap para compilar `vcpkg`:
   ```sh
   ./bootstrap-vcpkg.sh
   ```

## Instalación de paquetes

1. Crear la carpeta del build

   ```sh
   mkdir build
   ```

2. Ejecu el comando para build
   ```sh
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
   ```

`cd build`

`make`
