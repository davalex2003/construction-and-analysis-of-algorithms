import random


def dfs(v, graph, visited):
    stack = []
    visited[v] = True
    stack.append(v)
    while len(stack) > 0:
        u = stack.pop()
        for k in range(len(graph[u])):
            if graph[u][k] == 1 and not visited[k]:
                stack.append(k)
                visited[k] = True


def generate_not_complete(n):
    graph = [[0 for _ in range(n)] for _ in range(n)]
    m = n * (n - 1) // 2
    kol = int(0.5 * m)
    connected = False
    while not connected:
        for i in range(n):
            for j in range(n):
                graph[i][j] = 0
        for _ in range(kol):
            v1 = random.randint(0, n - 1)
            v2 = random.randint(0, n - 1)
            graph[v1][v2] = 1
            graph[v2][v1] = 1
        visited = [False] * n
        dfs(0, graph, visited)
        if all(visited):
            connected = True
    return graph


def generate_sparse(num_vertices):
    graph = [[0 for _ in range(num_vertices)] for _ in range(num_vertices)]
    vertices = list(range(1, num_vertices))
    for v in vertices:
        prev_vertex = random.choice(range(v))
        graph[v][prev_vertex] = 1
        graph[prev_vertex][v] = 1
    return graph


for i in range(10, 1011, 50):
    f = open("complete\\" + str(i) + ".txt", "w")
    f.write(f"{int((i * (i - 1)) / 2)}\n")
    for j in range(1, i + 1):
        for k in range(j + 1, i + 1):
            f.write(f"{j} {k} {random.randint(1, 10)}\n")
    f.close()

for ii in range(10, 1011, 50):
    f = open("sparse\\" + str(ii) + ".txt", "w")
    g = generate_sparse(ii)
    k = 0
    for i in range(ii):
        for j in range(i + 1, ii):
            if g[i][j] == 1:
                k += 1
    f.write(f"{k}\n")
    for i in range(ii):
        for j in range(i + 1, ii):
            if g[i][j] == 1:
                f.write(f"{i + 1} {j + 1} {random.randint(1, 10)}\n")
    f.close()

for ii in range(10, 1011, 50):
    f = open("not_complete\\" + str(ii) + ".txt", "w")
    g = generate_not_complete(ii)
    k = 0
    for i in range(ii):
        for j in range(i + 1, ii):
            if g[i][j] == 1:
                k += 1
    f.write(f"{k}\n")
    for i in range(ii):
        for j in range(i + 1, ii):
            if g[i][j] == 1:
                f.write(f"{i + 1} {j + 1} {random.randint(1, 10)}\n")
    f.close()
