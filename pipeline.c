#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define capacidade 5
#define total_itens 10

typedef struct {
    int dados[capacidade];
    int inicio;
    int fim;
    int quantidade;
    int finalizado; // indica que não haverá mais produção

    pthread_mutex_t mutex;
    pthread_cond_t nao_vazio;
    pthread_cond_t nao_cheio;
} buffer;

// buffers globais
buffer b1;
buffer b2;

// inicializa o buffer
void inicializar_buffer(buffer *b) {
    b->inicio = 0;
    b->fim = 0;
    b->quantidade = 0;
    b->finalizado = 0;

    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->nao_vazio, NULL);
    pthread_cond_init(&b->nao_cheio, NULL);
}

// insere um valor no buffer (produtor)
void inserir(buffer *b, int valor) {
    pthread_mutex_lock(&b->mutex);

    while (b->quantidade == capacidade) {
        pthread_cond_wait(&b->nao_cheio, &b->mutex);
    }

    b->dados[b->fim] = valor;
    b->fim = (b->fim + 1) % capacidade;
    b->quantidade++;

    pthread_cond_signal(&b->nao_vazio);
    pthread_mutex_unlock(&b->mutex);
}

// remove um valor do buffer (consumidor)
// retorna 0 se não houver mais dados e o buffer estiver finalizado
int remover(buffer *b, int *valor) {
    pthread_mutex_lock(&b->mutex);

    while (b->quantidade == 0 && !b->finalizado) {
        pthread_cond_wait(&b->nao_vazio, &b->mutex);
    }

    // se não há dados e já terminou produção, encerra
    if (b->quantidade == 0 && b->finalizado) {
        pthread_mutex_unlock(&b->mutex);
        return 0;
    }

    *valor = b->dados[b->inicio];
    b->inicio = (b->inicio + 1) % capacidade;
    b->quantidade--;

    pthread_cond_signal(&b->nao_cheio);
    pthread_mutex_unlock(&b->mutex);

    return 1;
}

// thread de leitura (produtora)
void* thread_leitura(void *arg) {
    for (int i = 1; i <= total_itens; i++) {
        printf("leitura: %d\n", i);
        inserir(&b1, i);
    }

    // sinaliza que não haverá mais produção no buffer 1
    pthread_mutex_lock(&b1.mutex);
    b1.finalizado = 1;
    pthread_cond_broadcast(&b1.nao_vazio);
    pthread_mutex_unlock(&b1.mutex);

    return NULL;
}

// thread de processamento (consome b1 e produz b2)
void* thread_processamento(void *arg) {
    int valor;

    while (remover(&b1, &valor)) {
        valor = valor * 2;
        printf("processamento: %d\n", valor);
        inserir(&b2, valor);
    }

    // sinaliza fim do buffer 2
    pthread_mutex_lock(&b2.mutex);
    b2.finalizado = 1;
    pthread_cond_broadcast(&b2.nao_vazio);
    pthread_mutex_unlock(&b2.mutex);

    return NULL;
}

// thread de escrita (consumidor final)
void* thread_escrita(void *arg) {
    int valor;

    while (remover(&b2, &valor)) {
        printf("escrita: %d\n", valor);
    }

    return NULL;
}

// função principal
int main() {
    pthread_t t1, t2, t3;

    inicializar_buffer(&b1);
    inicializar_buffer(&b2);

    pthread_create(&t1, NULL, thread_leitura, NULL);
    pthread_create(&t2, NULL, thread_processamento, NULL);
    pthread_create(&t3, NULL, thread_escrita, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}