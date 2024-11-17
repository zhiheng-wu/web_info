from stringskiplist import getDataSet
from stringskiplist import checkDataSet
from stringskiplist import searchInDataSet
from stringskiplist import releaseDataSet

a = '..\data\invertedindex\movie_processed_by_pkuseg.ii';
dataset = getDataSet(a)
searchInDataSet("('A' OR 'B')","1.txt",dataset)
releaseDataSet(dataset)