#include "stat_reader.h"

using namespace std;


namespace stat{
        std::string ParseBusName(std::string line){
            return line.substr(line.find_first_of(" ") + 1);
        }
        std::string ParseStopName(std::string line){
            return line.substr(line.find_first_of(" ") + 1);
        }

        RequestType GetRequestType( std::string line){
                if (line.rfind("Stop", 0) == 0)
                    return RequestType::STOP;
                else if(line.rfind("Bus", 0) == 0)
                    return RequestType::BUS;
                else
                    return RequestType::STOP;
        }

        void PrintRouteInfo(std::string_view bus_name,
                            size_t count_stops,
                            size_t count_unique_stops,
                            double route_length, double curvature, std::ostream& cout){

            cout<<"Bus "<<bus_name<<": "<<count_stops<<" stops on route, "<<count_unique_stops<<std::setprecision(6)<<
                " unique stops, "<<route_length<<" route length"<<", "<<std::setprecision(6)<< curvature<<" curvature"<<endl;

        }

    }

