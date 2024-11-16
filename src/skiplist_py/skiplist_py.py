from stringskiplist import getDataSet
from stringskiplist import checkDataSet
from stringskiplist import searchInDataSet
from stringskiplist import releaseDataSet

a = '..\..\data\skiplist\movie_processed_by_pkuseg.csv.stringskiplist';
dataset = getDataSet(a)
searchInDataSet("('A' OR 'B')","1.txt",dataset)
releaseDataSet(dataset)