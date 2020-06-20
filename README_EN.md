# Space Elevator

> [한글](./README.md)

## Background

* Today, people's interest in space has increased tremendously thanks to Space X. With this interest we were able to learn about space debris, the space junk.
* Space debris is usually any artificial object that orbits the Earth. The types of space waste mostly include satellites parts and rocket bodies. The debris were produced by separation of multiple rockets. 
* So how can we reduce this space junk problem. We found that a space elevator was being studied.  By inspired by Space Elevator, we have combined the concept of "Stack Tower" with Space Elevator.

## System Architecture

### Component Hierarchy

* Component Class is consisted of following diagram.

<img src="src\image-20200620150230575.png" alt="image-20200620150230575" style="zoom:50%;" />

* Graphic
  * Graphic은 opengl에서 render을 위한 data들을 관리한다. 
  * Any data which are required for render with OpenGL, are managed the Graphic Class.
  * It contains vertex, color, texture and all other values that is related to rendering graphic and the class generate and bind the Vertex Buffer Object and Vertex Array Object.
  * Cylinder,sphere, Rect2D, Cube are provided geometry primitive and if you want to load any objects with textures, please refer to Graphic constructor.
* RigidbodyWorld
  * RigidbodyWorld generate ode space.
  * Rigidbody3D holds the rigid body information of the object.
* Coordinate
  * Coordinate class renders the x,y,z, axis of the object.

### Empty Object

<img src="src\image-20200620151438456.png" alt="image-20200620151438456" style="zoom:50%;" />



* EmptyObject is the minimum unit class of object used for game playing.
* Analysis of the characteristics of the object to be produced can be used by adding or removing components.
* EmptyObject can have the same type of instance as itself and is similar to the concept of children in unity.

### Instance

* There are 4 instance that is based on EmptyObject.

| Meteor                                                       | Satellite                                                    |                      Gravity Attractor                       | Gravity Dependent                                            |
| ------------------------------------------------------------ | ------------------------------------------------------------ | :----------------------------------------------------------: | ------------------------------------------------------------ |
| <img src="src\image-20200620151814151.png" alt="image-20200620151814151" style="zoom:100%;" /> | <img src="src\image-20200620151828957.png" alt="image-20200620151828957" style="zoom:100%;" /> | <img src="src\image-20200620151836642.png" alt="image-20200620151836642" style="zoom:100%;" /> | <img src="src\image-20200620151843174.png" alt="image-20200620151843174" style="zoom:100%;" /> |

## Technical Details

### Satellite Motion

<img src="src\image-20200620152129506.png" alt="image-20200620152129506" style="zoom:50%;" />

* Satellite should always look at the Earth. That is why the y-axis of the satellite should match the earth with the gravitational vector.

* The methods are as follows.

  |                 1.Find Satellite's Up vector                 | 2. Find the angle between Up vector of satellite and y axis of satellite. |             3. Do Cross Product with the vectors             |
  | :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
  | <img src="src\image-20200620152742725.png" alt="image-20200620152742725" style="zoom:100%;" /> | <img src="src\image-20200620152747926.png" alt="image-20200620152747926" style="zoom:100%;" /> | <img src="src\image-20200620152755599.png" alt="image-20200620152755599" style="zoom:100%;" /> |

  4. Based on the values obtained, the quantitation value is obtained through the formula below.

  $$
  (\cos\frac{phi}{2},\text{axis}.x\times\sin\frac{phi}{2},\text{axis}.y\times\sin\frac{phi}{2},\text{axis}.z\times\sin\frac{phi}{2})
  $$

  

  5. Substitute the existing quaternion value of the satellite with the new quaternion value obtained above by using Hamilton product.

### Gravity

<img src="src\image-20200620152856006.png" alt="image-20200620152856006" style="zoom:50%;" />

* Gravity implementation requires disabling gravity provided by Open Dynamic Engine.

|                    1. Find Gravity Vector                    |                        2. Apply Force                        |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="src\image-20200620153049874.png" alt="image-20200620153049874" style="zoom:100%;" /><br />$$\text{vec3}(a.x-w.x,a.y-w.y,a.z-w.z)$$ | <img src="src\image-20200620153057228.png" alt="image-20200620153057228" style="zoom:100%;" /><br />Add Force: Gravity Vector $$\times \beta$$ |



### Judgement

<img src="src\image-20200620153422645.png" alt="image-20200620153422645" style="zoom:50%;" />

* The most important part of the game is judgment. In this game, decisions are made about the next two things.

  |                        Increase Score                        |                        Decrease Score                        |
  | :----------------------------------------------------------: | :----------------------------------------------------------: |
  | <img src="src\image-20200620153603799.png" alt="image-20200620153603799" style="zoom:100%;" /> | <img src="src\image-20200620153627705.png" alt="image-20200620153627705" style="zoom:100%;" /> |
  | The score increases at the time of creation of the cube in the above. | If the generated cube does not settle on the default cube and collided with earth, the score will decrease. |

  * The generated cube may collide with earth multiple times. In this case, the score can be continuously reduced, but at the same time as earth, adds the cube's geom id to the set-type variable, ensuring that it has always been a collided to reduce the score. Therefore, even if it has collided with redundancy, it can prevent points from being reduced because of  redundancy.

