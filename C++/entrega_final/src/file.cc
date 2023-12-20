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
 * Constructor parametrizado de la clase File a partir de un descriptor de archivo
 * 
 * @param fd: descriptor de archivo a instanciar
 */
File::File(const int fd, const std::optional<std::string>& message) {
  fd_ = fd;
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  buffer_.resize(sizeof(message).value());
}

/**
 * Constructor parametrizado de la clase File a partir de un std::string con el nombre
 * del archivo a instanciar
 * 
 * @param pathname: la ruta al archivo del que se sacará la información
 */
File::File(const std::string& pathname, const int mode, const std::string& ip_address, const uint16_t port) {
  if (mode == 0) {
    fd_ = open(pathname.c_str(), O_RDONLY); // Abrimos el archivo en modo lectura
    if (fd_ < 0) { // Si hay error, lo imprimimos por pantalla y salimos del programa
      std::cerr << "[NETCP]: ERROR AL ABRIR EL ARCHIVO PARA ENVIAR\n" << std::error_code(errno, std::system_category()).message() << std::endl;
      fd_ = -1;
      return;
    }
  } else if (mode == 1) {
    fd_ = open(pathname.c_str(), O_WRONLY | O_CREAT, 0666); // Abrimos el archivo en modo escritura, si no existe lo creamos
    if (fd_ < 0) { // Si hay error, lo imprimimos por pantalla y salimos del programa
      std::cerr << "[NETCP]: ERROR AL ABRIR EL ARCHIVO PARA RECIBIR\n" << std::error_code(errno, std::system_category()).message() << std::endl;
      fd_ = -1;
      return;
    }
  }
  stat(pathname.c_str(), &file_info_); // Obtenemos información del archivo
  std::vector<uint8_t> buffer; // Creamos un buffer
  buffer.resize(1024); // Le damos un tamaño inicial de 1024 bytes (1KB)
  if (mode == 0) {
    Socket socket{ip_address, port};
    if (socket.GetIpAddress() == std::nullopt) {
      std::cerr << "[NETCP]: ERROR AL CREAR EL SOCKET PARA ENVIAR\n";
      fd_ = -1;
      return;
    }
    std::error_code error = read_file(fd_, buffer); // Leemos el archivo y lo guardamos en el buffer
    if (error.value() != 0) { // Si hay error, lo imprimimos por pantalla y salimos del programa
      std::cerr << "[NETCP]: ERROR LEYENDO EL ARCHIVO" << error.message() << std::endl;
      fd_ = -1;
      return;
    }
    if (quit_app) {
      std::cout << "[NETCP]: SEÑAL DE TERMINACIÓN RECIBIDA, CERRANDO SOCKET...\n";
      close(fd_);
      socket.~Socket();
      exit(10);
    }
    int total_packets = (file_info_.st_size / 1024);
    if (file_info_.st_size % 1024 != 0) {
      total_packets++;
    }
    int packet_number{1};
    int sended_bytes{0};
    while (!buffer.empty()) { // Mientras haya datos que leer
      std::error_code error = socket.send_to(socket.GetFileDescriptor(), buffer, socket.make_ip_address(ip_address, port).value()); // Enviamos el buffer por red
      if (error.value() != 0) { // Si hay error, lo imprimimos por pantalla y salimos del programa
        std::cerr << "[NETCP]: ERROR ENVIANDO EL ARCHIVO\n" << error.message() << std::endl;
        fd_ = -1;
        return;
      }
      sended_bytes += buffer.size();
      std::cout << "[NETCP]: PAQUETE " << packet_number << " DE " << total_packets << " ENVIADO (";
      std::cout << sended_bytes << " B ---> " << file_info_.st_size << " B)" << std::endl;
      packet_number++;
      if (quit_app) {
        std::cout << "[NETCP]: SEÑAL DE TERMINACIÓN RECIBIDA, CERRANDO SOCKET...\n";
        close(fd_);
        socket.~Socket();
        exit(10);
      }
      error = read_file(fd_, buffer); // Leemos el archivo y lo guardamos en el buffer
      if (error.value() != 0) { // Si hay error, lo imprimimos por pantalla y salimos del programa
        std::cerr << "[NETCP]: ERROR LEYENDO EL ARCHIVO A ENVIAR\n" << error.message() << std::endl;
        fd_ = -1;
        return;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Pequeña espera para evitar errores en el envío
    }
    std::cout << "[NETCP]: ARCHIVO ENVIADO CON ÉXITO" << std::endl;
    buffer.clear();
    socket.send_to(socket.GetFileDescriptor(), buffer, socket.make_ip_address(ip_address, port).value());
    if (quit_app) {
      std::cout << "[NETCP]: SEÑAL DE TERMINACIÓN RECIBIDA, CERRANDO SOCKET...\n";
      close(fd_);
      socket.~Socket();
      exit(10);
    }
  }
}

/**
 * Constructor parametrizado de la clase File a partir de un descriptor de archivo y un
 * vector de bytes
 * 
 * @param fd: descriptor de archivo a instanciar
 * @param buffer: vector de bytes a instanciar
 */
File::File(const int fd, const std::string& ip_address, const uint16_t port) {
  fd_ = fd;
  fcntl(fd_, F_SETFL, O_NONBLOCK);
  buffer_.resize(1024);
  Socket socket{ip_address, port};
  while (!buffer_.empty()) {
    std::error_code error = read_file(fd_, buffer_);
    if (error.value() != 0) {
      std::cerr << "[NETCP]: ERROR LEYENDO EL ARCHIVO" << error.message() << std::endl;
      fd_ = -1;
      return;
    }
    socket.send_to(socket.GetFileDescriptor(), buffer_, socket.make_ip_address(ip_address, port).value());
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  // buffer_.clear();
  // socket.send_to(socket.GetFileDescriptor(), buffer_, socket.make_ip_address(ip_address, port).value());
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

/**
 * Método de la clase File que permite escribir los datos recibidos por red en un archivo
 * 
 * @param data: std::vector con los datos recibidos por red
 * 
 * @return el número de bytes escritos en el archivo
 */
int File::WriteFile(const std::vector<uint8_t>& data) const {
  if (quit_app) {
    std::cout << "[NETCP]: SEÑAL DE TERMINACIÓN RECIBIDA, CERRANDO SOCKET...\n";
    close(fd_);
    exit(10);
  }
  int bytes_written = write(fd_, data.data(), data.size());
  if (bytes_written < 0) {
    std::cerr << "[NETCP]: ERROR ESCRIBIENDO EN EL ARCHIVO\n" << std::error_code(errno, std::system_category()).message() << std::endl;
    return -1;
  }
  return bytes_written;
}

/**
 * Método de la clase File que permite escribir los datos recibidos por red en un archivo
 * 
 * @param data: std::string con los datos recibidos por red
 * 
 * @return el número de bytes escritos en el archivo
 */
int File::WriteFile(const std::string& data) const {
  if (quit_app) {
    std::cout << "[NETCP]: SEÑAL DE TERMINACIÓN RECIBIDA, CERRANDO SOCKET...\n";
    close(fd_);
    exit(10);
  }
  int bytes_written = write(fd_, data.data(), data.size());
  if (bytes_written < 0) {
    std::cerr << "[NETCP]: ERROR ESCRIBIENDO EN EL ARCHIVO\n" << std::error_code(errno, std::system_category()).message() << std::endl;
    return -1;
  }
  return bytes_written;
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
    if (errno == EAGAIN) {
      buffer_.clear();
      return std::error_code{0, std::system_category()};
    }
    return std::error_code(errno, std::system_category());
  }
  buffer.resize(bytes_read);
  buffer_.resize(bytes_read);
  buffer_ = buffer;
  return std::error_code{0, std::system_category()};
}