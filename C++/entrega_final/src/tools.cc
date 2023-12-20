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

/**
 * Función que muestra el uso del programa en caso de que se introduzca un argumento incorrecto o la opción --help
 * 
 * @param argc Número de argumentos
 * @param argv Argumentos
 * @param mode Variable tipo int donde se almacenará el modo de ejecución del programa (1 para escucha, 2 para envío, 3 para ayuda) 
 */
std::error_code Usage(int argc, char* argv[], int& mode, std::string& command, bool& cerrout, bool& cstdout) {
  mode = 2;
  bool using_c{false};
  for (int i{0}; i < argc; i++) { // Procesar argumentos
    if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h") {
      mode = 3;
      using_c = false;
      break;
    } else if ((std::string(argv[i]) == "--listen" || std::string(argv[i]) == "-l") && using_c == false) {
      mode = 1;
      using_c = false;
    } else if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--command") {
      using_c = true;
    } else {
      if (using_c) {
        command += argv[i];
        command += " ";
      }
      if (std::string(argv[i]) == "-1") {
        cstdout = true;
      } else if (std::string(argv[i]) == "-2") {
        cerrout = true;
      }
    }
  }
  if (mode == 1) { // Comprobar argumentos para el modo escucha
    if (argc != 3 && !using_c) {
      std::cerr << "Número de argumentos incorrecto para el modo escucha\n";
      std::cerr << "Uso: ./netcp [-h | --help] [--listen | -l] nombre_archivo\n";
      return std::make_error_code(std::errc(11));
    }
  } else if (mode == 2) { // Comprobar argumentos para el modo enviar
    if ((argc != 2)  && !using_c) {
      std::cerr << "Número de argumentos incorrecto para el modo enviar\n";
      std::cerr << "Uso: ./netcp nombre_archivo\n";
      return std::make_error_code(std::errc(12));
    }
  }
  if (mode == 3) { // Mostrar ayuda
    std::cout << "Este programa permite enviar la información de un archivo cualquiera desde una dirección ip pasada\n";
    std::cout << "por variables de entorno por un puerto también pasado de este modo, para recibir dicha infomación \n";
    std::cout << "se debe ejecutar el programa con la opción --listen o -l y el nombre del archivo en el que se guardará.\n";
    std::cout << "la información recibida.\n\n";
    std::cout << "Uso: ./netcp [-h | --help] [--listen | -l] nombre_archivo\n";
  }
  return std::make_error_code(std::errc(0));
}

/**
 * Manejador de señales por defecto para el programa netcp
 * 
 * @param signal Señal recibida (SIGINT, SIGTERM, SIGQUIT, SIGHUP)
 */
void SignalHandler(int signal) {
  std::string message;
  switch (signal) {
    case SIGINT:
      message = "[NETCP]: SEÑAL SIGINT (";
      message += std::to_string(signal);
      message += ") RECIBIDA TERMINANDO...\n";
      write(STDERR_FILENO, message.data(), message.size());
      quit_app = true;
      break;
    case SIGTERM:
      message = "[NETCP]: SEÑAL SIGTERM (";
      message += std::to_string(signal);
      message += ") RECIBIDA TERMINANDO...\n";
      write(STDERR_FILENO, message.data(), message.size());
      quit_app = true;
      break;
    case SIGQUIT:
      message = "[NETCP]: SEÑAL SIGQUIT (";
      message += std::to_string(signal);
      message += ") RECIBIDA TERMINANDO...\n";
      write(STDERR_FILENO, message.data(), message.size());
      quit_app = true;
      break;
    case SIGHUP:
      message = "[NETCP]: SEÑAL SIGHUP (";
      message += std::to_string(signal);
      message += ") RECIBIDA TERMINANDO...\n";
      write(STDERR_FILENO, message.data(), message.size());
      quit_app = true;
      break;
    default:
      message = "[NETCP]: SEÑAL DESCONOCIDA RECIBIDA\n";
      write(STDERR_FILENO, message.data(), message.size());
      break;
  }
}