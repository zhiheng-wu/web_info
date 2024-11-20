from predict_item_and_user import predict_combined_rating
import pandas as pd

def get_relevant_items():
    with open('D://src//src//1.txt', 'r') as file:
        lines = file.readlines()
        items = [line.strip() for line in lines[1:]]
        tag = lines[0].strip()
    return tag, items


def write_relevant_items(query, tag):
    with open('D://data2-2//sorted_combined_items.txt', 'w') as file:
        file.write(tag + '\n')
        for item in query:
            file.write(item + '\n')
    return


def sort_combined_search_results(user_id, type, k=5, alpha=0.5):
    """
    对搜索结果进行排序，使用物品相似度和用户相似度共同预测评分。
    存入 sorted_combined_items.txt 文件。

    输入:
        user_id (int): 用户ID
        type (int): 物品类型，0表示电影，1表示书籍
        k (int): 邻居数量
        alpha (float): 物品相似度和用户相似度的加权系数
    """
    tag, items = get_relevant_items()

    # 读取相似度矩阵
    if type == 0:
        item_similarity_matrix = pd.read_csv('D://data2-2//similarity_metrix_tag//movie_similarity_tag.csv',
                                             index_col=0)
        user_similarity_matrix = pd.read_csv('D://data//similarity_metrix//movie_rank_matrix.csv',
                                             index_col=0)
        # 评分矩阵
        ratings_matrix = pd.read_csv('D://data//similarity_metrix//movie_matrix.csv', index_col=0)
        ratings_matrix2 = pd.read_csv('D://data//similarity_metrix//movie_matrix.csv')
        ratings_matrix2.set_index('User', inplace=True)
    else:
        item_similarity_matrix = pd.read_csv('D://data2-2//similarity_metrix_tag//book_similarity_tag.csv',
                                             index_col=0)
        user_similarity_matrix = pd.read_csv('D://data//similarity_metrix//book_rank_matrix.csv',
                                             index_col=0)
        # 评分矩阵
        ratings_matrix = pd.read_csv('D://data//similarity_metrix//book_matrix.csv', index_col=0)
        ratings_matrix2 = pd.read_csv('D://data//similarity_metrix//book_matrix.csv')
        ratings_matrix2.set_index('User', inplace=True)

    # ratings_matrix.set_index('User', inplace=True)

    # 预测评分
    predicted_ratings = predict_combined_rating(user_id, items, item_similarity_matrix, user_similarity_matrix,
                                                ratings_matrix, k=k, alpha=alpha)

    # 按照预测评分排序
    sorted_items = sorted(items, key=lambda x: predicted_ratings[x], reverse=True)

    # 写入文件
    write_relevant_items(sorted_items, tag)
    return


sort_combined_search_results(1386692, 0, 5, alpha=0.2)
# 可以将alpha设置的小一些，因为各用户对各Item的评价tag比较割裂，全部的tag很多，物品相似度被很多0影响
