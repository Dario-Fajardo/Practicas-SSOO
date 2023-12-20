/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Este archivo contiene la definición de la clase subprocess, para poder crear subprocesos
 *        que ejecuten comandos del sistema
 */

#include "../include/subprocess.h"

/**
 * Constructor de la clase SubProcess
 * 
 * @param args Vector de argumentos para ejecutar el comando
 * @param redirect_io Enumeración para redirigir la entrada/salida de un subproceso
 */
SubProcess::SubProcess(const std::vector<std::string>& args, SubProcess::stdio redirect_io) {
  args_ = args;
  redirect_io_ = redirect_io;
}

/**
 * Comprueba si el subproceso está vivo
 * 
 * @return true si el subproceso está vivo, false en caso contrario
 */
bool SubProcess::IsAlive() const {
  int status;
  pid_t result = waitpid(GetPid(), &status, WNOHANG);
  if (result == -1) {
    return false;
  }
  if (result == 0) {
    return true;
  }
  return false;
}

/**
 * Ejecuta el comando del subproceso
 * 
 * @return 0 si se ejecuta correctamente, cualquier otro valor en caso contrario
 */
std::error_code SubProcess::Exec(const std::string& message) {
  // Procesamos argumentos para pasarlos a execvp
  std::vector<char*> argv;
  for (auto& arg : args_) {
    argv.push_back(const_cast<char*>(arg.c_str()));
  }
  argv.emplace_back(nullptr);
  // Creamos la tubería
  int fds[2];
  int return_value = pipe(fds);
  if (return_value < 0) {
    std::cerr << "[NETCP]: ERROR AL CREAR LA TUBERÍA" << std::endl;
    return std::error_code(errno, std::system_category());
  }
  // Creamos el subproceso
  pid_t child = fork();
  if (child == 0) { // Estamos en el hijo
    pid_ = getpid();
    switch (redirect_io_) {
      case stdio::in:
        dup2(fds[0], STDIN_FILENO);
        break;
      case stdio::out:
        dup2(fds[1], STDOUT_FILENO);
        break;
      case stdio::err:
        dup2(fds[1], STDERR_FILENO);
        break;
      case stdio::outerr:
        dup2(fds[1], STDOUT_FILENO);
        dup2(fds[1], STDERR_FILENO);
        break;
    }
    close(fds[0]);
    close(fds[1]);
    execvp(argv[0], argv.data()); // Ejecutamos el comando
    // Si llegamos aquí es que ha habido un error
    std::cerr << "[NETCP]: ERROR AL EJECUTAR EL COMANDO QUIZÁ NO EXISTE" << std::endl;
    return std::error_code(errno, std::system_category());
  } else if (child > 0) { // Estamos en el padre
    if (redirect_io_ == stdio::in) { // Redirigimos la entrada
      close(fds[0]);
      stdin_fd_ = fds[1];
      stdout_fd_ = -1;
      File file(GetStdinFd(), std::optional<std::string>(message));
      file.WriteFile(message);
    } else if (redirect_io_ == stdio::out) { // Redirigimos la salida
      close(fds[1]);
      stdout_fd_ = fds[0];
      stdin_fd_ = -1;
    } else if (redirect_io_ == stdio::err) { // Redirigimos la salida de error
      close(fds[1]);
      stdout_fd_ = fds[0];
      stdin_fd_ = -1;
    } else if (redirect_io_ == stdio::outerr) { // Redirigimos la salida y la salida de error
      close(fds[1]);
      stdout_fd_ = fds[0];
      stdin_fd_ = -1;
    }
  } else {
    std::cerr << "[NETCP]: ERROR AL CREAR EL SUBPROCESO" << std::endl;
    return std::error_code(errno, std::system_category());
  }
  return std::error_code(0, std::system_category());
}

/**
 * Espera a que el subproceso termine
 * 
 * @return 0 si se ejecuta correctamente, cualquier otro valor en caso contrario
 */
std::error_code SubProcess::Wait() {
  int status;
  while (IsAlive()) {
    if (quit_app) {
      Kill();
    }
    waitpid(GetPid(), &status, 0);
  }
  return std::error_code(0, std::system_category());
}

/**
 * Mata al subproceso
 * 
 * @return 0 si se ejecuta correctamente, cualquier otro valor en caso contrario
 */
std::error_code SubProcess::Kill() {
  int return_value = kill(GetPid(), SIGTERM);
  if (return_value < 0) {
    std::cerr << "[NETCP]: ERROR AL MATAR AL SUBPROCESO" << strerror(errno) << std::endl;
    return std::error_code(errno, std::system_category());
  }
  return std::error_code(0, std::system_category());
}