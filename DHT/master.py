import sys
import zmq


if(len(sys.argv) == 3):
    my_ip = sys.argv[1];
    my_port = sys.argv[2]
    
    context = zmq.Context()
    socket = context.socket(zmq.REP)

     socket.bind('tcp://*:' + my_port)

    notify = consumer_receiver.recv_json()
    notify



