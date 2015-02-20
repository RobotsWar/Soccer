#include <iostream>
#include <sstream>
#include "HardwareClient.h"
#include "zhelpers.h"
        
HardwareClient::HardwareClient()
{
}

void HardwareClient::imageProcess()
{
    while (true) {
        zmq_msg_t message;
        zmq_msg_init (&message);
        int size = zmq_msg_recv(&message, imagesClient, 0);
        std::string data = std::string((char*)zmq_msg_data(&message), size);
        image = HardwareImage::decode(data);
        printf("Image: %d x %d\n", image.width, image.height);
    }
}

void HardwareClient::connect()
{
    ctx = zmq_ctx_new();

    // Creating command server
    commandClient = zmq_socket(ctx, ZMQ_REQ);
    zmq_connect(commandClient, "tcp://localhost:7272");

    // Creating images server
    imagesClient = zmq_socket(ctx, ZMQ_SUB);
    zmq_connect(imagesClient, "tcp://localhost:7273");
    zmq_setsockopt(imagesClient, ZMQ_SUBSCRIBE, NULL, 0);

    clientThread = std::thread(&HardwareClient::imageProcess, this);
}

void HardwareClient::setAngle(int motor, float value)
{
    motors[motor] = value;
}

void HardwareClient::flush()
{
    std::stringstream ss;

    for (unsigned int k=0; k<motors.size(); k++) {
        ss << motors[k] << " ";
    }

    // std::cout << "Sending " << ss.str() << std::endl;
    s_send(commandClient, ss.str().c_str());
    char *r = s_recv(commandClient);
    free(r);
}

HardwareImage HardwareClient::getImage()
{
    return image;
}
