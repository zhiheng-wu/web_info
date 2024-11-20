import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity



def calculate_cosine_similarity(df, item_column):
    """
    计算各个 Item 之间的余弦相似度，并返回相似度矩阵。

    参数:
        df (pd.DataFrame): 向量化后的 DataFrame，包含 Item ID 和标签向量。
        item_column (str): Item ID 列的名称，用于获取每个项目。

    返回:
        similarity_matrix (pd.DataFrame): 余弦相似度矩阵，行和列是 Item ID，值是相似度。
    """
    # 提取 Item 的标签向量（去除 Item ID 列）
    vectors = df.drop(columns=[item_column])

    # 计算余弦相似度
    cosine_sim = cosine_similarity(vectors)

    # 将余弦相似度结果转换为 DataFrame，并添加 Item ID
    similarity_matrix = pd.DataFrame(cosine_sim, index=df[item_column], columns=df[item_column])

    return similarity_matrix


# 1. 加载保存的向量化后的电影和书籍数据
movie_vectorized = pd.read_csv('D://data2-2//movie_score_vectorized.csv')
book_vectorized = pd.read_csv('D://data2-2//book_score_vectorized.csv')

# 2. 计算电影的余弦相似度
movie_similarity = calculate_cosine_similarity(movie_vectorized, item_column='Movie')

# 3. 计算书籍的余弦相似度
book_similarity = calculate_cosine_similarity(book_vectorized, item_column='Book')

# 4. 保存电影和书籍的余弦相似度矩阵
movie_similarity.to_csv('D://data2-2//similarity_metrix_tag//movie_similarity_tag.csv', index=True)
book_similarity.to_csv('D://data2-2//similarity_metrix_tag//book_similarity_tag.csv', index=True)

print("电影和书籍各Item余弦相似度矩阵生成并保存完成！")

