/*  COMP.CS.110 Project 4: snake_gui
 *
 *  Header tiedosta, pelin toiminnasta kerrottu mainwindow.cpp
 *  alkukommentissa
 *
 * Ohjelman kirjoittaja:
 * Nimi: Lauri Lehtonen
 * Opiskelijanumero: 050023118
 * Käyttäjätunnus: txlale
 * E-Mail: lauri.er.lehtonen@tuni.fi
 * */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameboard.hh"
#include <QMainWindow>
#include <QTextBrowser>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLayout>
#include <QPushButton>
#include <QPixmap>
#include <QSpinBox>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*
     * omia slotteja, joilla päivitetään käyttäjän inputit muuttujiin
     * pelin rakentamista varten.
     * */
private slots:
    /*
     *  päivittää laudan leveyttä käyttäjän muuttaessa aloitusikkunan
     *  spinboxin arvoa.
     * */
    void update_width(int x);

    /*
     *  päivittää laudan korkeutta käyttäjän muuttaessa aloitusikkunan
     *  spinboxin arvoa.
     * */
    void update_height(int y);

    /*
     *  päivittää pelin satunnaisgeneraattorin siemenlukua käyttäjän
     *  muuttaessa aloitusikkunan spinboxin arvoa.
     * */
    void update_seed(int seed);

    /*
     *  päivittää pelin vaikeusastetta käyttäjän muuttaessa aloitusikkunan
     *  spinboxin arvoa.
     * */
    void update_level(int level);

    /*
     *  päivittää pelaajan nimeä käyttäjän muuttaessa aloitusikkunan
     *  line-editin arvoa. oletuksena tyhjä, jos täytetty päivittyy
     *  myös tulostaulukkoon pelin loputtua.
     * */
    void update_player(QString player);

private:
    Ui::MainWindow *ui;

    /* Lisäosa 15
     * luo pelin aloitusikkunan, ja siirtää käyttäjän syöttämiä tietoja
     * eteenpäin slottien kautta.
     * */
    void ask_init_values();

    /*
     * luo peli-ikkunan, ohjaimen ja pelilaudan.
     * kutsuu pelin logiikkaa käyttäjän aloitusikkunassa antamilla
     * arvoilla ja yhdistää ohjaimen napit tarvittaviin metodeihin.
     * */
    void init_game();

    /*
     * siirtää käärmettä pelilaudalla käyttäjän viimeksi määräämään
     * suuntaan, ottaa sisään GameBoard luokan instanssin s, ja siirtää
     * käärmettä vektorin inputs_ määräämillä tavoilla siihen saakka,
     * kunnes käärme on kuollut. kutsuu sen jälkeen s instanssilla
     * laudan piirtometodia.
     * */
    void move(GameBoard s);

    /* lisäosa 4 ja 10
     * piirtää pelilaudan, käärmeen ja hedelmän board_scene:lle, jotta
     * se voidaan näyttää peli-ikkunan view_board:lla, ottaa sisään muuttujana
     * GameBoard luokan instanssin s move metodilta, ja hakee sen kautta käärmeen
     * osien sijainnit Point luokan (x, y) - koordinaatteina ja skaalaa kuvat
     * pelilaudalle.
     * */
    void draw_board(GameBoard s);

    /*
     * liikuttaa käärmettä automaattisesti sen viimeksi liikkumaa kohdetta kohti,
     * lisää viimeksi lisätyn suunnan uudestaan inputs_ vektoriin ja siirtää
     * GameBoard luokan instanssin move metodille, jotta se voidaan piirtää laudalle.
     * */
    void moving_snake(GameBoard s);

    /*
     * Käsittelee peli-ikkunasta tulevan pelin alusta aloittamis komennon.
     * Tyhjentää inputs_ vektorin, nollaa pituuden ja kutsuu metodia
     * clear_window, joka tyhjentää ikkunan
     * */
    void reset();

    /*
     * Päivittää käärmeen pituutta peli-ikkunassa: lisäosa 2
     * */
    void update_length();

    /* lisäosa 8 ja 9
     * Avaa tulostaulun, ja täyttää sen tiedoston "scoreboard.txt" tiedoilla.
     * */
    void get_scoreboard();

    /* lisäosan 4 lisäosa
     * palauttaa satunnaisen hedelmän tiedostonimen fruits vektorista.
     * */
    std::string random_fruit();

    /*
     * pelin päättyessä tuo ylös ikkunan, jossa ilmoitetaan pelin loppuneen,
     * pelin lopputilanne ja pisteet, sekä annetaan mahdollisuus aloittaa alusta,
     * tai lopettaa peli.
     * */
    void game_over();

    /*
     * palauttaa peli-ikkunan tilaan, josta se alkoi, jotta peli voidaan aloittaa
     * alusta.
     * */
    void clear_window();

    // vaikeusaste (1-5): lisäosa 17
    int level_ = 1;
    // satunnaisgeneraattorin siemenluku (0-99)
    int seed_ = 0;
    // laudan korkeus (0-10): lisäosa 1
    int height_ = 3;
    // laudan leveys (0-10): lisäosa 1
    int width_ = 3;
    // madon pituus, käytetään myös pisteiden hetkelliseen säilömiseen: lisäosa 2
    float lenght_ = 1;
    // ilmoittaa onko peli päättynyt
    bool game_ended = 0;
    /*
     *  vektori, joka sisältää järjestyksessä käyttäjän tekemät siirrot,
     *  joiden mukaan pelin logiikalle syötetään viimeisin madon ja hedelmän
     *  lokaatiot.
     *  */
    std::vector< std::string > inputs_ = {};
    // pelaajan nimi
    std::string player_;
    // pelin lopputilanne (win/loss):
    QString end_state_;
    // tämänhetkisen hedelmän tiedostonimi, oletuksena cane.png koska joulu.
    std::string fruit_ = "cane.png";
    // vektori, joka sisältää kaikkien hedelmien tiedostonimet.
    std::vector< std::string > fruits = {"apple.png",
                                    "banana.png", "cane.png",
                                    "grapes.png", "pear.png",
                                        "strawberry.png"};

    // ajastin, jonka avulla liikutetaan käärmettä itsestään eteenpäin
    QTimer *move_snake_automatically;
    // pelilauta, jolle peli piirretään.
    QGraphicsScene *board_scene;
    // Madon pituustiedosta kertova tekstilaatikko.
    QLabel *length;
};
#endif // MAINWINDOW_H
