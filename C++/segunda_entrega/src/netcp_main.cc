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
  int usage_mode{2};
  Usage(argc, argv, usage_mode);
  // Manejo de señales
  struct sigaction signal_handler;
  signal_handler.sa_handler = &SignalHandler;
  signal_handler.sa_flags = 0;
  sigaction(SIGINT, &signal_handler, nullptr);
  sigaction(SIGTERM, &signal_handler, nullptr);
  sigaction(SIGQUIT, &signal_handler, nullptr);
  sigaction(SIGHUP, &signal_handler, nullptr);
  // Modo de ejecución
  bool recieve_mode{false};
  std::string file_name;
  switch (usage_mode) {
    case 1:
      recieve_mode = true;
      file_name = argv[2];
      break;
    case 2:
      recieve_mode = false;
      file_name = argv[1];
      break;
    case 3:
      exit(0);
      break;
  }
  // Enviar / Recibir mensajes
  if (recieve_mode) {
    File file(file_name, 1); // 1 para indicar que se va a escribir
    uint16_t port{0};
    if (getenv("NETCP_PORT")) {
      port = static_cast<uint16_t>(std::stoi(getenv("NETCP_PORT")));
    }
    if (port == 0) {
      port = 8080;
    }
    Socket socket(std::nullopt, port); // Crea el socket y lo bindea al puerto de escucha
    std::cout << "[NETCP]: ESCUCHANDO EN EL PUERTO " << port << std::endl;
    sockaddr_in remote_address{};
    std::string message{socket.Recieve(remote_address)}; // Recibe el mensaje y guarda dirección del emisor
    if (quit_app) {
      std::cout << "[NETCP]: SEÑAL DE TERMINACIÓN RECIBIDA, CERRANDO SOCKET...\n";
      close(socket.GetFileDescriptor());
      exit(10);
    }
    std::cout << "[NETCP]: MENSAJE RECIBIDO DESDE: " << socket.ip_address_to_string(remote_address) << std::endl;
    file.WriteFile(message); // Escribe el mensaje en el archivo
  } else {
    std::string ip_address{};
    if (getenv("NETCP_IP")) {
      ip_address = getenv("NETCP_IP");
    }
    uint16_t port{0};
    if (getenv("NETCP_PORT")) {
      port = static_cast<uint16_t>(std::stoi(getenv("NETCP_PORT")));
    }
    if (ip_address.empty()) {
      ip_address = "127.0.0.1";
    }
    if (port == 0) {
      port = 8080;
    }
    File file(file_name, 0, ip_address, port); // 0 para indicar que se va a leer, el constructor lee el archivo y lo va enviando por paquetes
  }
  return 0;
}