import numpy as np
import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity
# from dataset_get import read_ratings


"""此文件以及sort2_item.py是根据物品间的相似度矩阵(上一步求出)以及用户对物品的评分预测用户的评分"""


def _predict_item_rating(user_id, item_id, similarity_matrix, ratings_matrix, k=5):
    """
    预测用户对物品的评分，基于物品间的相似度矩阵

    参数:
        user_id (int): 用户的ID。
        item_id (int): 物品的ID。
        similarity_matrix (pandas.DataFrame): 物品间的相似度矩阵
        ratings_matrix (pandas.DataFrame): 用户对物品的评分矩阵
        k (int): 考虑的邻居物品数量，默认为5

    返回:
        float: 预测的评分。
    """
    # 检查输入的有效性
    if not isinstance(k, int) or k <= 0:
        raise ValueError("邻居数量k必须是正整数")
    # 检查用户和物品是否存在
    if user_id not in ratings_matrix.index or item_id not in similarity_matrix.columns:
        print(f"用户 {user_id} 或物品 {item_id} 不存在")
        return 0

    # 获取用户对物品的评分
    user_ratings = ratings_matrix.loc[user_id]

    if item_id not in similarity_matrix.columns:
        return 0  # 如果用户不在相似度矩阵中，返回0
    # 找到与该物品最相似的k个物品

    similar_items = similarity_matrix[item_id].sort_values(ascending=False).index[1:k+1]  # 排除自身

    # filtered_items = ratings_matrix[(ratings_matrix[item_id] != -1) | (ratings_matrix.index == user_id)].index
    # filtered_items_str = str(filtered_items)
    # filtered_similarity_matrix = similarity_matrix.loc[filtered_items, filtered_items_str]
    # similar_items = filtered_similarity_matrix.loc[user_id].sort_values(ascending=False).index[1:k + 1]

    # filtered_items = ratings_matrix.loc[user_id][ratings_matrix.loc[user_id] != -1 | (ratings_matrix.columns == item_id) ].index  # 该用户评分过的物品
    # # str_items = [str(item) for item in filtered_items]
    # filtered_similarity_matrix = similarity_matrix.loc[filtered_items, filtered_items]
    # # item_similarity_matrix = filtered_similarity_matrix.loc[item_id]
    # similar_items = filtered_similarity_matrix.sort_values(ascending=False).index[1:k + 1]

    numerator = 0
    denominator = 0

    # 计算加权平均评分
    for sim_item in similar_items:
        sim_item_str = str(sim_item)
        # sim_item = int(sim_item)

        if sim_item_str not in ratings_matrix.columns:  # 检查物品是否存在
            # print(f"物品 {sim_item} 不存在")
            continue  # 如果物品不存在，跳过
        if ratings_matrix.loc[user_id, sim_item_str] > 0:  # 用户对该物品有评分
            # print(f"有评分")
            sim_weight = similarity_matrix.loc[sim_item, item_id]
            numerator += sim_weight * ratings_matrix.loc[user_id, sim_item_str]
            denominator += abs(sim_weight)


    if denominator == 0:
        # print(f"Denominator is zero for user {user_id} and item {item_id}")
        return 0
    return numerator / denominator


def predict_item_rating(user_id, items, similarity_matrix, ratings_matrix, k=5):
    """
    预测用户对多个物品的评分。

    参数:
        user_id (int): 用户的ID。
        items (list): 物品的ID列表。
        similarity_matrix (pandas.DataFrame): 物品间的相似度矩阵。
        ratings_matrix (pandas.DataFrame): 用户对物品的评分矩阵。
        k (int): 考虑的邻居物品数量，默认为5。

    返回:
        dict: 预测的评分字典，key是物品ID，value是预测评分。
    """
    predicted_ratings = {}
    for item in items:
        predicted_ratings[item] = _predict_item_rating(user_id, item, similarity_matrix, ratings_matrix, k)
    return predicted_ratings