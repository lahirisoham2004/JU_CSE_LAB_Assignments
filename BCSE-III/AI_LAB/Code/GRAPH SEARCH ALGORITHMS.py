# 1. Breadth-First Search (BFS)
# Explores neighbors first before going deeper.
from collections import deque

def bfs(adj_matrix, start, goal):
    queue = deque([(start, [start])])
    visited = set()
    while queue:
        node, path = queue.popleft()
        if node == goal:
            return path
        if node not in visited:
            visited.add(node)
            for neighbor, is_connected in enumerate(adj_matrix[node]):
                if is_connected and neighbor not in visited:
                    queue.append((neighbor, path + [neighbor]))
    return None

# 2. Depth-First Search (DFS)
# Explores deeper nodes before neighbors.
def dfs(adj_matrix, start, goal):
    stack = [(start, [start])]
    visited = set()
    while stack:
        node, path = stack.pop()
        if node == goal:
            return path
        if node not in visited:
            visited.add(node)
            for neighbor in range(len(adj_matrix) - 1, -1, -1):
                if adj_matrix[node][neighbor] and neighbor not in visited:
                    stack.append((neighbor, path + [neighbor]))
    return None

# 3. Uniform Cost Search (UCS)
# Explores lowest-cost path first.
import heapq
def ucs(adj_matrix, start, goal):
    pq = [(0, start, [start])]
    visited = set()
    while pq:
        cost, node, path = heapq.heappop(pq)
        if node == goal:
            return path, cost
        if node not in visited:
            visited.add(node)
            for neighbor, edge_cost in enumerate(adj_matrix[node]):
                if edge_cost > 0 and neighbor not in visited:
                    heapq.heappush(pq, (cost + edge_cost, neighbor, path + [neighbor]))
    return None, float('inf')

# 4. Iterative Deepening Search (IDS)
# Combines depth-limited searches up to max depth.
def dls(adj_matrix, start, goal, depth_limit):
    stack = [(start, [start], 0)]
    visited = set()
    while stack:
        node, path, depth = stack.pop()
        if node == goal:
            return path
        if node not in visited:
            visited.add(node)
        if depth < depth_limit:
            for neighbor in range(len(adj_matrix) - 1, -1, -1):
                if adj_matrix[node][neighbor] and neighbor not in visited:
                    stack.append((neighbor, path + [neighbor], depth + 1))
    return None

def ids(adj_matrix, start, goal, max_depth):
    for depth in range(max_depth + 1):
        path = dls(adj_matrix, start, goal, depth)
        if path:
            return path
    return None

# 5. Best-First Search (Greedy Search)
# Uses heuristic to explore most promising node first.
def best_first_search(adj_matrix, heuristic, start, goal):
    pq = [(heuristic[start], start, [start])]
    visited = set()
    while pq:
        _, node, path = heapq.heappop(pq)
        if node == goal:
            return path
        if node not in visited:
            visited.add(node)
        for neighbor, is_connected in enumerate(adj_matrix[node]):
            if is_connected and neighbor not in visited:
                heapq.heappush(pq, (heuristic[neighbor], neighbor, path + [neighbor]))
    return None

# 6. A* Search
# Uses both cost (g) and heuristic (h) to find optimal path.
def a_star_search(adj_matrix, heuristic, start, goal):
    pq = [(heuristic[start], 0, start, [start])]
    visited = set()
    while pq:
        _, g, node, path = heapq.heappop(pq)
        if node == goal:
            return path
        if node not in visited:
            visited.add(node)
        for neighbor, cost in enumerate(adj_matrix[node]):
            if cost > 0 and neighbor not in visited:
                new_g = g + cost
                f = new_g + heuristic[neighbor]
                heapq.heappush(pq, (f, new_g, neighbor, path + [neighbor]))
    return None