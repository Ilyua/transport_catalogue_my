
#include "transport_catalogue.h"


void TransportCatalogue::AddBus(std::string bus_name, std::vector <std::string> stops_names) {
    Bus bus_to_insert{bus_name, std::deque<Stop *>{}, 0, 0, 0};
    std::unordered_set<std::string_view> unique_stops;
    for (std::string& s: stops_names) {
        bus_to_insert.stops.push_back(name_to_stop.at(s));
        unique_stops.insert(s);
    }
    bus_to_insert.count_unique_stops = unique_stops.size();

    buses_.emplace_back(std::move(bus_to_insert));

    name_to_bus[std::string_view(buses_.back().name)] = &buses_.back();

    auto buf_bus = &buses_.back();
    for (std::string_view s: unique_stops) {
        name_to_stop.at(s)->buses.insert(buf_bus);
    }
    // Compute route distance
    double distance = 0;
    double distance_geo = 0;
    if (buses_.back().stops.size() >= 2){

        Stop* prev_stop = buses_.back().stops[0];
        for (size_t i = 1; i < buses_.back().stops.size(); ++i){
            Stop * stop = buses_.back().stops[i];
            distance_geo += ComputeDistance(prev_stop->coordinates, stop->coordinates);
            distance += GetDistance(prev_stop->name, stop->name);
            prev_stop = stop;
        }

    }else if(buses_.back().stops.size() == 1){
        Stop * stop = buses_.back().stops[0];
        try{
            distance += GetDistance(stop->name, stop->name);
        }catch (...){
            throw "No stop";
        }
    }

    buses_.back().route_length = distance;
    buses_.back().curvature = static_cast<double>(distance) / distance_geo;
}

void TransportCatalogue::AddStop(std::string stop_name, double x, double y) {
    Stop stop_to_insert({stop_name, {x, y}, {}});
    stops_.push_back(std::move(stop_to_insert));
    name_to_stop[std::string_view(stops_.back().name)] = &stops_.back();
}
void TransportCatalogue::AddStopDistance(std::string_view stop_from_name, double distance, std::string_view stop_to_name){
//    std::cout<<"DISTANCE FROM "<< stop_from_name <<"/"<<stop_to_name<<std::endl;
    Stop * stop_from = name_to_stop.at(stop_from_name);
    Stop * stop_to = name_to_stop.at(stop_to_name);
    distances_.insert({
            std::make_pair(stop_from, stop_to),
            distance});
}

Stop TransportCatalogue::GetStop(std::string_view stop_name) const{
    return *name_to_stop.at(stop_name);
}
Bus TransportCatalogue::GetBus(std::string_view stop_name) const{
    return *name_to_bus.at(stop_name);
}
double TransportCatalogue::GetDistance(std::string_view stop_from_name, std::string_view stop_to_name){
//    std::cout<<"DISTANCE FROM "<< stop_from_name <<"/"<<stop_to_name<<std::endl;
    Stop * stop_from = name_to_stop.at(stop_from_name);
    Stop * stop_to = name_to_stop.at(stop_to_name);
    try{
        return distances_.at(std::make_pair(stop_from, stop_to));
    }
    catch (...){
        try{
            return distances_.at(std::make_pair(stop_to, stop_from));
        }
        catch(...){
//            std::cout<<"DISTANCE FROM "<< stop_from_name <<"/"<<stop_to_name<<std::endl;

            throw "Distance not found";
        }
    }

}

std::deque<Bus> TransportCatalogue::GetBuses(){
    return buses_;
}
std::deque<Stop> TransportCatalogue::GetStops(){
    return stops_;
}





