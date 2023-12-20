/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Este archivo contiene la declaración de la clase File, para implementar la lectura
 *        de archivos usando la llamada al sistema read().
 */
#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string>
#include <system_error>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "socket.h"
#include "tools.h"

/**
 * Clase File para enviar la información de los archivos instanciados usando la clase Socket
 */
class File {
 public:
  // Constructores
  File();
  File(const std::string& pathname = "", const int mode = 0, const std::string& ip_address = "127.0.0.1", const uint16_t port = 8080);
  File(const int fd, const std::string& ip_address = "127.0.0.1", const uint16_t port = 8080);
  File(const int fd, const std::optional<std::string>& message);
  // Destructor
  ~File();
  // Getters
  int GetFileDescriptor() const { return fd_; }
  std::vector<uint8_t> GetBuffer() const { return buffer_; }
  // Otros métodos
  void PrintFile() const;
  int WriteFile(const std::vector<uint8_t>& data) const;
  int WriteFile(const std::string& data) const;
  std::error_code read_file(int fd, std::vector<uint8_t>& buffer);
 private:
  int fd_;
  std::vector<uint8_t> buffer_;
  struct stat file_info_;
};

#endif