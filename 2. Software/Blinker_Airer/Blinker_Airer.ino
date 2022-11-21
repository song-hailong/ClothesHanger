#define BLINKER_WIFI//接入小爱同学
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>

char auth[] = "XXXXXXXXX";   //app中获取到的Secret Key(密钥)
char ssid[] = "XXXXXXXXX";   //WIFI名称
char pswd[] = "XXXXXXXXX";   //WIFI密码

// 变量定义
bool airer_state = false; //晾衣架状态
bool rain = false;//是否下雨
bool illu = false; //是否出太阳
int Humi = 84;//湿度
int Temp = 17;//温度
char uart_char[20];
char sprintf_char[20];

// 新建组件对象
BlinkerText Text_airer("tex-airer");//晾衣架状态
BlinkerText Text_rain("tex-rain");//是否下雨
BlinkerText Text_illu("tex-illu");//是否出太阳

BlinkerButton Button_on("btn-on");//打开衣架
BlinkerButton Button_off("btn-off");//关闭衣架

BlinkerNumber Num_humi("num-humi");//湿度
BlinkerNumber Num_temp("num-temp");//温度

// 按下打开按键即会执行该函数
void Button_on_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  airer_state = true;
  digitalWrite(LED_BUILTIN, LOW);
  //BUILTIN_SWITCH.print(airer_state ? "on" : "off");//发送外部开关状态
  //Text_airer.print(airer_state ? "晾衣架已开启" : "晾衣架已关闭");//发送晾衣架状态

  sprintf(sprintf_char, "BA%dR%dI%dH%02dT%02dE\r\n", airer_state, rain, illu, Humi, Temp);
  Serial.print(sprintf_char);

  Blinker.vibrate();//发送手机振动指令
}
// 按下关闭按键即会执行该函数
void Button_off_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  airer_state = false;
  digitalWrite(LED_BUILTIN, HIGH);
  //BUILTIN_SWITCH.print(airer_state ? "on" : "off");//发送外部开关状态
  //Text_airer.print(airer_state ? "晾衣架已开启" : "晾衣架已关闭");//发送晾衣架状态

  sprintf(sprintf_char, "BA%dR%dI%dH%02dT%02dE\r\n", airer_state, rain, illu, Humi, Temp);
  Serial.print(sprintf_char);

  Blinker.vibrate();//发送手机振动指令
}

//用于处理 BUILTIN_SWITCH 收到数据的回调函数
void switch_callback(const String & state)
{
  BLINKER_LOG("get switch state: ", state);
  if (state == BLINKER_CMD_ON)
  {
    airer_state = true;
    digitalWrite(LED_BUILTIN, LOW);
    //BUILTIN_SWITCH.print("on");//发送开关当前的状态(多用于反馈开关状态)到APP
    //Text_airer.print(airer_state ? "晾衣架已开启" : "晾衣架已关闭");//发送晾衣架状态
  }
  else
  {
    airer_state = false;
    digitalWrite(LED_BUILTIN, HIGH);
    //BUILTIN_SWITCH.print("off");
    //Text_airer.print(airer_state ? "晾衣架已开启" : "晾衣架已关闭");//发送晾衣架状态
  }
  sprintf(sprintf_char, "BA%dR%dI%dH%02dT%02dE\r\n", airer_state, rain, illu, Humi, Temp);
  Serial.print(sprintf_char);
}

//小爱电源类操作的回调函数:
//当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);
  if (state == BLINKER_CMD_ON)
  {
    airer_state = true;
    digitalWrite(LED_BUILTIN, LOW);

    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();

    //BUILTIN_SWITCH.print("on");//发送开关当前的状态(多用于反馈开关状态)到APP
    //Text_airer.print(airer_state ? "晾衣架已开启" : "晾衣架已关闭");//发送晾衣架状态
  }
  else if (state == BLINKER_CMD_OFF)
  {
    airer_state = false;
    digitalWrite(LED_BUILTIN, HIGH);

    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();

    //BUILTIN_SWITCH.print("off");
    //Text_airer.print(airer_state ? "晾衣架已开启" : "晾衣架已关闭");//发送晾衣架状态
  }
  sprintf(sprintf_char, "BA%dR%dI%dH%02dT%02dE\r\n", airer_state, rain, illu, Humi, Temp);
  Serial.print(sprintf_char);
}

