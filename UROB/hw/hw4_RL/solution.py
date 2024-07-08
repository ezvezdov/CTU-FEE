import torch

def policy_gradient_loss_simple(logp, tensor_r):
    """given the policy (T, N) log-probabilities and (T, N) rewards,
    compute the scalar loss for pytorch based on the policy gradient"""
    policy_loss = torch.tensor(0.)  # placeholder

    T = logp.shape[0]
    N = logp.shape[1]
    logsum = torch.tensor(0.)

    # with torch.no_grad():
        # pass
    logsum = torch.sum(logp, dim=0)
        # TODO: compute returns of the trajectories from the reward tensor

    policy_loss = -torch.sum(tensor_r * logsum) / (N*T)
    # TODO: compute the policy loss with trajectory returns
    return policy_loss


def discount_cum_sum(rewards, gamma):
    """rewards is (T, N) tensor, gamma is scalar, output should be (T, N) tensor.
    Here we want to compute the discounted trajectory returns at each timestep.
    At each timestep, produce the exponentially weighted sum of (only) the following rewards on a given trajectory
    i.e. $R(\tau_i, t) = \sum_{t'=t}^{T-1} \gamma^{t'-t} r_{t'}$"""
    T = rewards.shape[0]
    returns = torch.zeros_like(rewards)  # placeholder

    # TODO: implement the discounted cummulative sum, i.e. the discounted returns computed from rewards and gamma
    for t in range(T-1,-1,-1):
        if t==T-1: returns[t]= rewards[t]
        else: returns[t]=rewards[t]+gamma*returns[t+1]

    return returns


def policy_gradient_loss_discounted(logp, tensor_r, gamma):
    """given the policy (T, N) log-probabilities, (T, N) rewards
    and the discount factor gamma, compute the scalar loss for pytorch based
    on the policy gradient with discounted returns"""
    policy_loss = torch.tensor(0.)  # placeholder

    T = logp.shape[0]
    N = logp.shape[1]

    # with torch.no_grad():
    # TODO: Compute discounted returns of the trajectories from the reward tensor
    returns = discount_cum_sum(tensor_r, gamma)

    # Compute the policy loss with discounted returns
    policy_loss = torch.sum(-logp * returns) / (N * T)

    return policy_loss


def policy_gradient_loss_advantages(logp, advantage_estimates):
    """given the policy (T, N) log-probabilities and (T, N) advantage estimates,
    compute the scalar loss for pytorch based on the policy gradient with advantages"""
    policy_loss = torch.tensor(0.)  # placeholder

    # TODO: Compute the policy gradient estimate using the advantage estimate weighting
    policy_loss = -torch.mean(logp * advantage_estimates)

    return policy_loss

def value_loss(values, value_targets):
    """ given (T, N) values, (T, N) value targets, compute the scalar regression loss for pytorch"""
    value_loss = torch.tensor(0.)  # placeholder

    # TODO: compute the value function L2 loss
    value_loss = torch.mean((values - value_targets) ** 2)
    
    return value_loss

def ppo_loss(p_ratios, advantage_estimates, epsilon):
    """ given (T, N) p_ratios probability ratios, (T, N) advantage_estimates, and epsilon clipping ratio,
    compute the scalar loss for pytorch based on the PPO surrogate objective"""
    policy_loss = torch.tensor(0.)  # placeholder

    # TODO: compute the PPO loss
    clipped_p_ratios = torch.clamp(p_ratios, 1 - epsilon, 1 + epsilon)
    surrogate_1 = torch.min(p_ratios * advantage_estimates, clipped_p_ratios * advantage_estimates)
    policy_loss = -torch.mean(surrogate_1)

    return policy_loss


