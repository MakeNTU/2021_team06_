from vpython import *
import time, serial

COM_PORT = '/dev/cu.usbserial-0001'
BAUD_RATES = 9600
ser = serial.Serial(COM_PORT, BAUD_RATES)
ser.readline()

scene = canvas(align='left', width=0, height=0, background=vec(1, 0, 1))

v_graph = graph(align='left', width='700', xtitle='t', ytitle='時速(km/h)', background=vector(0, 0, 0), ymin=0, ymax=255)
v_t = gcurve(color=color.blue, graph=v_graph, width=5)

T_graph = graph(align='right', width='700', xtitle='t', ytitle='引擎溫度(°C)', background=vector(0, 0, 0), ymin=0, ymax=255)
T_t = gcurve(color=color.red, graph=T_graph, width=5)

scene1 = canvas(align='left', width=0, height=0, background=vec(1, 0, 1))

t0 = time.time()
train = 1
speed_limit = 150
invasion = 0

v = [0, 0]
T = [0, 0]
brakeState = [0, 0]
overspeed = [0, 0]
overtemp = [0, 0]


def train_select(m):
    global train
    train_dict = {'普悠瑪號': 0, '太魯閣號': 1}
    train = train_dict[m.selected]
    v_t.delete()
    T_t.delete()
    # print(train)


def reset_button(b):
    global t0
    t0 = time.time()
    v_t.delete()
    T_t.delete()


menu(choices=['普悠瑪號', '太魯閣號'], bind=train_select, pos=scene.caption_anchor, selected='太魯閣號')
button(text='Reset', bind=reset_button, pos=scene.caption_anchor)


def set_speed(s):
    global speed_limit
    speed_limit = s.number
    # print(s.number)
    out = str(speed_limit) + ',' + str(int(invasion)) + '\n'
    ser.write(out.encode('ascii'))
    print(out)


def set_invasion(r):
    global invasion
    invasion = r.checked
    out = str(speed_limit) + ',' + str(int(invasion)) + '\n'
    ser.write(out.encode('ascii'))
    print(out)


scene1.append_to_caption('速限:')
speed_control = winput(bind=set_speed, pos=scene1.caption_anchor, text=str(speed_limit), width=50)
scene1.append_to_caption('\n')

checkbox(bind=set_invasion, pos=scene1.caption_anchor, text='軌道異物入侵', width=50)
scene1.append_to_caption('\n')


def f(b):
    pass


brake_display0 = button(background=color.white, bind=f, text='普悠瑪號煞車')
brake_display1 = button(background=color.white, bind=f, text='太魯閣號煞車')
scene1.append_to_caption('\n')
overspeed_display0 = button(background=color.white, bind=f, text='普悠瑪號超速')
overspeed_display1 = button(background=color.white, bind=f, text='太魯閣號超速')
scene1.append_to_caption('\n')
overtemp_display0 = button(background=color.white, bind=f, text='普悠瑪號過熱')
overtemp_display1 = button(background=color.white, bind=f, text='太魯閣號過熱')
scene1.append_to_caption('\n')


def state_to_color(state):
    if state:
        return color.red
    else:
        return color.green


while 1:
    rate(20)

    data = ser.readline().decode().strip('\n').split(',')
    # data = [0, 0, 0, 0, 0, 0]

    i = int(data[0])
    v[i] = int(data[1])
    T[i] = int(data[2])
    brakeState[i] = int(data[3])
    overspeed[i] = int(data[4])
    overtemp[i] = int(data[5])

    v_t.plot(time.time() - t0, v[train])
    T_t.plot(time.time() - t0, T[train])

    out = str(speed_limit) + ',' + str(int(invasion)) + '\n'
    print(i, v, T, brakeState, overspeed, overtemp)
    # print(train, out)
    # ser.write(out.encode('ascii'))

    brake_display0.background = state_to_color(brakeState[0])
    brake_display1.background = state_to_color(brakeState[1])
    overspeed_display0.background = state_to_color(overspeed[0])
    overspeed_display1.background = state_to_color(overspeed[1])
    overtemp_display0.background = state_to_color(overtemp[0])
    overtemp_display1.background = state_to_color(overtemp[1])
