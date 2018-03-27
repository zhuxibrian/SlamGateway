#RW-ZERO MQTT 接口文档

##概述：
RW-ZERO及其控制系统包含Ctrl Module、SLAM Gateway、Terminal Device、Schedule System、MQTT broker及部分组成，本文档用以说明系统间接口协议。
* 系统使用MQTT作为消息总线，MQTT broker使用EMQ。
* MQTT topic为消息目的地。
* MQTT message使用json作为格式，包含消息类型、时间戳、参数等内容。
* Ctrl Module及SLAM SDK控制均有SLAM Gateway 进行控制及同步，Schedule System发送的消息由SLAM Gateway及Terminal Device接收。

###主题（topic）
1. Ctrl Module：（由于所有ctrl均由网关程序管理，应该只需要用到single）
    | topic                     | qos       |  from  | to |
    | --------                  | -----     | :----: |:----: |
    | rw/ctrl/all               | 2        |   slam    |ctrl|
    | rw/ctrl/group/{index}     | 2        |   slam    |ctrl|
    | rw/ctrl/single/{index}    | 2        |   slam    |ctrl|

2. SLAM Gateway:
    | topic                     | qos       |  from  | to |
    | --------                  | -----     | :----: |:----: |
    | rw/slam/all               | 2        |   sch    |slam|
    | rw/slam/group/{index}     | 2        |   sch    |slam|
    | rw/slam/single/{index}    | 2        |   sch    |slam|
    | rrw/slam/info/{index}    | 0        |   ctrl    |slam|
3. Terminal Device(schecule system->terminal device qos:2)
    | topic                     | qos       |  from  | to |
    | --------                  | -----     | :----: |:----: |
    | rw/term/all               | 2        |   sch    |term|
    | rw/term/group/{index}     | 2        |   sch    |term|
    | rw/term/single/{index}    | 2        |   sch    |term|
4. Schedule System
    | topic                     | qos       |  from  | to |
    | --------                  | -----     | :----: |:----: |
    | rw/sch               | 0        |   others    |sch|
5. All/<Class> Device:
    | topic                     | qos       |  from  | to |
    | --------                  | -----     | :----: |:----: |
    | rw/all                    | 2        |   sch    |all|
    | rw/{class}/all             | 2        |   sch    |{class}|



###消息（message）
message为json格式，其中参数内容随命令类型变更，发送给SLAM时，参数可包含多条子命令，子命令的分发工作由SLAM Gateway进行

移动/路径移动：
```
{
    "messageType":"command",
    "text":"move",
    "timestamp":"1515762490",
    "from":"rw/term/single/1",
    "submessages":[
        {"submessage":"moveTo", 
        "points":[
            {"x":"100", "y":"100", "z":"0"},
            {"x":"100", "y":"200", "z":"0"},
            {"x":"100", "y":"243", "z":"0"},
        ], 
        "appending":"0",
        "isMilestone":"0",
        ... }
    ]
}
```

充电：
```
{
    "messageType":"command",
    "text":"charge",
    "timestamp":"1515762490",
    "from":"sch",
    "submessages":[
        {"submessage":"moveTo", "points":[{"x":"100", "y":"100", "z":"0"}], ... },
        {"submessage":"chargeConnect"}
    ]
}
```
上料/菜：
```
{
    "messageType":"command",
    "text":"serving",
    "timestamp":"1515762490",
    "from":"rw/term/single/2",
    "submessages":[
        {"submessage":"moveTo", "points":[{"x":"100", "y":"100", "z":"0"}], ... },
        {"subMessget":"serving"},
        {"submessage":"moveTo", "points":[{"x":"100", "y":"100", "z":"0"}], ... }
    ]
}
```

