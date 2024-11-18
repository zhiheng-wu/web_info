import pandas as pd


def read_contacts(filename):
    """读取社交网络数据"""
    contacts = {}
    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split(':')
            user_id = int(parts[0])
            friends = [int(friend) for friend in parts[1].split(',')]
            contacts[user_id] = friends
    return contacts


def read_ratings(filename, type):
    """
    读取评分数据,type=0,读取movie数据,type=1,读取book数据
    将其按照时间排序，仅保留最近的评分记录
    将同一个用户的数据合并为评分矩阵的一行
    缺省值填充为0
    注：User列对应用户ID，缺省的用户ID不补充
    """
    if(type == 0):
        mode = 'Movie'
    else:
        mode = 'Book'
    # 读取CSV文件
    ratings = pd.read_csv(filename)
    # 解析时间戳列
    ratings['Time'] = pd.to_datetime(ratings['Time'])
    # 根据User和Time列进行排序，最新的记录排在前面
    ratings = ratings.sort_values(by=['User', mode, 'Time'], ascending=[True, True, False])
    # 删除重复的记录，只保留每个用户和书籍组合的最新记录
    ratings = ratings.drop_duplicates(subset=['User', mode], keep='first')
    # 构建评分矩阵
    ratings_matrix = ratings.pivot(index='User', columns=mode, values='Rate')
    ratings_matrix = ratings_matrix.fillna(0)
    return ratings_matrix
