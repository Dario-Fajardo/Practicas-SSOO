/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Archivo main del programa netcp.
 */

#include "../include/file.h"
#include "../include/socket.h"
#include "../include/tools.h"

/**
 * Función main del programa netcp
 * 
 * @param argc Número de argumentos
 * @param argv Vector de argumentos
 * 
 * @return 0 si se ejecuta correctamente, cualquier otro valor en caso contrario
 */
int main(int argc, char* argv[]) {
  Usage(argc, argv); 
  std::string nombre_archivo;
  bool soption{false};
  for (int i{1}; i < argc; ++i) { // Comprobamos parámetros
    std::string parameter{argv[i]};
    if (parameter == "-s" || parameter == "--size") {
      soption = true;
    } else {
      nombre_archivo = parameter;
    }
  }
  // Creamos y leemos archivo
  File file{nombre_archivo};
  // Enviamos la información del archivo
  Socket socket{std::nullopt, 8080};
  socket.send_to(socket.GetFileDescriptor(), file.GetBuffer(), socket.make_ip_address("127.0.0.1", 8080).value());
  if (soption) { // Si se hubiera introducido -s o --size se imprime el tamaño con el nombre del archivo
    std::cout << "Se ha enviado el fichero con nombre " << nombre_archivo << "\n";
    std::cout << "Que tiene " << file.GetFileInfo().st_size << " bytes\n";
  }
  std::cout << std::endl;
  return 0;
}