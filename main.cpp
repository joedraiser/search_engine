#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

int main()
{
    nlohmann::json films, film;

    film["origin"]="USA";
    film["created"]=2003;
    film["studio"]="Wiseau-Films";
    film["writer"]="Tommy Wiseau";
    film["cast"]={{"Tommy Wiseau", "Johnny"}, {"Juliette Danielle", "Lisa"},{"Greg Sestero", "Mark"}};
    films["the Room"]=film;

    film["origin"]="USA";
    film["created"]=1975;
    film["studio"]="United Artists";
    film["writer"]="Richard Chew";
    film["cast"]={{"Jack Nicholson","Randle Patrick \"R.P.\" McMurphy"}, {"Louise Fletcher", "Nurse Mildred Ratched"},{"Will Sampson", "\"Chief\" Bromden"}};
    films["One Flew Over the Cuckoo's Nest"]=film;

    film["origin"]="Russia";
    film["created"]=1986;
    film["studio"]="Lenfilm";
    film["writer"]="Leda Semyonova";
    film["cast"]={{"Yevgeniy Yevstigneyev","Professor Philipp Philippovich Preobrazhensky"}, {"Boris Plotnikov","Dr. Ivan Arnoldovich Bormental, the professor's assistant"},{"Vladimir Tolokonnikov","Polygraph Polygraphovich Sharikov"}};
    films["Heart of a Dog"]=film;

    film["origin"]="USA";
    film["created"]=1997;
    film["studio"]="Paramount Pictures";
    film["writer"]="Conrad Buff";
    film["cast"]={{"Leonardo DiCaprio","Jack Dawson"},{"Kate Winslet","Rose DeWitt Bukater"},{"Billy Zane","Caledon Hockley"}};
    films["Titanic"]=film;

    film["origin"]="USA";
    film["created"]=1941;
    film["studio"]="RKO Radio Pictures";
    film["writer"]="Orson Welles";
    film["cast"]={{"Joseph Cotten","Jedediah Leland"}, {"Dorothy Comingore","Susan Alexander Kane"},{"Agnes Moorehead","Mary Kane"}};
    films["Citizen Kane"]=film;

    std::ofstream json_file("films.json");

    json_file << films;

    json_file.close();


    std::ifstream films_file("films.json");
    nlohmann::json films_json;
    films_file >> films_json;

    films_file.close();

    std::string actor_name;

    std::cout << "Input actor's name to search: ";
    std::getline(std::cin, actor_name);

    for(auto film_it=films_json.begin();film_it!=films_json.end();film_it++)
    {
        for(auto actor_it=(*film_it)["cast"].begin();actor_it!=(*film_it)["cast"].end();actor_it++)
        {
            if(actor_it.key().find(actor_name)!= std::string::npos)
            {
                std::cout << "He/She was acting in " << film_it.key() << " as " << *actor_it << std::endl;
            }
        }
    }

    return 0;
}
