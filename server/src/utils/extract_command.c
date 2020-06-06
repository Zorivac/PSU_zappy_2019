/*
** EPITECH PROJECT, 2020
** zappy_server
** File description:
** Created by emilien
*/

#include "server.h"
#include "sockets/select.h"
#include <stdlib.h>
#include <string.h>

static int len_to_n(char const *buf)
{
    int output = 0;

    if (!buf)
        return (-1);
    while (buf[output]) {
        if (buf[output] == '\n')
            return (output);
        ++output;
    }
    return (-1);
}

static void set_new_buffer(client_t *cli, char *tmp_buffer)
{
    size_t len = strlen(tmp_buffer);
    char *new;

    if (tmp_buffer[0] == 0) {
        free(cli->buffer);
        cli->buffer = NULL;
        return;
    }
    new = malloc(len + 1);
    for (int a = 0; tmp_buffer[a]; ++a) {
        new[a] = tmp_buffer[a];
        new[a + 1] = 0;
    }
    free(cli->buffer);
    cli->buffer = new;
}

static bool extract_data(client_t *cli, char *to_add, char **tmp_buffer,
    int *len_to_back_n)
{
    for (int a = 0; (*tmp_buffer)[a] != '\n'; ++a) {
        to_add[a] = (*tmp_buffer)[a];
        to_add[a + 1] = 0;
    }
    (*tmp_buffer) = (*tmp_buffer) + (*len_to_back_n) + 1;
    (*len_to_back_n) = len_to_n((*tmp_buffer));
    if (add_to_client(cli, to_add) == false)
        return (false);
    free(to_add);
    return (true);
}

static bool cli_too_many_cmd(client_t *cli)
{
    command_queue_t *cur = cli->cmd_queue;
    char counter = 0;

    if (!cur)
        return (true);
    while (cur) {
        cur = cur->next;
        if (counter > 10) {
            free(cli->buffer);
            cli->buffer = NULL;
            return (false);
        }
    }
    return (true);
}

void extract_command(client_t *cli)
{
    char *tmp_buffer = cli->buffer;
    char *to_add;
    int len_to_back_n = len_to_n(cli->buffer);

    if (!cli->buffer || cli_too_many_cmd(cli))
        return;
    while (len_to_back_n > 0) {
        to_add = malloc(len_to_back_n + 1);
        if (!to_add)
            continue;
        if (extract_data(cli, to_add, &tmp_buffer, &len_to_back_n) == false) {
            free(cli->buffer);
            cli->buffer = NULL;
            return;
        }
    }
    set_new_buffer(cli, tmp_buffer);
}