// TryoutMastersDegreeProject.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, double> templateInside;
typedef std::unordered_map<std::string, templateInside> templateMap;
typedef std::unordered_map<std::string, std::string> closestPointsTemplate;


int main()
{
    templateMap points;
    templateInside dist;
    std::string name = "test";
    std::string zew = "main";
    std::unordered_map<std::string, std::string> closestNeighboursMap;
    //      A   B   C   D   E
    // A    0   10  5   6   2
    // B   10   0   3   4   1
    // C    5   3   0   18  7
    // D    6   4   18  0   9
    // E    2   1   7   9   0 

    templateInside LA = { {"A", INT_MAX}, { "B", 10 } , { "C", 5 }, { "D", 6 }, { "E", 2 } };
    templateInside LB = { {"A", 10}, { "B", INT_MAX } , { "C", 3 }, { "D", 4 }, { "E", 1 } };
    templateInside LC = { {"A", 5}, { "B", 3 } , { "C", INT_MAX }, { "D", 18 }, { "E", 7 } };
    templateInside LD = { {"A", 6}, { "B", 4 } , { "C", 18 }, { "D", INT_MAX }, { "E", 9 } };
    templateInside LE = { {"A", 2}, { "B", 1 } , { "C", 7 }, { "D", 9 }, { "E", INT_MAX } };

    templateMap mainTable = { {"A", LA}, { "B", LB } , { "C", LC }, { "D", LD }, { "E", LE } };

    auto test = mainTable;
    //closestNeigbour[iter->second] << "closest od iter" <<

    std::unordered_map<std::string, std::string> closestPoints = { {"A","E"},{"B","E"},{"C","B"},{"D","B"},{"E","B"} };

    templateInside::const_iterator minimal = mainTable.begin()->second.begin(); // pkt [0][0]
    templateMap::const_iterator it = test.begin();
    templateInside::const_iterator iter = it->second.begin();
    for (auto coppy = it; coppy != test.end(); ++coppy) {
        for (auto inCoppy = iter; inCoppy != coppy->second.end(); ++inCoppy) {
            //std::cout << iter->second << "\t";
            auto candidate = closestPoints.find(coppy->first);
            std::string closestNeigbour = candidate->second;
            auto count = coppy->second.find(closestNeigbour);
            auto data = coppy->second.find(inCoppy->first);
            std::cout << "Data: " << data->second << std::endl;
            if (minimal->second > inCoppy->second) {
                std::cout << minimal->second << "<" << inCoppy->second << std::endl;
                minimal = inCoppy;
                std::cout << minimal->first << std::endl;  
            }
           
        }
        std::cout << "next stage" << std::endl;
        
    }

    auto considerBpoint = closestPoints.begin();
    std::string pointB;
    double distance = INT_MAX;
    for (auto closestIterator = considerBpoint; closestIterator != closestPoints.end(); ++closestIterator) {
        if (closestIterator->second == minimal->first) {
            auto candidate = it->second.find(closestIterator->second);
            if (minimal->second > candidate->second) {
                std::cout << "Minimal " << minimal->second << " candidate" << candidate->second << std::endl;
            }
        }

    }

    //    for (closestPointsTemplate::const_iterator it = closestPoints.begin(); it != closestPoints.end(); ++it) {
    //        std::cout << "From " << it->first << "is closest to " << it->second << std::endl;
    //    }
    //
    //
    //    for (templateMap::const_iterator it = test.begin(); it != test.end(); ++it) {
    //        for (templateInside::const_iterator iter = it->second.begin(); iter != it->second.end(); ++iter) {
    //            std::cout << iter->second << "\t";
    //            auto candidate = closestPoints.find(it->first);
    //            //it->second.find(candidate);
    //            //std::cout << candidate->first << '\t' << candidate->second << '\n';
    //            //std::cout << it->second[candidate];
    //            std::string closestNeigbour = candidate->second;
    //            auto count = it->second.find(closestNeigbour);
    //            std::cout << "" << iter->first << "Closest " << count->first << std::endl;
    //
    //        }
    //    }
    //    //for (templateMap::const_iterator it = mainTable.begin(); it != mainTable.end(); ++it) {
    //    //    std::cout << it->first << std::endl;
    //    //    auto candidate = closestPoints.find(it->first);
    //    //    //it->second.find(candidate);
    //    //    std::cout << candidate->first << '\t' << candidate->second << '\n';
    //    //    //std::cout << it->second[candidate];
    //    //    std::string closestNeigbour = candidate->second;
    //    //    auto count = it->second.find(closestNeigbour);
    //    //    //for (auto&iter = it->second; iter != it->second.end(); )
    //    //    for (auto& iter : it->second) {
    //    //        if (iter.second > count->second)
    //    //            std::cout << "Znaleziono" << std::endl;
    //    //        std::cout << it->first << " to " << iter.first << " takes " << iter.second << '\t';
    //    //        
    //    //    }  
    //    //    std::cout <<  std::endl;
    //
    //    //}
    //
    //    //for (int i = 0; i < 10; i++) {
    //    //    //closestNeighboursMap.insert(templateMap::value_type(name, zew));
    //    //    for (int j = 0; j < 10; j++) {
    //    //        dist.insert(templateInside::value_type(name, rand() % 25));
    //    //        name += 't';
    //    //    }
    //    //    points.insert(templateMap::value_type(zew, dist));
    //    //    name = "test";
    //    //    zew += 'w';
    //    //}
    //
    //    for (templateMap::const_iterator it = points.begin(); it != points.end(); ++it) {
    //        for (templateInside::const_iterator iter = it->second.begin(); iter != it->second.end(); ++iter) {
    //            //std::cout << it->first << "\t" << iter->first << "\t" << iter->second << std::endl;
    //        }
    //
    //    }
    //
    //    templateMap::const_iterator it = points.begin();
    //
    //
    //    while (points.size() > 1) {
    //
    //        for (templateMap::const_iterator it = points.begin(); it != points.end(); ++it) {
    //            auto minimalCandidate = closestNeighboursMap[it->first];
    //            int minimum = it->second.find(minimalCandidate)->second;
    //            //wyszukaj minimum dla aktualnego kandydata
    //            for (templateInside::const_iterator itin = it->second.begin(); itin != it->second.end(); ++itin) {
    //                if (minimum > itin->second) {
    //                    minimalCandidate = itin->first;
    //                    minimum = itin->second;
    //                }
    //            }
    //           // closestNeighboursMap[it->second] = minimalCandidate;
    //
    //        }
    //    }
    //
    //
    //}
    //
}
