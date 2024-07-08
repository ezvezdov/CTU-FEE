import torch
from torch import nn
import numpy as np
from torch.distributions.normal import Normal

from solution import discount_cum_sum

def compute_advantage_estimates(tensor_r, values, gamma, bootstrap=False):
    """given reward tensor (T, N), value estimates tensor (T+1, N) and gamma scalar"""
    if bootstrap:  # use last value estimates as a return estimate
        terminal_value_estimates = values[-1].unsqueeze(0)  # values of the last states (1, N)
        rs_v = torch.cat((tensor_r, terminal_value_estimates), dim=0)
        value_targets = discount_cum_sum(rs_v, gamma)[:-1]
    else:
        value_targets = discount_cum_sum(tensor_r, gamma)
    advantages = value_targets - values[:-1]
    return value_targets, advantages

def compute_gae(tensor_r, values, gamma, lambda_):
    """generalized advantage estimation (GAE) implementation"""
    delta_t = tensor_r + gamma * values[1:] - values[:-1]
    advantages = discount_cum_sum(delta_t, gamma * lambda_)
    value_targets = advantages + values[:-1]
    return value_targets, advantages

def sample_trajectories(env, pi, T):    
    """given an environment env, a stochastic policy pi and number of timesteps T, interact with the environment for T steps 
    using actions sampled from policy. Return torch tensors of collected states, actions and rewards"""
    states = np.zeros((T + 1, N, env.num_states), dtype=float)  # states from s(0) to s(T+1)
    actions = np.zeros((T, N, env.num_actions), dtype=float)  # actions from a(0) to a(T)
    rewards = np.zeros((T, N), dtype=float)  # rewards from r(0) to r(T)
    
    s = env.vector_reset()
    states[0] = s
    for t in range(T):
        a = pi.sample_actions(torch.tensor(states[t]).float())  # policy needs float torch tensor (N, state_dim)
        s_next, r = env.vector_step(np.array(a))  # env needs numpy array of (Nx1)
        states[t + 1], actions[t], rewards[t] = s_next, a, r    
        
    tensor_s = torch.tensor(states).float()  # (T+1, N, state_dim)  care for the extra timestep at the end!
    tensor_a = torch.tensor(actions).float()  # (T, N, 1)
    tensor_r = torch.tensor(rewards).float()  # (T, N)
    
    return tensor_s, tensor_a, tensor_r 

class WalkerPolicy(nn.Module):
    def __init__(self, state_dim=29, action_dim=8, load=True, fixed_sigma=None):
        super(WalkerPolicy, self).__init__()
        
        # shared layers between the action and value heads
        self.shared_layers = nn.Sequential(
            nn.Linear(state_dim, 64),
            nn.Tanh(),
        )
        
        # action head
        self.action_layers = nn.Sequential(
            nn.Linear(64, 32),
            nn.Tanh(),
            nn.Linear(32, 2*action_dim),
        )

        # value head
        self.value_layers = nn.Sequential(
            nn.Linear(64, 32),
            nn.ReLU(),
            nn.Linear(32, 1),
        )

        if fixed_sigma is not None:
            self.sigma = torch.nn.Parameter(torch.tensor(fixed_sigma), requires_grad=False)
        else:
            self.sigma = torch.nn.Parameter(torch.tensor(0.01), requires_grad=True)  # Small constant as an example

        if load == True:
            self.load_weights()  # load learned stored network weights after initialization
        
    def determine_actions(self, states):
        """states is (N, state_dim) tensor, returns (N, action_dim) actions tensor. 
        This function returns deterministic actions based on the input states. This would be used for control. """
        params = self.action_layers(self.shared_layers(states))  # map states to distribution parameters
        mu, _ = torch.chunk(params, 2, -1)  # split the parameters into mean and std, return mean
        return mu
    
    def save_weights(self, path='walker_weights.pt'):
        # helper function to save your network weights
        torch.save(self.state_dict(), path)

    def load_weights(self, path='walker_weights.pt'):
        # helper function to load your network weights
        self.load_state_dict(torch.load(path))

    def sample_actions(self, states):
        """states is (T, N, state_dim) tensor, returns (T, N, action_dim) actions tensor. 
        This function returns probabilistically sampled actions. This would be used for training the policy."""
        params = self.action_layers(self.shared_layers(states))  # map states to distribution parameters
        mu, _ = torch.chunk(params, 2, -1)  # split the parameters into mean and std

        sigma = self.sigma

        sigma = torch.nn.functional.softplus(sigma)  # make sure std is positive
        distribution = Normal(mu, sigma)  # create distribution of size (T, N, action_dim)
        actions = distribution.sample()  # sample actions
        return actions
    
    def log_prob(self, actions, states):
        """states is (T, N, state_dim) tensor. actions is (T, N, action_dim) tensor.
        This function returns the log-probabilities of the actions given the states. $\log \pi_\theta(a_t | s_t)$"""
        params = self.action_layers(self.shared_layers(states))  # map states to distribution parameters
        mu, sigma = torch.chunk(params, 2, -1)  # split the parameters into mean and std

        sigma = self.sigma

        sigma = torch.nn.functional.softplus(sigma)  # make sure std is positive
        distribution = Normal(mu, sigma)  # create distribution of size (T, N, action_dim)
        logp = distribution.log_prob(actions)
        if len(logp.shape) == 3 and logp.shape[2] > 1:  # this allows generalization to multi-dim action spaces
            logp = logp.sum(dim=2, keepdim=True)  # sum over the action dimension
        return logp
    
    def value_estimates(self, states):
        """states is (T, N, state_dim) tensor, returns (T, N) values tensor. Useful for value estimation during training."""
        return self.value_layers(self.shared_layers(states)).squeeze()


