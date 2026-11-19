#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

const int N = 5;
const int VECES_COMER = 3;

pthread_mutex_t tenedores[N];
pthread_mutex_t mutexSalida;

void imprimir(const string& mensaje) {
    pthread_mutex_lock(&mutexSalida);
    cout << mensaje << endl;
    pthread_mutex_unlock(&mutexSalida);
}

void* filosofo(void* argumento) {
    int id = *static_cast<int*>(argumento);

    // Cada filosofo necesita los dos tenedores que tiene a sus lados.
    int izquierdo = id;
    int derecho = (id + 1) % N;

    // Tomar primero el tenedor con el indice menor evita el interbloqueo
    // por espera circular.
    int primero = (izquierdo < derecho) ? izquierdo : derecho;
    int segundo = (izquierdo < derecho) ? derecho : izquierdo;

    for (int i = 0; i < VECES_COMER; ++i) {
        imprimir("Filosofo " + to_string(id + 1) + " esta pensando.");
        sleep(1);

        pthread_mutex_lock(&tenedores[primero]);
        pthread_mutex_lock(&tenedores[segundo]);

        imprimir("Filosofo " + to_string(id + 1) + " esta comiendo.");
        sleep(1);

        pthread_mutex_unlock(&tenedores[segundo]);
        pthread_mutex_unlock(&tenedores[primero]);

        imprimir("Filosofo " + to_string(id + 1) + " termino de comer.");
    }

    return nullptr;
}

int main() {
    pthread_t hilos[N];
    int ids[N];

    pthread_mutex_init(&mutexSalida, nullptr);

    for (int i = 0; i < N; ++i) {
        pthread_mutex_init(&tenedores[i], nullptr);
    }

    for (int i = 0; i < N; ++i) {
        ids[i] = i;
        pthread_create(&hilos[i], nullptr, filosofo, &ids[i]);
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(hilos[i], nullptr);
    }

    for (int i = 0; i < N; ++i) {
        pthread_mutex_destroy(&tenedores[i]);
    }

    pthread_mutex_destroy(&mutexSalida);

    cout << "Todos los filosofos han terminado." << endl;
    return 0;
}
