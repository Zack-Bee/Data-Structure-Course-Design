#include "treeCount.h"


string treeCount(char *path) {
    string s = "{", s1 = "";
    tree t;
    int count, i , j ,p,q,n;
    char cityName[100], treeName[100];
    char str1[100], str2[100];
    memset(str1, 0, 100);
    memset(str2, 0, 100);
    FILE *fp = fopen(path, "rb");
    while (!feof(fp)) {
        fscanf(fp, "%s %s %d", treeName, cityName, &count);
        j=0;
        for (tree::iterator it = t.begin(); it != t.end(); it++) {
            if (treeName == (*it).first) {
                i=0;
                for (city::iterator iter = ((*it).second).begin();
                     iter != ((*it).second).end(); iter++) {
                    if (cityName == (*iter).first) {
                        (*iter).second += count;
                        i = 1;
                        break;
                    }
                }
                if (i == 0) {
                    ((*it).second).insert(make_pair(cityName, count));
                }
            j = 1;
            break;
            }
        }
        if (j == 0) {
            city c;
            c.insert(make_pair(cityName, count));
            t.insert(make_pair(treeName, c));
        }
    }
    p=0;
    for (tree::iterator it = t.begin(); it != t.end(); it++) {
        if(p==0){
               p=1;
           }else{
               s1.append(",");
           }
        s1.append("\"");
        s1.append((*it).first);
        s1.append("\":[");
        q=0;
        for (city::iterator iter = (*it).second.begin();
             iter != (*it).second.end(); iter++) {
                 if(q==0){
               q=1;
           }else{
               s1.append(",");
           }
                s1.append("{\"name\":\"");
            i = 0;
           
            while (1) {
                str1[i] = ((*iter).second % 10) + '0';
                i++;
                (*iter).second = (*iter).second / 10;
                if ((*iter).second == 0)
                    break;
            }
            for (j = 0, i = i - 1; i >= 0; i--, j++) {
                str2[j] = str1[i];
            }
            s1.append((*iter).first);
            s1.append("\",\"value\":\"");
            s1.append(str2);
            s1.append("\"}");
        }
        s1.append("]");
        s.append(s1);
        s1 = "";
    }
    s.append("}");
    return s;
}

// #define TREE_TEST
#ifdef TREE_TEST

void testTreeCount() {
    char path[] = "./treeTest.txt";
    string s = treeCount(path);
    string s1="{\"柏树\":[{\"name\":\"山西\",""\"value\":\"15\"},{\"name\":\"沈阳\",""\"value\":\"10\"}]}";
    string s2="{\"柏树\":[{\"name\":\"沈阳\",""\"value\":\"10\"},{\"name\":\"山西\",""\"value\":\"15\"}]}";
    std::cout<<s<<std::endl;
    if(s.compare(s1)==0||s.compare(s2)==0){
        std::cout<<"treeCount success"<<std::endl;
    }else{
        std::cout<<"treeCount fail"<<std::endl;
    }

}

int main(){
    testTreeCount() ;
    return 0;
}

#endif