/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Este archivo contiene la definición de la clase socket, para enviar la información
 *        de los archivos instanciados usando la clase File en la dirección LOOPBACK del sistema
 */
#include "../include/socket.h"

/**
 * Constructor por defecto de la clase Socket por si se instancia sin parámetros
 */
Socket::Socket() {
  fd_ = -1;
  ip_address_ = std::nullopt;
}

/**
 * Constructor de la clase Socket
 * 
 * @param ip_address Dirección IP en formato std::string (opcional)
 * @param port Puerto
 */
Socket::Socket(std::optional<std::string> ip_address, const uint16_t port) {
  if (ip_address.has_value()) {
    ip_address_ = make_ip_address(ip_address, port);
  } else {
    ip_address_ = std::nullopt;
  }
  auto fd_or_error = make_socket(ip_address_).value();
  if (fd_or_error) {
    fd_ = fd_or_error;
  }
}

/**
 * Destructor de la clase Socket, cierra el socket para evitar problemas
 */
Socket::~Socket() {
  close(fd_);
}

/**
 * Método de la clase Socket que crea un socket y lo bindea a una dirección IP en caso de introducirla
 * 
 * @param address Dirección IP en formato sockaddr_in (opcional)
 * 
 * @return Estructura make_socket_result que contiene el descriptor de archivo o un código de error
 */
make_socket_result Socket::make_socket(const std::optional<sockaddr_in>& address) const {
  int fd{socket(AF_INET, SOCK_DGRAM, 0)};
  if (address.has_value()) {
    if (fd) {
      bind(fd, reinterpret_cast<const sockaddr*>(&address.value()), sizeof(sockaddr_in));
      return fd;
    } else {
      return std::make_error_code(std::errc(1)).value();
    }
  } else {
    return fd;
  }
}


/**
 * Método de la clase Socket que crea una dirección IP a partir de una dirección IP en formato std::string y un puerto
 * 
 * @param ip_address_string Dirección IP en formato std::string
 * @param port Puerto
 * 
 * @return Dirección IP en formato sockaddr_in
 */
std::optional<sockaddr_in> Socket::make_ip_address(const std::optional<std::string>& ip_address_string, uint16_t port) const {
  sockaddr_in ip_address;
  ip_address.sin_family = AF_INET;
  ip_address.sin_port = htons(port);
  if (ip_address_string.has_value()) {
    inet_aton(ip_address_string.value().c_str(), &ip_address.sin_addr);
  } else {
    ip_address.sin_addr.s_addr = INADDR_ANY;
  }
  return ip_address;
}

/**
 * Método de la clase Socket que envía un mensaje a una dirección IP
 * 
 * @param fd Descriptor de archivo
 * @param message Mensaje a enviar
 * @param address Dirección IP a la que enviar el mensaje
 * 
 * @return Código de error
 */
std::error_code Socket::send_to(int fd, const std::vector<uint8_t>& message, const sockaddr_in& address) const {
  std::error_code error{std::make_error_code(std::errc(0))};
  int bytes_sent = sendto(fd, message.data(), message.size(), 0, reinterpret_cast<const sockaddr*>(&address), sizeof(sockaddr_in));
  if (bytes_sent < 0) { // Si hay error en el envío
    error = std::make_error_code(std::errc(2));
  }
  return error;
}

std::string Socket::Recieve(sockaddr_in& transmitter) const {
  std::string message_text{};
  message_text.resize(1024);
  std::string final_message;
  socklen_t addr_len{sizeof(transmitter)};
  int bytes_read = recvfrom(fd_, message_text.data(), message_text.size(), 0, reinterpret_cast<sockaddr*>(&transmitter), &addr_len);
  while (bytes_read != 0) {
    std::cout << "[NETCP]: PAQUETE RECIBIDO\n";
    bytes_read = recvfrom(fd_, message_text.data(), message_text.size(), 0, reinterpret_cast<sockaddr*>(&transmitter), &addr_len);
    if (bytes_read < 0) {
      std::cerr << "Error al recibir el mensaje\n";
      exit(1);
    }
    message_text.resize(bytes_read);
    final_message += message_text;
  }
  std::cout << "[NETCP]: MENSAJE RECIBIDO CON ÉXITO\n";
  std::cout << final_message << "\n";
  return final_message;
}

/**
 * Método de la clase Socket que convierte una dirección IP en formato sockaddr_in a std::string, útil para debuggear
 * 
 * @param address Dirección IP en formato sockaddr_in
 * 
 * @return Dirección IP en formato std::string
 */
std::string Socket::ip_address_to_string(const sockaddr_in& address) const {
  const char* string_address = inet_ntoa(address.sin_addr);
  return string_address;
}
