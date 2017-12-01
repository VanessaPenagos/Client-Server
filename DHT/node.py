import hashlib
import sys
import threading
import zmq


def server(socket_server, node_data, succesor_data, predeccesor_data):
    socket_server.bind('tcp://*:' + node_data['port'])
    print("server started")

    while True:
        request = socket_server.recv_json()
        print("request receive")
        if request['request'] == "join the ring":
            if (request['id'] > node_data['id']) and (request['id'] <=
                                                      succesor_data['id']):
                print("Esta en el rango")
                socket_server.send_string("yes")
                new_succesor = socket_server.recv_json()
                socket_server.send_json(succesor_data)

                succesor_data['ip'] = new_succesor['ip']
                succesor_data['port'] = new_succesor['port']
                succesor_data['id'] = new_succesor['id']
                print("Successor updated")
            elif (request['id'] > node_data['id']) and (request['id'] >
                                                        succesor_data['id']):
                socket_server.send_string("yes")
                new_succesor = socket_server.recv_json()
                socket_server.send_json(succesor_data)

                succesor_data['ip'] = new_succesor['ip']
                succesor_data['port'] = new_succesor['port']
                succesor_data['id'] = new_succesor['id']
                print("Successor updated")

            else:
                print("Es el else //No")
                socket_server.send_string("no")
                message = socket_server.recv_string()
                if (message == "give me your successor"):
                    socket_server.send_json(succesor_data)
                    print("No, better the next node")

        if request['request'] == "Update your predecessor":
            socket_server.send_string("Ok, send me your data")
            new_predeccesor = socket_server.recv_json()
            socket_server.send_string("ok")
            predeccesor_data['ip'] = new_predeccesor['ip']
            predeccesor_data['port'] = new_predeccesor['port']
            predeccesor_data['id'] = new_predeccesor['id']


def main():
    if len(sys.argv) == 3:
        my_address = (sys.argv[1] + sys.argv[2]).encode('utf-8')
        node_data = {
            'ip': sys.argv[1],
            'port': sys.argv[2],
            'id': hashlib.sha256(my_address).hexdigest()
        }
        succesor_data = {'ip': '-1', 'port': '-1', 'id': '-1'}
        predeccesor_data = {'ip': '-1', 'port': '-1', 'id': '-1'}

    if len(sys.argv) == 5:
        my_address = (sys.argv[1] + sys.argv[2]).encode('utf-8')
        node_data = {
            'ip': sys.argv[1],
            'port': sys.argv[2],
            'id': hashlib.sha256(my_address).hexdigest()
        }
        succesor_data = {}
        predeccesor_data = {'ip': sys.argv[3], 'port': sys.argv[4]}

    context = zmq.Context()
    socket_client = context.socket(zmq.REQ)
    socket_server = context.socket(zmq.REP)

    thread_server = threading.Thread(
        target=server,
        args=(
            socket_server,
            node_data,
            succesor_data,
            predeccesor_data,
        ))
    thread_server.start()

    flag = False

    while True:
        if len(sys.argv) == 5:
            while not flag:
                socket_client.connect("tcp://" + predeccesor_data['ip'] + ":" +
                                      predeccesor_data['port'])
                message = {'request': "join the ring", 'id': node_data['id']}
                socket_client.send_json(message)
                answer = socket_client.recv_string()

                if answer == 'yes':
                    socket_client.send_json(node_data)
                    new_succesor = socket_client.recv_json()
                    succesor_data['ip'] = new_succesor['ip']
                    succesor_data['port'] = new_succesor['port']
                    succesor_data['id'] = new_succesor['id']
                    socket_client.disconnect("tcp://" + predeccesor_data['ip']
                                             + ":" + predeccesor_data['port'])

                    socket_client.connect("tcp://" + succesor_data['ip'] +
                                          ":" + succesor_data['ip'])
                    message = {'request': "Update your predecessor"}
                    socket_client.send_json(message)
                    answer = socket_client.recv()
                    if answer == "Ok, send me your data":
                        socket_client.send_json(node_data)
                        ok = socket_client.recv_string()
                        socket_client.disconnect("tcp://" + succesor_data['ip']
                                                 + ":" + succesor_data['port'])

                    flag = True
                    print("I joined in this node")

                if answer == 'no':
                    socket_client.send_string("give me your successor")
                    new_predeccesor = socket_client.recv_json()

                    socket_client.disconnect("tcp://" + predeccesor_data['ip']
                                             + ":" + predeccesor_data['port'])

                    predeccesor_data['ip'] = new_predeccesor['ip']
                    predeccesor_data['port'] = new_predeccesor['port']
                    predeccesor_data['id'] = new_predeccesor['id']

                    print("trying with the next node")


main()
