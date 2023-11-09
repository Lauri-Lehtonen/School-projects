/* COMP.CS.110 Project 2: GIGS
 *
 * Luokka gig_calendar_methods, jonka on tarkoitus toimia rajapintana
 * tietorakenteemme ja käyttäjän välillä, luokka sisältää kaikkien komentojen
 * toteuttavat koodit ja niiden tukemiseen kaivatut funktiot.
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
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

gig_calendar_methods::gig_calendar_methods(const std::map< std::string, std::vector< std::vector <std::string >>>& gig):
    gigs_(gig)
{

}

void gig_calendar_methods::remove_duplicates(std::vector< std::vector< std::string >>& v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }
    v.erase(end, v.end());
}

void gig_calendar_methods::print_artists()
{
    std::vector < std::string > artists;
    for (auto const& gigs : gigs_)
    {
        artists.push_back(gigs.first);
    }
    std::sort(artists.begin(),artists.end());
    std::cout << "All artists in alphabetical order:" << std::endl;
    for (int i = 0; i < int(artists.size()); ++i)
    {
        std::cout << artists.at(i) << std::endl;
    }
}

void gig_calendar_methods::find_gigs(std::string artist)
{
    std::vector< std::vector < std::string >> gigs = gigs_.at(artist);
    std::vector< std::string > dates;
    for (auto x : gigs)
    {
        dates.push_back(x.at(0));
    }
    std::sort(dates.begin(),dates.end());

    std::cout << "Artist " << artist << " has the following gigs in the order they are listed:" << std::endl;
    for (auto date : dates)
    {
        for (auto gig : gigs)
        {
            if (gig.at(0) == date)
            {
                std::cout << " - "  << gig.at(0) << " : " << gig.at(1) << ", " << gig.at(2) << std::endl;
            }
        }
    }
}

void gig_calendar_methods::print_stages()
{
    std::vector < std::vector < std::string >> stages;
    std::cout << "All gig places in alphabetical order:" << std::endl;
    for (auto& x : gigs_)
    {
        for (int i = 0; i < int(x.second.size()); ++i)
        {
            stages.push_back({x.second.at(i).at(1),x.second.at(i).at(2)});
        }
    }
    remove_duplicates(stages);
    std::sort(stages.begin(), stages.end());
    for (auto x : stages)
    {
        std::cout << x.at(0) << ", " << x.at(1) << std::endl;
    }
}

int gig_calendar_methods::performers_at_stage(std::string stage)
{
    std::vector< std::string > artists;
    for (const auto& elements : gigs_)
    {
        for (const auto& gigs : elements.second)
        {
            if (gigs.at(2) == stage)
            {
                artists.push_back(elements.first);
            }
        }
    }
    std::sort(artists.begin(), artists.end());
    if (artists.size() == 0)
    {
        std::cout << "Error: Not found." << std::endl;
        return 0;
    }
    std::cout << "Stage " << stage << " has gigs of the following artists:" << std::endl;
    for (const auto& artist : artists)
    {
        std::cout << " - " << artist << std::endl;
    }
    return 1;
}

void gig_calendar_methods::add_artist(std::string artist)
{
    if (gigs_.count(artist))
    {
        std::cout << "Error: Already exists." << std::endl;
    }
    else
    {
        std::cout << "Artist added." << std::endl;
        gigs_[artist];
    }
}

int gig_calendar_methods::add_gig(std::vector<std::string> gig, std::vector< std::vector< std::string >> all_gigs)
{
    std::string artist = gig.at(0);
    gig.erase(gig.begin());
    if (!gigs_.count(artist))
    {
        std::cout << "Error: Not found." << std::endl;
        return 0;
    }
    for (auto& date : gigs_[artist])
    {
        if (date.at(0) == gig.at(0) or std::count(all_gigs.begin(),all_gigs.end(),gig))
        {
            std::cout << "Error: Already exists." << std::endl;
            return 0;
        }
    }
    for (const auto& artist: gigs_[artist])
    {
        if (artist.at(0) == gig.at(1))
        {
            std::cout << "Error: Already exists." << std::endl;
            return 0;
        }
    }
    gigs_[artist].push_back(gig);
    std::cout << "Gig added." << std::endl;
    return 1;
}

bool gig_calendar_methods::compare_dates(std::string date_a, std::string date_b)
{
    std::vector<int> result_a;
    std::vector<int> result_b;
    std::string separator = "-";
    std::string::size_type left = 0;
    std::string::size_type right = date_a.find(separator, left);
    while (right != std::string::npos)
    {
        right = date_a.find(separator, left);
        std::string part = date_a.substr(left, right - left);
        if (part != "")
        {
            result_a.push_back(stoi(part));
        }
        left = right + 1;
    }
    std::string::size_type left_b = 0;
    std::string::size_type right_b = date_a.find(separator, left_b);
    while (right_b != std::string::npos)
    {
        right_b= date_b.find(separator, left_b);
        std::string part = date_b.substr(left_b, right_b - left_b);
        if (part != "")
        {
            result_b.push_back(stoi(part));
        }
        left_b = right_b + 1;
    }
    if (result_a == result_b)
    {
        return 1;
    }
    for (int i = 0; i < 3; ++i)
    {
        if (result_a.at(i) < result_b.at(i))
        {
            return 0;
        }
        else if (result_a.at(i) > result_b.at(i))
        {
            return 1;
        }
    }
    return 0;
}

int gig_calendar_methods::cancel_gig(std::vector<std::string> dates_to_remove)
{
    std::vector< std::vector < std::string >> gigs_not_canceled;
    std::string artist = dates_to_remove.at(0);
    std::string date = dates_to_remove.at(1);
    if (!gigs_.count(artist))
    {
        std::cout << "Error: Not found." << std::endl;
        return 0;
    }
    for (auto gig : gigs_[artist])
    {
        if (compare_dates(date, gig.at(0)))
        {
            gigs_not_canceled.push_back(gig);
        }
    }
    if (gigs_not_canceled == gigs_[artist])
    {
        std::cout << "Error: No gigs after the given date." << std::endl;
        return 0;
    }
    std::cout << "Artist's gigs after the given date cancelled." << std::endl;
    gigs_[artist].swap(gigs_not_canceled);
    return 1;
}
