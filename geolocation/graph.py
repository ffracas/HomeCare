import matplotlib.pyplot as plt
import numpy as np
import json

def load_data(file_path):
    with open(file_path, 'r') as file:
        return json.load(file)

def find(id, patients):
    for i, patient in enumerate(patients):
        if patient['id'] == id:
            return i

# genera il percorso nel grafo
def generateGraph(solution, patients):
    graph = []
    edges = []
    for path in solution:
        i = 0
        j = 1
        nodesPath = []
        nodesEdges = []
        for node in path:
            index = find(node, patients)
            info = {
                'x': patients[index]['location'][0],
                'y': patients[index]['location'][1]
            }
            p = [i, j]
            j += 1
            i += 1
            if j == len(path):
                j = 0
            nodesEdges.append(p)
            nodesPath.append(info)
        edges.append(nodesEdges)
        graph.append(nodesPath)
    return graph, edges

# disegna il singolo grafo (percorso)
def plot_graph(points, edges, color):
    # Estrai le coordinate x e y dai punti
    x_coords = [point['x'] for point in points]
    y_coords = [point['y'] for point in points]

    # Plot dei punti
    print (color)
    plt.scatter(x_coords, y_coords, color=[c/255 for c in color])

    # Annotazione dei punti
    for i, point in enumerate(points):
        plt.text(point['x'], point['y'], f' {i}', fontsize=12, ha='right')
    
    print(points)
    # Unisci i punti con delle linee secondo gli archi
    for edge in edges:
        point1, point2 = edge
        x_values = [points[point1]['x'], points[point2]['x']]
        y_values = [points[point1]['y'], points[point2]['y']]
        plt.plot(x_values, y_values, color=[c/255 for c in color])

    # Aggiungi etichette e griglia
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Grafo con punti e percorsi')
    #plt.grid(True)
    
# prepara i dati dell'istanza per la trasformazione in grafo
def formatPatients(patients, depot):
    co = { "id": depot['id'], "location": depot['location'] }
    nodes = [co]
    for patient in patients:
        node = {
            "id": patient['id'],
            "location": patient['location']
        }
        nodes.append(node)
    return nodes

# prepara i dati della soluzione per la trasformazione in grafo
def formatSolution(routes, depot):
    paths = []
    for route in routes:
        path = [depot['id']]
        for location in route['locations']:
            id = location['patient']
            path.append(id)
        paths.append(path)
    return paths

# crea un colore
def randomColorV():
    return np.random.choice(range(256))

if __name__ == "__main__":
    # caricamento file
    sol  = load_data('../resources/hr_solutions/001-firenze-2.json')
    data = load_data('../resources/hr_input/instance_001-florence-r19-p165-s3-sim20.3-seq22.1.json')
    # formattazione dati
    nodes = formatPatients(data['patients'], data['central_offices'][0])
    sol   = formatSolution(sol['routes'], data['central_offices'][0])
    # creazione grafo
    graph, edge = generateGraph(sol, nodes)
    #print (graph, edge)

    # Chiama la funzione per disegnare il grafo con i punti e i percorsi
    for path, edges in zip(graph, edge):
        color = (randomColorV(), randomColorV(), randomColorV())
        #print(color)
        plot_graph(path, edges, color)
    #mostra grafo
    plt.show()
