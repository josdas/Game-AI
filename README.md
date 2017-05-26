# Game-AI
A game with several bots. One of them is a neural network.

Training of a neural network is a battle with simple bots and with itself. The method of training is a method of simulation of annealing.

The current configuration of the neural network:
There are 3 layers

The first layer with a linear function of activation and an added constant neuron. 

Two other with the hyperbolic tangent function.

The size of the layers equals {18, 20, 16}

The result is argmax of the last layer.

Traning of the network сonsists of three steps. 
The first is getting main control of the game. Function evaluating is (Average damage * (1 + coefficient of aggressive) - Average health) / number of game.

The second is improving the worst of cases. Function evaluating is (Average damage - Average health) / number of game + (Sum of minimal results) / number of game.

The third is decreasing of the number of dead. Function evaluating is (Average damage - Average health) / number of game - (number of dead) / number of game * coefficient.

Plan:
1) Create a game OK
2) Create several simple bots OK
3) Create a neural bot OK
4) Training with the simplte bots OK
5) Training with itself OK
6) Search a good option of the neural network
