#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <regex>

namespace input_reader{
        std::vector<std::string> tokenize(std::string line , std::string del);
        enum class RequestType{
            BUS,
            STOP
        };
        std::string ParseBusName(std::string line);
        std::string ParseStopName(std::string line);
        std::vector<std::pair<double, std::string>> ParseStopDistances(std::string line);
        std::pair<double,double> ParseStopCoordinates(std::string line);
        std::vector<std::string> ParseBusStops(std::string line);
        RequestType GetRequestType( std::string_view line);
}
