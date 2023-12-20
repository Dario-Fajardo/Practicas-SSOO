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
#include "../include/subprocess.h"

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
  std::string command{""};
  bool cerrout{false};
  bool cstdout{false};
  std::error_code error{Usage(argc, argv, usage_mode, command, cerrout, cstdout)};
  std::vector<std::string> args{};
  while (command.find(" ") != std::string::npos) {
    args.push_back(command.substr(0, command.find(" ")));
    command = command.substr(command.find(" ") + 1);
  }
  if (error.value() != 0) {
    return error.value();
  }
  if (!args.empty()) {  
    std::cout << "[NETCP]: COMANDO A EJECUTAR: " ;
    for (auto arg : args) {
      std::cout << arg << " ";
    }
    std::cout << "\n";
  }
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
    uint16_t port{8080};
    if (getenv("NETCP_PORT")) {
      port = static_cast<uint16_t>(std::stoi(getenv("NETCP_PORT")));
    }
    Socket socket(std::nullopt, port); // Crea el socket y lo bindea al puerto de escucha en todos sus terminales
    if (socket.GetIpAddress() == std::nullopt) {
      std::cerr << "[NETCP]: ERROR AL CREAR EL SOCKET PARA RECIBIR\n";
      return 1;
    }
    std::cout << "[NETCP]: ESCUCHANDO EN EL PUERTO " << port << std::endl;
    sockaddr_in remote_address{};
    std::string message{};
    std::error_code error{socket.Recieve(remote_address, message)}; // Recibe el mensaje y guarda dirección del emisor
    if (error.value() != 0) {
      return 1;
    }
    if (quit_app) {
      std::cout << "[NETCP]: SEÑAL DE TERMINACIÓN RECIBIDA, CERRANDO SOCKET...\n";
      close(socket.GetFileDescriptor());
      return 10;
    }
    if (args.empty()) {
      File file(file_name, 1); // 1 para indicar que se va a escribir
      if (file.GetFileDescriptor() == -1) {
        return errno;
      }
      if (file.WriteFile(message) < 0) { // Escribe el mensaje en el archivo
        return errno;
      }
    } else if (!args.empty()) {
      SubProcess sub_process(args, SubProcess::stdio::in);
      std::error_code error{sub_process.Exec(message)};
      if (error.value() != 0) {
        return error.value();
      }
    }
  } else {
    std::string ip_address{};
    if (getenv("NETCP_IP")) {
      ip_address = getenv("NETCP_IP");
    }
    uint16_t port{8080};
    if (getenv("NETCP_PORT")) {
      port = static_cast<uint16_t>(std::stoi(getenv("NETCP_PORT")));
    }
    if (ip_address.empty()) {
      ip_address = "127.0.0.1";
    }
    if (args.empty()) {
      File file(file_name, 0, ip_address, port); // 0 para indicar que se va a leer, el constructor lee el archivo y lo va enviando por paquetes
      if (file.GetFileDescriptor() == -1) {
        return errno;
      }
    } else {
      // Vemos las opciones -1 y -2
      SubProcess::stdio redirect_io{SubProcess::stdio::out};
      if (cerrout && cstdout) {
        redirect_io = SubProcess::stdio::outerr;
      } else if (cerrout) {
        redirect_io = SubProcess::stdio::err;
      } else if (cstdout) {
        redirect_io = SubProcess::stdio::out;
      } 
      SubProcess sub_process(args, redirect_io);
      std::error_code error{sub_process.Exec("")};
      if (error.value() != 0) {
        return error.value();
      }
      sub_process.Wait();
      // Leemos la salida del subproceso y la enviamos
      File sending{sub_process.GetStdoutFd(), ip_address, port};
    }
  }
  return 0;
}