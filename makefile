.RECIPEPREFIX = >
test : test.c
> gcc test.c; ./a.out; rm ./a.out;

listTest : src/contacts/list.c src/contacts/list.h
> gcc src/contacts/list.c src/contacts/sds.c -o listTest; ./listTest; rm ./listTest;

sdsTest : src/contacts/sds.c src/contacts/sds.h
> gcc src/contacts/sds.c -o sdsTest; ./sdsTest; rm ./sdsTest;

dictTest : src/contacts/dict.c src/contacts/dict.h
> gcc src/contacts/dict.c src/contacts/sds.c src/contacts/list.c -o dictTest; ./dictTest; rm ./dictTest;

contacts : 
> gcc src/contacts/server.c src/contacts/sds.c src/contacts/list.c src/contacts/account.c src/contacts/dict.c -o server1;

treeTest : 
> g++ src/treeCount/treeCount.cpp src/treeCount/sds.cpp -o treeCount -std=c++11; ./treeCount; rm ./treeCount;