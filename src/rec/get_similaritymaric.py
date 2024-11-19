from dataset_get import read_ratings
from dataset_process import calculate_similarity
import numpy as np
import pandas as pd



# 读取评分数据并储存
movie_rankings = read_ratings('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//score_contact//movie_score.csv', 0)
book_rankings = read_ratings('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//score_contact//book_score.csv', 1)
movie_rankings.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//movie_matrix.csv')
book_rankings.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//book_matrix.csv')

# 计算相似度矩阵并储存
movie_rankings = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//movie_matrix.csv')
book_rankings = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//book_matrix.csv')
movie_rank_matrix = calculate_similarity(movie_rankings)
movie_rank_matrix.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//movie_rank_matrix.csv')
book_rank_matrix = calculate_similarity(book_rankings)
book_rank_matrix.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//similarity_metrix//book_rank_matrix.csv')