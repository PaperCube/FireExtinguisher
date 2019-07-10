# “灭火”Arduino机器人示例与接口解释
## 万能头文件
`robot_all.h`包含了所有你可能用到的**本类库的**头文件。
## 内置的方向
有许多地方以一个特定的方向为参数。它有四个值，其书写方式和含义
```cpp
direction::FORWARD      //向前
direction::BACKWARD     //向后
direction::LEFTWARD     //向左
direction::RIGHTWARD    //向右
```
如果提示找不到符号，请先`#include "definitions.h"`或使用万能头文件`"robot_all.h"`
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

## **`quad_directional`对象**
代表一个机器人的四个轮子/电机。你可以通过robot的motor_group来获得一个指向这个机器人的唯一`quad_directional`对象的指针。例如
```cpp
robot r;
quad_directional *motors = r.motor_group;
motors->set_direction(direction::FORWARD);
motors->go();
delay(1000);
motors->stop();
motors->go(1000);
```
### `quad_directional::stop()`函数
使之立即停止。

### `quad_directional::go([int v])`函数
使之以100的速度前进。如果不填参数，机器人会以速度100运行。速度的范围是0-100，可以超出这个范围。当填写100时其实际速度是每个电机定义的最大速度。其值请见`robot.cpp`中的`FULL_SPEED`。


### `quad_directional::set_direction(direction v)`函数
设置机器人的前进方向。如果在机器人运行途中调用此函数，机器人会立刻更改方向。
> 警告：机器人在高速运行时突然更改方向可能导致打滑/漂移。

## **传感器**
通过`prox_sensor::sensor_at(direction v)`函数获得一个指向面向方向v的传感器对象的指针。例如：
```cpp
prox_sensor::prepare_built_in_sensors();
prox_sensor *p = prox_sensor::sensor_at(direction::FORWARD);
int value = p->read(); //将传感器的值读入value
```
调用`sensor_at`函数之前 **_必须_** 调用`prox_sensor::prepare_built_in_sensors();`，它可以安全的被调用多次。
要获得一个合适的值，你应该多次测试它读出的值。


# 示例 - 四方向自动避障机器人
如果我们希望实现一个程序，使得机器人可以在四个方向上行走，遇到障碍物则改变方向，我们需要考虑：
* 机器人在遇到障碍物时停止并且改变方向
* 机器人在应该尽可能的在撞到障碍物之前就停止
* 需要测量传感器的值，以确定距离传感器读到什么值的时候代表机器人距离障碍物已经很近了
* 控制机器人的速度，以减少机器人打滑/漂移/因惯性撞上

首先编写基本代码
```cpp
// app.ino
#include "robot_all.h"
robot r;
quad_directional *motors;
void setup(){
    r.setup();
    motors = r.motor_group;
}
```
在`loop`函数（上面还未写出）中，首先遍历所有方向
```cpp
for(int i = 0; i < 4; i++){
    direction d = (direction) i;
}
```
这里我们使用了强制类型转换，因为direction中所定义的四个方向其实际值分别按照定义的顺序是0, 1, 2, 3，所以将0-3这四个数强制转换成direction类型就可以得到它们四个值。
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
#include "robot_all.h"
robot r;
quad_directional *motors;
void setup(){
    r.setup();
    motors = r.motor_group;
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
    motors->set_direction(d);
    motors->go();
    prox_sensor *sensor = prox_sensor::sensor_at(direction::FORWARD);
     // 更改下面这个60来测试机器人到底在什么位置停止比较合适
    while(sensor->read() > 60)
        ; //不要忘记这个分号
    motors->stop();
}
```
只需要把`loop`函数中的`r.move_until_blocked(d)` 换成 `move_until_blocked(d)`，并且把它放在 `void setup(){...`之前就可以了。

这么做简单明朗。然而如果你观察robot::move_until_blocked的定义，你会发现还是少了一些东西。你可以试着找出缺少了什么，并且描述一下这些缺少的部分起到了什么样的功能。
