from collections import deque

def bfs(adj_matrix, start, goal):
    num_nodes = len(adj_matrix)
    visited = set()
    queue = deque([(start, [start])])  # Maintain path in the queue

    while queue:
        #print("Queue:", list(queue))  # Print queue state
        node, path = queue.popleft()
        #print("Processing node:", node, "| Path so far:", path)

        if node == goal:
            print("Final Visited nodes in BFS:", visited)
            return path  # Goal found, return path

        if node not in visited:
            visited.add(node)
            #print("Visited nodes:", visited)

            for neighbor, is_connected in enumerate(adj_matrix[node]):
                #print(neighbor,is_connected)
                if is_connected and neighbor not in visited:
                    #print(f"Adding neighbor {neighbor} to queue")
                    queue.append((neighbor, path + [neighbor]))  # Extend path

    print("Visited nodes in BFS:", visited)
    return None  # Goal not found

def dfs(adj_matrix, start, goal):
    num_nodes = len(adj_matrix)
    visited = set()
    stack = [(start, [start])]  # Stack stores (node, path)

    while stack:
        #print("Stack:", stack)  # Print stack state
        node, path = stack.pop()
        #print("Processing node:", node, "| Path so far:", path)

        if node == goal:
            print("Visited nodes in DFS:", visited)
            return path  # Goal found, return path

        if node not in visited:
            visited.add(node)
            #print("Visited nodes:", visited)

            # Add neighbors in reverse order (so lowest index is explored first)
            for neighbor in range(num_nodes - 1, -1, -1):
                if adj_matrix[node][neighbor] and neighbor not in visited:
                    #print(f"Adding neighbor {neighbor} to stack")
                    stack.append((neighbor, path + [neighbor]))  # Extend path

    print("Visited nodes in DFS:", visited)
    return None  # Goal not found

def dls(adj_matrix, start, goal, depth_limit):
    num_nodes = len(adj_matrix)
    visited = set()
    stack = [(start, [start], 0)]  # Stack stores (node, path, depth)

    while stack:
        #print("Stack:", stack)  # Print stack state
        node, path, depth = stack.pop()
        #print("Processing node:", node, "| Depth:", depth, "| Path so far:", path)

        if node == goal:
            print("Visited nodes in DLS:", visited)
            return path  # Goal found, return path

        if node not in visited:
            visited.add(node)
            #print("Visited nodes:", visited)

        # Continue search only if depth limit is not reached
        if depth < depth_limit:
            for neighbor in range(num_nodes - 1, -1, -1):
                if adj_matrix[node][neighbor] and neighbor not in visited:
                    #print(f"Adding neighbor {neighbor} to stack at depth {depth + 1}")
                    stack.append((neighbor, path + [neighbor], depth + 1))  # Extend path

    print("Visited nodes in DLS:", visited)
    return None  # Goal not found within depth limit
'''
# Example usage
adj_matrix = [
    [0, 1, 0, 1],
    [1, 0, 1, 0],
    [0, 1, 0, 1],
    [1, 0, 1, 0]
]
print("Adjacency Matrix: ", adj_matrix)
start_node = int(input("Enter the start node (0-indexed): "))
goal_node = int(input("Enter the goal node (0-indexed): "))
#depth_limit = int(input("Enter the depth limit: "))

#bfs_path = bfs(adj_matrix, start_node, goal_node)
#print("BFS Traversal Path:", bfs_path if bfs_path else "Goal not found")

#dfs_path = dfs(adj_matrix, start_node, goal_node)
#print("DFS Traversal Path:", dfs_path if dfs_path else "Goal not found")

#dls_path = dls(adj_matrix, start_node, goal_node, depth_limit)
#print("DLS Traversal Path:", dls_path if dls_path else "Goal not found within depth limit")
'''
import heapq

