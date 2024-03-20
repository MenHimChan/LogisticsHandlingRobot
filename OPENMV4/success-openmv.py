# uart to arduino - By: HIM    for GONGXUN

# openmv：  1）获取二维码、物料放置位置顺序，并串口输出  QRx_XXX/WLx_XXX
#           2）解算机械臂在原料区的抓取顺序，并串口输出       CTx_XXX (X = 1,2,3)
#      是否需要识别色环的顺序？ 即粗加工、半成品区色环的顺序，然后对应放置？
#
#      TIPS:  以原料区为例，有【左-1|中-2|右-3】三个位置放置有物料，对应三个动作组.
#             而所谓抓取顺序，即为机械臂【执行抓取动作组的顺序】。
#             如 312，即为先抓右侧、再抓左侧、最后抓中间。

import sensor, image, time, math, lcd
from pyb import Pin, Timer
#      屏幕 320*240
height = 120
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking

clock = time.clock()
red_block_x=1
green_block_x=2
blue_block_x=3

red_w = 0
green_w = 0
blue_w = 0

message=000


lcd.init() # Initialize the lcd screen.


threshold_index = 0 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...

#thresholds = [(30, 83, 47, 127, -128, 127),   # generic_red_thresholds
#              (19, 93, -128, -26, -128, 127), # generic_green_thresholds
#              (0, 100, -20, 126, -128, -40)]  # generic_blue_thresholds
thresholds = [(49, 100, -10, 127, 10, 127),
              (41, 100, -128, -6, -128, 127),
              (0, 100, -128, 127, -128, -10)]




#################### Openmv数据处理 ###################
# 直接向arduino发送处理好的数据
# 红-1 绿-2 蓝-3

PP_s = "123"        # 色环顺序（固定） 逆时针方向

# 需要获取的顺序
above_order=0       # 上层物料抓取顺序
bottom_order=0      # 下层物料抓取顺序    二维码

Aorder=0          # 上层物料放置顺序
Border=0          # 下层物料放置顺序      OPENMV识别的顺序


Pos1 = "000"     # 上层放置位置
Pos2 = "000"     # 下层放置位置

QRCode1 = "000"  # 二维码，原料区上层搬运顺序
QRCode2 = "000"  # 二维码，原料区下层搬运顺序

def find_min(blobs):
    min_size=1000000
    for blob in blobs:
        if blob[2]*blob[3] <= min_size:
            min_blob=blob
            min_size = blob[2]*blob[3]
    return min_blob

# 输入： QRCode|任务码  Pos|物料放置位置
# 求 QRCode在Pos中的顺序
def Index_find(QRCode,Pos):   # 解算出机械臂的动作顺序
    result = 0
    for i in range(3):
        cc =  Pos.find(QRCode[i])+1
        #print(cc)
        result = result*10 + cc
    return result

#Move1 = Index_find(QRCode1,Pos1)    # 解算出机械臂抓取上层物料的动作顺序
#Move2 = Index_find(QRCode2,Pos2)	# 解算出机械臂抓取下层物料的动作顺序


#################### UART TO ARDUINO ###################
# 导入串口
from pyb import UART
# 串口3  [TX-P4, RX-P5]
uart = UART(3,9600,timeout_char = 50)   # 100 可改

# 串口收发数据
recv_data = ""    # 串口接收的数据 【 CM+QR|扫描二维码、CM+WL|
QR_flag = 0       # 扫描二维码标志位
WL_flag = 0       # 获取上层物料放置顺序标志位


# 串口发送 【QR1_XXX\QR2_XXX  WL1_XXX\WL2_XXX  CT1_XXX\CT2_XXX】
#  对应 QR|任务码  WL|物料位置  CT|机械臂抓取顺序（依据QR&WL计算）

