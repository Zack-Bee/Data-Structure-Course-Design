#include "sds.h"
#include <iostream>
#include <malloc.h>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, unsigned int> city;

typedef std::unordered_map<std::string, city> tree;

#define INIT_SIZE 10000


/**
 *     从path中读取文件, 树的分布转换为json格式存储在sds中
 *     @param path 文件的位置
 *     @return 返回存储了json的sds
 */
sds *treeCount(char *path);

sds *treeCount() {
    tree t;
    int i = 0, j = 0;
    city *c = (city *)malloc(100 * sizeof(city));
    city *pre = c;
    FILE *fp;
    int i = 0, j = 0;
    sds *s = newSds(), *s1 = newSds();
    char *fileName, *cityName, *count, *treeName;
    // cout << "Please input the name of the file:" << endl;
    // cin >> fileName;
    // if ((fp = fopen("fileName", "r")) == NULL) {
    // cout << "ERROR:can't open  the file\n" << endl;
    // exit(0);
    // }
    while (!feof(fp)) {
        fscanf(fp, "%s %s %d", cityName, treeName, count);
        for (tree::iterator it = t.begin(); it != t.end(); it++) {
            if (treeName == (*it).first) {
                for (city::iterator iter = ((*it).second).begin();
                     iter != ((*it).second).end(); (*it.second)++) {
                    if (cityName == (*iter).first) {
                        (*iter).second += count;
                        i = 1;
                        break;
                    }
                }
                if (i == 0) {
                    ((*it).second).insert(make_pair(cityName, count));
                }
            }
            j = 1;
            break;
        }
        if (j == 0) {
            pre.insert(make_pair(cityName, count));
            t.insert(make_pair(treeName, pre));
            pre = pre->next;
        }
    }
    for (tree::iterator it = t.begin(); it != t.end(); t++) {
        ;
        s1 = sdsStrcatStr(s1, (*it).first);
        s1 = sdsStrcatStr(s1, ((*it).second).first);
        s1 = sdsStrcatStr(s1, ((*it).second).second);
        s = sdsStrcatSds(s, s1);
        clearSds(s1);
    }
    return (s->str);
}


#ifdef TREE_TEST

void testTreeCount() {
    const char *path = "./treeTest.txt";
    sds *s = treeCount(path);
    if (sdsCompareStr(("{\"柏树\":[{\"name\":\"沈阳\", "
                       "\"value\":\"10\"},{\"name\":\"山西\", "
                       "\"value\":\"15\"}]}") == 0) ||
        (sdsCompareStr(s, "{\"柏树\":[{\"name\":\"山西\", "
                          "\"value\":\"15\"},{\"name\":\"沈阳\", "
                          "\"value\":\"10\"}]}") == 0))
}

int main() {}

#endif