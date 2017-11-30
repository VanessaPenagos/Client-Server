import hashlib
import sys
import zmq

def server():
    socket_server.bind('tcp://*:' + node_data['port'])
    print("server started")

    while True:
        request = socket_server.recv_json()

        if request['request'] == "joint the ring":
            if (request['id'] > node_data['id']) and (request['id'] <= succesor_data['id']):
                socket_server.send("yes")
                new_succesor = socket_server.recv_json()
                socket_server.send_json(succesor_data)

                succesor_data['ip'] = new_succesor['ip']
                succesor_data['port'] = new_succesor['port']
                succesor_data['id'] = new_succesor['id']
                print("Successor updated")
            elif:
                pass

def main():                                                                 
    if len(sys.argv) == 3:
        node_data = {'ip' : sys.argv[1], 'port' : sys.argv[2], 'id' : hashlib.sha256(my_address).hexdigest()]
        succesor_data = {}
        predeccesor_data = {}

    if len(sys.argv) == 5:
        node_data = {'ip' : sys.argv[1], 'port' : sys.argv[2], 'id' : hashlib.sha256(my_address).hexdigest()]
        succesor_data = {}
        predeccesor_data = {'ip' : sys.argv[3], 'port' : sys.argv[4]}

    context = zmq.Context()
    socket_client = context.socket(zmq.REQ)
    socket_server = context.socket(zmq.REP)
    # Hilo para el servidor

    bool flag = False

    while True:
        if len(sys.argv) == 5:
            while not flag:
                socket_client.connect("tcp://" + predeccesor_data['ip'] + ":" + predeccesor_data['ip'])
                message = {'request' : "join the ring", 'id' : node_data['id']
                socket_client.send_json(message)
                answer = socket_client.recv()

                if answer == 'yes':
                    socket_client.send_json(node_data)
                    new_succesor = socket_client.recv_json()
                    succesor_data['ip'] = new_succesor['ip']
                    succesor_data['port'] = new_succesor['port']
                    succesor_data['id'] = new_succesor['id']

                    socket_client.disconnect("tcp://" + predeccesor_data['ip'] + ":" + predeccesor_data['ip'])
                    socket_client.connect("tcp://" + succesor_data['ip'] + ":" + succesor_data['ip'])
                    socket_client.send_json(node_data)

                    flag = True
                    print("I joined in this node")

                if answer == 'no':
                    socket_client.send("give me your successor")
                    new_predeccesor = socket_client.recv_json()

                    socket_client.disconnect("tcp://" + predeccesor_data['ip'] + ":" + predeccesor_data['ip'])

                    predeccesor_data['ip'] = new_predeccesor['ip']
                    predeccesor_data['port'] = new_predeccesor['port']
                    predeccesor_data['id'] = new_predeccesor['id']

                    print("trying with the next node")

main()
