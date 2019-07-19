# “灭火”Arduino机器人示例与接口解释
## 头文件
只需要包含`"robot.h"`便可以使用本类库的主要功能。
## 内置的方向
有许多地方以一个特定的方向为参数。它有四个值，其书写方式和含义以及等效的数值为：
```cpp
direction::FORWARD      //向前  =0
direction::LEFTWARD     //向左  =1
direction::BACKWARD     //向后  =2
direction::RIGHTWARD    //向右  =3
```
请先`#include "robot.h"`
## **文件`robot.h`**
`robot.h`定义了控制机器人的基本内容。在使用前，请
1. 声明robot对象。比如：
   
2. 使用`robot::setup()`进行初始化。例如：
    ```cpp
    robot r;
    r.setup();
    ```
    **_注意：这非常重要。否则你的机器人将不能正常运行_**

### `robot::move_until_blocked(direction v1, [int v2])`函数
v1指定机器人前进的方向，v2指定机器人到底在预定距离范围之内多少毫秒才停止。v2可以忽略不填，此时它和填0意思相同，代表一旦进入预定范围之内就立刻停止。

### `robot::set_max_speed(int v1)`
用于设置机器人的全局最大速度。这个设置会覆盖掉你对每个电机单独设置的最大速度

## 通过`robot`对象控制机器人行进
```cpp
robot r;
r.set_direction(direction::FORWARD);
r.go();
delay(1000);
r.stop();
delay(1000);
r.go(50);
```
如果你在`void setup()`中运行上面这段代码，你将发现机器人有如下动作：  
* 机器人向前行进1秒
* 机器人停止1秒
* 机器人以原来速度的一半（大约一半）运行下去，并且不停止。
### `robot::stop()`函数
使之立即停止。

### `robot::go([int v])`函数
使之以100的速度前进。如果不填参数，机器人会以速度100运行。速度的范围是0-100，可以超出这个范围。当填写100时其实际速度是每个电机定义的最大速度。其值请见`robot.cpp`中的`FULL_SPEED`。


### `robot::set_direction(direction v)`函数
设置机器人的前进方向。如果在机器人运行途中调用此函数，机器人会立刻更改方向。
> 警告：机器人在高速运行时突然更改方向可能导致打滑/漂移。

## **传感器**
### `robot::read_sensor(direction v1, [signed char v2]`函数
此函数可以读方向v1上的传感器的值。此函数也可以写第二个参数，用于指定从哪个传感器读取值。例如
```cpp
robot r;
r.read_sensor(direction::FORWARD); // 综合考虑前方两个数值
r.read_sensor(direction::LEFT, -1); // 读以前方左侧为编号1，逆时针依次编号，编号为3的传感器的数值
```
这样读出的值是转换过的。你也可以调用`read_sensor_raw`来读出传感器的原始值。其参数和含义和`read_sensor`函数完全相同。

------------------------

# 示例 - 四方向自动避障机器人
若要掌握此类库，最终实现“灭火”的目的，我们可以先从一个非“灭火”的简单例子开始。比如，我们可以先实现实现一个程序，使得机器人可以在轮流四个方向上行走，遇到障碍物则改变方向，我们需要考虑：
* 机器人在遇到障碍物时停止并且改变方向
* 机器人在应该尽可能的在撞到障碍物之前就停止
* 需要测量传感器的值，以确定距离传感器读到什么值的时候代表机器人距离障碍物已经很近了
* 控制机器人的速度，以减少机器人打滑/漂移/因惯性撞上

程序的基本思路可能是这样的：
* 建立机器人。开始运行软件。
* 初始化机器人。
* 遍历四个方向，对于每个方向：
  * 前进。直到机器人遇到障碍停止。
* 不断重复上述过程。

