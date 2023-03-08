#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <limits>
#include <queue>
#include <fstream>
#include <sstream>
#include <cmath>


template <class T>
std::ostream & operator<< (std::ostream & ss, const std::vector<T> & w)
{
    ss << "(size == " << w.size() << ") ";
    ss << "[ ";
    for (const auto & n : w)
        ss << n << " ";
    ss << "]";
    return ss;
}

using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, double>>;


struct ClusterDiscance
{
    std::vector<std::string> l, p;
    double distance;
};

std::ostream & operator<< (std::ostream & ss, const ClusterDiscance & cd)
{
    ss << "{ cluster distance:" << std::endl;
    ss << "  d(l, p) == " << cd.distance << std::endl;
    ss << "  l == " << cd.l << std::endl;
    ss << "  p == " << cd.p << std::endl;
    ss << "} ";

    return ss;
}

// Priority queue includes track of the minimum distance between clusters rather than iterating trough all pairs -> O(n^3) to O(n^2logn)

class links_queue 
{
// using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, double>>;

    public:
    std::vector<ClusterDiscance> single_link(const DistanceMatrix& distances) 
    {
        std::unordered_map<std::size_t, std::vector<std::string>> clusters;
        std::size_t cluster_id = 0;
        std::vector<ClusterDiscance> dendogramData;
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
            double min_distance = distances.at(clusters[min_i].front()).at(clusters[min_j].front());
            pq.pop();
            if (clusters.find(min_i) == clusters.end() || clusters.find(min_j) == clusters.end()) {
                // One of the clusters has already been merged, skip this pair
                continue;
            }
            dendogramData.push_back(ClusterDiscance{ clusters[min_i], clusters[min_j], min_distance });
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
        return dendogramData;
    }

    ////////////////////// Complete link 
// using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, double>>;

public:
    std::vector<ClusterDiscance> complete_link(const std::unordered_map<std::string, std::unordered_map<std::string, double>>&distances) {
        std::unordered_map<std::size_t, std::vector<std::string>> clusters;
        std::vector<ClusterDiscance> dendogramData;
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
            double min_distance = distances.at(clusters[min_i].front()).at(clusters[min_j].front());
            pq.pop();
            if (clusters.find(min_i) == clusters.end() || clusters.find(min_j) == clusters.end()) {
                // One of the clusters has already been merged, skip this pair
                continue;
            }
            dendogramData.push_back(ClusterDiscance{ clusters[min_i], clusters[min_j], min_distance });

            // Merge the two closest clusters            
            clusters.erase(min_j);
            // Update the priority queue with the new distances to the merged cluster
            for (const auto& c : clusters) {
                if (c.first != min_i) {
                    pq.emplace(min_i, c.first);
                }
            }
        }
        return dendogramData;
    }
    
}; 

class links 
{
// using DistanceMatrix = std::unordered_map<std::string, std::unordered_map<std::string, double>>;

    std::vector<ClusterDiscance> results;

public: 
    std::vector<ClusterDiscance> complete_link(const DistanceMatrix& distances) 
    {
        std::vector<ClusterDiscance> dendogram_data;
        std::unordered_map<std::size_t, std::vector<std::string>> clusters;
        std::size_t cluster_id = 0;
        // Initialize each point as its own cluster
        for (const auto& point : distances) 
        {
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
            // Update results vector for dendogram creator 
            dendogram_data.push_back({ clusters[max_i], clusters[max_j], max_distance });

            // Merge the two closest clusters
            clusters[max_i].insert(clusters[max_i].end(), clusters[max_j].begin(), clusters[max_j].end());
            clusters.erase(max_j);
        }
        return dendogram_data;
    }
};




class punkt 
{
public:
    std::vector<double> iksy;
};

std::ostream & operator<< (std::ostream & s, const punkt & p)
{
    for (const auto & x : p.iksy)
        s << x << " ";
    return s;
}

std::ostream & operator<< (std::ostream & s, const std::vector<punkt> & pp)
{
    for (const auto & p : pp)
        s << "(" << p << ")" << std::endl;
    return s;
}

