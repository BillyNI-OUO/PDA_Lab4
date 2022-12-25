#include "src/io.cpp"
#include "src/entity.cpp"
#include "src/algorihm.cpp"
using namespace std;
Channel *channel = new(Channel);
int main(int argc, char** argv){
    io::readChannel(channel, argv[1]);
    channel->printContent();
    channel->init();
    algor::greedyRouting(channel);
    io::writeNets(channel, argv[2]);
    io::drawNets(argv[1], argv[2], argv[3]);
}
