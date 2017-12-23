#include<iostream>
#include<malloc.h>
#include"treeCount.h"
#include"sds.h"
#define INIT_SIZE 10000

using namespace std;

char *treeCount() {
 tree t;
 int i = 0, j = 0;
 city *c = (city*)malloc(100 * sizeof(city));
 city *pre = c;
 FILE *fp;
 int i = 0, j = 0;
 sds *s = newSds(), *s1 = newSds();
 char *fileName, *cityName, *count, *treeName;
 cout << "Please input the name of the file:" << endl;
 cin >> fileName;
 if ((fp = fopen("fileName", "r")) == NULL) {
  cout << "ERROR:can't open  the file\n" << endl;
  exit(0);
 }
 while (!feof(fp)) {
  fscanf(fp, "%s %s %d", cityName, treeName, count);
  for (tree::iterator it = t.begin(); it != t.end(); it++) {
   if (treeName == (*it).first) {
    for (city::iterator iter = ((*it).second).begin(); iter != ((*it).second).end(); (*it.second)++) {
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

