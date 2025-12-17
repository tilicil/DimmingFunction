#! python3
#coding=utf-8

"""
Python版本：3.x
外部库：matplotlib1.5.3、numpy1.11.2

运行方式：
在命令行中输入：python index_wave.py 

运行结果：
命令行中会打印计算得的各点数据，
在当前目录下会生成py_index_wave.c文件，包含上述数据，
并且会弹出描绘曲线的对话框。
"""

import matplotlib.pyplot as plt 
import numpy as np
import math

#修改本变量可以更改点数，如16、32、64等
POINT_NUM = 36

#指数曲线最大为2的MAX_POWER次方
MAX_POWER = 8


# POINT_NUM 个点
x1 = np.linspace(0,MAX_POWER,int(POINT_NUM/2))

#f = 2^(x)
up =[]
for i in x1:
    temp = round(2**i)
    #得到升序列
    up.append( temp )
   


x2 = np.linspace(MAX_POWER,2*MAX_POWER,int(POINT_NUM/2))

#f = 2^(2*MAX_POWER-x)
down=[]
for i in x2:
    temp = round(2**(MAX_POWER*2-i))

    #得到降序列
    down.append( temp )


# 合并升序和降序的点
line = list(x1)+list(x2)
val = list(up)+list(down)

#最大值减一防止溢出
max_val = max(val)
for i in range(len(val)):
    if val[i] == max_val:
        val[i] = val[i] - 1

print(line)
print("*"*80)
print("整数序列:")
print(list(map(int,val)))

# 将序列等比例转换为浮点型，范围为 0.0 - 1.0（以当前最大值为基准）
max_val_after = max(val) if len(val) > 0 else 1
float_vals = [v / float(max_val_after) for v in val]
# 保留 6 位小数便于查看和写入 C 文件
float_vals_rounded = [round(f, 6) for f in float_vals]
print("等比例浮点序列 (范围 0.0-1.0):")
print(float_vals_rounded)

#写入序列到文件
with open("py_exponent_Wave.c",'w',encoding= 'UTF-8') as f:
    # 先将每个值取整，再转换为字符串并组合为一个字符串
    int_values = ', '.join(map(lambda x: str(round(x)), val))  # 对每个值取整并转换为字符串
    f.write(int_values)  # 将结果写入文件
with open("py_exponent_Wave_hex.c",'w',encoding= 'UTF-8') as f:
    # 转换为十六进制并将其格式化为字符串，不带逗号、方括号和引号
    hex_values = ', '.join(map(lambda x: hex(int(x)), val))  # 生成格式化的字符串
    f.write(hex_values)  # 写入文件

# 将浮点序列写入新文件，便于在 C/其他环境中引用
with open("py_exponent_Wave_float.c",'w',encoding= 'UTF-8') as f:
    # 使用固定小数位格式输出
    float_values = ', '.join(map(lambda x: "{:.6f}".format(x), float_vals_rounded))
    f.write(float_values)

#绘图
plt.plot(line,val,"-o")
plt.show()



