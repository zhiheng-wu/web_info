import pandas as pd
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.preprocessing import MultiLabelBinarizer
from sklearn.feature_extraction.text import CountVectorizer
from scipy.sparse import csr_matrix

# 1. 加载数据
def load_data(file_path):
    """
    加载评分数据文件，并返回 DataFrame
    """
    df = pd.read_csv(file_path)
    return df

# 导入数据
movie_df = load_data('D://data//score_contact//movie_score.csv')
book_df = load_data('D://data//score_contact//book_score.csv')


# 2. 数据预处理

def preprocess_data(df):
    """
    对数据进行预处理，处理缺失值（如标签列）等
    """
    # 处理缺失值（如标签列）# 用空字符串填充缺失值
    # 检查列名是否存在
    if 'Tag' not in df.columns:
        raise ValueError("The 'Tag' column is missing from the input data.")

    # 填充缺失值
    df['Tag'] = df['Tag'].fillna('')
    # 确保标签列以列表形式存储
    df['Tag_list'] = df['Tag'].apply(lambda x: x.split(', ') if x else [])
    return df
    # 将标签列拆分为多个独立标签
    # 这是比助教给出的简单合并tag为单一字符串优化的地方之一，分为独立标签比简单聚合Tag更准确



# 处理电影和书籍数据
movie_df = preprocess_data(movie_df)
book_df = preprocess_data(book_df)

# 查看处理后的数据
# print("Processed Movie DataFrame: \n", movie_df.head())
# print("Processed Book DataFrame: \n", book_df.head())


# 3. 合并标签并统计频率
# csv中会出现不同的用户对同一个电影或电影评价的情况，应该将他们添加的tag放在一起，并计算每个tag出现的次数，存放在向量或矩阵中
def aggregate_tags(df, item_column, tag_column):
    """
    合并每个 Item 的标签并统计每个标签的出现频率。
    参数:
        df (pd.DataFrame): 包含评分和标签数据的 DataFrame
        item_column (str): Item 列的名称
        tag_column (str): 标签列表列的名称
    返回:
        aggregated_df: 每个 Item 的标签频率 DataFrame。
    """
    # 展开标签列
    exploded_tags = df.explode(tag_column)

    # 按 Item 和标签统计出现次数
    tag_counts = exploded_tags.groupby([item_column, tag_column]).size().reset_index(name='Tag_Frequency')

    # 按 Item 汇总为标签文本
    tag_aggregates = tag_counts.groupby(item_column).apply(
        lambda x: ' '.join([f"{row[tag_column]} " * row['Tag_Frequency'] for _, row in x.iterrows()])
    ).reset_index(name='Aggregated_Tags')

    # 确保保留 Item ID 列
    tag_aggregates[item_column] = tag_aggregates[item_column].astype(int)  # 确保 Item ID 是整型

    return tag_aggregates

# 对电影和书籍数据进行标签聚合
movie_tags = aggregate_tags(movie_df, item_column='Movie', tag_column='Tag')
book_tags = aggregate_tags(book_df, item_column='Book', tag_column='Tag')


# 之前会出现错误
# 尝试为每个标签创建非常大的稀疏矩阵。由于标签类别数量非常多，导致内存无法分配☆☆☆
# 4. 对聚合后的标签进行向量化
def vectorize_tags(tag_df, item_column, text_column):
    """
    使用 CountVectorizer 或 TfidfVectorizer 对标签进行向量化。
    参数:
        tag_df (pd.DataFrame): 包含标签数据的 DataFrame。
        text_column (str): 包含聚合标签文本的列名称。
    返回:
        vectorized_df: 包含 Item ID 和标签向量的 DataFrame
        vectorizer: 用于转换的向量化工具实例
    """
    vectorizer = CountVectorizer()
    # 对标签文本进行向量化
    vectorized_matrix = vectorizer.fit_transform(tag_df[text_column])

    # 将 Item ID 加入矩阵
    vectorized_df = pd.DataFrame.sparse.from_spmatrix(vectorized_matrix, columns=vectorizer.get_feature_names_out())
    vectorized_df[item_column] = tag_df[item_column].values  # 添加 Item ID 列

    return vectorized_df, vectorizer

# 对电影和书籍标签进行向量化
movie_vectorized, movie_vectorizer = vectorize_tags(movie_tags, item_column='Movie', text_column='Aggregated_Tags')
book_vectorized, book_vectorizer = vectorize_tags(book_tags, item_column='Book', text_column='Aggregated_Tags')


# 查看处理后的数据
# print("Processed Movie DataFrame with one-hot encoded tags: \n", movie_df.head())
# print("Processed Book DataFrame with one-hot encoded tags: \n", book_df.head())

# 5. 保存结果
def save_sparse_matrix(df, output_path):
    """
    保存带有 Item ID 的稀疏矩阵到文件。
    """
    df.to_csv(output_path, index=False)

# 保存电影和书籍的向量化结果
save_sparse_matrix(movie_vectorized, 'D://data2-2//movie_score_vectorized.csv')
save_sparse_matrix(book_vectorized, 'D://data2-2//book_score_vectorized.csv')

print("标签频率矩阵生成并保存完成！")

