# https://youtu.be/71CEj4gKDnE
# https://youtu.be/-L-WgKMFuhE
# https://youtu.be/ySN5Wnu88nE
# https://youtu.be/i0x5fj4PqP4
# https://youtu.be/PzEWHH2v3TE

from blockworld import BlockWorld

from copy import deepcopy
import numpy as np
from queue import PriorityQueue

class BlockWorldHeuristic(BlockWorld):
	def __init__(self, num_blocks=5, state=None):
		BlockWorld.__init__(self, num_blocks, state)
		self.history = []
		self.h = -1
		self.g = -1

	def heuristic(self, goal):
		self_state = list(self.get_state())
		goal_state = list(goal.get_state())

		h = 0
		for s in self_state:
			same_boxes = 0

			for g in goal_state:
				index = -1
				
				while index + min(len(s),len(g)) >= 0:
					if s[index] == g[index]:
						same_boxes+=1
					else:
						break
					index -= 1

				if same_boxes != 0:
					break

			h += len(s) - same_boxes
	
		return h

class AStar():
	def search(self, start, goal):
		pq = PriorityQueue()
		visited = []
		current = start
		
		while current.conf != goal.conf:
			for action, neighbor in current.get_neighbors():
				if neighbor in visited: continue
				neighbor.h = neighbor.heuristic(goal)
				neighbor.history = deepcopy(current.history)
				neighbor.history.append(action)
				neighbor.g = len(neighbor.history)

				pq.put(((neighbor.h + neighbor.g,neighbor.h),neighbor))
			
			visited.append(current)
			current = pq.get()[1]
			
		return current.history

if __name__ == '__main__':
	# Here you can test your algorithm. You can try different N values, e.g. 6, 7.
	N = 6

	start = BlockWorldHeuristic(N)
	goal = BlockWorldHeuristic(N)
	
	print(start.conf)

	
	print("Searching for a path:")
	print(f"{start} -> {goal}")
	print()

	astar = AStar()
	
	path = astar.search(start, goal)

	if path is not None:
		print("Found a path:")
		print(path)

		print("\nHere's how it goes:")

		s = start.clone()
		print(s)

		for a in path:
			s.apply(a)
			print(s)

	else:
		print("No path exists.")

	print("Total expanded nodes:", BlockWorld.expanded)