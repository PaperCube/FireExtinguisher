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

## **传感器**
通过`prox_sensor::sensor_at(direction v)`函数获得一个指向面向方向v的传感器对象的指针。例如：
```cpp
prox_sensor::prepare_built_in_sensors();
prox_sensor *p = prox_sensor::sensor_at(direction::FORWARD);
int value = p->read(); //将传感器的值读入value
```
调用`sensor_at`函数之前 **_必须_** 调用`prox_sensor::prepare_built_in_sensors();`，它可以安全的被调用多次。
要获得一个合适的值，你应该多次测试它读出的值。
