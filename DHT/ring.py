import node
import sys
import threading
import zmq

def server():
    pass

if __name__ == "__main__":

    if(len(sys.argv) != 3 and len(sys.argv) != 5):
        print "Missing arguments"

    if(len(sys.argv) == 3):
        nodo = node.Node(sys.argv[1],sys.argv[2])

    if(len(sys.argv) == 5):
        nodo = node.Node(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    
    context = zmq.Context()
    work_message = ""
    zmq_socket.send_json(work_message)
    socket_client = context.socket(zmq.REQ) #Cliente
    socket_server = context.socket(zmq.REP) #Server