/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Archivo tools.h para implementar las funciones auxiliares del programa netcp.
 */

#include "../include/tools.h"

void Usage(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    std::cerr << argv[0] << ": Falta un número como parámetro." << std::endl;
    std::cerr << "Pruebe " << argv[0] << " --help para más información." << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string parameter{argv[1]};
  if (parameter == "--help" || parameter == "-h") {
    std::cout << "Este programa recibe como parámetro un archivo y lo envía a la dirección IP de LOOPBACK del sistema.\n";
    std::cout << "Sintaxis: " << argv[0] << " <nombre_archivo>\n";
    std::cout << "Los archivos solo pueden ser de máximo 1KB. Para ver que se reciba, corra en otra terminal el comando netcat -lu 8080\n";
    exit(EXIT_SUCCESS);
  }
}