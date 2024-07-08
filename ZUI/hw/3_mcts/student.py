import random, time
import numpy as np

import ox

class Node:
    def __init__(self) -> None:
        self.n = 0
        self.w = 0


class Bandit:
    def total_visits(self) -> int:
        raise NotImplementedError()
    def select(self) -> int:
        raise NotImplementedError()
    def update(self, value: float) -> None:
        self.visits[self.last_idx] += 1
        self.qs[self.last_idx] += (value - self.qs[self.last_idx]) \
                                  / self.visits[self.last_idx]
    def best_action(self) -> int:
        raise NotImplementedError()

class UcbBandit(Bandit):
    def __init__(self, state: ox.Board, c_uct):
        self.c_uct = c_uct
        self.actions = state.get_actions()
        self.qs = np.zeros(len(self.actions))
        self.visits = np.zeros(len(self.actions))  
        self.last_idx = None

    def total_visits(self) -> int:
        return np.sum(self.visits)

    def select(self) -> int:
        total_visits = self.total_visits()
        ucb_vals = (
            self.qs + self.c_uct * np.sqrt(np.log(total_visits)/self.visits)
        )
        self.last_idx = np.argmax(ucb_vals)

        return list(self.actions)[self.last_idx]

    def best_action(self) -> int:
        return list(self.actions)[np.argmax(self.qs)]

def random_simulation(state: ox.Board) -> list[float]:
    while not state.is_terminal():
        random_action = np.random.choice(list(state.get_actions()))
        state.apply_action(random_action)
    return state.get_rewards()

def node_depth(node: str) -> int:
  if node == "": 
    return 0
  else:
    return int((len(node) - 1) / 3 + 1)

class MCTSBot:
    def __init__(self, play_as: int, time_limit: float):
        self.play_as = play_as
        self.time_limit = time_limit * 0.9

        self.node_table = dict()
        
    def has_node(self, state: ox.Board) -> bool:
        return str(state.history) in self.node_table
    def get_node(self, state: ox.Board):
        return self.node_table[str(state.history)]
    def make_node(self, state: ox.Board):
        self.node_table[str(state.history)] = UcbBandit(state,2)

    def deepest_node(self) -> int:
        return max(node_depth(node) for node in self.node_table.keys())

    def node_stats(self):
      # Count the number of visited nodes at each depth. 
      nodes = sorted(self.node_table.keys())
      node_stats = defaultdict(int)

      for node in nodes:
        node_stats[node_depth(node)] += 1
      
      return node_stats

    def best_action(self, state: ox.Board) -> int:
        return self.get_node(state).best_action()

    def step(self, state: ox.Board):
      # An invariant we rely on: there is always a bandit available 
      # for the current state to search from.
      if not self.has_node(state):
          self.make_node(state)

      # Therefore, there is always a trace:
      trace, last_state = self.select(state.clone())

      # Compute values, either by asking the game, or by making a simulation.
      if last_state.is_terminal():
          values = last_state.get_rewards()
      else:
          self.expand(last_state)
          values = random_simulation(last_state.clone())

      # Backpropagate the values to make bandit updates along the trace,
      # except for the last node on the trace -- the leaf or the terminal.
      self.backpropagate(trace, values)

    def select(self, state: ox.Board):
        trace = [] # <- push state.clone() here

        while self.has_node(state):
          if state.is_terminal():
            break

          trace.append(state.clone())
          bandit = self.get_node(state)
          action = bandit.select()
          state.apply_action(action)
        
        return trace, state.clone() # trace & last_state

    def expand(self, state: ox.Board) -> None:
        self.make_node(state)



    def backpropagate(self, trace, values) -> None:
        for state in reversed(trace):
          self.get_node(state).update(values[state.current_player()])

    def play_action(self, board: ox.Board):
        # TODO: implement MCTS bot
        # https://youtu.be/UXW2yZndl7U
        # https://matthewdeakos.me/2018/03/10/monte-carlo-tree-search/
        

        start_time = time.time()
        while (time.time() - start_time) < self.time_limit:
            self.step(board)

        return self.get_node(board).best_action()


if __name__ == '__main__':
    board = ox.Board(8)  # 8x8
    bots = [MCTSBot(0, 0.1), MCTSBot(1, 1.0)]

    # try your bot against itself
    while not board.is_terminal():
        current_player = board.current_player()
        current_player_mark = ox.MARKS_AS_CHAR[ ox.PLAYER_TO_MARK[current_player] ]

        current_bot = bots[current_player]
        a = current_bot.play_action(board)
        board.apply_action(a)

        print(f"{current_player_mark}: {a} -> \n{board}\n")