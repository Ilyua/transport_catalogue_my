#pragma once

#include <cassert>
#include "transport_catalogue.h"
#include "input_reader.h"
#include "geo.h"
#include "stat_reader.h"


const double EPS = 1e-6;

using namespace std;

//TransportCatalogue CreateTestCatalogue(){
//    TransportCatalogue catalogue;
//    catalogue.AddStop("Tolstopaltsevo", 55.611087, 37.20829);
//    catalogue.AddStop("Marushkino", 55.595884, 37.209755);
//    catalogue.AddStop("Rasskazovka", 55.632761, 37.333324);
//    catalogue.AddStop("Biryusinka", 55.581065, 37.64839);
//    catalogue.AddStop("Universam", 55.587655, 37.645687);
//    catalogue.AddStop("Prazhskaya", 55.611678, 37.603831);
//    catalogue.AddStop("Biryulyovo Passazhirskaya", 55.580999, 37.659164);
//    catalogue.AddStop("Biryulyovo Tovarnaya", 55.592028, 37.653656);
//
//
//    catalogue.AddStopDistance("Tolstopaltsevo", 3900, "Marushkino");
//    catalogue.AddStopDistance("Marushkino", 9900, "Rasskazovka");
//    catalogue.AddStopDistance("Marushkino", 100, "Marushkino");
//
//
//    catalogue.AddBus("One Point Route",{"Tolstopaltsevo"});
//    catalogue.AddBus("Circle Route Two Points",{"Tolstopaltsevo", "Marushkino", "Tolstopaltsevo"});
//    catalogue.AddBus("Circle Route Three",{"Tolstopaltsevo", "Marushkino", "Prazhskaya","Marushkino", "Tolstopaltsevo" });
//    catalogue.AddBus("Route Two Points One Unique",{"Tolstopaltsevo", "Tolstopaltsevo"});
//    catalogue.AddBus("Route Four Points Three Unique",{"Tolstopaltsevo", "Biryulyovo Passazhirskaya","Biryusinka", "Tolstopaltsevo" });
//    return catalogue;
//}
void TestAddStop() {
    TransportCatalogue catalogue;
    catalogue.AddStop("Tolstopaltsevo", 55.611087, 37.20829);
    catalogue.AddStop("Marushkino", 55.595884, 37.209755);
    catalogue.AddStop("Rasskazovka", 55.632761, 37.333324);
    catalogue.AddStop("Biryusinka", 55.581065, 37.64839);
    catalogue.AddStop("Universam", 55.587655, 37.645687);
    catalogue.AddStop("Prazhskaya", 55.611678, 37.603831);
    catalogue.AddStop("Biryulyovo Passazhirskaya", 55.580999, 37.659164);
    catalogue.AddStop("Biryulyovo Tovarnaya", 55.592028, 37.653656);

    assert(catalogue.GetStop("Tolstopaltsevo").coordinates == Coordinates({55.611087, 37.20829}));
    assert(catalogue.GetStop("Tolstopaltsevo").name == "Tolstopaltsevo");
    assert(catalogue.GetStop("Marushkino").coordinates == Coordinates({55.595884, 37.209755}));
    assert(catalogue.GetStops().size() == 8);
}

void TestAddBus(){
        TransportCatalogue catalogue;
        catalogue.AddStop("Tolstopaltsevo", 55.611087, 37.20829);
        catalogue.AddStop("Marushkino", 55.595884, 37.209755);
        catalogue.AddStop("Rasskazovka", 55.632761, 37.333324);

        catalogue.AddStopDistance("Tolstopaltsevo", 10, "Marushkino" );
        catalogue.AddStopDistance("Marushkino", 20, "Rasskazovka");



        catalogue.AddBus("Test 1",{"Tolstopaltsevo", "Marushkino", "Rasskazovka"});

        assert(catalogue.GetBuses().size() == 1);
        assert(catalogue.GetBus("Test 1").name == "Test 1");
        assert(catalogue.GetBus("Test 1").route_length == 30);

}
void TestAddDistances(){
    TransportCatalogue catalogue;
    catalogue.AddStop("Tolstopaltsevo", 55.611087, 37.20829);
    catalogue.AddStop("Marushkino", 55.595884, 37.209755);
    catalogue.AddStopDistance("Tolstopaltsevo", 12, "Tolstopaltsevo");
    catalogue.AddStopDistance("Tolstopaltsevo", 10, "Marushkino");
    catalogue.AddStopDistance("Marushkino", 100, "Tolstopaltsevo");

    assert(catalogue.GetDistance("Tolstopaltsevo", "Tolstopaltsevo") == 12);
    assert(catalogue.GetDistance("Tolstopaltsevo","Marushkino") == 10);
    assert(catalogue.GetDistance("Marushkino","Tolstopaltsevo") == 100);

}


