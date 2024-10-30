# encoding=utf-8
import jieba
import csv
import pkuseg
import re
import gensim
import gc
from langconv import Converter
def convert(text, flag=1):  #text为要转换的文本，flag=0代表简化繁，flag=1代表繁化简    
    rule = 'zh-hans' if flag else 'zh-hant'    
    return Converter(rule).convert(text)    

model = gensim.models.KeyedVectors.load_word2vec_format('/home/wu/web_info/lab1/models/word2vec/baike_26g_news_13g_novel_229g.bin',binary=True)

def jieba_process(book_addr, movie_addr, stop_addr):
    # 加载停用词库
    with open(stop_addr, encoding='utf-8') as f: # 可根据需要打开停用词库，然后加上不想显示的词语
        con = f.readlines()
        stop_words = set() # 集合可以去重
        for i in con:
            i = i.replace("\n", "")   # 去掉读取每一行数据的\n
            stop_words.add(i)

    book = open(book_addr, mode='r', encoding="utf-8")
    books = csv.reader(book)
    books_pred = []
    books_pred.append(['Book','Tags'])
    for row in books:
        titles = row[1].strip('}')
        titles = titles.strip('{')
        titles = titles.split(', ')
        words = []
        for word in titles:
            seg_list = jieba.cut(word)  # 默认是精确模式
            for seg in seg_list:
                if seg not in stop_words:
                    seg = "".join(re.findall('([\u4e00-\u9fa5]+|[\u0061-\u007a]+|[\u0041-\u005a]+)', seg, re.S))
                    seg = convert(seg)
                    if(seg!=''):
                        similar = False
                        if (len(words)==0) or (seg not in model):
                            words.append(seg)
                        else:
                            for w in words:
                                if w in model:
                                    similarity = model.similarity(seg, w)
                                else:
                                    similarity = 0
                                if similarity > 0.75:
                                    similar = True
                                    break
                            if not similar:
                                words.append(seg)
        books_pred.append([row[0],words])
    book.close()
    book = open("./data/book_processed_by_jieba.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)
    print('jieba book finished')
    del books_pred
    gc.collect()
    book.close()

    book = open(movie_addr, mode='r', encoding="utf-8")
    books = csv.reader(book)
    books_pred = []
    books_pred.append(['Book','Tags'])
    for row in books:
        titles = row[1].strip('}')
        titles = titles.strip('{')
        titles = titles.split(', ')
        words = []
        for word in titles:
            seg_list = jieba.cut(word)  # 默认是精确模式
            for seg in seg_list:
                if seg not in stop_words:
                    seg = "".join(re.findall('([\u4e00-\u9fa5]+|[\u0061-\u007a]+|[\u0041-\u005a]+)', seg, re.S))
                    seg = convert(seg)
                    if(seg!=''):
                        similar = False
                        if (len(words)==0) or (seg not in model):
                            words.append(seg)
                        else:
                            for w in words:
                                if w in model:
                                    similarity = model.similarity(seg, w)
                                else:
                                    similarity = 0
                                if similarity > 0.75:
                                    similar = True
                                    break
                            if not similar:
                                words.append(seg)
        books_pred.append([row[0],words])
    book.close()
    book = open("./data/movie_processed_by_jieba.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)
    print('jieba movie finished')
    del books_pred
    gc.collect()
    book.close()

def pkuseg_process(book_addr, movie_addr, stop_addr):
    # 加载停用词库
    with open(stop_addr, encoding='utf-8') as f: # 可根据需要打开停用词库，然后加上不想显示的词语
        con = f.readlines()
        stop_words = set() # 集合可以去重
        for i in con:
            i = i.replace("\n", "")   # 去掉读取每一行数据的\n
            stop_words.add(i)

    book = open(book_addr, mode='r', encoding="utf-8")
    segment = pkuseg.pkuseg()
    books = csv.reader(book)
    books_pred = []
    books_pred.append(['Book','Tags'])
    for row in books:
        titles = row[1].strip('}')
        titles = titles.strip('{')
        titles = titles.split(', ')
        words = []
        for word in titles:
            seg_list = segment.cut(word)  # 默认是精确模式
            for seg in seg_list:
                if seg not in stop_words:
                    seg = "".join(re.findall('([\u4e00-\u9fa5]+|[\u0061-\u007a]+|[\u0041-\u005a]+)', seg, re.S))
                    seg = convert(seg)
                    if(seg!=''):
                        similar = False
                        if (len(words)==0) or (seg not in model):
                            words.append(seg)
                        else:
                            for w in words:
                                if w in model:
                                    similarity = model.similarity(seg, w)
                                else:
                                    similarity = 0
                                if similarity > 0.75:
                                    similar = True
                                    break
                            if not similar:
                                words.append(seg)
            del seg_list
            gc.collect
        books_pred.append([row[0],words])
        del words
        del titles
        gc.collect()
    book.close()
    book = open("./data/book_processed_by_pkuseg.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)
    print('pku book finished')
    del books_pred
    gc.collect()
    book.close()

    book = open(movie_addr, mode='r', encoding="utf-8")
    books = csv.reader(book)
    books_pred = []
    books_pred.append(['Book','Tags'])
    for row in books:
        titles = row[1].strip('}')
        titles = titles.strip('{')
        titles = titles.split(', ')
        words = []
        for word in titles:
            seg_list = segment.cut(word)  # 默认是精确模式
            for seg in seg_list:
                if seg not in stop_words:
                    seg = "".join(re.findall('([\u4e00-\u9fa5]+|[\u0061-\u007a]+|[\u0041-\u005a]+)', seg, re.S))
                    seg = convert(seg)
                    if(seg!=''):
                        similar = False
                        if (len(words)==0) or (seg not in model):
                            words.append(seg)
                        else:
                            for w in words:
                                if w in model:
                                    similarity = model.similarity(seg, w)
                                else:
                                    similarity = 0
                                if similarity > 0.75:
                                    similar = True
                                    break
                            if not similar:
                                words.append(seg)
            del seg_list
            gc.collect()
        books_pred.append([row[0],words])
        del words
        del titles
        gc.collect
    book.close()
    book = open("./data/movie_processed_by_pkuseg.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)
    print('pku movie finished')
    del books_pred
    gc.collect
    book.close()


def main():
    book_addr = "/home/wu/web_info/lab1/data/selected_book_top_1200_data_tag.csv"
    movie_addr = "/home/wu/web_info/lab1/data/selected_movie_top_1200_data_tag.csv"
    stop_addr = "/home/wu/web_info/lab1/help_files/stopwords_hit.txt"
    jieba_process(book_addr, movie_addr, stop_addr)
    pkuseg_process(book_addr, movie_addr, stop_addr)


if __name__ == '__main__':
    main()
    