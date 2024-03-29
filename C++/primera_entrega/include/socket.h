/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Este archivo contiene la declaración de la clase socket, para enviar la información
 *        de los archivos instanciados usando la clase File en la dirección LOOPBACK del sistema
 */
#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <optional>
#include <string>
#include <arpa/inet.h>
#include <expected>
#include <system_error>
#include "file.h"

using make_socket_result = std::expected<int, std::error_code>; // Tipo de dato para el resultado de la función make_socket

/**
 * Clase Socket para enviar la información de los archivos instanciados usando la clase File
 */
class Socket {
 public:
  // Constructores
  Socket();
  Socket(std::optional<std::string> ip_address = std::nullopt, uint16_t port = 0);
  // Destructor
  ~Socket();
  // Getters
  int GetFileDescriptor() const { return fd_; }
  std::optional<sockaddr_in> GetIpAddress() const { return ip_address_; }
  // Otros métodos
  std::optional<sockaddr_in> make_ip_address(const std::optional<std::string>& ip_address, uint16_t port = 0) const;
  std::string ip_address_to_string(const sockaddr_in& address) const;
  make_socket_result make_socket(const std::optional<sockaddr_in>& address = std::nullopt) const;
  std::error_code send_to(int fd, const std::vector<uint8_t>& message, const sockaddr_in& address);
 private:
  int fd_;
  std::optional<sockaddr_in> ip_address_;
};

#endif