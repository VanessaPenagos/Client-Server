'''Recibe notificaciones de todos los nodos que se conectan al anillo'''
import zmq

context = zmq.Context()
socket = context.socket(zmq.REP)
my_ip = '127.0.0.1'
my_port = '5000'
print("Iniciando ...")
socket.bind('tcp://*:' + my_port)

while True:
    notify = socket.recv_json()
    print(notify['message'])
    answer = {'message': "Ok"}
    socket.send_json(answer)