def ucs(adj_matrix, start, goal):
    num_nodes = len(adj_matrix)
    visited = set()
    priority_queue = [(0, start, [start])]  # Min-heap: (cost, node, path)

    while priority_queue:
        #print("Priority Queue:", priority_queue)  # Print queue state
        cost, node, path = heapq.heappop(priority_queue)
        #print(f"Processing node: {node} | Cost: {cost} | Path so far: {path}")

        if node == goal:
            print("Visited nodes in UCS:", visited)
            return path, cost  # Goal found, return path and cost

        if node not in visited:
            visited.add(node)
            #print("Visited nodes:", visited)

            for neighbor in range(num_nodes):
                if adj_matrix[node][neighbor] > 0 and neighbor not in visited:
                    new_cost = cost + adj_matrix[node][neighbor]
                    #print(f"Adding neighbor {neighbor} with cost {new_cost} to priority queue")
                    heapq.heappush(priority_queue, (new_cost, neighbor, path + [neighbor]))

    print("Visited nodes in UCS:", visited)
    return None, float('inf')  # Goal not found

'''
# Example adjacency matrix (weighted graph)
adj_matrix = [
    [0, 1, 0, 4],  # Node 0 -> Node 1 (cost 1), Node 3 (cost 4)
    [1, 0, 2, 0],  # Node 1 -> Node 0 (cost 1), Node 2 (cost 2)
    [0, 2, 0, 3],  # Node 2 -> Node 1 (cost 2), Node 3 (cost 3)
    [4, 0, 3, 0]  # Node 3 -> Node 0 (cost 4), Node 2 (cost 3)
]
print("Adjacency Matrix: ", adj_matrix)
# User input for start and goal nodes
start_node = int(input("Enter the start node (0-indexed): "))
goal_node = int(input("Enter the goal node (0-indexed): "))

ucs_path, ucs_cost = ucs(adj_matrix, start_node, goal_node)
print("UCS Traversal Path:", ucs_path if ucs_path else "Goal not found")
print("Total Cost:", ucs_cost if ucs_path else "N/A")
'''
def ids(adj_matrix, start, goal, max_depth):
    """Performs Iterative Deepening Search (IDS) up to max_depth."""
    for depth in range(max_depth + 1):
        print(f"\n=== Searching with depth limit = {depth} ===")
        path = dls(adj_matrix, start, goal, depth)
        if path:
            print("Visited nodes in IDS:", path)
            return path  # Return the first successful path found

    print("Goal not found within depth limit")
    return None  # Goal not found within max depth
'''
# Example adjacency matrix (unweighted graph)
adj_matrix = [
    [0, 1, 0, 1],  # Node 0 is connected to 1 and 3
    [1, 0, 1, 0],  # Node 1 is connected to 0 and 2
    [0, 1, 0, 1],  # Node 2 is connected to 1 and 3
    [1, 0, 1, 0]   # Node 3 is connected to 0 and 2
]
print("Adjacency Matrix: ", adj_matrix)
# User input for start, goal, and max depth
start_node = int(input("Enter the start node (0-indexed): "))
goal_node = int(input("Enter the goal node (0-indexed): "))
max_depth = int(input("Enter the maximum depth: "))

ids_path = ids(adj_matrix, start_node, goal_node, max_depth)
print("\nIDS Traversal Path:", ids_path if ids_path else "Goal not found")
'''
from collections import deque


def ibfs(adj_matrix, start, goal, max_depth):
    """Performs Iterative Breadth-First Search (IBFS) up to max_depth."""
    num_nodes = len(adj_matrix)

    for depth in range(max_depth + 1):
        #print(f"\n=== Searching with depth limit = {depth} ===")
        visited = set()
        queue = deque([(start, [start], 0)])  # (node, path, depth)

        while queue:
            #print("Queue:", queue)  # Print queue state
            node, path, depth_level = queue.popleft()
            #print(f"Processing node: {node} | Depth: {depth_level} | Path so far: {path}")

            if node == goal:
                print("Visited nodes in IBFS:", visited)
                return path  # Goal found, return path

            if node not in visited:
                visited.add(node)
                #print("Visited nodes:", visited)

            # Continue search only if within the depth limit
            if depth_level < depth:
                for neighbor, is_connected in enumerate(adj_matrix[node]):
                    if is_connected and neighbor not in visited:
                        #print(f"Adding neighbor {neighbor} to queue at depth {depth_level + 1}")
                        queue.append((neighbor, path + [neighbor], depth_level + 1))  # Extend path

    print("Goal not found within depth limit")
    return None  # Goal not found within max depth

