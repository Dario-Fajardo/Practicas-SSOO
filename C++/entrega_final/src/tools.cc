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
      std::cerr << "Uso: ./netcp [--listen | -l] ARCHIVO o ./netcp -l -c COMANDO [ARG...]\n";
      std::cerr << "Utilice opción -h o --help para mostrar la ayuda\n";
      return std::make_error_code(std::errc(11));
    }
  } else if (mode == 2) { // Comprobar argumentos para el modo enviar
    if ((argc != 2)  && !using_c) {
      std::cerr << "Número de argumentos incorrecto para el modo enviar\n";
      std::cerr << "Uso: ./netcp ARCHIVO o ./netcp [-1] [-2] -c COMANDO [ARG...]\n";
      std::cerr << "Utilice opción -h o --help para mostrar la ayuda\n";
      return std::make_error_code(std::errc(12));
    }
  }
  if (mode == 3) { // Mostrar ayuda
    std::cout << "------------------------------------------------------------------------------------------\n";
    std::cout << "FUNCIONAMIENTO:\n";
    std::cout << "------------------------------------------------------------------------------------------\n";
    std::cout << "Este programa tiene dos modos de funcionamiento, uno de envío de datos y otro de escucha de los mismos, en el primero de estos\n";
    std::cout << "se envía un archivo a través de una conexión a un puerto determinado, mientras que en el segundo se escucha en un puertohasta recibir datos\n";
    std::cout << "y se almacenan en un archivo.\n\n";
    std::cout << "Otra manera de usar el programa es, en el envío, leer los datos de la entrada estándar (y/o error estándar) y enviarlos a través de la conexión\n";
    std::cout << "y en el modo escucha, escribir los datos recibidos en la entrada estándar de otro comando.\n\n";
    std::cout << "------------------------------------------------------------------------------------------\n";
    std::cout << "MODOS DE USO:\n";
    std::cout << "------------------------------------------------------------------------------------------\n";
    std::cout << "\nnetcp -h | --help                                                   [MODO AYUDA DEL PROGRAMA]\n";
    std::cout << "\nnetcp [-l | --listen] ARCHIVO                                       [MODO ESCUCHA]\n";
    std::cout << "\nnetcp [-1] [-2] -c | --command COMANDO [ARGS...]                    [MODO ENVÍO DE COMANDO]\n";
    std::cout << "\nnetcp [-1] [-2] ARCHIVO                                             [MODO ENVÍO DE ARCHIVO]\n";
    std::cout << "\nnetcp -l -c | --command COMANDO [ARGS...]                           [MODO ESCUCHA DE COMANDO]\n\n";
    std::cout << "▐▓█▀▀▀▀▀▀▀▀▀█▓▌░▄▄▄▄▄░\n▐▓█░░▀░░▀▄░░█▓▌░█▄▄▄█░\n▐▓█░░▄░░▄▀░░█▓▌░█▄▄▄█░\n▐▓█▄▄▄▄▄▄▄▄▄█▓▌░█████░\n░░░░▄▄███▄▄░░░░░█████░\n\n";
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