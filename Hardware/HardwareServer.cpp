#include <sstream>
#include <thread>
#include <vector>
#include "HardwareServer.h"
#include "zhelpers.h"

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> parts;

    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        parts.push_back(item);
    }
    unsigned int c;
    std::string end = "";
    while ((c = ss.get()) <= 256) {
        end += (char)c;
    }
    if (end != "") {
        parts.push_back(end);
    }
    return parts;
}
        
HardwareServer::HardwareServer()
{
}

float HardwareServer::getAngle(int motor)
{
    if (motors.count(motor)) {
        return motors[motor];
    } else {
        return 0;
    }
}

void HardwareServer::sendImage(HardwareImage &image)
{
    std::string data = image.encode();
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, data.size());
    memcpy(zmq_msg_data(&msg), data.c_str(), image.data.size());
    zmq_msg_send(&msg, imagesServer, 0);
    zmq_msg_close(&msg);
}

void HardwareServer::runCommand()
{
    while (true) {
        char *request = s_recv(commandServer);
        auto str = std::string(request, strlen(request));
        auto parts = split(str, ' ');

        for (unsigned int k=0; k<parts.size(); k++) {
            motors[k] = atof(parts[k].c_str());
        }

        s_send(commandServer, "ok");
        free(request);
    }
}

void HardwareServer::run()
{
    ctx = zmq_ctx_new();

    // Creating command server
    commandServer = zmq_socket(ctx, ZMQ_REP);
    zmq_bind(commandServer, "tcp://*:7272");

    // Creating images server
    imagesServer = zmq_socket(ctx, ZMQ_PUB);
    zmq_bind(imagesServer, "tcp://*:7273");

    serverThread = std::thread(&HardwareServer::runCommand, this);
}
