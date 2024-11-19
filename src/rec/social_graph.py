import networkx as nx
import pickle

# 创建一个空的无向图
social_network_graph = nx.Graph()

# 打开文件并读取所有内容
with open('C://Users//Administrator//Desktop//file//课程//大三//web_info//lab1//web_info//data//score_contact//Contacts.txt', 'r') as file:
    social_network_data = file.read()

# 添加用户及其好友到图中
for line in social_network_data.strip().split('\n'):
    parts = [part.strip() for part in line.split(':')]
    if len(parts) < 2:
        continue
    user_id = parts[0]
    friends = [friend.strip() for friend in parts[1].split(',')]
    
    # 确保用户节点存在
    social_network_graph.add_node(user_id)
    
    for friend in friends:
        if friend:  # 避免添加空字符串作为节点
            # 确保好友节点存在
            social_network_graph.add_node(friend)
            # 添加边
            social_network_graph.add_edge(user_id, friend)


# 假设social_network_graph已经是构建好的图
with open('C:\\Users\\Administrator\\Desktop\\file\\课程\\大三\\web_info\\lab1\\web_info\\data\\social_graph', 'wb') as f:
    pickle.dump(social_network_graph, f)