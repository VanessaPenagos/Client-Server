import hashlib
import sys
import zmq


class Node:
    def __init__(self, my_ip, my_port, succesor_ip=None, succesor_port=None):
        ''' Crea el nodo'''
        my_address = str(my_ip + my_port)
        my_address = my_address.encode('utf_8')
        self.my_id = hashlib.sha256(my_address).hexdigest()
        self.my_ip = my_ip
        self.my_port = my_port
        self.predeccesor_ip = None
        self.predeccesor_port = None
        self.succesor_ip = succesor_ip
        self.succesor_port = succesor_port
        self.finger_table = {}

    def get_predeccesor(self):
        ''' Muestra el predecesor del nodo'''
        return self.predeccesor

    def get_succesor(self):
        ''' Muestra los sucesores del nodo'''
        return self.finger_table

    def set_predeccesor(self, predeccesor):
        ''' Cambia el predecesor'''
        self.predeccesor = predeccesor

    def set_succesor(self, succesor):
        ''' Entrega el sucesor'''
        self.succesor = succesor


def main():

    context = zmq.Context()
    socket_client = context.socket(zmq.REQ)
    socket_server = context.socket(zmq.REP)
    bool flag = False

    if len(sys.argv) == 3:
        node = Node(sys.argv[1], sys.argv[2])

    if len(sys.argv) == 5:
        node = Node(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])

    while not flag:
        socket_client.connect("tcp://" + node.succesor_ip + ":" + node.succesor_port)
        message = {'message': "Puedo ser tu sucesor?", 'id': node.my_id}
        socket_client.send_json(message)
        answer = socket_client.recv()

        if answer == 'Si':
            message = {'id': node.my_id, 'ip': node.my_ip, 'port': node.my_port}
            socket_client.send_json(message)
            
