import numpy as np
import random
from math import sqrt, pi, exp

def gaussian_prob(obs, mu, sig):
    num = (obs - mu)**2
    denum = 2*sig**2
    norm = 1 / sqrt(2*pi*sig**2)
    return norm * exp(-num/denum)

class GNB(object):

	def __init__(self):
		self.possible_labels = ['left', 'keep', 'right']

	def train(self, data, labels):
		"""
        X is an array of training data, each entry of which is a 
        length 4 array which represents a snapshot of a vehicle's
        s, d, s_dot, and d_dot coordinates.

        Y is an array of labels, each of which is either 'left', 'keep',
        or 'right'. These labels indicate what maneuver the vehicle was 
        engaged in during the corresponding training data snapshot. 
    """
		
	 # Prepare totals_by_label array
		num_vars = 4
		
		totals_by_label = {
            "left" : [], 
            "keep" : [],
            "right": [],
		}
		
		
		# Create empty array for each label & variable
		for label in self.possible_labels:    
			for i in range(num_vars):                
				totals_by_label[label].append([])
		
		# Fill array with values in data
		for x, label in zip(data, labels):
			for i,val in enumerate(x):
				totals_by_label[label][i].append(val)		
		
		means = []
		stds = []
		for label in self.possible_labels:
			means.append([])
			stds.append([])
			for list in totals_by_label[label]:
				std = np.std(list)
				stds[-1].append(std)
				
				mean = np.mean(list)
				means[-1].append(mean)
		
		self._means = means
		self._stds = stds
			

	def _predict(self, obs):
		"""
		Private method used to assign a probability to each class.
		"""
		
		probs = []
		for (means, stds, lab) in zip(self._means, self._stds, self.possible_labels):
			product = 1
			for mu, sig, o in zip(means, stds, obs):
				likelihood = gaussian_prob(o, mu, sig)
				product *= likelihood
			probs.append(product)
		t = sum(probs)
		return [p/t for p in probs]

	def predict(self, observation):
		probs = self._predict(observation)
		idx = 0
		best_p = 0
		for i, p in enumerate(probs):
			if p > best_p:
				best_p = p
				idx = i
		names = ['left','keep','right']
		return names[idx]