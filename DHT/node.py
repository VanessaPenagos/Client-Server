import hashlib
import random
import sys
import threading
import zmq
import os

stop = False
def generate_id():
    id = random.randrange(15)
    return id

def server(socket_server, node_data, succesor_data, predeccesor_data):
    socket_server.bind('tcp://*:' + node_data['port'])

    while not stop:
        request = socket_server.recv_json()
        if request['request'] == "join the ring":
            if (request['id'] >= node_data['id']) and (request['id'] <= succesor_data['id']):
                socket_server.send_string("yes")
                new_succesor = socket_server.recv_json()
                socket_server.send_json(succesor_data)

                requ = socket_server.recv_string()
                socket_server.send_string(str(node_data['id']))

                succesor_data['ip'] = new_succesor['ip']
                succesor_data['port'] = new_succesor['port']
                succesor_data['id'] = new_succesor['id']

            elif (request['id'] > node_data['id']) and (request['id'] > succesor_data['id']):
                socket_server.send_string("yes")
                new_succesor = socket_server.recv_json()
                socket_server.send_json(succesor_data)

                requ = socket_server.recv_string()
                socket_server.send_string(str(node_data['id']))

                succesor_data['ip'] = new_succesor['ip']
                succesor_data['port'] = new_succesor['port']
                succesor_data['id'] = new_succesor['id']

            elif (request['id'] < node_data['id']) and (request['id'] < succesor_data['id']) and (node_data['id'] >= succesor_data['id']):
                    socket_server.send_string("yes")
                    new_succesor = socket_server.recv_json()
                    socket_server.send_json(succesor_data)

                    requ = socket_server.recv_string()
                    socket_server.send_string(str(node_data['id']))

                    succesor_data['ip'] = new_succesor['ip']
                    succesor_data['port'] = new_succesor['port']
                    succesor_data['id'] = new_succesor['id']

            else:
                print("Es el else //No")
                socket_server.send_string("no")
                message = socket_server.recv_string()
                if (message == "give me your successor"):
                    socket_server.send_json(succesor_data)

        if request['request'] == "Update your predecessor":
            socket_server.send_string("Ok, send me your data")
            new_predeccesor = socket_server.recv_json()
            socket_server.send_string("ok")
            predeccesor_data['ip'] = new_predeccesor['ip']
            predeccesor_data['port'] = new_predeccesor['port']
            predeccesor_data['id'] = new_predeccesor['id']

        if request['request'] == "Update your successor":
            socket_server.send_string("Ok, send me your data")
            new_succesor = socket_server.recv_json()
            socket_server.send_string("ok")
            succesor_data['ip'] = new_succesor['ip']
            succesor_data['port'] = new_succesor['port']
            succesor_data['id'] = new_succesor['id']


def main():
    if len(sys.argv) == 3:
        my_address = (sys.argv[1] + sys.argv[2]).encode('utf-8')
        node_data = { 'ip': sys.argv[1], 'port': sys.argv[2], 'id': generate_id()}
        succesor_data = {'ip': sys.argv[1], 'port': sys.argv[2], 'id': node_data['id']}
        predeccesor_data = {'ip': sys.argv[1], 'port': sys.argv[2], 'id': node_data['id']}
        flag = True

    if len(sys.argv) == 5:
        my_address = (sys.argv[1] + sys.argv[2]).encode('utf-8')
        node_data = {'ip': sys.argv[1], 'port': sys.argv[2], 'id': generate_id()}
        succesor_data = {}
        predeccesor_data = {'ip': sys.argv[3], 'port': sys.argv[4]}
        flag = False

    context = zmq.Context()
    socket_client = context.socket(zmq.REQ)
    socket_server = context.socket(zmq.REP)
    run = True
    thread_server = threading.Thread(target=server, args=(socket_server, node_data,succesor_data,predeccesor_data,))
    thread_server.start()


    while True:
        while not flag:
            socket_client.connect("tcp://" + predeccesor_data['ip'] + ":" + predeccesor_data['port'])
            message = {'request': "join the ring", 'id': node_data['id']}
            socket_client.send_json(message)
            answer = socket_client.recv_string()

            if answer == 'yes':
                socket_client.send_json(node_data)
                new_succesor = socket_client.recv_json()
                succesor_data['ip'] = new_succesor['ip']
                succesor_data['port'] = new_succesor['port']
                succesor_data['id'] = new_succesor['id']

                socket_client.send_string("give me your id")
                predeccesor_data['id'] = int(socket_client.recv_string())

                socket_client.disconnect("tcp://" + predeccesor_data['ip'] + ":" + predeccesor_data['port'])
                socket_client.connect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])
                message = {'request': "Update your predecessor"}
                socket_client.send_json(message)
                answer = socket_client.recv_string()

                if answer == "Ok, send me your data":
                    socket_client.send_json(node_data)
                    ok = socket_client.recv_string()
                    socket_client.disconnect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])

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

        print("My id is : " + str(node_data['id']))
        print("My succesor is : " + str(succesor_data['id']))
        print("My predeccesor is : " + str(predeccesor_data['id']))

        exit = input("Do you want to get out of the ring? yes/no \n")
        if exit == "yes":
            #update predeccesor_data in my successor
            socket_client.connect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])
            message = {'request': "Update your predecessor"}
            socket_client.send_json(message)
            answer = socket_client.recv_string()

            if answer == "Ok, send me your data":
                socket_client.send_json(predeccesor_data)
                ok = socket_client.recv_string()
                socket_client.disconnect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])

            #update succesor_data in my predecessor
            socket_client.connect("tcp://" + predeccesor_data['ip'] + ":" + predeccesor_data['port'])
            message = {'request': "Update your successor"}
            socket_client.send_json(message)
            answer = socket_client.recv_string()

            if answer == "Ok, send me your data":
                socket_client.send_json(succesor_data)
                ok = socket_client.recv_string()
                socket_client.disconnect("tcp://" + predeccesor_data['ip'] + ":" + predeccesor_data['port'])

            print("Good bye")
            os._exit(0)

main()
