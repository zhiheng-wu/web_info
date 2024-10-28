# encoding=utf-8
import jieba
import csv
import pkuseg
import re


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
                    if(seg!=''):
                        words.append(seg)
        books_pred.append([row[0],words])
    book.close()
    book = open("../data/book_processed_by_jieba.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)

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
                    seg = "".join(re.findall('([\u4e00-\u9fa5]|[\u0061-\u007a]|[\u0041-\u005a])+', seg, re.S))
                    if(seg!=''):
                        words.append(seg)
        books_pred.append([row[0],words])
    book.close()
    book = open("../data/movie_processed_by_jieba.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)


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
                    if(seg!=''):
                        words.append(seg)
        books_pred.append([row[0],words])
    book.close()
    book = open("../data/book_processed_by_pkuseg.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)

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
                    if(seg!=''):
                        words.append(seg)
        books_pred.append([row[0],words])
    book.close()
    book = open("../data/movie_processed_by_pkuseg.csv", mode='w', encoding="utf-8")
    writer = csv.writer(book)
    writer.writerows(books_pred)


def main():
    book_addr = "../data/selected_book_top_1200_data_tag.csv"
    movie_addr = "../data/selected_movie_top_1200_data_tag.csv"
    stop_addr = "../help_files/stopwords_hit.txt"
    jieba_process(book_addr, movie_addr, stop_addr)
    pkuseg_process(book_addr, movie_addr, stop_addr)


if __name__ == '__main__':
    main()
    