﻿// TryoutMastersDegreeProject.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, double> templateInside;
typedef std::unordered_map<std::string, templateInside> templateMap;


struct distance {
    std::string pointName;
    double disctance;
};



namespace std {
    
    template<>
    struct hash<templateMap> {
        
        size_t operator()(const templateMap& key) {
            //return hash<std::string>()(templateInside);
        }
        
    };

}



int main()
{
    std::cout << "Hello World!\n";

    templateMap points;
    templateInside dist;
    std::string name = "test";
    std::string zew = "main";

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            dist.insert(templateInside::value_type(name, rand() % 25));
            name += 't';
        }
        points.insert(templateMap::value_type(zew, dist));
        name = "test";
        zew += 'w';
    }

    for (templateMap::const_iterator it = points.begin(); it != points.end(); ++it) {
        for (templateInside::const_iterator iter = it->second.begin(); iter != it->second.end(); ++iter) {
            std::cout << it->first << "\t" << iter->first << "\t" << iter->second << std::endl;
        }

    }




}