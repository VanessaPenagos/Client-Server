import hashlib

class Node:
    def __init__(self, my_ip, my_port, succesor_ip= None, succesor_port= None):
        ''' Crea el nodo'''
        my_address = str(my_ip + my_port)
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