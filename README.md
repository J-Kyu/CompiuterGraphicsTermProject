# Space Elevator

> [English](./README_EN.md)

## Background

* 최근 Space X에서 Falcon 9을 발사하면서 대중들이 다시 한번 우주 관련 사업에 관심을 갖게 되었다. 이것을 기점으로 우주에 대해서 조사 해보니, "Space Debris"에 대하여 관심을 갖게 되었다.
* "Space Debris"는 우주 쓰레기라는 의미로, 인간이 지속적인 로켓 발사로 우주에 버려진 쓰레기를 의미한다. 이러한 쓰레기들은 앞으로의 우주 발사에 있어서 문제가 될 수 있다는 생각이 들었다. 그래서 이 우주 쓰레기를 치울 아이디어를 고민하다가 우주 엘레베이터를 생각하게 되었다.
* 우주 엘레베이터의 영감을 받아, "Stack Tower"와 비슷한 게임을 만들어보면 좋지 않을 까 생각하여 Space Elevator를 제작하게 되었다.



## System Architecture

### Component Hierarchy

<img src="src\image-20200620150230575.png" alt="image-20200620150230575" style="zoom:50%;" />

* Component class는 위 diagram과 같이 구성이 되었다. 
* Graphic
  * Graphic은 opengl에서 render을 위한 data들을 관리한다. 
  * vertex, color, texture 등등 render을 위한 값들을 다루며, Vertex Buffer Object와 Vertex Array Object를 generate하고 bind하는 작업 전부 여기서 진행이 된다.
  * Cylinder,sphere, Rect2D, Cube는 기본적으로 제공되는 geometry primitive이며, 이미 만들어진 obj와 texture를 load할 경우 Graphic Class의 생성자를 참조하기 바란다.
* RigidbodyWorld
  * RigidbodyWorld는 게임 내에서 작동되는 물리 공간을 생성한다.
  * Rigidbody3D는 해당 object에 대한 rigidbody 정보를 갖고 있다.
* Coordinate
  * 개발 시, object의 방향을 확인하기 위해 object에 대한 x,y,z 축을 render한다.

### Empty Object

<img src="src\image-20200620151438456.png" alt="image-20200620151438456" style="zoom:50%;" />



* EmptyObject는 해당 게임에서 작동할 수 있는 최소 단위의 object라고 보면 된다. 
* 제작하려는 object의 특성을 분석하여, component를 추가하거나 제거하여 사용할 수 있다.
* EmptyObject는 자신과 동일한 type의 instance를 갖을 수 있으며, unity의 children 개념과 유사하다.

### Instance

* 게임 내에서 크게 EmptyObject를 기반으로 크게 4가지 instance가 존재한다.

| Meteor                                                      | Satellite                                                   |                      Gravity Attractor                      | Gravity Dependent                                           |
| ----------------------------------------------------------- | ----------------------------------------------------------- | :---------------------------------------------------------: | ----------------------------------------------------------- |
| ![image-20200620151814151](src\image-20200620151814151.png) | ![image-20200620151828957](src\image-20200620151828957.png) | ![image-20200620151836642](src\image-20200620151836642.png) | ![image-20200620151843174](src\image-20200620151843174.png) |

## Technical Details

### Satellite Motion

<img src="src\image-20200620152129506.png" alt="image-20200620152129506" style="zoom:50%;" />

* Satellite가 지구를 항상 바라봐야 한다. 그렇기 때문에 satellite의 y 축이 지구와 중력 vector와 일치해야 한다.

* 방법은 다음과 같다.

  |           1. 먼저 Satellite의 Up vector를 구한다            |    2.Satellite의 Up vector와 y 축 사이의 angle을 구한다.    |     3.구한 두 vector에 대하여 cross product를 시행한다.     |
  | :---------------------------------------------------------: | :---------------------------------------------------------: | :---------------------------------------------------------: |
  | ![image-20200620152742725](src\image-20200620152742725.png) | ![image-20200620152747926](src\image-20200620152747926.png) | ![image-20200620152755599](src\image-20200620152755599.png) |

  4. 구해진 값들을 기준으로 아래식을 통해서 quaternion 값을 구한다

  $$
  (\cos\frac{phi}{2},\text{axis}.x\times\sin\frac{phi}{2},\text{axis}.y\times\sin\frac{phi}{2},\text{axis}.z\times\sin\frac{phi}{2})
  $$

  

  5. 구한 quaternion 값과 기존의 satellite의 quaternion끼리 해밀턴 곱을 통해서 satellite에 적용시킨다.

### Gravity

<img src="src\image-20200620152856006.png" alt="image-20200620152856006" style="zoom:50%;" />

* Gravity를 구현하기 위해서는 먼저 Open Dynamic Engine에서 제공하는 gravity를 disable 시킨다. 이후 다음과 같은 방법으로 gravity를 구하면 된다.

|                1. Gravity Vector를 먼저 찾자.                |                     2. Force를 추가하자.                     |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20200620153049874](src\image-20200620153049874.png)<br />$$\text{vec3}(a.x-w.x,a.y-w.y,a.z-w.z)$$ | ![image-20200620153057228](src\image-20200620153057228.png)<br />Add Force: Gravity Vector $$\times \beta$$ |



### Judgement

<img src="src\image-20200620153422645.png" alt="image-20200620153422645" style="zoom:50%;" />

* 게임 내에서의 점수 판정은 다음 2가지 case에 의해서 일어난다.

  |                          점수 증가                          | 점수 감소                                                    |
  | :---------------------------------------------------------: | ------------------------------------------------------------ |
  | ![image-20200620153603799](src\image-20200620153603799.png) | ![image-20200620153627705](src\image-20200620153627705.png)  |
  |       위서에서 cube를 생성한 시점에 점수가 증가한다.        | 생성된 cube가 default cube위에 안착 되지 않고 earth와 collision을 일으키면 점수가 감소한다. |

  * 생성된 cube가 중복으로 earth와 충돌을 일으킬 수 있다. 이럴 경우, 점수가 지속적으로 감소 될 수 있는데, 이 문제는, earth와 충돌이 일어났을 동시에 set 형태의 변수에 cube의 geom id를 추가하며, 점수 감소를 위해 항상 충돌이 되었는지를 확인한다. 때문에 중복의 충돌이라도, 점수가 중복으로 감소 되는 것을 방지 할 수 있다.

