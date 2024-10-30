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
已完成，使用预训练的word2vec模型，将词表内的同义词合并（注意，合并可能在不同书间存在差异，如书本A '爱''喜欢'将合并为'爱'，B可能合并为'喜欢'），所以在倒序建表时可能需要再次合并同义词。

模型地址：https://blog.csdn.net/Artoria_QZH/article/details/103299140
### 额外添加繁体字转换器
github地址：https://github.com/skydark/nstools/tree/master/zhtools

已经施加
**第一阶段完毕**

**注**：src中数据处理文件为`data_process.py`对其余的**可能**存在依赖，勿动