/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 2º Curso en Grado en Ingeniería Informática
 * Sistemas Operativos
 * Programación de aplicaciones
 *
 * @author Dario Fajardo alu0101564630@ull.edu.es
 * @date 23 Nov 2023
 * @brief Archivo tools.h para implementar las funciones auxiliares del programa netcp.
 */
#ifndef TOOLS_H_
#define TOOLS_H_

#include <iostream>
#include <signal.h>
#include <stdatomic.h>
#include <vector>

extern std::atomic_bool quit_app;

void Usage(int argc, char* argv[], int& mode);
void SignalHandler(int signal);

#endif // TOOLS_H_