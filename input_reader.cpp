#include "input_reader.h"
using namespace std::string_view_literals;
namespace input_reader{
        std::vector<std::string> tokenize(std::string s, std::string del) {
            std::vector<std::string> result;
            int start, end = -1 * del.size();
            do {
                start = end + del.size();
                end = s.find(del, start);
                result.push_back(s.substr(start, end - start));
            } while (end != -1);
            return result;
        }
        std::string ParseStopName(std::string line){

            std::regex r("Stop (.+):");
            std::smatch result;
            std::regex_search(line, result, r);
//            std::cout<<line<<"---"<< result.str(1)<< std::endl;
            return result.str(1);
        }
        std::vector<std::pair<double, std::string>> ParseStopDistances(std::string line){
            std::vector<std::pair<double, std::string>> result_vec;
            std::regex r("(\\d+)m to ([a-zA-Z0-9 ]*?)([,]|$)");
//            std::vector<std::string> vec = tokenize(line, ",");
//            for (int i = 2; i< vec.size();++i){
//                std::vector<std::string> buf_vec = tokenize(vec[i], " to ");
//                std::string s = buf_vec[0];
//                s = s.substr(0, s.find_first_of("m"));
//                result_vec.push_back(make_pair(stoi(s), buf_vec[1]));
//
//            }
//            std::smatch result;
            auto words_begin = std::sregex_iterator(line.begin(), line.end(), r);
            auto words_end = std::sregex_iterator();

            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                std::smatch match = *i;
                std::string match_str = match.str();
                result_vec.push_back(make_pair(stoi(match.str(1)), match.str(2)));
            }
            return result_vec;
        }

        std::pair<double,double> ParseStopCoordinates(std::string line) {
            std::regex    r = std::regex("\\d+\\.\\d+");

            auto words_begin = std::sregex_iterator(line.begin(), line.end(), r);
            auto words_end = std::sregex_iterator();
            std::pair<double, double> pair(0,0);
            std::vector<double> buf;
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                std::smatch match = *i;
                std::string match_str = match.str();
                buf.push_back(std::stod(match_str));
            }
            pair.first = buf[0];
            pair.second = buf[1];
            return pair;
        }

        std::string ParseBusName(std::string line){
            std::regex r("Bus (.*):");
            std::smatch result;
            std::regex_search(line, result, r);
            return result.str(1);
        }

        std::vector<std::string> ParseBusStops(std::string line) {
            line = line.substr(line.find_first_of(":"sv) + 2);
            std::vector<std::string> tokens;
            if (line.find(">"sv) != std::string::npos) {
                return tokenize(line, " > ");
            } else {
                tokens = tokenize(line, " - ");
                std::vector<std::string> new_tokens(tokens);
                for (size_t i = 0; i < tokens.size() - 1; ++i) {
                    new_tokens.push_back(tokens[tokens.size() - 2 - i]);
                }
                return new_tokens;
            }
        }

        RequestType GetRequestType(std::string_view line) {
            if (line.rfind("Stop"sv, 0) == 0) {
                return RequestType::STOP;
            } else {
                return RequestType::BUS;
            }
        }

    }


