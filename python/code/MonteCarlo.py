import numpy as np
import math
import xlrd
import xlwt
import random


def get_data_from_excel(filename, raw, column, hasrawhead=0):    # 从excel读取数据 若有表头，hasrawhead=1
    data = np.zeros((raw, column), dtype=float)
    data_sheet = xlrd.open_workbook(filename).sheet_by_index(0)
    for i in range(0 + hasrawhead, raw + hasrawhead):
        for j in range(0 + hasrawhead, column + hasrawhead):
            data[i - 1][j - 1] = data_sheet.cell(i, j).value
    return data


def get_one_column_random(avg, deviation, num):    # 生成正态分布随机数据
    count = 100
    while True:
        result = np.random.normal(avg, deviation, num)    # 均值 标准差 数量
        if (math.fabs(np.mean(result) - avg) / avg <= 0.01) \
                and(math.fabs(np.var(result) - deviation) / deviation <= (count / 1000.0)):
            break
        count = count + 1

    return result.data


def adjustment(data, avg, low, high, adjust_coefficient=0.3):    # 要调整的数据集 平均值 下限 上限 调整系数
    not_adjust = True
    count = len(data)
    while not_adjust:
        for i in range(0, count):    # 调整上下限
            if data[i] < low or data[i] > high:
                coefficient = random.uniform(adjust_coefficient * 0.5, adjust_coefficient)
                if data[i] < low:    # 调整小于下限的
                    temp = (data[i] - low * (1 + coefficient)) / count
                    data[i] = low * (1 + coefficient)
                else:    # 调整大于上限的
                    temp = (data[i] - high * (1 - coefficient)) / count
                    data[i] = high * (1 - coefficient)
                for j in range(0, count):
                    data[j] = data[j] + temp

        data_sum = 0
        for i in range(0, count):
            data_sum = data_sum + data[i]
        temp = (avg * count - data_sum) / 20
        for i in range(0, count):
            data[i] = data[i] + temp

        not_adjust = False
        for i in range(0, count):
            if data[i] < low or data[i] > high:
                not_adjust = True
                break

    return sorted(data)


def process():
    filename = "F:\\data-set\\MonteCarlo\\rawdata.xlsx"
    raw = 7
    column = 13
    expert_count = 20
    data = get_data_from_excel(filename, raw, column, 1)    # 前几行是数据，倒数第三行标准差，倒数第二和一行是约束min和max
    work = xlwt.Workbook()
    for i in range(0, raw - 3):
        sheet = work.add_sheet("方案a" + str(i + 1))
        column_count = 0
        for j in range(0, column):
            simulation_data = get_one_column_random(data[i][j], data[raw - 3][j], expert_count)
            simulation_data = adjustment(simulation_data, data[i][j], data[raw - 2][j], data[raw - 1][j])
            print(str(i), str(j))
            for k in range(0, expert_count):
                sheet.write(k, column_count, simulation_data[k])
            column_count = column_count + 1

    work.save("F:\\data-set\\MonteCarlo\\simulation.xls")


def show(data):
    for i in data:
        print(i)
    print()


process()



