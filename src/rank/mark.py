import pandas as pd
import torch
import pickle
from tqdm import tqdm

from transformers import BertTokenizer, BertModel
import torch


def load_data(type):
    """
    加载数据并使用BERT模型对标签进行编码。
    type: 0 代表电影数据，1 代表图书数据。
    """
    tokenizer = BertTokenizer.from_pretrained('C:\\Users\\Administrator\\Desktop\\file\\课程\\大三\\web_info\\lab1\\web_info\\bert-base-uncased')
    model = BertModel.from_pretrained('C:\\Users\\Administrator\\Desktop\\file\\课程\\大三\\web_info\\lab1\\web_info\\bert-base-uncased').cuda()
    if type == 0:
        mode = 'movie'
    else:
        mode = 'book'
    loaded_data = pd.read_csv('C:\\Users\\Administrator\\Desktop\\file\\课程\\大三\\web_info\\lab1\\web_info\\data\\score_contact\\selected_'+mode+'_top_1200_data_tag.csv')
    tag_embedding_dict = {}

    with torch.no_grad():
        for index, rows in tqdm(loaded_data.iterrows()):
            # 将标签列表转换为字符串
            tags_str = " ".join(rows.Tags)
            # 使用BERT中文模型对标签进行编码
            inputs = tokenizer(tags_str, truncation=True, return_tensors='pt')
            outputs = model(inputs.input_ids.cuda(), inputs.token_type_ids.cuda(), inputs.attention_mask.cuda())
            # 使用最后一层的平均隐藏状态作为标签的向量表示
            tag_embedding = outputs.last_hidden_state.mean(dim=1).cpu()
            tag_embedding_dict[rows.Book] = tag_embedding
            # 将映射表存储为二进制文件
            with open('C:\\Users\\Administrator\\Desktop\\file\\课程\\大三\\web_info\\lab1\\web_info\\data\\model\\'+mode+'_tag_embedding_dict.pkl', 'wb') as f:
                pickle.dump(tag_embedding_dict, f)
            return tag_embedding_dict
        
load_data(1)
load_data(0)