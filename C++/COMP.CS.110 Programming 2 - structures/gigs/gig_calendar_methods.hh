#ifndef GIG_CALENDAR_METHODS_HH
#define GIG_CALENDAR_METHODS_HH
#include <map>
#include <vector>
#include <string>

class gig_calendar_methods
{
public:
    gig_calendar_methods(const std::map< std::string, std::vector< std::vector <std::string >>>& gig);
    // Printtaa artistit aakkosjärjestyksessä tietorakenteen avaimista.
    void print_artists();
    // Etsii artistin keikat, ja printtaa ne aikajärjestyksessä.
    void find_gigs(std::string artist);
    // printtaa kaikki esiintymispaikat aakkosjärjestyksessä.
    void print_stages();
    // Poistaa vektorista useammin kuin kerran esiintyvät jäsenet
    void remove_duplicates(std::vector< std::vector< std::string >>& v);
    // Tulostaa tietyllä lavalla esiintyvät artistit aikajärjestyksessä.
    int performers_at_stage(std::string stage);
    // Lisää tietorakenteeseen uuden avaimen, johon voidaan liittää tai poistaa tietoa
    void add_artist(std::string artist);
    // Lisää keikan avaimeen tietorakenteen sisälle
    int add_gig(std::vector< std::string > gig, std::vector< std::vector< std::string >> all_gigs);
    // Vertaa keikkojen päivämääriä keskenään, ja palauttaa 0 tai 1 arvon cancel-funktiolle.
    bool compare_dates(std::string date_a, std::string date_b);
    // Poistaa keikat tietyn päivämäärän jälkeen.
    int cancel_gig(std::vector< std::string > gig);
private:
    std::map< std::string, std::vector< std::vector <std::string >>> gigs_;
};

#endif // GIG_CALENDAR_METHODS_HH
