// Programación de una solución paralela.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <omp.h>

#define MOSTRAR 10

int main() {
    int N;
    std::cout << "Ingrese la dimension de los arreglos: ";
    std::cin >> N;

    if (N <= 0) {
        std::cout << "N debe ser > 0\n";
        return 1;
    }

    std::vector<float> a(N), b(N), c(N);
    std::vector<int> hilo_usado(N, -1);

	// Configuración de la función para la generación de números aleatorios con el metodo Mersenne Twister
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist_val(0.0f, 100.0f);

    for (int i = 0; i < N; i++) {
        a[i] = dist_val(gen);
        b[i] = dist_val(gen);
    }
//Comprobacion del funcionamiento de OpenMP
#ifdef _OPENMP
    std::cout << "OpenMP habilitado. _OPENMP=" << _OPENMP << "\n";
#else
    std::cout << "OpenMP no habilitado al momento de compilar\n";
#endif

//Prueba del numero maximo de threads
#pragma omp parallel
    {
#pragma omp single
        {
            std::cout << "Numero de hilos en ejecucion=" << omp_get_num_threads()
                << ", Numero maximo de hilos=" << omp_get_max_threads() << "\n";
        }
    }

	// Preparacion de los pedazos para la distribución estática
    int T = omp_get_max_threads();
    int pedazos = std::max(1, N / (T * 4));   
    std::cout << "Funcion schedule(static," << pedazos << ")\n";

#pragma omp parallel for schedule(static, pedazos)
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
        hilo_usado[i] = omp_get_thread_num();
    }

    // Imprimir 10 muestras aleatorias
    int muestras = std::min(N, MOSTRAR);
    std::vector<int> indices(N);
    for (int i = 0; i < N; i++) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), gen);

    std::cout << "\n===== Muestras aleatorias =====\n";
    for (int k = 0; k < muestras; k++) {
        int i = indices[k];
        std::cout << "i=" << i
            << "  a=" << a[i]
            << "  b=" << b[i]
            << "  c=" << c[i]
            << "  -> hilo " << hilo_usado[i]
            << "\n";
    }

    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
