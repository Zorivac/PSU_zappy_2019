/*
** EPITECH PROJECT, 2020
** zappy
** File description:
** Created by emilien
*/

#include "server.h"
#include "sockets/select.h"
#include <stdlib.h>
#include <string.h>

static void add_to_valid_list(data_server_t *data, client_t *cli)
{
    if (data->l_connected.first == NULL && data->l_connected.last == NULL) {
        data->l_connected.first = cli;
        data->l_connected.last = cli;
        cli->next = NULL;
        cli->prev = NULL;
    } else if (data->l_connected.first && data->l_connected.last) {
        data->l_connected.last->next = cli;
        cli->prev = data->l_connected.last;
        cli->next = NULL;
        data->l_connected.last = cli;
    }
}

static void remove_from_first_list(data_server_t *data, client_t *cli)
{
    if (cli->next == NULL && cli->prev == NULL) {
        data->l_waiting.first = NULL;
        data->l_waiting.last = NULL;
    } else if (cli->prev) {
        data->l_waiting.last = cli->prev;
    } else {
        data->l_waiting.first = cli->next;
    }
}

static void client_validation(data_server_t *data, client_t *cli, int team_id)
{
    if (cli->next != NULL && cli->prev != NULL) {
        cli->next->prev = cli->prev;
        cli->prev->next = cli->next;
    } else {
        remove_from_first_list(data, cli);
    }
    cli->team_id = (unsigned short)team_id;
    add_to_valid_list(data, cli);
}

void valid_client(data_server_t *data, client_t *cli)
{
    char *cmd;
    command_queue_t *to_delete;

    if (!cli->cmd_queue)
        return;
    cmd = cli->cmd_queue->command;
    to_delete = cli->cmd_queue;
    if (!cmd) {
        cli->cmd_queue = cli->cmd_queue->next;
        free(to_delete);
    }
    for (int a = 0; data->params.team_names && data->params.team_names[a]; ++a)
        if (strcmp(data->params.team_names[a], cli->cmd_queue->command) == 0)
            client_validation(data, cli, a);
    remove_first_cmd_queue(cli);
}