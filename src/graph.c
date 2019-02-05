#include "graph.h"

node_t create_node(void *data, int data_len)
{
    node_t n;

    n.data = data;
    n.data_len = data_len;
    n.links = 0;
    n.link_count = 0;

    return n;
}