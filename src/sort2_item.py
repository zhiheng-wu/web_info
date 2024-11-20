from predict2_item import predict_item_rating
import pandas as pd

def get_relevant_items():
    """
    从文件中读取相关的物品列表和标签信息
    """
    with open('D://src//src//1.txt', 'r') as file:
        lines = file.readlines()
        items = [line.strip() for line in lines[1:]]
        tag = lines[0].strip()
    return tag, items


def write_sorted_items(query, tag):
    """
    将排序后的物品写入文件
    """
    with open('D://data2-2//sorted_items_1_item.txt', 'w') as file:
        file.write(tag + '\n')
        for item in query:
            file.write(item + '\n')


def sort_item_search_results(user_id, type, k=5):
    """
    对搜索结果进行排序并存入文件。

    输入:
        user_id (int): 用户ID。
        type (int): 物品类型，0表示电影，1表示书籍。
        k (int): 邻居数量，默认为5。
    """
    tag, items = get_relevant_items()

    # 加载物品相似度矩阵
    if type == 0:
        similarity_matrix = pd.read_csv('D://data2-2//similarity_metrix_tag//movie_similarity_tag.csv', index_col=0)
        mode = 'movie'
    else:
        similarity_matrix = pd.read_csv('D://data2-2//similarity_metrix_tag//book_similarity_tag.csv', index_col=0)
        mode = 'book'

    # 加载用户评分矩阵
    ratings_matrix = pd.read_csv(f'D://data//similarity_metrix//{mode}_matrix.csv', index_col=0)

    # 预测评分
    predicted_ratings = predict_item_rating(user_id, items, similarity_matrix, ratings_matrix, k=k)

    # 按照预测评分排序
    sorted_items = sorted(items, key=lambda x: predicted_ratings[x], reverse=True)

    # 写入排序后的物品
    write_sorted_items(sorted_items, tag)


# 示例：排序电影搜索结果
sort_item_search_results(1386692, 0, 5)

