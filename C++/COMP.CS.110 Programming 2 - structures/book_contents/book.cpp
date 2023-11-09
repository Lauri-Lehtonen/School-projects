/* COMP.CS.110 Project 3: book_contenta
 *
 * Luokka book.cpp käsittelee käyttäjän CLI-luokalle antamat komennot,
 * ja käsittelee ne tietoluokan osoittimien osoittamien tietueiden
 * sisältaämillä tiedoilla.
 *
 * Ohjelman kirjoittaja
 * Nimi: Lauri Lehtonen
 * Opiskelijanumero: 050023118
 * Käyttäjätunnus: txlale
 * E-Mail: lauri.er.lehtonen@tuni.fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 * Koodi toimii ku VR:n vessa ja osoittimet on ihanan intuitiivisia! :D
 * */

#include "book.hh"
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

Book::Book() :
    Chapter_pointers_()
{

}

Book::~Book()
{

}

void Book::addNewChapter(const std::string &id, const std::string &fullName, int length)
{
    bool is_duplicate_id = 0;
    for (auto i : Chapter_pointers_)
    {

        if (i->id_ == id && is_duplicate_id == 0)
        {
            cout << "Error: Already exists." << endl;
            is_duplicate_id = 1;
        }
    }
    if (is_duplicate_id == 0)
    {
        Chapter* new_chapter = new Chapter;
        new_chapter->id_ = id;
        new_chapter->fullName_ = fullName;
        new_chapter->length_ = length;
        Chapter_pointers_.push_back(new_chapter);
    }
}

void Book::addRelation(const std::string &subchapter, const std::string &parentChapter)
{
    Chapter* parent = nullptr;
    Chapter* sub = nullptr;
    if (parentChapter != "")
    {
        for (auto i : Chapter_pointers_)
        {
            if (i->id_ == parentChapter)
            {
                parent = i;
            }
            if (i->id_ == subchapter)
            {
                sub = i;
            }
        }
        if (parent != nullptr)
        {
            parent->subchapters_.push_back(sub);
        }
        if (sub != nullptr)
        {
            sub->parentChapter_ = parent;
        }
    }

}

void Book::printIds(Params params) const
{
    (void)params;
    cout << "Book has " << Chapter_pointers_.size() << " chapters:" << endl;
    vector<tuple<string,string>> sorted_ids;
    for (auto i : Chapter_pointers_)
    {
        sorted_ids.push_back(make_tuple(i->fullName_, i->id_));
    }
    sort(sorted_ids.begin(), sorted_ids.end());
    for (auto i : sorted_ids)
    {
        cout << get<0>(i) << " --- " << get<1>(i) << endl;
    }
}

void Book::printContents(Params params) const
{
    (void)params;
    int main_chapter_number = 0;
    for (auto i : Chapter_pointers_)
    {
        Chapter* chapter_to_print = i;
        if (chapter_to_print->is_closed && chapter_to_print->parentChapter_ == nullptr)
        {
            ++main_chapter_number;
            cout << "+ " << main_chapter_number << ". " << chapter_to_print->fullName_
                 << " ( " << chapter_to_print->length_ << " )" << endl;
        }
        else if (chapter_to_print->parentChapter_ == nullptr)
        {
            ++main_chapter_number;
            cout << "- " << main_chapter_number << ". " << chapter_to_print->fullName_
                 << " ( " << chapter_to_print->length_ << " )" << endl;
            print_subchapter(chapter_to_print, 2);
        }
    }
}

void Book::close(Params params) const
{
    if(is_chapter_in_book(params[0]))
    {
        Chapter* i = findChapter(params[0]);
        i->is_closed = 1;
        close_subchapters(i);
    }
}

void Book::open(Params params) const
{
    if(is_chapter_in_book(params[0]))
    {
        Chapter* i = findChapter(params[0]);
        i->is_closed = 0;
    }
}

void Book::openAll(Params params) const
{
    (void)params;
    for (auto i : Chapter_pointers_)
    {
        i->is_closed = 0;
    }
}

void Book::printParentsN(Params params) const
{
    int N = stoi(params[1]);
    int amount_of_parents = 0;
    vector< string > parents;
    if (N < 1)
    {
        cout << "Error. Level can't be less than 1." << endl;
    }
    else
    {
        if (is_chapter_in_book(params[0]))
        {
            Chapter* i = findChapter(params[0]);
            string origin = i->id_;
            if (i->parentChapter_ == nullptr)
            {
                cout << origin << " has no parent chapters." << endl;
            }
            else
            {
                while (i->parentChapter_ != nullptr && amount_of_parents < N)
                {
                    i = i->parentChapter_;
                    parents.push_back(i->id_);
                    ++amount_of_parents;
                }
                cout << origin << " has " << amount_of_parents << " parent chapters:" << endl;
                sort(parents.begin(), parents.end());
                for (auto i : parents)
                {
                    cout << i << endl;
                }
            }
        }
    }
}

void Book::printSubchaptersN(Params params) const
{
    int N = stoi(params[1]);
    vector< string > subchapters;
    if (N < 1)
    {
        cout << "Error. Level can't be less than 1." << endl;
    }
    else
    {
        if(is_chapter_in_book(params[0]))
        {
            Chapter* i = findChapter(params[0]);
            string origin = i->id_;
            if (i->subchapters_.size() == 0)
            {
                cout << origin << " has no subchapters." << endl;
            }
            else
            {
                N_subchapters(i, subchapters, N);
                cout << origin << " has " << subchapters.size() << " subchapters:" << endl;
                sort(subchapters.begin(), subchapters.end());
                for (auto i : subchapters)
                {
                    cout << i << endl;
                }
            }
        }
    }
}