if __name__ == "__main__":
    import numpy as np
    import torch
    from torch import nn 
    from environment.WalkerEnv import WalkerEnv
    from solution import ppo_loss
    from torch import nn
    from torch.distributions.normal import Normal
    from solution import discount_cum_sum
    from solution import policy_gradient_loss_advantages, value_loss, policy_gradient_loss_discounted 
    from environment.WalkerEnv import base_config
    def test_policy(pi, T=512, config=base_config, deterministic=True):
        test_env = WalkerEnv(config)
        mean_reward = 0
        
        s = test_env.vector_reset()
        for i in range(T):
            with torch.no_grad():
                if deterministic:
                    actions = pi.determine_actions(torch.tensor(s).float())  # use deterministic actions based on the states
                else:
                    actions = pi.sample_actions(torch.tensor(s).float())  # use random actions conditioned on the states
            s, r = test_env.vector_step(actions.numpy())
            mean_reward += sum(r)/(T*config['N'])
            
        test_env.close()
        return mean_reward
        
    ###############################################################################################################

    def walker_reward(state, action):
        """reward function for the walker environment, state is [29] vector, action is [8] vector"""
        pos = state[:15]  # first 15 elements of state vector are generalized coordinates [xyz, quat, joint_angles]
        vel = state[15:]  # last 14 elements of state vector are generalized velocities [xyz_vel, omega, joint_velocities]
        
        # Extract values
        x_velocity = vel[0]
        pitch, roll = pos[3:5]  # Extract pitch and roll angles

        
        # Encourage forward movement
        forward_reward = max(0, x_velocity)

        # Penalize for low leg activity (sum of absolute joint velocities)
        leg_activity_penalty = -0.01 * sum(abs(vel[15:]))

        # Penalize for flipping onto the back (negative pitch angle)
        flipping_penalty = -0.1 * min(0, pitch)  # Adjust the coefficient as needed


        # Combine forward movement reward, stability penalty, and leg activity penalty
        reward = forward_reward + leg_activity_penalty + flipping_penalty

        return reward

    # training parameters
    N = 64
    T = 256
    config = {'N': N, 'vis': 1,'reward_fcn':walker_reward}
    epochs = 500
    lr = 0.01
    gamma=0.95
    epsilon = 0.2

    sgd_iters = 1

    # policy, environment and optimizer
    pi = WalkerPolicy(load=True)
    train_env = WalkerEnv(config)
    # optim = torch.optim.SGD(pi.parameters(), lr=lr)
    optim = torch.optim.Adam(pi.parameters(), lr=lr)

    mean_rewards, p_losses, v_losses = np.zeros(epochs), np.zeros(epochs), np.zeros(epochs)  # for logging mean rewards over epochs
    for epoch in range(epochs):
        tensor_s, tensor_a, tensor_r = sample_trajectories(train_env, pi, T)  # collect trajectories using current policy
            
        with torch.no_grad():  # compute the old probabilities
            logp_old = pi.log_prob(tensor_a, tensor_s[:T]).squeeze(2)  # compute log(pi(a_t | s_t))
        
        for i in range(sgd_iters):  # we can even do multiple gradient steps
            values = pi.value_estimates(tensor_s)  # estimate value function for all states 
            logp = pi.log_prob(tensor_a, tensor_s[:T]).squeeze(2)  # compute log(pi(a_t | s_t))

            with torch.no_grad():  # no need for gradients when computing the advantages and value targets
                # value_targets, advantage_estimates = compute_advantage_estimates(tensor_r, values, gamma, bootstrap=True)
                value_targets, advantage_estimates = compute_gae(tensor_r, values, gamma, lambda_=0.97)
                advantage_estimates = (advantage_estimates - advantage_estimates.mean()) / advantage_estimates.std()  # normalize advantages
                
            L_v = value_loss(values[:T], value_targets)  # add the value loss
            
            p_ratios = torch.exp(logp - logp_old)  # compute the ratios r_\theta(a_t | s_t)
            L_ppo = ppo_loss(p_ratios, advantage_estimates, epsilon=epsilon)  # compute the policy gradient loss
            total_loss = L_v + L_ppo
            
            optim.zero_grad()
            total_loss.backward()  # backprop and gradient step
            optim.step()
        
        if epoch % 10 == 0:
            print('Epoch %d, mean reward: %.3f, value loss: %.3f' % (epoch, tensor_r.mean(), L_v.item()))
        mean_rewards[epoch] = tensor_r.mean()
        v_losses[epoch] = L_v.item()
        p_losses[epoch] = L_ppo.item()
        
    train_env.close()

# plot_training(mean_rewards, p_losses, v_losses)

    pi.save_weights()

    test_policy(pi)