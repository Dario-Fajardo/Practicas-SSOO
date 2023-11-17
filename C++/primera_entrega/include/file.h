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
#include <iostream>
#include <string>
#include <system_error>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>

class File {
 public:
  // Constructores
  File();
  File(const std::string& pathname);
  // Getters
  int GetFileDescriptor() const { return fd_; }
  std::vector<uint8_t> GetBuffer() const { return buffer_; }
  // Otros métodos
  void PrintFile();
  std::error_code read_file(int fd, std::vector<uint8_t>& buffer); 
 private:
  int fd_;
  std::vector<uint8_t> buffer_;
};