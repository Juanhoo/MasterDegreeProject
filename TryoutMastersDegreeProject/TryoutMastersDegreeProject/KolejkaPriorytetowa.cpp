#include <unordered_map>
#include <string>
#include <queue>
#include <unordered_set>
#include <math.h>

// Priority queue includes track of the minimum distance between clusters rather than iterating trough all pairs -> O(n^3) to O(n^2logn)

class links_queue
{
    using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, double>>;

public:
    std::unordered_map<std::size_t, std::vector<std::string>> single_link(const DistanceMatrix& distances) {
        std::unordered_map<std::size_t, std::vector<std::string>> clusters;
        std::size_t cluster_id = 0;
        // Initialize each point as its own cluster
        for (const auto& point : distances) {
            clusters[cluster_id++] = std::vector<std::string>{ point.first };
        }
        // Create a priority queue to track the minimum distance between clusters
        using ClusterDistance = std::pair<std::size_t, std::size_t>;
        auto cmp = [&](const ClusterDistance& c1, const ClusterDistance& c2) {
            return distances.at(clusters[c1.first].front()).at(clusters[c1.second].front()) >
                distances.at(clusters[c2.first].front()).at(clusters[c2.second].front());
        };
        std::priority_queue<ClusterDistance, std::vector<ClusterDistance>, decltype(cmp)> pq(cmp);
        for (auto i_c = clusters.begin(); i_c != clusters.end(); ++i_c) {
            const std::size_t i = i_c->first;
            const auto& ci = i_c->second;
            for (auto j_c = std::next(i_c); j_c != clusters.end(); ++j_c) {
                const std::size_t j = j_c->first;
                pq.emplace(i, j);
            }
        }
        // Merge clusters until there is only one left
        while (clusters.size() > 1) {
            // Find the closest pair of clusters using the priority queue
            std::size_t min_i = pq.top().first;
            std::size_t min_j = pq.top().second;
            pq.pop();
            if (clusters.find(min_i) == clusters.end() || clusters.find(min_j) == clusters.end()) {
                // One of the clusters has already been merged, skip this pair
                continue;
            }
            // Merge the two closest clusters
            clusters[min_i].insert(clusters[min_i].end(), clusters[min_j].begin(), clusters[min_j].end());
            clusters.erase(min_j);
            // Update the priority queue with the new distances to the merged cluster
            for (const auto& c : clusters) {
                if (c.first != min_i) {
                    pq.emplace(min_i, c.first);
                }
            }
        }
        return clusters;

    }

    ////////////////////// Complete link 
// using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, double>>;

public:
    std::unordered_map<std::size_t, std::vector<std::string>> complete_link(const std::unordered_map<std::string, std::unordered_map<std::string, double>>& distances) {
        std::unordered_map<std::size_t, std::vector<std::string>> clusters;
        std::size_t cluster_id = 0;
        // Initialize each point as its own cluster
        for (const auto& point : distances) {
            clusters[cluster_id++] = std::vector<std::string>{ point.first };
        }
        // Create a priority queue to track the minimum distance between clusters
        using ClusterDistance = std::pair<std::size_t, std::size_t>;
        auto cmp = [&](const ClusterDistance& c1, const ClusterDistance& c2) {
            return distances.at(clusters[c1.first].front()).at(clusters[c1.second].front()) >
                distances.at(clusters[c2.first].front()).at(clusters[c2.second].front());
        };
        std::priority_queue<ClusterDistance, std::vector<ClusterDistance>, decltype(cmp)> pq(cmp);
        for (auto i_c = clusters.begin(); i_c != clusters.end(); ++i_c) {
            const std::size_t i = i_c->first;
            const auto& ci = i_c->second;
            for (auto j_c = std::next(i_c); j_c != clusters.end(); ++j_c) {
                const std::size_t j = j_c->first;
                pq.emplace(i, j);
            }
        }
        // Merge clusters until there is only one left
        while (clusters.size() > 1) {
            // Find the closest pair of clusters using the priority queue
            std::size_t min_i = pq.top().first;
            std::size_t min_j = pq.top().second;
            pq.pop();
            if (clusters.find(min_i) == clusters.end() || clusters.find(min_j) == clusters.end()) {
                // One of the clusters has already been merged, skip this pair
                continue;
            }
            // Merge the two closest clusters
            clusters[min_i].insert(clusters[min_i].end(), clusters[min_j].begin(), clusters[min_j].end());
            clusters.erase(min_j);
            // Update the priority queue with the new distances to the merged cluster
            for (const auto& c : clusters) {
                if (c.first != min_i) {
                    pq.emplace(min_i, c.first);
                }
            }
        }
        return clusters;
    }

};


/*
double is_less (const fuzzyTriangleVarriable& first, const fuzzyTriangleVarriable& second)
{
    ///@todo jeżeli w ogóle będzie potrzebne
    return -1.0;
}
*/

////////////////////////////////////////////////// operatory //////////////////////////////////////////////////////////////////