std::vector<punkt> wczytaj(const std::string & nazwa)
{
    std::vector<punkt> punkty;
    std::ifstream plik (nazwa);
    
    if (plik)
    {
        std::string linia;
        
        while (std::getline(plik, linia))
        {
            punkt p;
            std::stringstream s;
            s << linia;
            double wartosc;
            while (s >> wartosc)
                p.iksy.push_back(wartosc);
            if (not p.iksy.empty())
                punkty.push_back(p);
        }
    }
    
    return punkty;
}

double euklides (const punkt & l, const punkt & p)
{
	double suma {0};
	double roznica;
	auto size = l.iksy.size();
	for (std::size_t i = 0; i < size; i++)
	{
		roznica = l.iksy[i] - p.iksy[i];
		suma += roznica * roznica;
	}
	return std::sqrt(suma);
}

DistanceMatrix wylicz_odleglosci (const std::vector<punkt> & punkty)
{
    DistanceMatrix dd; 
    
    auto size = punkty.size();
    for (std::size_t i = 0; i < size; i++)
    {
        for (std::size_t j = i + 1; j < size; j++)
        {
            auto d = euklides(punkty[i], punkty[j]);
            auto et_i = "[" + std::to_string(i) + "]";
            auto et_j = "[" + std::to_string(j) + "]";
            dd[et_i][et_j] = d;
            dd[et_j][et_i] = d;
        }
    }
    
    return dd;
}

std::ostream & operator<< (std::ostream & s, const std::unordered_map<std::size_t, std::vector<std::string>> & clusters)
{
    for (const auto& c : clusters) 
    {
        s << "Cluster " << c.first << ": ";
        for (const auto& p : c.second) {
            s << p << " ";
        }
        s << std::endl;
    }
    return s;
}



std::ostream & operator<< (std::ostream & s, const DistanceMatrix & dd)
{
    for (const auto & [etykieta, sasiedzi] : dd)
    {
        s << etykieta << "{";
        for (const auto & [sasiad, odleglosc] : sasiedzi)
            s << "(" << sasiad << " : " << odleglosc << ") ";
        s << "}" << std::endl;
    }
    return s;
}

int main() 
{
    {
        // Construct the distance matrix
        DistanceMatrix distances = {
            {"A", {{"A", 0.0}, {"B", 2.0}, {"C", 1.0}}},
            {"B", {{"A", 2.0}, {"B", 0.0}, {"C", 4.0}}},
            {"C", {{"A", 1.0}, {"B", 4.0}, {"C", 0.0}}},
        };

        std::cout << distances << std::endl;
        // Compute the complete-link clustering
        links complete_link;
        
        auto clusters = complete_link.complete_link(distances);

        std::cout << clusters.size() << std::endl;
        std::cout << clusters << std::endl;
   
         
    }
    
    
    std::cout << "=====================" << std::endl;
    {
        std::vector<std::string> nazwy { 
                       "10.link",  "20.link",   "50.link",
                      "100.link", "200.link",  "500.link",
                   //  "1000.link"
        };
        
        for (const auto & nazwa : nazwy)
        {
            std::cout << "plik: " << nazwa << "\t" << std::flush;
            auto punkty = wczytaj(nazwa);
            auto distances = wylicz_odleglosci(punkty);
            {
                std::cout << "complete_link" << "\t" << std::flush;
                links complete_link;
                auto clusters_complete = complete_link.complete_link(distances);
                std::ofstream plik (nazwa + "-complete-link.out");
                if (plik)
                    plik << clusters_complete;
            }
            {
                links_queue linkage;
                /*
                // Odkomentowanie tego powoduje
                // make: *** [makefile:32: release] Naruszenie ochrony pamięci (zrzut pamięci)
                {
                    std::cout << "complete_link_queue" << "\t" << std::flush;
                    auto clusters_complete = linkage.complete_link(distances);
                    std::ofstream plik (nazwa + "-complete-link-queue.out");
                    if (plik)
                        plik << clusters_complete;
                }
                */
                /*
                // Odkomentowanie tego powoduje
                // make: *** [makefile:32: release] Naruszenie ochrony pamięci (zrzut pamięci)
                {
                    std::cout << "single_link_queue" << "\t" << std::flush;
                    auto clusters_single = linkage.single_link(distances);
                    std::ofstream plik (nazwa + "-single-link-queue.out");
                    if (plik)
                        plik << clusters_single;
                }
                */

            }
            
        }
    }
     
    return 0;
}
