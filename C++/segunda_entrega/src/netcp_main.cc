/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 27 Nov 2023
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
  // Argumentos y uso básico
  // Usage(argc, argv);
  bool recieve_mode{false};
  std::string file_name;
  for (int i{1}; i < argc; ++i) { // Manejo de argumentos para ver en que modo se ejecuta el programa
    std::string parameter{argv[i]};
    if (parameter == "-l") {
      recieve_mode = true;
    } else {
      file_name = parameter;
    }
  }
  // Enviar / Recibir mensajes
  if (recieve_mode) {
    File file(file_name, 1);
    Socket socket{"10.0.2.15", 8080};
    sockaddr_in remote_address{};
    std::string message{socket.Recieve(remote_address)};
    file.WriteFile(message);
  } else {
    File file(file_name, 0);
  }
  return 0;
}