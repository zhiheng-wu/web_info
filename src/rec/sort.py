from predict import predict_rating


def get_relevant_items():
    with open('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//src//1.txt', 'r') as file:
        lines = file.readlines()
        items = [line.strip() for line in lines[1:]]
        tag = lines[0].strip()
    return tag, items


def write_relevant_items(query, tag):
    with open('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//src//sorted_items_1.txt', 'w') as file:
            file.write(tag + '\n')
            for item in query:
                file.write(item + '\n')
    return


def sort_search_results(user_id, type, k=5):
    """
    对搜索结果进行排序
    存入sorted_items_1.txt文件
    输入:
        user_id (int): 用户ID
        type (int): 物品类型，0表示电影，1表示书籍
        k (int): 邻居数量
    """
    tag, items = get_relevant_items()
    predicted_ratings = predict_rating(user_id, items, type, k=k)
    
    # 按照预测评分排序
    sorted_items = sorted(items, key=lambda x: predicted_ratings[x], reverse=True)
    
    # 写入文件
    write_relevant_items(sorted_items, tag)
    return

sort_search_results(1386692, 0, 5)