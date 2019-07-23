#include "parameters.h"
#include <Arduino.h>

/*
 * All the parameters that can affect the behavior of the program are listed
 * here. You can directly modify the content of parameters in this file.
 * 所有可以影响程序行为的参数都在此列出。你可以直接更改下列参数的内容。
 *
 * Please do not modify any other parts of this file unless you are clearly
 * aware of what you are doing.
 * 请不要修改此文件的其他内容，除非您很清楚您在做什么。
 */

/**
 * 定义 红外距离传感器 的端口。以前方左侧的传感器为1，逆时针
 * 旋转按顺序标号
 * ==[1]====Front=====[8]===
 * |                       |
 * [2]                     [7]
 * |                       |
 * | Left                  | Right
 * |                       |
 * |                       |
 * [3]                    [6]
 * |                       |
 * ==[4]====Back======[5]===
 */
int SENSOR_PINS[8] = {1, 2, 3, 4, 5, 6, 7, 8};

/**
 * 定义上方旋转的灭火电机的标号
 */
int MECHANIC_ARM_MOTOR_ID = 1;

/**
 * 设置上方旋转灭火电机的转速
 */
int MECHANIC_ARM_ROTATION_SPEED = 255;

/**
 * 设置机器人的默认最大速度
 */
int FULL_SPEED = 130;

/**
 * 设定机器人自动停止时的传感器值，仅对move_until_blocked有效
 */
int STOP_THRESHOLD = 55;

/**
 * 设置机器人开始减速时的最低传感器值，仅对move_until_blocked有效
 */
int SLOW_THRESHOLD = 115;

/*
 * Above are all parameters that you can modify if necessary.
 * 以上是您可以按需更改的参数。
 * Please do not modify anything below, unless you are clear what modifications
 * to contents below may cause.
 * 请不要修改任何下方的内容，除非您清楚对下面内容的修改可能导致什么。
 */