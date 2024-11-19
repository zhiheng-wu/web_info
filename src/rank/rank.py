from sklearn.model_selection import train_test_split
from sklearn.metrics import ndcg_score
import sys
sys.path.append('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//src//rec')
from dataset_get import read_ratings
from dataset_process import calculate_similarity
from predict import _predict_rating
from tqdm import tqdm
import pandas as pd

# 读取数据

def get_dataset(type):
    if(type == 0):
        mode = 'movie'
    else:
        mode = 'book'
    # 读取CSV文件
    ratings = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//score_contact//'+mode+'_score.csv')
    # 解析时间戳列
    if(type == 0):
        mode = 'Movie' 
    else:
        mode = 'Book'
    ratings['Time'] = pd.to_datetime(ratings['Time'])
    # 根据User和Time列进行排序，最新的记录排在前面
    ratings = ratings.sort_values(by=['User', mode, 'Time'], ascending=[True, True, False])
    # 删除重复的记录，只保留每个用户和书籍组合的最新记录
    ratings = ratings.drop_duplicates(subset=['User', mode], keep='first')
    # 构建评分矩阵
    train_data, test_data = train_test_split(ratings, test_size=0.1, random_state=42)
    train_data = train_data.pivot(index='User', columns=mode, values='Rate')
    train_data = train_data.fillna(-1)
    test_data = test_data.pivot(index='User', columns=mode, values='Rate')
    test_data = test_data.fillna(-1)
    train_data.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//rank//train_data.csv')
    test_data.to_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//rank//test_data.csv')
    train_data = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//rank//train_data.csv')
    test_data = pd.read_csv('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//rank//test_data.csv')
    return train_data, test_data



# 按用户分组计算NDCG
def compute_ndcg(group):
    true_ratings = group['true']
    pred_ratings = group['pred']
    return ndcg_score([true_ratings], [pred_ratings], k = 50)

def rank(type):
    train_data, test_data = get_dataset(type)
    similarity_matrix = calculate_similarity(train_data)
    train_data.set_index('User', inplace=True)
    test_data.set_index('User', inplace=True)
    predicting = {}
    predicting['true'] = []
    predicting['pred'] = []

    ndcgs = []
    for index, row in tqdm(test_data.iterrows(), total=len(test_data)):
        user_id = index
        for item_id, value in row.items():
            if value == -1:
                continue
            predicting['true'].append(value)
            predicting['pred'].append(_predict_rating(user_id, str(item_id), similarity_matrix ,train_data, 5))
        ndcgs.append(compute_ndcg(predicting))
        predicting['true'].clear()
        predicting['pred'].clear()
    ndcg = sum(ndcgs) / len(ndcgs)
    if(type == 0):
        print("Movie")
    else:            
        print("Book")   
    print("NDCG: ", ndcg)
    return

if __name__ == '__main__':
    rank(0)
    rank(1)

