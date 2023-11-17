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

int main() {
  File file{"testfile"};
  std::vector<uint8_t> buffer(16ul * 1024 * 1024);
  std::error_code error = file.read_file(file.GetFileDescriptor(), buffer);
  if (error) {
    std::cerr << "Error reading file: " << error.message() << std::endl;
    return 1;
  }
  std::cout << "File contents: " << std::endl;
  file.PrintFile();
  std::cout << std::endl;
  return 0;
}