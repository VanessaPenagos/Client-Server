import hashlib
import random
import sys
import threading
import zmq
import os
import math
#9645


space_keys = 16
m = int(math.log2(space_keys))

def generate_finger(node_id):
 	finger = {}
 	for i in range(0,m):
        key = (node_id + (2 ** i)) % space_keys
 		finger[key] = {}
 	return(finger)

def generate_id():
    id = random.randrange(15)
    return id

def server(socket_server, node_data, succesor_data, predecessor_data):
    socket_server.bind('tcp://*:' + node_data['port'])

    while True:
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
            new_predecessor = socket_server.recv_json()
            socket_server.send_string("ok")
            predecessor_data['ip'] = new_predecessor['ip']
            predecessor_data['port'] = new_predecessor['port']
            predecessor_data['id'] = new_predecessor['id']

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
        predecessor_data = {'ip': sys.argv[1], 'port': sys.argv[2], 'id': node_data['id']}
        finger_table = generate_finger(node_data['id'])
        joined = True

    if len(sys.argv) == 5:
        my_address = (sys.argv[1] + sys.argv[2]).encode('utf-8')
        node_data = {'ip': sys.argv[1], 'port': sys.argv[2], 'id': generate_id()}
        succesor_data = {}
        predecessor_data = {'ip': sys.argv[3], 'port': sys.argv[4]}
        finger_table = generate_finger(node_data['id'])
        joined = False

    context = zmq.Context()
    socket_client = context.socket(zmq.REQ)
    socket_server = context.socket(zmq.REP)
    thread_server = threading.Thread(target=server, args=(socket_server, node_data,succesor_data,predecessor_data,))
    thread_server.start()

    # Cliente
    while True:
        while not joined:
            socket_client.connect("tcp://" + predecessor_data['ip'] + ":" + predecessor_data['port'])
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
                predecessor_data['id'] = int(socket_client.recv_string())

                socket_client.disconnect("tcp://" + predecessor_data['ip'] + ":" + predecessor_data['port'])
                socket_client.connect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])
                message = {'request': "Update your predecessor"}
                socket_client.send_json(message)
                answer = socket_client.recv_string()

                if answer == "Ok, send me your data":
                    socket_client.send_json(node_data)
                    ok = socket_client.recv_string()
                    socket_client.disconnect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])

                joined = True
                print("I joined in this node")

            if answer == 'no':
                socket_client.send_string("give me your successor")
                new_predecessor = socket_client.recv_json()

                socket_client.disconnect("tcp://" + predecessor_data['ip']
                                         + ":" + predecessor_data['port'])

                predecessor_data['ip'] = new_predecessor['ip']
                predecessor_data['port'] = new_predecessor['port']
                predecessor_data['id'] = new_predecessor['id']

        found = False
        for key in finger_table:
            connection_id = succesor_data['id']
            connection_ip = succesor_data['ip']
            connection_port = succesor_data['port']
            while not found:
                if key < id_succesor:
                    finger_table[key]['id'] = connection_id
                    finger_table[key]['ip'] = connection_ip
                    finger_table[key]['port'] = connection_port
                    found = True  
                else:
                    socket_client.connect("tcp://" + connection_ip + ":" + connection_port)
                    message = {'request': "Give me the data of your successor"}
                    socket_client.send_json(message)
                    answer = socket_client.recv_json()
                    connection_id = answer['id']

        print("My id is : " + str(node_data['id']))
        print("My succesor is : " + str(succesor_data['id']))
        print("My predecessor is : " + str(predecessor_data['id']))
        print(finger_table)

        exit = input("Do you want to get out of the ring? yes/no \n")
        if exit == "yes":
            #update predecessor_data in my successor
            socket_client.connect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])
            message = {'request': "Update your predecessor"}
            socket_client.send_json(message)
            answer = socket_client.recv_string()

            if answer == "Ok, send me your data":
                socket_client.send_json(predecessor_data)
                ok = socket_client.recv_string()
                socket_client.disconnect("tcp://" + succesor_data['ip'] + ":" + succesor_data['port'])

            #update succesor_data in my predecessor
            socket_client.connect("tcp://" + predecessor_data['ip'] + ":" + predecessor_data['port'])
            message = {'request': "Update your successor"}
            socket_client.send_json(message)
            answer = socket_client.recv_string()

            if answer == "Ok, send me your data":
                socket_client.send_json(succesor_data)
                ok = socket_client.recv_string()
                socket_client.disconnect("tcp://" + predecessor_data['ip'] + ":" + predecessor_data['port'])

            print("Good bye")
            os._exit(0)

main()
