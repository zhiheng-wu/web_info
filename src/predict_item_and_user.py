import numpy as np
import pandas as pd

"""此文件以及sort_item_and_user.py是根据物品间的相似度矩阵(上一步求出)、用户间的相似度 以及用户对物品的评分 共同预测用户的评分"""


def _normalize_ratings(ratings, method="minmax"):
    """
    标准化评分，使评分在[0, 1]或[-1, 1]之间。

    参数:
        ratings (list or pd.Series): 需要标准化的评分列表。
        method (str): 标准化方法，支持 "minmax" 或 "zscore"。

    返回:
        pd.Series: 标准化后的评分。
    """
    if method == "minmax":
        return (ratings - ratings.min()) / (ratings.max() - ratings.min())  # [0, 1]
    elif method == "zscore":
        return (ratings - ratings.mean()) / ratings.std()  # Z-score标准化
    else:
        raise ValueError("不支持的标准化方法")

def _predict_combined_rating(user_id, item_id, item_similarity_matrix, user_similarity_matrix, ratings_matrix, k=5,
                             alpha=0.5):
    """
    预测用户对物品的评分，结合物品相似度矩阵和用户相似度矩阵。

    参数:
        user_id (int): 用户的ID。
        item_id (int): 物品的ID。
        item_similarity_matrix (pandas.DataFrame): 物品间的相似度矩阵。
        user_similarity_matrix (pandas.DataFrame): 用户间的相似度矩阵。
        ratings_matrix (pandas.DataFrame): 用户对物品的评分矩阵。
        k (int): 邻居数量，默认为5。
        alpha (float): 权重系数，控制物品相似度与用户相似度的影响力，默认0.5。

    返回:
        float: 预测的评分。
    """
    # 检查用户和物品是否存在
    if user_id not in ratings_matrix.index:
        print(f"用户 {user_id} 不存在于评分矩阵中")
        return 0

    if item_id not in ratings_matrix.columns:
        print(f"物品 {item_id} 不存在于评分矩阵中")
        return 0

    # 获取用户对物品的评分
    user_ratings = ratings_matrix.loc[user_id]

    # 获取最相似的k个物品和用户
    similar_items = item_similarity_matrix[item_id].sort_values(ascending=False).index[1:k + 1]  # 排除自身
    # similar_users = user_similarity_matrix.loc[user_id].sort_values(ascending=False).index[1:k+1]  # 排除自身

    filtered_users = ratings_matrix[(ratings_matrix[item_id] != -1) | (ratings_matrix.index == user_id)].index
    # filtered_users_str = str(filtered_users)
    filtered_users_str = [str(item) for item in filtered_users]
    filtered_similarity_matrix = user_similarity_matrix.loc[filtered_users, filtered_users_str]
    similar_users = filtered_similarity_matrix.loc[user_id].sort_values(ascending=False).index[1:k + 1]
    # 计算用户相似度预测
    user_numerator = 0
    user_denominator = 0
    item_id_str = str(item_id)
    for neighbor in similar_users:
        neighbor_int = int(neighbor)
        if neighbor_int not in ratings_matrix.index or ratings_matrix.loc[neighbor_int, item_id] <= 0:
            # print("没有评分或不存在")
            continue
        weight = user_similarity_matrix.loc[user_id, str(neighbor)]
        rating = ratings_matrix.loc[neighbor_int, item_id]
        user_numerator += weight * rating
        user_denominator += abs(weight)

    # 计算物品相似度预测
    item_numerator = 0
    item_denominator = 0
    for sim_item in similar_items:
        sim_item_str = str(sim_item)
        # sim_item = int(sim_item)
        if sim_item_str not in ratings_matrix.columns:  # 检查物品是否存在
            # print(f"物品 {sim_item} 不存在")
            continue  # 如果物品不存在，跳过
        if ratings_matrix.loc[user_id, sim_item_str] > 0:  # 用户对该物品有评分
            # print(f"有评分")
            sim_weight = item_similarity_matrix.loc[sim_item, item_id]
            item_numerator += sim_weight * ratings_matrix.loc[user_id, sim_item_str]
            item_denominator += abs(sim_weight)


    # 计算最终预测评分
    if item_denominator == 0:
        item_prediction = 0
        # print(f"Denominator is zero.1 for user {user_id} and item {item_id}")
    else:
        item_prediction = item_numerator / item_denominator
        # print(f"Denominator is  not zero.1 for user {user_id} and item {item_id}")

    if user_denominator == 0:
        user_prediction = 0
        # print(f"Denominator is zero.2 for user {user_id} and item {item_id}")
    else:
        user_prediction = user_numerator / user_denominator
        # print(f"Denominator is not zero.2 for user {user_id} and item {item_id}")

    # 标准化物品预测和用户预测
    # item_prediction_normalized = _normalize_ratings(pd.Series([item_prediction]), method=normalization_method).iloc[0]
    # user_prediction_normalized = _normalize_ratings(pd.Series([user_prediction]), method=normalization_method).iloc[0]

    # 结合物品预测和用户预测
    final_prediction = alpha * item_prediction + (1 - alpha) * user_prediction
    return final_prediction
# 因为不知道计算出来的item_prediction和user_prediction分别大概是多大，甚至量级不一样
# 直接用final_prediction = alpha * item_prediction + (1 - alpha) * user_prediction可能会导致某一项影响很大而另一项很小
# 所以我新增了 _normalize_ratings 函数，用来对评分进行标准化处理。支持两种标准化方法：
#   minmax：将评分归一化到 [0, 1] 范围内。
#   zscore：采用 Z-score 标准化，将评分转换为标准正态分布
# 有错误，不搞这个了，输出验证两者相差不大


def predict_combined_rating(user_id, items, item_similarity_matrix, user_similarity_matrix, ratings_matrix,  k=5,
                            alpha=0.5):
    """
    预测用户对物品的评分，结合物品相似度和用户相似度。

    参数:
        user_id (int): 用户的ID。
        items (list): 需要预测评分的物品列表。
        item_similarity_matrix (pandas.DataFrame): 物品间的相似度矩阵。
        user_similarity_matrix (pandas.DataFrame): 用户间的相似度矩阵。
        ratings_matrix (pandas.DataFrame): 用户对物品的评分矩阵。
        k (int): 邻居数量，默认为5。
        alpha (float): 权重系数，默认0.5。

    返回:
        dict: 物品与预测评分的字典。
    """
    predicted_ratings = {}

    for item in items:
        predicted_ratings[item] = _predict_combined_rating(user_id, item, item_similarity_matrix,
                                                           user_similarity_matrix, ratings_matrix, k, alpha)

    return predicted_ratings
