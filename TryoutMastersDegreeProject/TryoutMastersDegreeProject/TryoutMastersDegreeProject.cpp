#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <limits>

using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, double>>;

std::unordered_map<std::size_t, std::vector<std::string>> complete_link(const DistanceMatrix& distances) {
    std::unordered_map<std::size_t, std::vector<std::string>> clusters;
    std::size_t cluster_id = 0;
    // Initialize each point as its own cluster
    for (const auto& point : distances) {
        clusters[cluster_id++] = std::vector<std::string>{ point.first };
    }
    // Merge clusters until there is only one left
    while (clusters.size() > 1) {
        double max_distance = std::numeric_limits<double>::min();
        std::size_t max_i = 0, max_j = 0;
        // Find the closest pair of clusters
        for (const auto& i_c : clusters) {
            const std::size_t i = i_c.first;
            const auto& ci = i_c.second;
            for (const auto& j_c : clusters) {
                const std::size_t j = j_c.first;
                const auto& cj = j_c.second;
                if (i >= j) {
                    continue;
                }
                double distance = std::numeric_limits<double>::min();
                // Calculate the distance between each pair of points in the clusters
                for (const auto& p1 : ci) {
                    for (const auto& p2 : cj) {
                        distance = std::max(distance, distances.at(p1).at(p2));
                    }
                }
                // Update the maximum distance and indices if necessary
                if (distance > max_distance) {
                    max_distance = distance;
                    max_i = i;
                    max_j = j;
                }
            }
        }
        // Merge the two closest clusters
        clusters[max_i].insert(clusters[max_i].end(), clusters[max_j].begin(), clusters[max_j].end());
        clusters.erase(max_j);
    }
    return clusters;
}


int main() {
    // Construct the distance matrix
    DistanceMatrix distances = {
         {"A", {{"A", 0.0}, {"B", 2.0}, {"C", 1.0}}},
         {"B", {{"A", 2.0}, {"B", 0.0}, {"C", 4.0}}},
         {"C", {{"A", 1.0}, {"B", 4.0}, {"C", 0.0}}},
    };

    // Compute the complete-link clustering
    auto clusters = complete_link(distances);

    // Print the resulting clusters
    for (const auto& c : clusters) {
        std::cout << "Cluster " << c.first << ": ";
        for (const auto& p : c.second) {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}