'''
# Example adjacency matrix (unweighted graph)
adj_matrix = [
    [0, 1, 0, 1],  # Node 0 is connected to 1 and 3
    [1, 0, 1, 0],  # Node 1 is connected to 0 and 2
    [0, 1, 0, 1],  # Node 2 is connected to 1 and 3
    [1, 0, 1, 0]  # Node 3 is connected to 0 and 2
]
print("Adjacency Matrix: ", adj_matrix)
# User input for start, goal, and max depth
start_node = int(input("Enter the start node (0-indexed): "))
goal_node = int(input("Enter the goal node (0-indexed): "))
max_depth = int(input("Enter the maximum depth: "))

ibfs_path = ibfs(adj_matrix, start_node, goal_node, max_depth)
print("\nIBFS Traversal Path:", ibfs_path if ibfs_path else "Goal not found")
'''
import heapq


def best_first_search(adj_matrix, heuristic, start, goal):
    num_nodes = len(adj_matrix)
    visited = set()
    priority_queue = [(heuristic[start], start, [start])]  # (heuristic, node, path)

    while priority_queue:
        #print("Priority Queue:", priority_queue)  # Debugging

        _, node, path = heapq.heappop(priority_queue)  # Pop node with the lowest heuristic
        #print(f"Processing node: {node} | Path so far: {path}")

        if node == goal:
            print("Visited nodes in Best-First Search:", visited)
            return path  # Goal found, return path

        if node not in visited:
            visited.add(node)

        for neighbor, is_connected in enumerate(adj_matrix[node]):
            if is_connected and neighbor not in visited:
                heapq.heappush(priority_queue, (heuristic[neighbor], neighbor, path + [neighbor]))
                #print(f"Adding neighbor {neighbor} to priority queue with heuristic {heuristic[neighbor]}")

    print("Goal not found")
    return None
'''
adj_matrix = [
    [0, 1, 0, 1],  # Node 0 is connected to 1 and 3
    [1, 0, 1, 0],  # Node 1 is connected to 0 and 2
    [0, 1, 0, 1],  # Node 2 is connected to 1 and 3
    [1, 0, 1, 0]  # Node 3 is connected to 0 and 2
]
print("Adjacency Matrix: ", adj_matrix)
# User input for start, goal, and max depth
start_node = int(input("Enter the start node (0-indexed): "))
goal_node = int(input("Enter the goal node (0-indexed): "))
#Example Heuristic Values (h(n))
heuristic = [3, 2, 1, 4]  # Example heuristic estimates for each node
print("Heuristic values:",heuristic )

best_path = best_first_search(adj_matrix, heuristic, start_node, goal_node)
print("\nBest-First Search Path:", best_path if best_path else "Goal not found")
'''

import heapq

def a_star_search(adj_matrix, heuristic, start, goal):
    num_nodes = len(adj_matrix)
    visited = set()
    priority_queue = [(heuristic[start], 0, start, [start])]  # (f(n), g(n), node, path)

    while priority_queue:
        _, g, node, path = heapq.heappop(priority_queue)  # Pick node with lowest f(n)

        if node == goal:
            return path  # Goal found

        if node not in visited:
            visited.add(node)

        for neighbor, cost in enumerate(adj_matrix[node]):  # Iterate through all neighbors
            if cost > 0 and neighbor not in visited:  # If there's an edge
                new_g = g + cost  # Update path cost g(n)
                f = new_g + heuristic[neighbor]  # Compute f(n) = g(n) + h(n)
                heapq.heappush(priority_queue, (f, new_g, neighbor, path + [neighbor]))

    return None  # Goal not found

adj_matrix = [
    [0, 1, 4, 0],  # Edge from 0 → 1 (cost 1), 0 → 2 (cost 4)
    [1, 0, 2, 5],  # Edge from 1 → 0 (cost 1), 1 → 2 (cost 2), 1 → 3 (cost 5)
    [4, 2, 0, 1],  # Edge from 2 → 0 (cost 4), 2 → 1 (cost 2), 2 → 3 (cost 1)
    [0, 5, 1, 0]   # Edge from 3 → 1 (cost 5), 3 → 2 (cost 1)
]
print("Adjacency Matrix: ", adj_matrix)
# User input for start, goal, and max depth
start = int(input("Enter the start node (0-indexed): "))
goal = int(input("Enter the goal node (0-indexed): "))

heuristic = [7, 6, 2, 0]  # Estimated cost to reach goal (example values)
print("Heuristic values:",heuristic )

path = a_star_search(adj_matrix, heuristic, start, goal)
print("A* Path:", path)
