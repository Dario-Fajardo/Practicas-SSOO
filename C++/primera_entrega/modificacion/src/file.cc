/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Este archivo contiene la definición de la clase File, para implementar la lectura
 *        de archivos usando la llamada al sistema read().
 */
#include "../include/file.h"

/**
 * Constructor por defecto de la clase File, por si se instancia sin parámetros
 */
File::File() {
  fd_ = -1;
}

/**
 * Constructor parametrizado de la clase File a partir de un std::string con el nombre
 * del archivo a instanciar
 * 
 * @param pathname: la ruta al archivo del que se sacará la información
 */
File::File(const std::string& pathname) {
  fd_ = open(pathname.c_str(), O_RDONLY); // Abrimos el archivo en modo lectura
  if (fd_ < 0) { // Si hay error, lo imprimimos por pantalla y salimos del programa
    std::cerr << "Error opening file:" << std::error_code(errno, std::system_category()).message() << std::endl;
    exit(1);
  }
  std::vector<uint8_t> buffer(16ul * 1024 * 1024); // Creamos un buffer
  std::error_code error = read_file(fd_, buffer); // Leemos el archivo y lo guardamos en el buffer
  if (error) { // Si hay error, lo imprimimos por pantalla y salimos del programa
    std::cerr << "Error reading file: " << error.message() << std::endl;
    exit(1);
  }
  stat(pathname.c_str(), &file_info_); // Obtenemos información del archivo
  if (file_info_.st_size > 1024) { // Si el archivo es mayor de 1KB, salimos del programa
    std::cerr << "File size is bigger than 1KB" << std::endl;
    exit(1);
  }
}

/**
 * Método para imprimir por pantalla el buffer con los bytes de un archivo de la clase
 * File
 */
void File::PrintFile() {
  for (const auto& byte : buffer_) {
    std::cout << byte;
  }
  std::cout << std::endl;
}

/**
 * Método para leer la información de un archivo e incluirlo en el atributo
 * buffer de la clase File
 * 
 * @param fd: descriptor de archivo a acceder para obtener la información
 * @param buffer: vector donde se guardará la información del archivo con el
 *                objetivo de poder mandarla por red
 * 
 * @return el código de error (si no hay, 0) por si falla la lectura
 */
std::error_code File::read_file(int fd, std::vector<uint8_t>& buffer) {
  ssize_t bytes_read = read(fd, buffer.data(), buffer.size());
  if (bytes_read < 0) {
    return std::error_code(errno, std::system_category());
  }
  buffer.resize(bytes_read);
  buffer_.resize(bytes_read);
  buffer_ = buffer;
  return std::error_code{0, std::system_category()};
}