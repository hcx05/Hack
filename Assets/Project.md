---

## How Reinforcement Learning Works

In `RL`, an agent interacts with an environment by acting and observing the consequences. The environment provides feedback through rewards or penalties, guiding the agent toward learning an optimal policy. A `policy` is a strategy for selecting actions that maximize cumulative rewards over time.

1. `Model-Free RL:` The agent learns directly from experience without explicitly modeling the environment. This is like navigating a maze without a map, where the agent relies solely on trial and error and feedback from the environment to learn the best actions. The agent gradually improves its policy by exploring different paths and learning from the rewards or penalties it receives.

## Value Function

The `value function` estimates the long-term value of being in a particular state or taking a specific action. It predicts the expected cumulative reward that the agent can obtain from that state or action onwards. The value function is a crucial component in many `RL` algorithms, as it guides the agent towards choosing actions that lead to higher long-term rewards.

There are two main types of value functions:

- `State-value function:` Estimates the expected cumulative reward from starting in a given state and following a particular policy.
- `Action-value function:` Estimates the expected cumulative reward from taking a specific action in a given state and then following a particular policy.

## Discount Factor

The `discount factor` (`γ`) is a `RL` parameter that determines future rewards' present value. It ranges between 0 and 1, with values closer to 1 giving more weight to long-term rewards and values closer to 0 emphasizing short-term rewards.

- `γ=0` means the agent only considers immediate rewards.
- `γ=1` means the agent values all future rewards equally.

`Episodic tasks` involve the agent interacting with the environment in episodes, each ending at a terminal state (e.g., reaching the goal in a maze).