// TESTING INPUT READER

void TestGetRequestType(){
    assert(input_reader::GetRequestType("Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka") == input_reader::RequestType::BUS);
    assert(input_reader::GetRequestType("Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye") == input_reader::RequestType::BUS);
    assert(input_reader::GetRequestType("Stop Tolstopaltsevo: 55.611087, 37.20829") == input_reader::RequestType::STOP);
}

void TestParseBusName(){
    assert(input_reader::ParseBusName("Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka") == "750");
    assert(input_reader::ParseBusName("Bus 75 0: Tolstopaltsevo - Marushkino - Rasskazovka") == "75 0");
    assert(input_reader::ParseBusName("Bus 7 50: Tolstopaltsevo - Marushkino - Rasskazovka") == "7 50");
}

void TestParseStopName(){
    assert(input_reader::ParseStopName("Stop Biryulyovo Zapadnoye Z: 55.574371, 37.6517") == "Biryulyovo Zapadnoye Z");
    assert(input_reader::ParseStopName("Stop Biryusinka: 55.581065, 37.64839") == "Biryusinka");
}

void TestParseBusStops(){

    vector<string> stops = input_reader::ParseBusStops("Bus 750: T olstopaltsevo - M1arushkino - Rasskazovka");

    assert(stops.size() == 5);
    assert(stops[0] == "T olstopaltsevo");
    assert(stops[1] == "M1arushkino");
    assert(stops[2] == "Rasskazovka");
    assert(stops[3] == "Marushkino");
    assert(stops[4] == "Tolstopaltsevo");

    stops = input_reader::ParseBusStops("Bus 75 0: T olstopaltsevo > M1arushkino > Rasskazovka");
    assert(stops.size() == 3);
    assert(stops[0] == "T olstopaltsevo");
    assert(stops[1] == "M1arushkino");
    assert(stops[2] == "Rasskazovka");

}

void TestParseStopCoordinates(){
    auto pair = input_reader::ParseStopCoordinates("Stop Biryulyovo Zapadnoye: 55.574371, 37.6517");
    assert(pair.first == 55.574371 and pair.second ==37.6517);
    pair = input_reader::ParseStopCoordinates("Stop Tolstopaltsevo: 55.574371, 37.6517");

    assert(pair.first == 55.574371 and pair.second == 37.6517);
}

void TestParseStopDistances(){
    auto result = input_reader::ParseStopDistances("Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to M arushkino, 1m to xwsxwx");
    assert(result[0].first == 3900 and result[0].second == "M arushkino");


    assert(result[1].first == 1 and result[1].second == "xwsxwx");
}


// TEST STAT READER
void TestStatGetRequestType(){
    assert(stat::GetRequestType("Stop Samara") == stat::RequestType::STOP);
    assert(stat::GetRequestType("Bus 75 1") == stat::RequestType::BUS);

}
void TestParseStatStopName(){
    assert(stat::ParseStopName("Stop Samara") == "Samara");
    assert(stat::ParseStopName("Stop Biryulyovo Zapadnoye") == "Biryulyovo Zapadnoye");
}
void TestParseStatBusName(){

    assert(stat::ParseBusName("Bus 751") == "751");
    assert(stat::ParseBusName("Bus 75 1") == "75 1");
}

void TestPrintRouteInfo(){
    {
        stringstream s;
        stat::PrintRouteInfo("na me", 1, 0, 0, 0, s);
        assert(s.str() == "Bus na me: 1 stops on route, 0 unique stops, 0 route length, 0 curvature\n");
    }
//    {
//        stringstream s;
//        stat::PrintRouteInfo("na me", 0, 0, 0, s);
//        assert(s.str() == "Bus na me: not found\n");
//    }
}
void TestPrintStopInfo(){

    {
        vector<string> buses({"a","b"});
        stringstream s;
        stat::PrintStopInfo("na me", buses,s);
        assert(s.str() == "Stop na me: buses a b\n");
    }
    {
        vector<string> buses;
        stringstream s;
        stat::PrintStopInfo("na me", buses,s);
        assert(s.str() == "Stop na me: no buses\n");
    }

}



void RunTests(){
    TestAddStop();
    TestAddDistances();
    TestAddBus();

//    TransportCatalogue catalogue = CreateTestCatalogue();
//    TestResponces(catalogue);

//    TestGetRequestType();
//    TestParseBusName();
//    TestParseStopName();
//    TestParseBusStops();
//    TestParseStopCoordinates();
//    TestParseStopDistances();
//
//
//    TestStatGetRequestType();
//    TestParseStatStopName();
//    TestParseStatBusName();
//    TestPrintRouteInfo();
//    TestPrintStopInfo();
    std::cout<<"ALL TESTS PASSED"<<std::endl;
}

