import numpy as np
from decimal import *
#import matplotlib.pyplot as plt
#from matplotlib.collections import LineCollection

# defines the reward/connection graph
'''r = np.array([[-1, -1,  0, -1, -1],
              [-1, -1,  0, -1, -1],
              [0 ,  0, -1,  0,  0],
              [-1, -1,  0, -1, -1],
              [-1, -1,  0, -1, -1]]).astype("float32")'''
r = np.array([[0, 0, -1, 0, -1, -1, -1, -1, -1],
                 [0, 0, 0, -1, 0, -1, -1, -1, -1],
                 [-1, 0, 0, -1, -1, 0, -1, -1, -1],
              [0, -1, -1, 0, 0, -1, 0, -1, -1],
              [-1, 0, -1, 0, 0, 0, -1, 0, -1],
              [-1, -1, 0, -1, 0, 0, -1, -1, 0],
              [-1, -1, -1, 0, -1, -1, 0, 0, -1],
              [-1, -1, -1, -1, 0, -1, 0, 0, 0],
              [-1, -1, -1, -1, -1, 0, -1, 0, 0]]).astype("float32")
q = np.zeros_like(r)


def update_q(state, next_state, action, alpha, gamma):
    rsa = r[state, action]
    qsa = q[state, action]
    new_q = qsa + alpha * (rsa + gamma * max(q[next_state, :]) - qsa)
    q[state, action] = new_q
    # renormalize row to be between 0 and 1
    #rn = q[state][q[state] > 0] / np.sum(q[state][q[state] > 0])
    #q[state][q[state] > 0] = rn
    return r[state, action]


for ii in range(0,512):
    st='{0:09b}'.format(ii)
    print st
    if(int(st[4])==0):
        r = np.array([[0, 0, -1, 0, -1, -1, -1, -1, -1],
                     [0, 0, 0, -1, 0, -1, -1, -1, -1],
                     [-1, 0, 0, -1, -1, 0, -1, -1, -1],
                  [0, -1, -1, 0, 0, -1, 0, -1, -1],
                  [-1, 0, -1, 0, 0, 0, -1, 0, -1],
                  [-1, -1, 0, -1, 0, 0, -1, -1, 0],
                  [-1, -1, -1, 0, -1, -1, 0, 0, -1],
                  [-1, -1, -1, -1, 0, -1, 0, 0, 0],
                  [-1, -1, -1, -1, -1, 0, -1, 0, 0]]).astype("float32")
        q = np.zeros_like(r)

        for jj in range (0,9):
           if(int(st[jj])==1):
               for jj1 in range (0,9):
                   r[jj,jj1]=-1
               for jj1 in range (0,9):
                   r[jj1,jj]=-1

        cnt=0
        for jj in range (0,9):
            if(jj%2==0 and jj!=4):
                if(int(st[jj])==0):
                    cnt= cnt + 1
                    
        #for iii in range(0,5):
        iii=-1
        qq=-1
        while (iii<cnt):
            iii=iii+1
            qq=qq+1
            while(not (iii!=cnt and qq<9 and qq%2==0 and int(st[qq])==0 and qq!=4)):
                qq=qq+1
                if(qq>8):
                    break
            if(qq<9):
                v4=0
                for v2 in range(0,9):
                    if(r[qq,v2]==-1):
                        v4=v4+1
                        
            if(qq<9 and v4<7):
                for iiii in range(0,9):
                    if(r[iiii,qq]==0):
                        r[iiii,qq]=100
                    r[qq,qq]=100
                
                #print st
                # Core algorithm
                gamma = 0.8
                alpha = 1.
                n_episodes = 1E3
                n_states = 9
                n_actions = 9
                epsilon = 0.05
                random_state = np.random.RandomState(1999)
                for e in range(int(n_episodes)):
                    states = list(range(n_states))
                    random_state.shuffle(states)
                    #print(states)
                    current_state = states[0]
                    goal = False
                    if e % int(n_episodes / 10.) == 0 and e > 0:
                        pass
                        # uncomment this to see plots each monitoring
                        #show_traverse()
                        #show_q()
                    v1=0
                    while ((not goal) and v1!=8):
                        #print v1
                        #print current_state
                        
                        #print st
                        # epsilon greedy
                        v = 0
                        valid_moves = r[current_state] >= 0
                        if random_state.rand() < epsilon:
                            actions = np.array(list(range(n_actions)))
                            actions = actions[valid_moves == True]
                            if type(actions) is int:
                                actions = [actions]
                            random_state.shuffle(actions)
                            if(actions.size!=0):
                                action = actions[0]
                                next_state = action
                                #print next_state
                                v1=0
                                for vv in range(0,9):
                                    if(r[next_state,vv]==-1):
                                        v1=v1+1  
                            else:
                                v = 1
                        else:
                            if np.sum(q[current_state]) > 0:
                                action = np.argmax(q[current_state])
                                next_state = action
                                #print next_state
                                v1=0
                                for vv in range(0,9):
                                    if(r[next_state,vv]==-1):
                                        v1=v1+1
                            else:
                                # Don't allow invalid moves at the start
                                # Just take a random move
                                actions = np.array(list(range(n_actions)))
                                actions = actions[valid_moves == True]
                                random_state.shuffle(actions)
                                #print(actions)
                                if(actions.size!=0):
                                    action = actions[0]
                                    next_state = action
                                    #print next_state
                                    v1=0
                                    for vv in range(0,9):
                                        if(r[next_state,vv]==-1):
                                            v1=v1+1
                                else:
                                    v = 1
                        if(v == 0):
                            reward = update_q(current_state, next_state, action,
                                          alpha=alpha, gamma=gamma)
                        else:
                            reward = 0
                            next_state = goal
                            goal = True
                        
                        # Goal state has reward 100
                        #print(current_state),
                        #print(next_state)
                        if reward > 1:
                            goal = True
                        current_state = next_state
                        #print st
                    #print v1
                for iiii in range(0,9):
                    if(r[iiii,qq]==100):
                        r[iiii,qq]=0
                    r[qq,qq]=-1
                    print ('abcd'),
                    print qq
                print(q)
                q = np.zeros_like(r)
#show_traverse()
'''show_q()'''
