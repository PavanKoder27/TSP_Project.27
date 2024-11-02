#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;

double totalDistance(const vector<pair<int, int>>& cities, const vector<int>& tour) {
    double distance = 0.0;
    for (int i = 0; i < tour.size() - 1; i++) {
        int city1 = tour[i];
        int city2 = tour[i + 1];
        distance += sqrt(pow(cities[city1].first - cities[city2].first, 2) + pow(cities[city1].second - cities[city2].second, 2));
    }
    distance += sqrt(pow(cities[tour.back()].first - cities[tour.front()].first, 2) + pow(cities[tour.back()].second - cities[tour.front()].second, 2));
    return distance;
}

vector<int> simulatedAnnealing(vector<pair<int, int>>& cities) {
    int n = cities.size();
    vector<int> currentTour(n);
    for (int i = 0; i < n; i++) currentTour[i] = i;
    double currentDistance = totalDistance(cities, currentTour);

    double temperature = 10000.0;
    double coolingRate = 0.999;
    srand(time(NULL)); 

    while (temperature > 1) {
        vector<int> newTour = currentTour;
        int city1 = rand() % n;
        int city2 = (city1 + 1 + rand() % (n - 1)) % n;
        swap(newTour[city1], newTour[city2]);

        double newDistance = totalDistance(cities, newTour);
        double deltaDistance = newDistance - currentDistance;

        if (deltaDistance < 0 || exp(-deltaDistance / temperature) > ((double) rand() / RAND_MAX)) {
            currentTour = newTour;
            currentDistance = newDistance;
        }

        temperature *= coolingRate;
    }

    return currentTour;
}

int main() {
    ifstream TSP("pma343.tsp");
    if (!TSP) {
        cerr << "Error in opening the given file" << endl;
        return 1;
    }

    vector<pair<int, int>> pairs;
    int first, second;

    while (TSP >> first >> second) {
        pairs.emplace_back(first, second);
    }

    TSP.close();

    vector<int> bestPath = simulatedAnnealing(pairs);

    for (int city : bestPath) {
        cout << city << " ";
    }
    cout << endl;

    double bestDistance = totalDistance(pairs, bestPath);
    cout << "Best distance : " << bestDistance << endl;

    return 0;
}