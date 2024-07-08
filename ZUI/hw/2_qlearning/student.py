from blockworld import BlockWorldEnv
import numpy as np
import random
from collections import defaultdict

class QLearning():
	# don't modify the methods' signatures!
	def __init__(self, env: BlockWorldEnv):
		self.env = env
		self.qtable = defaultdict(lambda: 0, dict())

	def train(self):
		# https://youtube.com/playlist?list=PLZbbT5o_s2xoWNVdDudn51XM8lOuZ_Njv
		# https://github.com/simoninithomas/Deep_reinforcement_learning_Course/blob/master/Q%20learning/FrozenLake/Q%20Learning%20with%20FrozenLake.ipynb

		total_episodes = 15000        # Total episodes (for non-while training)
		learning_rate = 0.7           # Learning rate
		max_steps = 100               # Max steps per episode
		gamma = 0.9                   # Discounting rate

		# Exploration parameters
		epsilon = 1.0                 # Exploration rate
		max_epsilon = 1.0             # Exploration probability at start
		min_epsilon = 0.01            # Minimum exploration probability 
		decay_rate = 0.005            # Exponential decay rate for exploration prob

        # List of rewards
		rewards = []


		for episode in range(total_episodes):
			# Reset the environment
			state = self.env.reset()

			step = 0
			done = False
			total_rewards = 0
			
			for step in range(max_steps):
				
				# Choose an action a in the current world state
				# Randomize exploitation/exploration tradeoff
				tradeoff = random.uniform(0, 1)
			
				actions = state[0].get_actions()

				if tradeoff > epsilon:
					# Exploitation (taking the biggest Q value for this state)
					action = actions[0]
					q_value = self.qtable["default"]
					for a in actions:
						q = self.qtable[(state,a)]
						if q > q_value:
							q_value = q
							action = a
				else:
					# Doing a random choice --> exploration
					action = random.choice(actions)

				# Make step
				new_state, reward, done = self.env.step(action)

				
				new_state_max_q = np.max([self.qtable[(new_state,a)] for a in new_state[0].get_actions()])

				# Update Q(s,a):= Q(s,a) + lr [R(s,a) + gamma * max Q(s',a') - Q(s,a)]
				self.qtable[(state,action)] = self.qtable[(state,action)] + learning_rate * (reward + gamma * new_state_max_q - self.qtable[(state,action)])
				
				total_rewards += reward
				
				# Our new state is state
				state = new_state
				
				# If we receive goal
				if done: break

			# Reduce epsilon (because we need less and less exploration)
			epsilon = min_epsilon + (max_epsilon - min_epsilon)*np.exp(-decay_rate*episode)
			rewards.append(total_rewards)

	def act(self, state):
		actions = state[0].get_actions()
		action = actions[0]
		q_value = self.qtable["default"]
		for a in actions:
			q = self.qtable[(state,a)]
			if q > q_value:
				q_value = q
				action = a
		return action

if __name__ == '__main__':
	# Here you can test your algorithm. Stick with N <= 4
	N = 4

	env = BlockWorldEnv(N)
	qlearning = QLearning(env)

	# Train
	qlearning.train()

	# Evaluate
	test_env = BlockWorldEnv(N)

	test_problems = 10
	solved = 0
	avg_steps = []

	for test_id in range(test_problems):
		s = test_env.reset()
		done = False

		print(f"\nProblem {test_id}:")
		print(f"{s[0]} -> {s[1]}")

		for step in range(50): 	# max 50 steps per problem
			a = qlearning.act(s)
			s_, r, done = test_env.step(a)

			print(f"{a}: {s[0]}")

			s = s_

			if done:
				solved += 1
				avg_steps.append(step + 1)
				break

	avg_steps = sum(avg_steps) / len(avg_steps)
	print(f"Solved {solved}/{test_problems} problems, with average number of steps {avg_steps}.")