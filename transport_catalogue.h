#pragma once

#include <deque>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>
#include <algorithm>

#include "geo.h"



struct Bus;

struct Stop{
    std::string name;
    Coordinates coordinates;
    std::unordered_set<Bus*> buses;

};
struct Bus{
    std::string name;
    std::deque<Stop*> stops;
    size_t count_unique_stops;
    double route_length;
    double curvature;
};


class TransportCatalogue {
public:

    void AddBus(std::string bus_name, std::vector<std::string> stops_names);
    void AddStop(std::string stop_name, double x, double y);
    void AddStopDistance(std::string_view, double distance, std::string_view stop_to);
    double GetDistance(std::string_view stop_from_name, std::string_view stop_to_name);
    Stop GetStop(std::string_view stop_name) const;
    Bus GetBus(std::string_view stop_name) const;
    std::deque<Bus> GetBuses();
    std::deque<Stop> GetStops();
private:
    struct PairStopHasher {
        size_t operator()(const std::pair<Stop*,Stop*>& pair) const {
            return hasher(pair.first) + 43 * hasher(pair.second);
        }
        std::hash<const void*> hasher;
    };
    std::deque<Bus> buses_;
    std::deque<Stop> stops_;
    std::unordered_map<std::string_view, Stop *> name_to_stop;
    std::unordered_map<std::string_view, Bus *> name_to_bus;
    std::unordered_map<std::pair<Stop*, Stop*>, double, PairStopHasher> distances_;




};