/*
   Add your heartbeat message detail in this function

   When get heartbeat command {"get": "state"}, device will call this function
   当get heartbeat命令{“get”:“state”}时，设备将调用此函数
   For example, you can print message back  例如，您可以打印回消息

   Every 30s will get a heartbeat command from app
   每30分钟就会从应用程序收到一个心跳命令
*/
void heartbeat()
{
  BUILTIN_SWITCH.print(airer_state ? "on" : "off");//发送外部开关状态
  Text_airer.print(airer_state ? "晾衣架已开启" : "晾衣架已关闭");//发送晾衣架状态
  Num_humi.print(Humi);//发送湿度的数值, 并将以上设置一并发送到APP
  Num_temp.print(Temp);//发送温度的数值, 并将以上设置一并发送到APP
  Text_rain.print(rain ? "正在下雨" : "未下雨");//发送是否下雨
  Text_illu.print(illu ? "未出太阳" : "出太阳了" ); //发送是否出太阳

  BLINKER_LOG("heartbeat!");
}

//当有设备收到APP发来的数据时会调用对应的回调函数
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();//发送手机振动指令

  uint32_t BlinkerTime = millis();
  Blinker.print("millis", BlinkerTime);
}

void setup()
{
  Serial.begin(115200);  // 初始化串口
  //BLINKER_DEBUG.stream(Serial);// 开启调试信息
  //BLINKER_DEBUG.debugAll();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Blinker.begin(auth, ssid, pswd);// 初始化blinker
  //注册回调函数
  Blinker.attachData(dataRead); //注册回调函数，当有设备收到APP发来的数据时会调用对应的回调函数
  Blinker.attachHeartbeat(heartbeat);//注册心跳包回调函数
  BUILTIN_SWITCH.attach(switch_callback); //设置开关的回调函数, 当收到指令时会调用该回调函数
  Button_on.attach(Button_on_callback);//注册打开按键的回调函数, 当收到指令时会调用该回调函数
  Button_off.attach(Button_off_callback);//注册关闭按键的回调函数, 当收到指令时会调用该回调函数
  BlinkerMIOT.attachPowerState(miotPowerState);//注册回调函数//小爱同学务必在回调函数中反馈该控制状态
#if defined(BLINKER_BUTTON)
  Blinker.attachClick(singleClick); //按键单次点击时的回调函数
  Blinker.attachDoubleClick(doubleClick);//注册按键双击时的回调函数, 当设备检测到按键双击时将调用该函数
  attachInterrupt(BLINKER_BUTTON_PIN, buttonTick, CHANGE); //使用跳变沿中断:
#endif

}

void loop()
{
  Blinker.run();
  if (Serial.available() > 0)// 如果串口接收缓冲区中有数据
  {
    char i = 0;
    while (Serial.available() > 0) // 如果串口接收缓冲区中有数据，则读取并输出
      uart_char[i++] = Serial.read();
    if (uart_char[0] == 'B' && uart_char[13] == 'E' )
    {
      Serial.print("OK\r\n");
      airer_state = uart_char[2] - '0'; //晾衣架状态
      rain = uart_char[4] - '0';//是否下雨
      illu = uart_char[6] - '0'; //是否出太阳
      Humi = (uart_char[8] - '0') * 10 + (uart_char[9] - '0'); //湿度
      Temp = (uart_char[11] - '0') * 10 + (uart_char[12] - '0'); //温度
      heartbeat();
    }
  }
}
