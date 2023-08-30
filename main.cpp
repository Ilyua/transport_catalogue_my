#include <fstream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
//#include "tests.h"
// TODO request struct, distances container, stop buses filed in stop, delete 2 in naming
using namespace std;



void ProcessInputStream(TransportCatalogue& catalogue, std::istream& stream, size_t count) {
    using namespace input_reader;
    vector<string> stop_lines;
    vector<string> bus_lines;
    string buf_line;
    for (size_t i = 1; i<= count ; ++i ){
        getline(stream, buf_line);
        if (GetRequestType(buf_line) == RequestType::STOP)
            stop_lines.emplace_back(move(buf_line));//(GetRequestType(buf_line) == RequestType::BUS)
        else 
            bus_lines.emplace_back(move(buf_line));
    }

    for (string s:stop_lines){
        auto coordinates = ParseStopCoordinates(s);
        catalogue.AddStop(std::string(ParseStopName(s)), coordinates.first, coordinates.second );
    }


    for (string s: stop_lines){
        string curr_stop_name  = ParseStopName(s);
        auto curr_distances = ParseStopDistances(s);
        for (auto [distance, stop_to_name]: curr_distances){
//            std::cout<<"Trying to add distance"<<curr_stop_name<<"/"<<distance<<"/"<<stop_to_name<<endl;
            catalogue.AddStopDistance(curr_stop_name, distance, stop_to_name);
        }

    }
    for (string s:bus_lines){
        auto stops = ParseBusStops(s);
        catalogue.AddBus(ParseBusName(s), stops);
    }

}

void MakeRequest(TransportCatalogue& catalogue, std::string line, std::ostream& output_stream){
    using namespace stat;
    RequestType type = GetRequestType(line);
    if (type == RequestType::BUS) {
        string bus_to_find = ParseBusName(line);
        try{
            Bus bus = catalogue.GetBus(bus_to_find );
            stat::PrintRouteInfo(bus.name, bus.stops.size(),
                                         bus.count_unique_stops,
                                         bus.route_length,
                                         bus.curvature, output_stream );
        }
        catch (...){
            output_stream<<"Bus "<<bus_to_find<<": not found"<<endl;
        }

    }else if (type == RequestType::STOP) {
        string stop_to_find = stat::ParseStopName(line);
        try{
            Stop stop  = catalogue.GetStop(stop_to_find);
            vector<string> buses_on_stop;
            for (auto s: stop.buses){
                buses_on_stop.push_back(s->name);
            }
            sort(buses_on_stop.begin(), buses_on_stop.end());
            stat::PrintStopInfo<vector<string>>(stop.name, buses_on_stop, output_stream);
        }
        catch(...){
            output_stream<<"Stop "<<stop_to_find<<": not found"<<endl;
        }
    }



}

int main(){
//    RunTests();
    std::istream & input_stream = cin;
    std::ostream & output_stream = cout;

//    std::ifstream input_stream("/Users/geshma/Documents/CLionProjects/cpp-transport-catalogue/data/my_input.txt");
//    std::ifstream input_stream("/Users/geshma/Documents/CLionProjects/cpp-transport-catalogue/data/test.txt");
//    std::ifstream input_stream("/Users/geshma/Documents/CLionProjects/cpp-transport-catalogue/data/tsC_case1_input.txt");
//
//    std::ofstream output_stream("/Users/geshma/Documents/CLionProjects/cpp-transport-catalogue/output.txt");

//    std::ostream & output_stream = cout;
//    istringstream input_stream(
//            R"(13
//Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino
//Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino
//Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
//Bus 750: Tolstopaltsevo - Marushkino - Marushkino - Rasskazovka
//Stop Rasskazovka: 55.632761, 37.333324, 9500m to Marushkino
//Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
//Stop Biryusinka: 55.581065, 37.64839, 750m to Universam
//Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya
//Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya
//Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye
//Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
//Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
//Stop Prazhskaya: 55.611678, 37.603831
//6
//Bus 256
//Bus 750
//Bus 751
//Stop Samara
//Stop Prazhskaya
//Stop Biryulyovo Zapadnoye
//)"s);
//
   TransportCatalogue catalogue;

    string buf_string;
    getline(input_stream, buf_string);

    size_t count_inputs = stoi(buf_string);

    ProcessInputStream(catalogue, input_stream, count_inputs);

    getline(input_stream, buf_string);
    count_inputs = stoi(buf_string);
//    cout<<"START REQUESTING"<<endl;
    for (size_t i = 1; i<= count_inputs;++i){
        getline(input_stream, buf_string);
        MakeRequest(catalogue, buf_string, output_stream);
    }

//    input_stream.close();
//    output_stream.close();

}
