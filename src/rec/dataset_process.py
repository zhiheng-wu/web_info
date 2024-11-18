from sklearn.metrics.pairwise import cosine_similarity
from dataset_get import read_ratings
import pandas as pd

# 计算用户间的余弦相似度,返回值为DataFrame
def calculate_similarity(ratings_matrix):
    matrix = ratings_matrix.drop('User', axis=1)
    similarity_matrix = cosine_similarity(matrix)
    similarity_matrix = pd.DataFrame(similarity_matrix, index=ratings_matrix['User'], columns=ratings_matrix['User'])
    return similarity_matrix
