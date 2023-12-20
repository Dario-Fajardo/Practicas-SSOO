/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 17 Nov 2023
 * @brief Este archivo contiene la declaración de la clase subprocess, para poder crear subprocesos
 *        que ejecuten comandos del sistema
 */
#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <system_error>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <experimental/scope>
#include "file.h"

class SubProcess {
 public:
  enum class stdio {in, out, err, outerr}; // Enumeración para redirigir la entrada/salida de un subproceso
  SubProcess(const std::vector<std::string>& args, SubProcess::stdio redirect_io); // Constructor
  // Getters
  pid_t GetPid() const { return pid_;}
  std::vector<std::string> GetArgs() const { return args_;}
  int GetStdinFd() const { return stdin_fd_;}
  int GetStdoutFd() const { return stdout_fd_;}
  // Otras funciones
  bool IsAlive() const; // Comprueba si el subproceso está vivo
  std::error_code Exec(const std::string& message); 
  std::error_code Wait();
  std::error_code Kill();
 private:
  stdio redirect_io_;
  std::vector<std::string> args_;
  int stdin_fd_;
  int stdout_fd_;
  pid_t pid_;
};

#endif