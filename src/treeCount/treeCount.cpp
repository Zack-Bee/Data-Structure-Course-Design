#include "sds.h"
#include <iostream>
#include <string>
#include<vector>
#include <unordered_map>

using std::make_pair;
using std::string;

typedef std::unordered_map<std::string, unsigned int> city;

typedef std::unordered_map<std::string, city> tree;

#define INIT_SIZE 10000


char *treeCount(char *path) {
 string s= "",s1 ="";
    tree t;
    int count, i=0,j=0;
    char *cityName, *treeName;
    FILE *fp = fopen(path, "rb");
    while (!feof(fp)) {
        fscanf(fp, "%s %s %d", cityName, treeName, &count);
        for (tree::iterator it = t.begin(); it != t.end(); it++) {
            if (treeName == (*it).first) {
                for (city::iterator iter = ((*it).second).begin();
                     iter != ((*it).second).end(); ((*it).second)++) {
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
            city c;
            c.insert(make_pair(cityName, count));
            t.insert(make_pair(treeName, c));
        }
    }
    s=str1("{");
    for (tree::iterator it = t.begin(); it != t.end();it++) {
            s1.append("\"");
            s1.append((*it).first);
            s1.append("\":{");
        for(city::iterator iter=(*it).second.begin();iter!=(*it).second.end();iter++){
            s1.append( "\"");
            s1.append(((*it).second).first);
            s1.append("\":\"");
            s1.append(((*it).second).second);
            s1.append( "\",");
        }
        s1.append( "},");
        s.append(s1);
        s1="";
    }
    s.append("}");
    return s;
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