# 要让OPENMV触发物料识别，只需在STM32端发送一个'$'字符
def Uart_recv():  # 串口接收数据
    global WL_flag
    if (uart.any()):                                                # 更新串口接收数据
        recv_data = eval(str(uart.read())).decode()                 # 将得到的数据进行解码并保存，一定要解码.decode()不然返回的是b'xxx'的形式
        #print(recv_data)
        #recv_data_decode = recv_data.decode()
        #print(recv_data_decode)
        #uart.write(recv_data)
        if(recv_data == '$$$'):
            #print("Openmv has recved CMD data.")
            WL_flag = 1
            #print("Ready for WLpose task !")


# 主循环
while(True):
    clock.tick()
    img = sensor.snapshot()
    #img = image.Image("/123.jpeg", copy_to_fb=True)
    Uart_recv()                                                     # 串口接收（接收STM32发送的指令）
    #light = Timer(2, freq=50000).channel(1, Timer.PWM, pin=Pin("P6"))
    #light.pulse_width_percent(70)
    # 二维码的识别被省略，详情看原.py文件
    #WL_flag = 1
    ## --------------------------- 物料颜色识别 ------------------------------------------------------
    ## 先下后上
    #print("down:%d,%d,%d", red_block_x, green_block_x, blue_block_x)
    if(WL_flag): # 识别物料    WL_flag   此处说明小车前行到原料区域了   ！！！！！！！！！！！！！！！！！！
        #uart.write("WL_"+Pos1+Pos2+"\r\n")
        clock.tick()
        light = Timer(2, freq=50000).channel(1, Timer.PWM, pin=Pin("P6"))
        light.pulse_width_percent(100)                                                                                                       # 控制亮度 0~100                                                                                                       # 读取一帧照片
        img = sensor.snapshot()
        red_w = 0
        green_w = 0
        blue_w = 0
        for r in img.find_blobs([thresholds[0]],roi=[60,150,260,50],pixels_threshold=200, area_threshold=200, merge=True):                   # 寻找红色的色块 注意参数roi前两个参数是指感兴趣区域的起始坐标 后两个参数是框选的范围大小
            # These values depend on the blob not being circular - otherwise they will be shaky.
             ## 通过设置颜色的rgb来查找红色在哪，然后通过查找到的x，y画出一个长方形，长方形的大小为 20
            # These values are stable all the time.
            img.draw_rectangle(r.rect())
            img.draw_cross(r.cx(), r.cy())
            # Note - the blob rotation is unique to 0-180 only.
            img.draw_keypoints([(r.cx(), r.cy(), int(math.degrees(r.rotation())))], size=20)
        #print(clock.fps())
            if(red_w < r.w()):
                red_block_x = r.cx()        # 获取最终的X坐标
                red_w = r.w()

        for g in img.find_blobs([thresholds[1]],roi=[60,150,260,50], pixels_threshold=200, area_threshold=200, merge=True):                  # 寻找绿色的色块
            # These values depend on the blob not being circular - otherwise they will be shaky.
            # These values are stable all the time.
            img.draw_rectangle(g.rect())
            img.draw_cross(g.cx(), g.cy())
            # Note - the blob rotation is unique to 0-180 only.
            img.draw_keypoints([(g.cx(), g.cy(), int(math.degrees(g.rotation())))], size=20)
        #print(clock.fps())
            if(green_w < g.w()):
                green_block_x = g.cx()                                                 # 获取最终的X坐标
                green_w = g.w()

        for b in img.find_blobs([thresholds[2]],roi=[60,150,260,50], pixels_threshold=200, area_threshold=200, merge=True):                  # 寻找蓝色的色块
            # These values depend on the blob not being circular - otherwise they will be shaky.
            # These values are stable all the time.
            img.draw_rectangle(b.rect())
            img.draw_cross(b.cx(), b.cy())
            # Note - the blob rotation is unique to 0-180 only.
            img.draw_keypoints([(b.cx(), b.cy(), int(math.degrees(b.rotation())))], size=20)
            if(blue_w < b.w()):
                blue_block_x = b.cx()                                         # 获取最终的X坐标
                blue_w = b.w()
            #if g.cx()==None:
             #   print(1)
            #elif g.cx()!=None：
            ### 用来确定水平方向，红色，绿色，蓝色的方向，也就是通过x来确定物料顺序，原点建立在最左侧，则x轴一直数值增大
            ## 第一种情况，就是红色的x小于绿色的x，绿色x小于蓝色x, 下面同理
            print("down:%d,%d,%d", red_block_x, green_block_x, blue_block_x)

            ## ---------------------用来得出原料的摆放顺序------------------- ##
            ## 原理：遍历所有的情况
            if int(red_block_x)<int(green_block_x) and int(green_block_x)<int(blue_block_x):
                if Border!=123:  # 因为border一开始是 0
                        #print("上层物料顺序为：红绿蓝")
                        #print(red_block_x,green_block_x,blue_block_x)
                        #print(blob.h(),blob.w() )
                        Border=123
            elif int(blue_block_x)<int(green_block_x) and int(green_block_x)<int(red_block_x):
                if Border!=321:
                        #print("上层物料顺序为：蓝绿红")
                        #print(red_block_x,green_block_x,blue_block_x)
                        #print(blob.h(),blob.w() )
                        Border=321
            elif int(red_block_x)<int(blue_block_x) and int(blue_block_x)<int(green_block_x):
                if Border!=132:
                        #print("上层物料顺序为：红蓝绿")
                        #print(red_block_x,green_block_x,blue_block_x)
                        #print(blob.h(),blob.w() )
                        Border=132
            elif int(green_block_x)<int(blue_block_x) and int(blue_block_x)<int(red_block_x):
                if Border!=231:
                        #print("上层物料顺序为：绿蓝红")
                        # print(red_block_x,green_block_x,blue_block_x)
                        #print(blob.h(),blob.w() )
                        Border=231
            elif int(blue_block_x)<int(red_block_x) and int(red_block_x)<int(green_block_x):
                if Border!=312:
                        #print("上层物料顺序为：蓝红绿")
                        #print(red_block_x,green_block_x,blue_block_x)
                        #print(blob.h(),blob.w() )
                        Border=312
            elif int(green_block_x)<int(red_block_x) and int(red_block_x)<int(blue_block_x):
                if Border!=213:
                        #print("上层物料顺序为：绿红蓝")
                        #print(red_block_x,green_block_x,blue_block_x)
                        #print(blob.h(),blob.w() )
                        Border=213

             ####----------------------------------------------------------------------####
            #print(Aorder)########################################得到下层物料
                #if int(red_block_x)>int(blue_block_x):
                 #   print(1)

                #a = int(green_block_x) - int(red_block_x)
                #b = int(red_block_x) - int(blue_block_x)
                #c = int(green_block_x) - int(blue_block_x)
                #print("%d,%d,%d,%d,%d,%d"%(int(red_block_x),int(green_block_x),int(blue_block_x),a,b,c))
                #print(Aorder)
            red_w = 0
            green_w = 0
            blue_w = 0

            if 1:#(Border==123) or (Border==132) or (Border==213) or (Border==231) or (Border==312) or (Border==321):
                for r in img.find_blobs([thresholds[0]],roi=[50,60,265,60],pixels_threshold=200, area_threshold=200, merge=True):
                    img.draw_rectangle(r.rect())
                    img.draw_cross(r.cx(), r.cy())
                    # Note - the blob rotation is unique to 0-180 only.
                    img.draw_keypoints([(r.cx(), r.cy(), int(math.degrees(r.rotation())))], size=20)
                    #print(clock.fps())
                    if(red_w < r.w()):
                        red_block_x = r.cx()        # 获取最终的X坐标
                        red_w = r.w()

                for g in img.find_blobs([thresholds[1]],roi=[50,60,265,60], pixels_threshold=200, area_threshold=200, merge=True):
                    # These values depend on the blob not being circular - otherwise they will be shaky.

                    # These values are stable all the time.
                    img.draw_rectangle(g.rect())
                    img.draw_cross(g.cx(), g.cy())
                    # Note - the blob rotation is unique to 0-180 only.
                    img.draw_keypoints([(g.cx(), g.cy(), int(math.degrees(g.rotation())))], size=20)
                    #print(clock.fps())
                    if(green_w < g.w()):
                        green_block_x = g.cx()                                                 # 获取最终的X坐标
                        green_w = g.w()

                for b in img.find_blobs([thresholds[2]],roi=[50,60,265,60], pixels_threshold=200, area_threshold=200, merge=True):
                    # These values depend on the blob not being circular - otherwise they will be shaky.

                    # These values are stable all the time.
                    img.draw_rectangle(b.rect())
                    img.draw_cross(b.cx(), b.cy())
                    print(b.cx(),"|", b.cy(),"|", b.w(),"|", b.h())
                    # Note - the blob rotation is unique to 0-180 only.
                    img.draw_keypoints([(b.cx(), b.cy(), int(math.degrees(b.rotation())))], size=20)
                    if(blue_w < b.w()):
                        blue_block_x = b.cx()                                         # 获取最终的X坐标
                        blue_w = b.w()

                    if int(red_block_x)<int(green_block_x) and int(green_block_x)<int(blue_block_x):
                        if Aorder!=123:
                                #print("上层物料顺序为：红绿蓝")
                                print(red_block_x,green_block_x,blue_block_x)
                                #print(blob.h(),blob.w() )
                                Aorder=123
                    elif int(blue_block_x)<int(green_block_x) and int(green_block_x)<int(red_block_x):

                        if Aorder!=321:
                                #print("上层物料顺序为：蓝绿红")
                                #print(red_block_x,green_block_x,blue_block_x)
                                #print(blob.h(),blob.w() )
                                Aorder=321
                    elif int(red_block_x)<int(blue_block_x) and int(blue_block_x)<int(green_block_x):
                        if Aorder!=132:
                                #print("上层物料顺序为：红蓝绿")
                                #print(red_block_x,green_block_x,blue_block_x)
                                #print(blob.h(),blob.w() )
                                Aorder=132
                    elif int(green_block_x)<int(blue_block_x) and int(blue_block_x)<int(red_block_x):
                        if Aorder!=231:
                                #print("上层物料顺序为：绿蓝红")
                                # print(red_block_x,green_block_x,blue_block_x)
                                #print(blob.h(),blob.w() )
                                Aorder=231
                    elif int(blue_block_x)<int(red_block_x) and int(red_block_x)<int(green_block_x):
                        if Aorder!=312:
                                #print("上层物料顺序为：蓝红绿")
                                #print(red_block_x,green_block_x,blue_block_x)
                                #print(blob.h(),blob.w() )
                                Aorder=312
                    elif int(green_block_x)<int(red_block_x) and int(red_block_x)<int(blue_block_x):
                        if Aorder!=213:
                                #print("上层物料顺序为：绿红蓝")
                                #print(red_block_x,green_block_x,blue_block_x)
                                #print(blob.h(),blob.w() )
                                Aorder=213
                #if int(Aorder)!=int(Border):
                    ##print("%dand%d"%(int(Aorder),int(Border)))
                    #Above=Aorder
                    #Below=Border
                print("%dand%d"%(int(Aorder),int(Border)))                                                      # 打印出最终识别的结果
                if int(Aorder)!= 0 and int(Border)!=0 :
                    print("OK!")

                    Pos1 = str(Aorder)
                    Pos2 = str(Border)

                    # 2）当物料识别成功后 【处理数据，发送数据】
                    #print(Pos1+Pos2)
                    #uart.write("WL_"+Pos1+Pos2+"\r\n")
                    uart.write(Pos1+Pos2+"\r\n")                #
                    print(Pos1+Pos2+"\r\n")
                    WL_flag = 0
                    #print("Done! ")
                    light.pulse_width_percent(0)
                else:
                    print("fail")


#######出现问题（1）：312+213








