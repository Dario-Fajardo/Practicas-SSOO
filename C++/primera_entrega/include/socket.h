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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <optional>
#include <string>
#include <arpa/inet.h>

using make_socket_result = std::expected<int, std::error_code>

class Socket {
 public:
  // Constructores
  Socket();
  Socket(int domain, int type, int protocol = 0, std::optional<std::string> ip_address = std::nullopt, uint16_t port = 0);
  // Getters
  int GetFileDescriptor() const { return fd_; }
  std::optional<sockaddr_in> GetIpAddress() const { return ip_address_; }
  // Otros métodos
  std::optional<sockaddr_in> make_ip_address(const std::optional<std::string>& ip_address, uint16_t port = 0) const;
  std::string ip_address_to_string(const sockaddr_in& address) const;
 private:
  int fd_;
  std::optional<sockaddr_in> ip_address_;
};