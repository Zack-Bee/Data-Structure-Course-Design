#ifndef TREE_COUNT_H
#define TREE_COUNT_H

#include <string.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>



using std::make_pair;
using std::string;

typedef std::unordered_map<std::string, unsigned int> city;

typedef std::unordered_map<std::string, city> tree;

#define INIT_SIZE 10000


/**
 *     使用hash统计path文件中的树木分布, 转化为json格式返回
 *     @param path 需要统计的树木分布文件
 */ 
string treeCount(char *path);

#endif