首先编写第一步
```cpp
// app.ino
#include "robot.h"
robot r;
quad_directional *motors;
void setup(){
    r.setup();
    motors = r.motor_group;
}
```
上面这一段代码建立了程序的结构。`r.setup()`初始化了机器人。
`motors=r.motor_group`获取了机器人四个方向行进的控制器，也就是前文提到的`quad_directional`对象。尽管暂时用不到`motors`，我们可以先保留备用。当然不在此时保存`motors`对象也是可以的。你可以随时通过 `r.motor_group`获取它。  
**`r.motor_group`是一个变量，它没有`()`，因为它不是一个函数。对`r.motor_group`赋值可能带来不可预料的结果**

> ## 对`quad_directional`更深一层的解释
> `quad_directional`可以由两个`motor_pair`创建，前者代表四个轮子，
> 后者代表两个轮子。而**每个** `motor_pair` 可以由两个`motor_controller`创建。  
> 要创建一个`motor_controller`，首先要获得它在主板上的模拟端口号。配合L298(N)驱动器，每个电机需要2个模拟端口，然而主板上的端口数量不足以支撑4个电机，因此我们将其中一个模拟端口插到数字端口(digital pin)上以获得0或255。`motor_controller`就是为这种情况设计的。
> `motor_controller`的构造函数接受两个`int`参数，第一个代表模拟端口，第二个代表数字端口。假设它们分别是1和20，则
> ```cpp
> motor_controller motor_l(1, 20);
> ```
> 或
> ```cpp
> motor_controller *motor_lptr = new motor_controller(1, 20);
> ```
> 按照这种方式我们再获得另一个电机`*motor_rptr`，然后可以创建一个`motor_pair`对象。
> 在创建 `motor_pair` 对象之前，应该先确保这两个电机在接受相同值的情况下转动方向是一致的。如果不一致，则应该交换模拟端口和数字端口。  
> `motor_pair`的构造函数接受两个`motor_controller *`（指针）作为参数，分别代表以相同旋转方向的两个电机旋转方向为前方，左侧的电机和右侧的电机。其具体控制方式请见前面的API说明（可能还没写）。
> `quad_directional`的构造函数接受四个参数，分别为`(motor_pair *lr, direction dir_lr, motor_pair *fb, direcion dir_fb)`。`lr`, `fb`是两个电机对(`motor_pair`)，`dir_lr`和`dir_fb`代表两个电机对的速度为**正值**时电机的转动方向。否则`quad_directional`的方向设定功能会出现问题。

在`loop`函数（上面还未写出）中，首先遍历所有方向
```cpp
for(int i = 0; i < 4; i++){
    direction d = (direction) i;
}
```
这里我们使用了强制类型转换，因为direction中所定义的四个方向其实际值分别按照定义的顺序是0, 1, 2, 3，所以将0-3这四个数强制转换成direction类型就可以得到它们四个值。如果你不明白这是怎么运作的，请学习C++中的枚举(`enum`, Enumeration).  
接下来我们投机取巧，使用`robot::move_until_blocked`函数来使机器人在遇到障碍时停止。
```cpp
for(int i = 0; i < 4; i++){
    direction d = (direction) i;
    r.move_until_blocked(d);
}
```
然而不要忘记我们需要控制机器人的速度以防止它因为过快而不能及时停止，所以还要在之前加上`robot.set_max_speed(120)`。
于是我们就基本完成了。代码如下
```cpp
#include "robot.h"
robot r;
void setup(){
    r.setup();
    r.set_max_speed(120);
}
void loop(){
    for(int i = 0; i < 4; i++){
        direction d = (direction) i;
        r.move_until_blocked(d);
    }
}
```
然而你也可能想实现自己的`move_until_blocked`函数。那我们可以这么写：
```cpp
void move_until_blocked(direction d){
    r.set_direction(d);
    r.go();
     // 更改下面这个60来测试机器人到底在什么位置停止比较合适
    while(r.read_sensor(d) > 60)
        ; //不要忘记这个分号
    r.stop();
}
```
只需要把`loop`函数中的`r.move_until_blocked(d)` 换成 `move_until_blocked(d)`，并且把它放在 `void setup(){...`之前就可以了。

这么做简单明朗。然而如果你观察`robot::move_until_blocked`的定义，你会发现还是少了一些东西。你可以试着找出缺少了什么，并且描述一下这些缺少的部分起到了什么样的功能。
