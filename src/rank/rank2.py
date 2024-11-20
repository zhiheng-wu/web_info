from sklearn.model_selection import train_test_split
from sklearn.metrics import ndcg_score
import sys
sys.path.append('D://src//src//rec')
# from dataset_get import read_ratings
# from dataset_process import calculate_similarity
# from dataset_get_process import vectorize_tags
# from get_similaritymaric_addtag import calculate_cosine_similarity
from predict2_item import _predict_item_rating
# from predict_item_and_user import _predict_combined_rating
from tqdm import tqdm
import pandas as pd

# 读取数据

def get_dataset(type):
    if(type == 0):
        mode = 'movie'
    else:
        mode = 'book'
    # 读取CSV文件
    ratings = pd.read_csv('D://data//score_contact//'+mode+'_score.csv')
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
    train_data.to_csv('D://data2-2//train_or_test//train_data.csv')
    test_data.to_csv('D://data2-2//train_or_test//test_data.csv')
    train_data = pd.read_csv('D://data2-2//train_or_test//train_data.csv')
    test_data = pd.read_csv('D://data2-2//train_or_test//test_data.csv')
    return train_data, test_data



# 按用户分组计算NDCG
def compute_ndcg(group):
    true_ratings = group['true']
    pred_ratings = group['pred']
    return ndcg_score([true_ratings], [pred_ratings], k = 50)

def rank(type):
    train_data, test_data = get_dataset(type)
    # 加载物品相似度矩阵
    if type == 0:
        similarity_matrix = pd.read_csv('D://data2-2//similarity_metrix_tag//movie_similarity_tag.csv', index_col=0)
        mode = 'movie'
    else:
        similarity_matrix = pd.read_csv('D://data2-2//similarity_metrix_tag//book_similarity_tag.csv', index_col=0)
        mode = 'book'
    # similarity_matrix = calculate_cosine_similarity(train_data)
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
            predicting['pred'].append(_predict_item_rating(user_id, str(item_id), similarity_matrix ,train_data, 5))
        if len(predicting['true']) <= 1 or len(predicting['pred'])<= 1:
            continue
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

    # 得分结果：
    # Movie：0.7668432370340728
    # Book： 0.7679189119992363