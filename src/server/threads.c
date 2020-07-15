#include "server.h"

static t_comm *init_data(int connection_fd, char *status, t_meta **metadata) {
    t_comm *data = malloc(sizeof(t_comm));

    data->fd = connection_fd;
    data->status = status;
    data->db = (*metadata)->db;
    data->ssl = (*metadata)->ssl;
    data->ctx = (*metadata)->ctx;
    data->ft_status = (*metadata)->ft_status;

    return data;
}

int mx_get_free_thread(char *status, int *counter) {
    for (int j = 0; j < MX_MAX_THREADS; ++j)
        if (status[j] == 0) {
            *counter = j;
            status[j] = 1;
            return 0;
        }

    return 1;
}

t_meta *mx_init_threads(sqlite3 *db, SSL_CTX *ctx) {
    pthread_t *threads = malloc(sizeof(pthread_t) * MX_MAX_THREADS);
    char *status = malloc(sizeof(char) * MX_MAX_THREADS);
    char *ft_status = malloc(sizeof(char) * MX_MAX_THREADS);
    t_meta *data = malloc(sizeof(t_meta));

    mx_memset(status, 0, MX_MAX_THREADS);
    mx_memset(ft_status, 0, MX_MAX_THREADS);
    data->threads = threads;
    data->status = status;
    data->db = db;
    data->ssl = NULL;
    data->ctx = ctx;
    data->ft_status = ft_status;

    return data;
}

void mx_thread_manager(int connection_fd, t_meta **metadata) {
    t_comm *data = NULL;
    int counter = 0;
    int free_thr = -1;
    char *status = (*metadata)->status;
    pthread_t *thr = (*metadata)->threads;

    while ((free_thr = mx_get_free_thread((*metadata)->status, &counter)) != 0)
        printf("%d\n", free_thr);

    data = init_data(connection_fd, &status[counter], metadata);

    if (pthread_create(&thr[counter], NULL, mx_communicate, (void *)data) == 0)
        mx_printstr_endl("Connected to client!");
    else
        mx_printstr_endl("Connection failed :(");
}
