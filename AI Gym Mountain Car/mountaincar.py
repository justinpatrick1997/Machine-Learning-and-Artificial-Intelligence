#!/usr/bin/python

import argparse
import logging
import sys

import numpy as np

import gym
# import gym.scoreboard.scoring
from gym import wrappers, logger


################################################
# CS482: this is the function that changes the
# continuous values of the state of the cart-
# pole into a single integer state value, you'll
# have to adjust this for the mountain car task
################################################


def discretize_state(x, xdot):
    #x range for cart
    #velocity range
    x_range = [-1.2, 0.6]
    xdot_range = [-0.7, 0.7]
    #check if cart is out of map
    if x < x_range[0] or x > x_range[1]:
        return -1

    #creating bins (x, xdot_range)
    x_bins = np.arange(x_range[0], x_range[1], 0.3)[1:]
    xdot_bins = np.arange(xdot_range[0], xdot_range[1], 0.1)[1:]
    #getting indices of binned values
    a = np.digitize(x, x_bins)
    b = np.digitize(xdot, xdot_bins)
    #getting discrete state value
    box = a * (len(xdot_bins) + 1) + b
    return box


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=None)

    ############################################
    # CS482: This is the line you'll have to
    # change to switch to the mountain car task
    ############################################
    parser.add_argument('env_id', nargs='?', default='MountainCar-v0', help='Select the environment to run')
    args = parser.parse_args()

    logger = logging.getLogger()
    formatter = logging.Formatter('[%(asctime)s] %(message)s')
    handler = logging.StreamHandler(sys.stderr)
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    # You can set the level to logging.DEBUG or logging.WARN if you
    # want to change the amount of output.
    logger.setLevel(logging.INFO)

    env = gym.make(args.env_id)
    outdir = '/tmp/' + 'qagent' + '-results'
    env = wrappers.Monitor(env, outdir, write_upon_reset=True, force=True)

    env.seed(0)

    ############################################
    # CS482: This initial Q-table size should
    # change to fit the number of actions (columns)
    # and the number of observations (rows)
    ############################################
    Q = np.zeros([discretize_state(0.6, 0.7), env.action_space.n])

    ############################################
    # CS482: Here are some of the RL parameters
    # you can adjust to change the learning rate
    # (alpha) and the discount factor (gamma)
    ############################################

    #chaned alpha and gamma to improve convergence
    alpha = 0.1
    gamma = 0.99

    n_episodes = 50001
    for episode in range(n_episodes):
        tick = 0
        reward = 0
        done = False
        state = env.reset()
        s = discretize_state(state[0], state[1])
        while done != True:
            tick += 1
            action = 0
            ri = -999
            for q in range(env.action_space.n):
                if Q[s][q] > ri:
                    action = q
                    ri = Q[s][q]
            state, reward, done, info = env.step(action)
            sprime = discretize_state(state[0], state[1])
            predicted_value = np.max(Q[sprime])

            #documentation states reward is -1 until goal reached
            #postive reward given
            if tick <= 199:
                predicted_value = 0
                reward = 5


            Q[s, action] += alpha*(reward+gamma*predicted_value-Q[s, action])
            s = sprime

        if episode % 1000 == 0:
            alpha *= .996

        ############################################
        # CS482: When switching to the mountain car
        # task, you will have to change this to
        # reflect the success/failure of the mountain
        # car task
        ############################################

        #if time over 199(200), failed attempt
        if tick > 199:
            print("fail ", tick)
        else:
            print("success")
