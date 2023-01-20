# convert .csv to .root
run
```
g++ -v `root-config --cflags --libs` GL840_make_ttree.cpp -o GL840_make_ttree
g++ -v `root-config --cflags --libs` GL840_fiberQC.cpp -o GL840_fiberQC
g++ -v `root-config --cflags --libs` GL840_root_to_2Dhist.cpp -o GL840_root_to_2Dhist
g++ -v `root-config --cflags --libs` GL840_tree_to_histogram.cpp -o GL840_tree_to_histogram
```
and give 744 permisson to all executables and .sh files.
Then
```
source GL_to_ttree.sh
```
