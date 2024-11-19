
import pandas as pd
from scipy.spatial.distance import jaccard
import pickle

with open('C:\\Users\\Administrator\\Desktop\\file\\课程\\大三\\web_info\\lab1\\web_info\\data\\social_graph', 'rb') as f:
    social_network_graph = pickle.load(f)

def combined_similarity(ratings_similarities, social_network_graph, user1, user2):
    """计算综合了评分相似性和社交网络相似性的新相似度"""

    if user1 in ratings_similarities.index and user2 in ratings_similarities.columns:
        rating_similarity = ratings_similarities.loc[user1, user2]
    else:
        rating_similarity = 0
    
    # 计算社交网络上的Jaccard相似度
    common_friends = len(set(social_network_graph.neighbors(user1)) & set(social_network_graph.neighbors(user2)))
    total_friends = len(set(social_network_graph.neighbors(user1)).union(set(social_network_graph.neighbors(user2))))
    social_similarity = 1 - jaccard(list(social_network_graph.neighbors(user1)), list(social_network_graph.neighbors(user2))) if total_friends > 0 else 0
    
    # 结合两种相似度
    alpha = 0.5  # 可调节参数，控制两种相似度的重要性
    combined_similarity = alpha * rating_similarity + (1 - alpha) * social_similarity
    
    return combined_similarity

similarity_matrix = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//movie_rank_matrix.csv')
for index, row in similarity_matrix.iterrows():
    for column, value in row.items(): 
        similarity_matrix.loc[index, column] = combined_similarity(similarity_matrix, social_network_graph, index, column)
similarity_matrix.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//'+'movie'+'_rank_withsocial_matrix.csv')
similarity_matrix = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//book_rank_matrix.csv')
for index, row in similarity_matrix.iterrows():
    for column, value in row.items():
        similarity_matrix.loc[index, column] = combined_similarity(similarity_matrix, social_network_graph, index, column)
similarity_matrix.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//'+'book'+'_rank_withsocial_matrix.csv')