void Book::printSiblingChapters(Params params) const
{
    if(is_chapter_in_book(params[0]))
    {
        vector< string > sorted_ids;
        for (auto i : Chapter_pointers_)
        {
            if (i->id_ == params[0] && i->parentChapter_ != nullptr)
            {
                Chapter* parent = i->parentChapter_;
                for (auto k : parent->subchapters_)
                {
                    if (k->id_ != i->id_)
                        sorted_ids.push_back(k->id_);
                }
            }
        }
        sort(sorted_ids.begin(), sorted_ids.end());
        if (sorted_ids.size() == 0)
        {
            cout << params[0] << " has no sibling chapters." << endl;
        }
        else
        {
            cout << params[0] << " has " << sorted_ids.size()
                              << " sibling chapters:" << endl;
            for (auto &i : sorted_ids)
            {
                cout << i << endl;
            }
        }
    }
}

void Book::printTotalLength(Params params) const
{
    if(is_chapter_in_book(params[0]))
    {
        int length = 0;
        Chapter* i = findChapter(params[0]);
        length += i->length_;
        for (auto k : i->subchapters_)
            calculate_chapter_length(k,length);
        cout << "Total length of " << i->id_ << " is " << length << "." << endl;
    }
}

void Book::printLongestInHierarchy(Params params) const
{
    if(is_chapter_in_book(params[0]))
    {
        Chapter* longest = nullptr;
        Chapter* parent = findChapter(params[0]);
        set_chapters(parent, longest);
        find_longest_chapter(parent, longest);
        print_short_or_long(0, longest, params[0]);
    }
}

void Book::printShortestInHierarchy(Params params) const
{
    if(is_chapter_in_book(params[0]))
    {
        Chapter* shortest = nullptr;
        Chapter* parent = findChapter(params[0]);
        set_chapters(parent, shortest);
        find_shortest_chapter(parent, shortest);
        print_short_or_long(1, shortest, params[0]);
    }
}

void Book::print_subchapter(Chapter* c, int indent_size) const
{
    char indent = ' ';
    int chapter_number = 0;
    if (c->subchapters_.size() != 0)
    {
        for (auto i : c->subchapters_)
        {
            ++chapter_number;
            if (i->is_closed == 1)
            {
                cout << "+ ";
                for (int i = 0 ; i < indent_size ; ++i)
                {
                    cout << indent;
                }
                cout << chapter_number << ". " << i->fullName_
                     << " ( " << i->length_ << " )" << endl;
                continue;
            }
            else
            {
                cout << "- ";
                for (int i = 0 ; i < indent_size ; ++i)
                {
                    cout << indent;
                }
                cout << chapter_number << ". " << i->fullName_
                     << " ( " << i->length_ << " )" << endl;
                print_subchapter(i, indent_size + 2);
            }
        }
    }
}

void Book::calculate_chapter_length(Chapter* c, int &length) const
{
    length += c->length_;
    if (c->subchapters_.size() != 0)
    {
        for (auto i : c->subchapters_)
        {
            calculate_chapter_length(i, length);
        }
    }
}

void Book::find_longest_chapter(Chapter* parent, Chapter* &longest) const
{

    for (auto i : parent->subchapters_)
    {
        if (i->subchapters_.size() == 0 && i->length_ > longest->length_)
            longest = i;
        else
        {
            find_longest_chapter(i, longest);
        }
    }
}

bool Book::is_chapter_in_book(std::string chapter_id) const
{
    for (auto i : Chapter_pointers_)
    {
        if (chapter_id == i->id_)
        {
            return 1;
        }
    }
    cout << "Error: Not found: " << chapter_id << endl;
    return 0;
}

void Book::close_subchapters(Chapter *c) const
{
    if (c->subchapters_.size() != 0)
    {
        for (auto i : c->subchapters_)
        {
            if (i->subchapters_.size() != 0)
            {
                i->is_closed = 1;
                close_subchapters(i);
            }
        }
    }
}

void Book::N_subchapters(Chapter* c, std::vector<std::string> &v, int N) const
{
    if (c->subchapters_.size() != 0 && N > 0)
    {
        --N;
        for (auto i : c->subchapters_)
        {
            v.push_back(i->id_);
            N_subchapters(i,v,N);
        }
    }
}

void Book::set_chapters(Chapter *&parent, Chapter *&compared) const
{
    while (parent->parentChapter_ != nullptr)
    {
        parent = parent->parentChapter_;
    }

    // Longest pointer is given the starting value of parent pointer, so we can
    // compare the length of each sub-chapter.
    compared = parent;
}

void Book::print_short_or_long(int command, Chapter* c, std::string original_id) const
{
    vector< string > text = {"longest","shortest"};
    if (c->id_ == original_id)
    {
        cout << "With the length of " << c->length_ << ", "
             << original_id << " is the " << text[command] << " chapter in their hierarchy."
             << endl;
    }
    else
    {
        cout << "With the length of " << c->length_ << ", "
             << c->id_ << " is the " << text[command] << " chapter in "
             << original_id <<"'s hierarchy." << endl;
    }
}

Chapter *Book::findChapter(const std::string &id) const
{
    for (auto i : Chapter_pointers_)
    {
        if (i->id_ == id)
        {
            return i;
        }
    }
    return nullptr;
}

void Book::find_shortest_chapter(Chapter* parent, Chapter* &shortest) const
{

    for (auto i : parent->subchapters_)
    {
        if (i->subchapters_.size() == 0 && i->length_ < shortest->length_)
            shortest = i;
        else
        {
            find_shortest_chapter(i, shortest);
        }
    }
}
