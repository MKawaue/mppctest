# convert .csv to .root
run
```
g++ -v `root-config --cflags --libs` GL840_make_ttree.cpp -o GL840_make_ttree
```
then
```
source GL_to_ttree.sh <filename>
```

# make histogram
run
```
g++ -v `root-config --cflags --libs` GL840_tree_to_histogram.cpp -o GL840_tree_to_histogram
```
then
```
GL840_tree_to_histogram <datafile> <pedestalfile> <calibfile> <calibpedfile>
```
