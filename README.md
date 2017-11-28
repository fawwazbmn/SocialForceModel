# Social Force Model in C++

The *social force model* was originally introduced by [Helbing and Molnár (1995)](https://doi.org/10.1103/PhysRevE.51.4282), and is one of the microscopic technique in crowd simulation. It presents an idea that the agents’ internal motivation to perform certain movements affects their motion. Throughout the years, many improvements were made to the original model. One of the latest improvement was made by [Moussaïd *et al.* (2009)](https://doi.org/10.1098/rspb.2009.0405). In their research, the model parameters were calibrated to match the results of the experiment they have conducted on the real-world crowd. The *Social Force Model in C++* project  is created based on this research.

## Getting Started

This project consists of three header files and four source files. *Core.cpp* is used to setup the scene and display the position of all agents and obstacle walls, while the remaining header and source files are used to store the characteristics of agents and obstacle walls, and perform calculations.

### Prerequisites

This project requires the following libraries.
- [C++ Port of the *vecmath* Package](http://objectclub.jp/download/vecmath_e)
- [Open Graphics Library (OpenGL)](https://www.opengl.org/)

This project also requires users to use compilers that support C++ 11.

## Creating a Simple Scene

*Core.cpp* will create for you a corridor with 400 agents. Pressing the key <kbd>a</kbd> will start the simulation. However, if you wish to create your own scene, kindly follow the steps below.

**Create a Pointer to the <code>SocialForce</code> Object**
```cpp
SocialForce *socialForce;
```

**Create an Obstacle Wall**
```cpp
Wall *wall = new Wall(x1, y1, x2, y2);  // Step 1: Create wall and define its coordinates
socialForce->addWall(wall);             // Step 2: Add wall to SFM
```

**Add an Agent**
```cpp
Agent *agent = new Agent;            // Step 1: Create agent
agent->setPosition(x, y);            // Step 2: Set initial position
agent->setPath(x, y, targetRadius);  // Step 3: Set target position
```
You can set multiple targets by repeating step 3. Adding multiple targets will automatically loop the agent between all targets

**Retrieve Obstacle Wall Position**
```cpp
vector<Wall *> walls = socialForce->getWalls();

for (Wall *wall : walls) {
    wall->getStartPoint();
    wall->getEndPoint();
}
```

**Retrive Agent Position**
```cpp
Agent *agents = socialForce->getCrowd();

for (Agent *agent : agents)
    agent->getPosition();
```

## Authors

- Fawwaz Mohd Nasir
- Shamsul Mohamad
