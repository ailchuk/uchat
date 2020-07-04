#include "client.h"

char *mx_read_server_response(t_glade *g) {
    int bytes_read = 0;
    char buff[MX_MAX];

    bzero(buff, sizeof(buff));

    bytes_read = SSL_read(g->ssl, buff, sizeof(buff));
    buff[bytes_read] = '\0';

    if (bytes_read <= 0) {
        close(SSL_get_fd(g->ssl));
        mx_printstr_endl("Server not responding...");
    }

    return mx_strdup(buff);
}