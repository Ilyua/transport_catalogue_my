#pragma once


#include <iostream>
#include <iomanip>
namespace stat{
        enum class RequestType{
            BUS,
            STOP
        };

        std::string ParseBusName(std::string line);
        std::string ParseStopName(std::string line);
        RequestType GetRequestType( std::string line);

        void PrintRouteInfo(std::string_view bus_name,
                            size_t count_stops,
                            size_t count_unique_stops,
                            double route_length,double curvature, std::ostream& cout);


    template<typename BusContainer>
    void PrintStopInfo(std::string_view stop_name,
                       BusContainer buses, std::ostream& cout ){

        if (buses.size() == 0){
            cout<<"Stop "<<stop_name<<": no buses"<<std::endl;
        }else{
            cout<<"Stop "<<stop_name<<": buses";
            for (auto bus: buses){
                cout<<" "<<bus;
            }
            cout<<std::endl;
        }
    }
}