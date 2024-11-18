import numpy as np
import pandas as pd
import numpy as np
from dataset_get import read_ratings

def _predict_rating(user_id, item_id, similarity_matrix, ratings_matrix, k=5):
    """
    预测用户对物品的评分。
    
    参数:
        user_id (int): 用户的ID。
        item_id (int): 物品的ID。
        similarity_matrix (pandas.DataFrame): 用户间的相似度矩阵。
        ratings_matrix (pandas.DataFrame): 用户对物品的评分矩阵。
        k (int): 考虑的邻居数量，默认为5。
        
    返回:
        float: 预测的评分。
    """
    # 检查输入的有效性
    if not isinstance(k, int) or k <= 0:
        raise ValueError("邻居数量k必须是正整数")
    # 检查用户和物品是否存在
    if user_id not in similarity_matrix.index or item_id not in ratings_matrix.columns:
        if item_id not in ratings_matrix.columns:
            print('物品不存在')
        if user_id not in similarity_matrix.index:
            print('用户不存在')
        return 0
    # 找到邻居用户
    if user_id not in similarity_matrix.index:
        return 0  # 如果用户不在相似度矩阵中，返回0
    neighbors = similarity_matrix.loc[user_id].sort_values(ascending=False).index[:k]
    # 计算加权平均评分
    numerator = 0
    denominator = 0
    for neighbor in neighbors:
        neighbor = int(neighbor)
        if neighbor not in ratings_matrix.index or ratings_matrix.loc[neighbor, item_id] <= 0:
            continue
        weight = similarity_matrix.loc[user_id, str(neighbor)]
        rating = ratings_matrix.loc[neighbor, item_id]
        numerator += weight * rating
        denominator += abs(weight)
    
    if denominator == 0:
        return 0
    else:
        return numerator / denominator
    
def predict_rating(user_id, items, type = 0, k=5):
    if type == 0:
        mode = 'movie'
    else:
        mode = 'book'
    # 读取评分数据
    ratings_matrix = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//'+mode+'_matrix.csv')
    # 读取相似度矩阵
    similarity_matrix = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//'+mode+'_rank_matrix.csv', index_col=0)
    # 预测评分
    predict_rating = {}
    ratings_matrix.set_index('User', inplace=True)
    for item in items:
        predict_rating[item] = _predict_rating(user_id, item, similarity_matrix, ratings_matrix, k)
    return predict_rating

