#ifndef IO_H
#define IO_H 
#include "entity.hpp"
namespace io{
    void readChannel(Channel *channel, char const *file_path);
    void writeNets(Channel *channel, char const *file_path);
    void drawNets(char const *input_path, char const *result_path, char const *output_path);
}
#endif