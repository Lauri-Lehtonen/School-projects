/* COMP.CS.110 Project 2: GIGS
 *
 * Projektin tarkoituksena on toteuttaa keikkakalenteri, josta
 * voi hakea tietoa, lisätä tietoa tai poistaa tietoa.
 *
 * Ohjelman kirjoittaja
 * Nimi: Lauri Lehtonen
 * Opiskelijanumero: 050023118
 * Käyttäjätunnus: txlale
 * E-Mail: lauri.er.lehtonen@tuni.fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 * Koodi toimii ku VR:n vessa :D
 * */


#include "gig_calendar_methods.hh"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>


// Farthest year for which gigs can be allocated
const std::string FARTHEST_POSSIBLE_YEAR = "2030";




// Casual split func, if delim char is between "'s, ignores it.
std::vector<std::string> split(const std::string& str, char delim = ';')
{
    std::vector<std::string> result = {""};
    bool inside_quotation = false;
    for ( char current_char : str )
    {
        if ( current_char == '"' )
        {
            inside_quotation = not inside_quotation;
        }
        else if ( current_char == delim and not inside_quotation )
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if ( result.back() == "" )
    {
        result.pop_back();
    }
    return result;
}

// Checks if the given date_str is a valid date, i.e. if it has the format
// dd-mm-yyyy and if it is also otherwise possible date.
bool is_valid_date(const std::string& date_str)
{
    std::vector<std::string> date_vec = split(date_str, '-');
    if ( date_vec.size() != 3 )
    {
        return false;
    }

    std::string year = date_vec.at(0);
    std::string month = date_vec.at(1);
    std::string day = date_vec.at(2);
    std::vector<std::string> month_sizes
            = { "31", "28", "31", "30", "31", "30",
                "31", "31", "30", "31", "30", "31" };

    if ( month < "01" or month > "12" )
    {
        return false;
    }
    if ( year < "0001" or year > FARTHEST_POSSIBLE_YEAR )
    {
        return false;
    }
    unsigned int year_int = stoi(year);
    bool is_leap_year = (year_int % 400 == 0) or
            (year_int % 100 != 0 and year_int % 4 == 0);
    if ( day >= "01" and
         (day <= month_sizes.at(stoi(month) - 1) or
          (month == "02" and is_leap_year and day <= "29")) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Kysyy komentoa ja jakaa sen vektoriksi, jota on helpompi käsitellä.
void ask_command(std::vector < std::string >& vector_command)
{
    std::string command;
    std::cout << "gigs> ";
    getline(std::cin, command);
    vector_command = split(command, ' ');
}

// Tarkistaa onko syötetyn inputin koko tarpeeksi suuri haluttuun funktioon.
bool valid_size(int size, int expected)
{
    if (size < expected)
    {
        std::cout << "Error: Invalid input." << std::endl;
        return false;
    }
    return true;
}

int main()
{
    //Initioi käytetyt tietorakenteet.
    std::map< std::string, std::vector< std::vector <std::string >>> gigs;
    std::vector< std::string > stages;
    std::vector< std::vector< std::string >> all_venue_reservations;

    //Pyytää ja avaa tiedoston, josta luetaan keikkatiedot tietorakenteeseemme.
    std::string filename = "";
    std::cout << "Give a name for input file: ";
    getline(std::cin, filename);
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "Error: File could not be read." << std::endl;
        return EXIT_FAILURE;
    }


    //siirtää tiedoston tiedot tietorakenteisiimme ja sulkee tiedoston.
    else
    {
        std::string row;
        while (getline(file, row))
        {
            std::vector< std::string > current_row = split(row,';');
            if (current_row.size() != 4 or std::count(current_row.begin(), current_row.end(), ""))
            {
                std::cout << "Error: Invalid format in file." << std::endl;
                return EXIT_FAILURE;
            }
            else if (!is_valid_date(current_row.at(1)))
            {
                std::cout << "Error: Invalid date." << std::endl;
                return EXIT_FAILURE;
            }
            else
            {

                std::vector< std::vector < std::string >> current_row_artist;
                gigs.insert({current_row.at(0), current_row_artist});
                std::string artist = current_row.at(0);
                stages.push_back(current_row.at(3));
                current_row.erase(current_row.begin());
                gigs[artist].push_back(current_row);
                all_venue_reservations.push_back(current_row);

            }
        }
        file.close();
    }

    for (auto artist : gigs)
    {
        std::vector< std::string > dates;
        for (auto gig : artist.second)
        {
            std::string date = gig.at(0);
            if (std::count(dates.begin(),dates.end(),date))
            {
                std::cout << "Error: Already exists." << std::endl;
                return EXIT_FAILURE;
            }
            dates.push_back(date);
        }
    }
    for (auto venue : all_venue_reservations)
    {
        if(std::count(all_venue_reservations.begin(), all_venue_reservations.end(),venue) > 1)
        {
            std::cout << "Error: Already exists." << std::endl;
            return EXIT_FAILURE;
        }
    }
    gig_calendar_methods data_structure(gigs);
    //kutsuu luokkaa, jossa tietorakenteeseen kohdistuvat metodit ovat toteutettu.



    while(true)
    {
        //pyytää käyttäjältä käskyä, muuttaa sen vektoriksi ja ottaa siitä kutsuttavan metodin.
        std::vector< std::string > command;
        ask_command(command);
        std::string method_to_call = command[0];
        for (auto & c : method_to_call) c = toupper(c);


        if (method_to_call == "QUIT")
        {
            return EXIT_SUCCESS;
        }

        if (method_to_call == "ARTISTS")
        {
            data_structure.print_artists();
        }

        if (method_to_call == "ARTIST")
        {

            if ( command.size() < 2)
            {
                std::cout << "Error: Invalid input." << std::endl;
            }
            else if (gigs.find(command.at(1)) == gigs.end())
            {
                std::cout << "Error: Not found." << std::endl;
            }
            else
            {
                data_structure.find_gigs(command.at(1));
            }
        }

        if (method_to_call == "STAGES")
        {
            data_structure.print_stages();
        }

        if (method_to_call == "STAGE")
        {
            if (valid_size(command.size(),2))
            {
                if (std::count(stages.begin(), stages.end(), command.at(1)))
                {
                    data_structure.performers_at_stage(command.at(1));
                }
                else
                {
                    std::cout << "Error: Not found." << std::endl;
                }
            }
        }

        if (method_to_call == "ADD_ARTIST")
        {
            if (valid_size(command.size(),2))
            {
                data_structure.add_artist(command.at(1));
                gigs[command.at(1)];
            }
        }
        if (method_to_call == "ADD_GIG")
        {
            if (valid_size(command.size(),5))
            {
                if (is_valid_date(command.at(2)))
                {
                    command.erase(command.begin());
                    data_structure.add_gig(command, all_venue_reservations);
                }
                else if (!gigs.count(command.at(1)))
                {
                    std::cout << "Error: Not found." << std::endl;
                }
                else
                {
                    std::cout << "Error: Invalid date." << std::endl;
                }
            }
        }
        if (method_to_call == "CANCEL")
        {
            if (valid_size(command.size(),3))
            {
                if (is_valid_date(command.at(2)))
                {
                    command.erase(command.begin());
                    data_structure.cancel_gig(command);
                }
                else if (!gigs.count(command.at(1)))
                {
                    std::cout << "Error: Not found." << std::endl;
                }
                else
                {
                    std::cout << "Error: Invalid date." << std::endl;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
