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

Socket::Socket() {
  fd_ = -1;
  ip_address_ = std::nullopt;
}

Socket::Socket(int domain, int type, int protocol, std::optional<std::string> ip_address, uint16_t port) {
  fd_ = make_socket();
  ip_address_ = make_ip_address(ip_address, port);
  bind(fd_, reinterpret_cast<sockaddr*>(&ip_address_.value()), sizeof(ip_address_.value()));
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
  ip_address.sin_port = port;
  if (ip_address_string.has_value()) {
    ip_address.sin_addr.s_addr = inet_aton(ip_address_string.value().c_str(), &ip_address.sin_addr);
  } else {
    return std::nullopt;
  }
  return ip_address;
}

std::string Socket::ip_address_to_string(const sockaddr_in& address) const {
  const char* string_address = inet_ntoa(address.sin_addr);
  return string_address;
}
