# convert .csv to .root
run
```
g++ -v `root-config --cflags --libs` GL840_make_ttree.cpp -o GL840_make_ttree
```
then
```
source GL_to_ttree.sh
```
