from stringskiplist import getDataSet
from stringskiplist import checkDataSet
from stringskiplist import searchInDataSet
from stringskiplist import releaseDataSet
from rec.sort import sort_search_results


def search(type):
    if type == 0:
        mode = 'movie'
    else:
        mode = 'book'
    a = '..\\data\\invertedindex\\'+mode+'_processed_by_pkuseg.ii'
    dataset = getDataSet(a)
    searchInDataSet("('A' OR 'B')","1.txt",dataset)
    releaseDataSet(dataset)

def search_sort1(user_id, type):
    search(type)
    sort_search_results(user_id, type, 5)

search_sort1(1386692, 0)

