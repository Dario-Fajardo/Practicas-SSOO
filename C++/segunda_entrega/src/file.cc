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
File::File(const std::string& pathname, int mode) {
  if (mode == 0) {
    fd_ = open(pathname.c_str(), O_RDONLY); // Abrimos el archivo en modo lectura
  } else if (mode == 1) {
    fd_ = open(pathname.c_str(), O_WRONLY);
  }
  if (fd_ < 0) { // Si hay error, lo imprimimos por pantalla y salimos del programa
    std::cerr << "Error opening file:" << std::error_code(errno, std::system_category()).message() << std::endl;
    exit(1);
  }
  stat(pathname.c_str(), &file_info_); // Obtenemos información del archivo
  std::vector<uint8_t> buffer; // Creamos un buffer
  buffer.resize(1024); // Le damos un tamaño inicial de 1024 bytes (1KB)
  if (mode == 0) {
    Socket socket{"10.0.2.15", 8080};
    std::error_code error = read_file(fd_, buffer); // Leemos el archivo y lo guardamos en el buffer
    if (error) { // Si hay error, lo imprimimos por pantalla y salimos del programa
      std::cerr << "Error reading file: " << error.message() << std::endl;
      exit(1);
    }
    int total_packets = (file_info_.st_size / 1024);
    if (file_info_.st_size % 1024 != 0) {
      total_packets++;
    }
    int packet_number{1};
    int sended_bytes{0};
    while (!buffer.empty()) { // Mientras haya datos que leer
      socket.send_to(socket.GetFileDescriptor(), buffer, socket.make_ip_address("10.0.2.15", 8080).value());
      sended_bytes += buffer.size();
      std::cout << "[NETCP]: PAQUETE " << packet_number << " DE " << total_packets << " ENVIADO (";
      std::cout << sended_bytes << " B ---> " << file_info_.st_size << " B)" << std::endl;
      packet_number++;
      std::error_code error = read_file(fd_, buffer); // Leemos el archivo y lo guardamos en el buffer
      if (error) { // Si hay error, lo imprimimos por pantalla y salimos del programa
      std::cerr << "Error reading file: " << error.message() << std::endl;
      exit(1);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "[NETCP]: ARCHIVO ENVIADO CON ÉXITO" << std::endl;
    buffer.clear();
    socket.send_to(socket.GetFileDescriptor(), buffer, socket.make_ip_address("10.0.2.15", 8080).value());
  }
}

/**
 * Destructor de la clase File, cierra el descriptor de archivo
*/
File::~File() {
  close(fd_);
}

/**
 * Método para imprimir por pantalla el buffer con los bytes de un archivo de la clase
 * File
 */
void File::PrintFile() const {
  for (const auto& byte : buffer_) {
    std::cout << byte;
  }
  std::cout << std::endl;
}

int File::WriteFile(const std::vector<uint8_t>& data) const {
  return write(fd_, data.data(), file_info_.st_size);
}

int File::WriteFile(const std::string& data) const {
  return write(fd_, data.c_str(), file_info_.st_size);
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