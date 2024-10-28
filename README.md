# web_info
## sec1
对数据进行处理
### 分词
#### 工具一 jieba
项目地址：https://github.com/fxsjy/jieba?tab=readme-ov-file
![image](./others/1.png)

测试结果，搜索引擎模式最好

![image](./others/2.png)
#### 工具二 pkuseg
项目地址：https://github.com/lancopku/pkuseg-python

#### 对比
经过对比，pku的工具效果更好，其“装B”、“港台”可以成功分词，而jieba不太可以
### 去除停用词
使用哈工大停用词表
对停用词匹配并删除
### 去除符号
使用
```python
word = "".join(re.findall('([\u4e00-\u9fa5]|[\u0061,\u007a]|[\u0041,\u005a])+', word, re.S))
```
仅匹配中英文，去除其他一切符号
### 近义词合并
待完成