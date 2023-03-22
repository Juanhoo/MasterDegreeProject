//#include <iostream>
//#include <unordered_map>
//#include <vector>
//#include <string>
//#include <limits>
//#include <queue>
//#include <fstream>
//#include <sstream>
//#include <cmath>
//
//struct fuzzyTriangleVarriable {
//    double m_core;
//    double m_half_support;
//};
//
//struct ClusterDiscance
//{
//    std::vector<std::string> l, p;
//    fuzzyTriangleVarriable distance;
//};
//
//using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, fuzzyTriangleVarriable>>;
//
//fuzzyTriangleVarriable operator+(const fuzzyTriangleVarriable& first, const fuzzyTriangleVarriable& second) {
//    return fuzzyTriangleVarriable{ first.m_core + second.m_core, std::max(first.m_half_support, second.m_core) };
//}
//
//fuzzyTriangleVarriable operator-(const fuzzyTriangleVarriable& first, const fuzzyTriangleVarriable& second) {
//    return fuzzyTriangleVarriable{ first.m_core - second.m_core, std::max(first.m_half_support, second.m_core) };
//}
//
//fuzzyTriangleVarriable operator*(const fuzzyTriangleVarriable& first, const fuzzyTriangleVarriable& second) {
//    return fuzzyTriangleVarriable{ first.m_core * second.m_core, std::max(first.m_half_support, second.m_core) };
//}
//
//
//fuzzyTriangleVarriable square(const fuzzyTriangleVarriable& first)
//{
//    return first * first;
//}
//
//fuzzyTriangleVarriable sqrt(const fuzzyTriangleVarriable& first) {
//    return fuzzyTriangleVarriable{ std::sqrt(first.m_core) , first.m_half_support };
//
//}
//
//double operator>(const fuzzyTriangleVarriable& first, const fuzzyTriangleVarriable& second) {
//    // Roz³¹czne przypadki
//    if (first.m_core + first.m_half_support < second.m_core - second.m_half_support) {
//        return 1;
//    // A nachodzi na B
//    if (first.m_core + first.m_half_support > second.m_core - second.m_half_support && first.m_core + first.m_half_support < second.m_core + second.m_half_support)  {
//
//    }
//    }
//
//}
//
//
//
//class fuzzylinks {
//
//    std::vector<ClusterDiscance> complete_link(const std::unordered_map<std::string, std::unordered_map<std::string, fuzzyTriangleVarriable>>& distances) {
//        std::unordered_map<std::size_t, std::vector<std::string>> clusters;
//        std::vector<ClusterDiscance> dendogramData;
//        std::size_t cluster_id = 0;
//        // Initialize each point as its own cluster
//        for (const auto& point : distances) {
//            clusters[cluster_id++] = std::vector<std::string>{ point.first };
//        }
//        // Create a priority queue to track the minimum distance between clusters
//        using ClusterDistance = std::pair<std::size_t, std::size_t>;
//        auto cmp = [&](const ClusterDistance& c1, const ClusterDistance& c2) {
//            return distances.at(clusters[c1.first].front()).at(clusters[c1.second].front()).m_core -
//                distances.at(clusters[c1.first].front()).at(clusters[c1.second].front()).m_half_support >
//                distances.at(clusters[c2.first].front()).at(clusters[c2.second].front()).m_core -
//                distances.at(clusters[c2.first].front()).at(clusters[c2.second].front()).m_half_support;
//        };
//        std::priority_queue<ClusterDistance, std::vector<ClusterDistance>, decltype(cmp)> pq(cmp);
//        for (auto i_c = clusters.begin(); i_c != clusters.end(); ++i_c) {
//            const std::size_t i = i_c->first;
//            const auto& ci = i_c->second;
//            for (auto j_c = std::next(i_c); j_c != clusters.end(); ++j_c) {
//                const std::size_t j = j_c->first;
//                pq.emplace(i, j);
//            }
//        }
//        // Merge clusters until there is only one left
//        while (clusters.size() > 1) {
//            // Find the closest pair of clusters using the priority queue
//            std::size_t min_i = pq.top().first;
//            std::size_t min_j = pq.top().second;
//            fuzzyTriangleVarriable min_distance = distances.at(clusters[min_i].front()).at(clusters[min_j].front());
//            pq.pop();
//            if (clusters.find(min_i) == clusters.end() || clusters.find(min_j) == clusters.end()) {
//                // One of the clusters has already been merged, skip this pair
//                continue;
//            }
//            dendogramData.push_back(ClusterDiscance{ clusters[min_i], clusters[min_j], min_distance });
//
//            // Merge the two closest clusters            
//            clusters.erase(min_j);
//            // Update the priority queue with the new distances to the merged cluster
//            for (const auto& c : clusters) {
//                if (c.first != min_i) {
//                    pq.emplace(min_i, c.first);
//                }
//            }
//        }
//        return dendogramData;
//    }
//
//    std::vector<ClusterDiscance> single_link(const std::unordered_map<std::string, std::unordered_map<std::string, fuzzyTriangleVarriable>>& distances) {
//        std::unordered_map<std::size_t, std::vector<std::string>> clusters;
//        std::vector<ClusterDiscance> dendogramData;
//        std::size_t cluster_id = 0;
//        // Initialize each point as its own cluster
//        for (const auto& point : distances) {
//            clusters[cluster_id++] = std::vector<std::string>{ point.first };
//        }
//        // Create a priority queue to track the minimum distance between clusters
//        using ClusterDistance = std::pair<std::size_t, std::size_t>;
//        auto cmp = [&](const ClusterDistance& c1, const ClusterDistance& c2) {
//            const fuzzyTriangleVarriable& dist1 = distances.at(clusters[c1.first].front()).at(clusters[c1.second].front());
//            const fuzzyTriangleVarriable& dist2 = distances.at(clusters[c2.first].front()).at(clusters[c2.second].front());
//            return dist1.m_core - dist1.m_half_support > dist2.m_core - dist2.m_half_support;
//        };
//        std::priority_queue<ClusterDistance, std::vector<ClusterDistance>, decltype(cmp)> pq(cmp);
//        for (auto i_c = clusters.begin(); i_c != clusters.end(); ++i_c) {
//            const std::size_t i = i_c->first;
//            const auto& ci = i_c->second;
//            for (auto j_c = std::next(i_c); j_c != clusters.end(); ++j_c) {
//                const std::size_t j = j_c->first;
//                pq.emplace(i, j);
//            }
//        }
//        // Merge clusters until there is only one left
//        while (clusters.size() > 1) {
//            // Find the closest pair of clusters using the priority queue
//            std::size_t min_i = pq.top().first;
//            std::size_t min_j = pq.top().second;
//            const fuzzyTriangleVarriable& min_distance = distances.at(clusters[min_i].front()).at(clusters[min_j].front());
//            pq.pop();
//            if (clusters.find(min_i) == clusters.end() || clusters.find(min_j) == clusters.end()) {
//                // One of the clusters has already been merged, skip this pair
//                continue;
//            }
//            dendogramData.push_back(ClusterDiscance{ clusters[min_i], clusters[min_j], min_distance });
//
//            // Merge the two closest clusters            
//            clusters.erase(min_j);
//            // Update the priority queue with the new distances to the merged cluster
//            for (const auto& c : clusters) {
//                if (c.first != min_i) {
//                    pq.emplace(min_i, c.first);
//                }
//            }
//        }
//        return dendogramData;
//    }
//};