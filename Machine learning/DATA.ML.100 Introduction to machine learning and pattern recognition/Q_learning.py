import gym
import time

import numpy
import numpy as np
import matplotlib.pyplot as plt



def Q_algorithm(qtable):

    list_of_average_rewards = []
    env.reset()
    alpha = 0.5
    gamma = 0.9
    done = False
    episodes = 100
    steps = 100
    E = 10

    for i in range(episodes):
        current_state = env.reset()
        current_state = current_state[0]
        done = False
        number_of_steps = 0
        while not done and number_of_steps < steps:
            if np.max(qtable[current_state]) > 0:
                action = np.argmax(qtable[current_state])
            else:
                action = np.random.randint(0, 4)
                #action = int(input('0/left 1/down 2/right 3/up:'))

            new_state, reward, done, info, a = env.step(action)
            #qtable[current_state][action] = reward + gamma * np.max(qtable[new_state])
            qtable[current_state][action] = qtable[current_state][action] + alpha \
                                    * (reward + gamma * np.max(qtable[new_state]) - qtable[current_state][action])
            current_state = new_state
            number_of_steps += 1

        if i % E == 0:
            if np.max(qtable) != 0:
                avg_reward = eval_policy(qtable,1000,100)
                print(f'Average reward: {avg_reward}')
                list_of_average_rewards.append(avg_reward)
            else:
                print(f'Average reward: 0.0')
                list_of_average_rewards.append(0.0)

    return list_of_average_rewards


def eval_policy(qtable_, num_of_episodes_, max_steps_):
    rewards = []
    for episode in range(num_of_episodes_):
        state = env.reset()
        state = state[0]
        step = 0
        done = False
        total_rewards = 0
        for step in range(max_steps_):
            action = np.argmax(qtable_[state,:])
            new_state, reward, done, info, a = env.step(action)
            total_rewards += reward
            if done:
                rewards.append(total_rewards)
                break
            state = new_state

    avg_reward = sum(rewards)/num_of_episodes_
    return avg_reward

#rendering in ansi to make code run faster
#env = gym.make("FrozenLake-v1", is_slippery=False, render_mode="ansi")
env = gym.make("FrozenLake-v1", is_slippery=True, render_mode="ansi")

action_size = env.action_space.n
state_size = env.observation_space.n
qsize = (state_size, action_size)
qtable = np.zeros(qsize, dtype=float)

data = []
x = []
for i in range(10):
    dataset = Q_algorithm(qtable)
    data.append(dataset)
    x.append(i*10)

plt.xlabel("number of episodes")
plt.ylabel("average reward")
plt.title("question c")
for i in range(len(data[0])):
    plt.plot(x,[prt[i] for prt in data], label = 'run %s'%i)
plt.legend()
plt.show()

env.close()
