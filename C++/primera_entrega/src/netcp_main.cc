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

/**
 * Función main del programa netcp
 * 
 * @param argc Número de argumentos
 * @param argv Vector de argumentos
 * 
 * @return 0 si se ejecuta correctamente, cualquier otro valor en caso contrario
 */
int main(int argc, char* argv[]) {
  // Creamos y leemos archivo
  File file{argv[1]};
  std::vector<uint8_t> buffer(16ul * 1024 * 1024);
  std::error_code error = file.read_file(file.GetFileDescriptor(), buffer);
  if (error) {
    std::cerr << "Error reading file: " << error.message() << std::endl;
    return 1;
  }
  // Enviamos la información del archivo
  Socket socket{std::nullopt, 8080};
  socket.send_to(socket.GetFileDescriptor(), buffer, socket.make_ip_address("192.168.1.38", 8080).value());
  std::cout << std::endl;
  return 0;
}