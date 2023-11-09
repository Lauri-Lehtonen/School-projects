/* COMP.CS.110 Project 4: snake_gui
 * Kuvaus:
 *  Ohjelma toteuttaa klassisen matopelin graafisella käyttöliittymällä.
 *  Ohjelma käynnistää ensin aloitussivun, jolla kysytään haluttua
 *  laudan kokoa (leveys x korkeus) (3-10), satunnaisgeneraattorin
 *  siemenlukua (0-99), vaikeusastetta (1-5) ja halutessaan
 *  käyttäjä voi asettaa itselleen nimen, jolloin hän osallistuu
 *  tulostaulukkoon, ja hän voi myöhemmin löytää vanhojen peliensä
 *  tulokset. Vaikeusaste muuttaa madon automaattisen liikkeen nopeuteen
 *  kaavalla 2000/vaikeusaste (ms). Haluttujen asetusten syöttämisen
 *  jälkeen käyttäjä voi aloittaa pelin painamalla start nappia, ohjelman
 *  voi myös lopettaa painamalla quit nappia. Aloitettuaan pelin sulkeutuu
 *  aloitusikkuna, ja sen tilalle tulee peli-ikkuna, jossa on kaksi osaa:
 *  ensimmäinen osa on ohjain, jossa on suuntanapit, lopetusnappi,
 *  pelin uudelleenaloitusnappi ja scoreboard-nappi, joka avaa tulostaulun,
 *  jossa on kaikki edellisten ajojen tallennetut tulokset laskevassa
 *  suuruusjärjestyksessä. Tulokset haetaan "scoreboard.txt" tiedostosta,
 *  jonne ne tallennetaan aina pelin loputtua, jos käyttäjä on antanut
 *  alkuikkunassa itselleen nimen. pisteet lasketaan seuraavalla kaavalla:
 *
 *  voitettu peli: pituus*((leveys*korkeus)/2)
 *  Hävitty peli: pituus/sqrt((leveys*korkeus)/2)
 *
 *  Peli-ikkunan toinen osa on itse pelilauta, jonne piirretään madon osat ja
 *  hedelmä valmiin pohjakoodin antaman logiikan mukaan. hedelmiä on 6 erilaista,
 *  ja ne haetaan satunnaislukugeneraattion avulla vektorista, joka sisältää
 *  kuvatiedostojen nimet.
 *
 *  pelin loppuessa peli-ikkunan väri muuttuu pelin lopputilanteen mukaan joko
 *  vihreäksi tai punaiseksi, sekä deaktivoi ohjaimen napit ja avaa uuden ikkunan,
 *  joka ilmoittaa pelin loppuneen, voitosta tai häviöstä sekä käyttäjän ansaitsemat
 *  pisteet ja antaa mahdollisuuden lopettaa pelin tai uudelleenaloittaa sen
 *  painonapeilla.
 *
 * Ohjelman kirjoittaja
 * Nimi: Lauri Lehtonen
 * Opiskelijanumero: 050023118
 * Käyttäjätunnus: txlale
 * E-Mail: lauri.er.lehtonen@tuni.fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 * Koodi toimii tällä kertaa vähän huonommin ku VR:n vessa, ja
 * muistinhallinta oli hankalampaa kun ootin, melkein kaatoi miehen
 * tarmokkuuden, mutta sisulla selvittiin. Hyviä jouluja jengille!
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <gameboard.hh>
#include <fstream>
#include <map>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //kutsuu aloitusikkunaa
    ask_init_values();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ask_init_values()
{
    // nimetään ja uudelleenskaalataan pääikkuna
    this->resize(400, 200);
    this->setWindowTitle("snek: the game");

    /*
     * Qobjektit, joilla rakennetaan aloitusikkuna
     * */
    QWidget *start = new QWidget(this);
    QLabel *sizing_info = new QLabel(
                "Set size of the board and game seed:", start);
    QLabel *Y = new QLabel("height:", start);
    QLabel *X = new QLabel("width:", start);
    QLabel *seed = new QLabel("seed-value:", start);
    QPushButton *set_size = new QPushButton("start",start);
    set_size->setStyleSheet("background-color: green");
    QSpinBox *X_value = new QSpinBox(start);
    QSpinBox *Y_value = new QSpinBox(start);
    QSpinBox *seed_value = new QSpinBox(start);
    QLineEdit *player_name = new QLineEdit(start);
    player_name->setPlaceholderText(
                "insert player name to participate in scoreboard");
    player_name->setAlignment(Qt::AlignCenter);
    QSpinBox *level_value = new QSpinBox(start);
    QLabel *level = new QLabel("difficulty:", start);
    QPushButton *quit = new QPushButton("quit", start);
    quit->setStyleSheet("background-color: red");
    QLabel *image = new QLabel(start);
    QPixmap snek("snek.png");
    image->setPixmap(snek);


    start->resize(400, 200);

    /*
     * Sijoitetaan aloitusikkunan Qobjektit matriisilayouttiin.
     * */
    QGridLayout *start_game = new QGridLayout(start);
    start_game->addWidget(sizing_info,0, 0, 1, 3, Qt::AlignCenter);
    start_game->addWidget(X, 1, 0, Qt::AlignCenter);
    start_game->addWidget(X_value,2, 0);
    start_game->addWidget(Y, 3, 0, Qt::AlignCenter);
    start_game->addWidget(Y_value, 4, 0);
    start_game->addWidget(seed, 1, 2, Qt::AlignCenter);
    start_game->addWidget(seed_value, 2, 2);
    start_game->addWidget(level, 3, 2, Qt::AlignCenter);
    start_game->addWidget(level_value, 4, 2);
    start_game->addWidget(image, 1, 1, 4, 1);
    start_game->addWidget(player_name,5,0,1,3);
    start_game->addWidget(set_size, 6, 2, Qt::AlignCenter);
    start_game->addWidget(quit, 6, 0);

    // muuttujien minimi- ja maksimiarvorajat
    level_value->setMinimum(1);
    level_value->setMaximum(5);
    X_value->setMinimum(3);
    X_value->setMaximum(10);
    Y_value->setMinimum(3);
    Y_value->setMaximum(10);

    this->setLayout(start_game);

    /*
     * Yhdistetään aloitusnäytön inputit niitä vastaaviin slotteihin
     * */
    connect(player_name, SIGNAL(textChanged(QString)), this, SLOT(update_player(QString)));
    connect(Y_value, SIGNAL(valueChanged(int)), this, SLOT(update_height(int)));
    connect(X_value, SIGNAL(valueChanged(int)), this, SLOT(update_width(int)));
    connect(level_value, SIGNAL(valueChanged(int)), this, SLOT(update_level(int)));
    connect(seed_value, SIGNAL(valueChanged(int)), this, SLOT(update_seed(int)));
    connect(quit, &QPushButton::clicked, this, &MainWindow::close);
    connect(set_size, &QPushButton::pressed, this, [=]() {
        init_game(); delete start;
    });
}

void MainWindow::update_width(int x)
{
    width_ = x;
}

void MainWindow::update_height(int y)
{
    height_ = y;
}

void MainWindow::update_seed(int seed)
{
    seed_ = seed;
}

void MainWindow::update_level(int level)
{
    level_ = level;
}

void MainWindow::update_player(QString player)
{
    player_ = player.toStdString();
}

std::string MainWindow::random_fruit()
{
    int i = rand()%(6);
    std::string fruit = fruits.at(i);
    return fruit;
}

void MainWindow::init_game()
{
    // luodaan pelilauta käyttäjän syöttämillä tiedoilla
    GameBoard snake(width_, height_, seed_);

    QWidget *game = new QWidget(this);
    QWidget *controller = new QWidget(game);

    /*
     * Luodaan pelilaudan grafiikka ja piirretään se Graphicsviewlle
     * */
    board_scene = new QGraphicsScene(this);
    QGraphicsView *view_board = new QGraphicsView(this);
    length = new QLabel("snake length: 1", this);
    length->setAlignment(Qt::AlignCenter);
    length->setStyleSheet("background-color: white");
    move_snake_automatically = new QTimer(this);
    draw_board(snake);

    this->setStyleSheet("background-color: gray");

    // Kontrollerin sisältämät napit
    QPushButton *quit_controller = new QPushButton("quit", controller);
    quit_controller->setStyleSheet("background-color: red");
    QPushButton *restart = new QPushButton("reset", controller);
    restart->setStyleSheet("background-color: white");
    QPushButton *up = new QPushButton("^", controller);
    QPushButton *down = new QPushButton("v", controller);
    QPushButton *left = new QPushButton("<", controller);
    QPushButton *right = new QPushButton(">", controller);
    QPushButton *scores = new QPushButton("scoreboard", controller);
    scores->setStyleSheet("background-color: gold");

    // muutetaan pelilaudan kokoa sopivaksi
    QRect size = QRect(0,0,500,500);
    board_scene->setSceneRect(size);
    view_board->setScene(board_scene);
    view_board->resize(500, 500);

    /*
     * Asetetaan napit kontrollerille oikeaan järjestykseen
     * */
    controller->setFixedSize(200, 200);
    QGridLayout *controls = new QGridLayout(controller);
    controls->addWidget(length,1,0,1,3);
    controls->addWidget(up, 2, 1);
    controls->addWidget(down, 4, 1);
    controls->addWidget(left, 3, 0);
    controls->addWidget(right, 3, 2);
    controls->addWidget(scores,5,0,1,3);
    controls->addWidget(quit_controller, 6, 0);
    controls->addWidget(restart, 6, 2);

    // Asetetaan kontrolleri ja pelilauta vierekkäin pääikkunaan
    game->resize(700, 500);
    QHBoxLayout *game_window = new QHBoxLayout(game);
    game_window->addWidget(controller);
    game_window->addWidget(view_board);


    this->resize(700, 500);

    game->show();
    this->setLayout(game_window);

    // skaalataan pelilauta niin, ettei scrollbarit ilmesty
    view_board->scale(0.9, 0.9);

    // yhdistetään ajastin käärmeen automaattisen liikkeen mahdollistavaan funktioon
    connect(move_snake_automatically, &QTimer::timeout, this, [=]() {
        moving_snake(snake);
    });
    /*
    * lambda funktiot, joilla yhdistetään suuntanapit käärmeen liikkeeseen.
    * */
    connect(up, &QPushButton::pressed, this, [=]() {
        inputs_.push_back("w"); move(snake);
    });

    connect(down, &QPushButton::pressed, this, [=]() {
        inputs_.push_back("s"); move(snake);
    });

    connect(left, &QPushButton::pressed, this, [=]() {
        inputs_.push_back("a"); move(snake);
    });

    connect(right, &QPushButton::pressed, this, [=]() {
        inputs_.push_back("d"); move(snake);
    });

    // Yhdistetään kontrollerin muut napit haluttuihin ominaisuuksiin
    connect(scores, &QPushButton::clicked, this, &MainWindow::get_scoreboard);
    connect(quit_controller, &QPushButton::clicked, this, &MainWindow::close);
    connect(restart, &QPushButton::pressed, this, [=]() {
        reset();
    });
}


void MainWindow::draw_board(GameBoard s)
{
    // puhdistaa laudan, jotta sille piirretään vain yksi käärme kerrallaan
    board_scene->clear();
    // laudan tausta
    board_scene->setBackgroundBrush(QBrush(QPixmap("tile.png")));

    // Laskee käärmeen pituuden joka iteraatiolla, käytetään myös pistelaskussa
    float score = lenght_;
    lenght_ = 1;

    // hakee hedelmän kuvan Pixmappiin
    QPixmap fruit(QString::fromStdString(fruit_));
    fruit = fruit.scaled(480/width_, 480/height_);

    // piirtää pelilaudan ja sille objektit pelin logiikan avulla
    for (int row = 0; row < height_; ++row)
    {
        if (game_ended)
            continue;
        else
        {
            for (int col = 0; col < width_; ++col)
            {

                // piste laudalla
                int x = (500/width_)*col;
                int y = (500/height_)*row;
                const Point position(col, row);

                // piirtää hedelmän laudalle
                if (position == s.getfood())
                {
                    QGraphicsPixmapItem *visual_fruit = board_scene->addPixmap(fruit);
                    visual_fruit->setPos(x, y);
                }

                // skippaa iteraation, jos ei ole mitään piirrettävää
                else if (not s.checkSnakePoint(position))
                {
                    continue;
                }

                // käsittelee pelin loppumistilanteen ja laskee pisteet tilanteen mukaan
                else if (s.gameLost() or s.gameWon())
                {
                    game_ended = 1;
                    float avg_side = ((float)width_*(float)height_)/2;

                    if (s.gameLost())
                    {
                        /*
                         * jos peli on hävitty, muuttaa ikkunan värin punaiseksi,
                         * ja laskee pisteet asianmukaisella kaavalla.
                         * */
                        this->setStyleSheet("background-color: red");
                        score = score/sqrt(avg_side/2);
                        end_state_ = "You lost.";
                    }

                    else
                    {
                        /*
                         * jos peli on voitettu, muuttaa ikkunan värin vihreäksi,
                         * ja laskee pisteet asianmukaisella kaavalla.
                         * */
                        ++score;
                        this->setStyleSheet("background-color: green");
                        score = score*float(avg_side);
                        end_state_ = "You Won!";
                    }
                    // päivittää pisteet pituusmuuttujaan, ja piilottaa sen kontrollerilta
                    lenght_ = score;
                    length->hide();

                    // kutsuu game_over funktiota
                    if (inputs_.size() != 0)
                        game_over();
                    inputs_.clear();
                }

                // dpiirtää käärmeen pään
                else if (position == s.head_to_gui())
                {
                    QPixmap head("head.png");
                    head = head.scaled(480/width_, 480/height_);
                    QGraphicsPixmapItem *visual_head = board_scene->addPixmap(head);
                    visual_head->setPos(x, y);
                }

                // piirtää käärmeen hännän
                else if (position == s.tail_to_gui())
                {
                    lenght_ += 1;
                    QPixmap tail("tail.png");
                    tail = tail.scaled(480/width_, 480/height_);
                    QGraphicsPixmapItem *visual_tail = board_scene->addPixmap(tail);
                    visual_tail->setPos(x, y);
                }

                // piirtää käärmeen kehon
                else
                {
                    lenght_ += 1;
                    QPixmap body("body.png");
                    body = body.scaled(480/width_, 480/height_);
                    QGraphicsPixmapItem *visual_body = board_scene->addPixmap(body);
                    visual_body->setPos(x, y);
                }
            }
        }
    }
    // vaihtaa hedelmän, jos se on syöty viime liikkeellä
    if (score != lenght_)
    {
        fruit_ = random_fruit();
        draw_board(s);
    }
    update_length();
}


void MainWindow::moving_snake(GameBoard s)
{
    if (!game_ended && inputs_.size() != 0)
    {
        inputs_.push_back(inputs_.back());
    }
    move(s);
}

void MainWindow::move(GameBoard s)
{
    /*
     * Pysäyttää ajastimen ja käynnistää sen alusta liikkeen tapahduttua,
     * tämä tehdään sen takia, jottei käärme liiku kahta liikettä samaan aikaan,
     * kun ajastin lyö ja käyttäjä liikkuu samalla hetkellä
     * */
    move_snake_automatically->stop();
    for (auto &i : inputs_)
    {
        if (!s.gameOver())
            s.moveSnake(i);
    }
    move_snake_automatically->start(2000/level_);
    draw_board(s);
}

void MainWindow::game_over()
{
    game_ended = 1;
    this->setEnabled(false);

    QWidget *Game_over_window = new QWidget();
    QLabel *GG = new QLabel("Game over.", Game_over_window);
    QLabel *game_state = new QLabel(end_state_, Game_over_window);
    QLabel *end_score = new QLabel(Game_over_window);
    QString score = "score: " + QString::number(lenght_);
    end_score->setText(score);
    QPushButton *reset_button = new QPushButton("reset", Game_over_window);
    QPushButton *quit_button = new QPushButton("quit", Game_over_window);

    /*
    * Päivittää pisteet pisteenlaskutiedostoon, jos käyttäjä on antanut nimen
    * */
    if (player_.length() > 0)
    {
        std::fstream scoreboard;
        scoreboard.open("scoreboard.txt", std::fstream::in
                        | std::fstream::out | std::fstream::app);
        scoreboard <<lenght_ << "|" << player_<< std::endl;
        scoreboard.close();
    }

    QVBoxLayout *lost_game = new QVBoxLayout(Game_over_window);
    lost_game->addWidget(GG);
    lost_game->addWidget(end_score);
    lost_game->addWidget(game_state);
    lost_game->addWidget(reset_button);
    lost_game->addWidget(quit_button);

    Game_over_window->show();
    connect(reset_button, &QPushButton::pressed, this, [=]() {
        game_ended = 0; clear_window();
        Game_over_window->close();delete Game_over_window;
    });
    connect(quit_button, &QPushButton::pressed, this, [=]() {
        delete Game_over_window; this->close();
    });

}

void MainWindow::reset()
{
    inputs_.clear();
    lenght_ = 0;
    clear_window();
}

void MainWindow::update_length()
{
    QString text = "snake lenght: " + QString::number(lenght_);
    length->setText(text);
}

void MainWindow::clear_window()
{
    length->show();
    this->setStyleSheet("background-color: gray");
    this->setEnabled(true);
    move_snake_automatically->start(100);
}

void MainWindow::get_scoreboard()
{
    move_snake_automatically->stop();
    QWidget *display_score = new QWidget(this);
    display_score->resize(200, 500);
    display_score->show();
    QTextBrowser *scoreboard = new QTextBrowser(display_score);
    QPushButton *close_scoreboard = new QPushButton("close", display_score);

    std::ifstream score_file("scoreboard.txt");
    std::map < float, std::vector< std::string >> scores;
    std::string delimiter = "|";
    std::string part;
    std::string line;
    /*
     * Lukee pistetiedoston sisällön, ja asettaa sen scores mappiin,
     * josta se sijoitetaan laskevassa suuruusjärjestyksessä QTextBrowseriin
     * */
    while (getline(score_file, line))
    {
        if (line != "")
        {
            size_t pos = 0;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                part = line.substr(0, pos);
                {
                    line.erase(0, pos+delimiter.length());
                    scores.insert({stof(part), {}});
                    scores[stof(part)].push_back(line);
                }
            }

        }
    }

    scoreboard->append("player | score \n");
    for (auto iter = scores.rbegin(); iter != scores.rend(); ++iter) {
        for (auto &i : iter->second)
        {
            scoreboard->append(QString::fromStdString(i) + " | "
                               + QString::number(iter->first));
        }
    }
    scoreboard->setStyleSheet("background-color: gold");

    QVBoxLayout *pop_up_scoreboard = new QVBoxLayout(display_score);
    pop_up_scoreboard->addWidget(scoreboard);
    pop_up_scoreboard->addWidget(close_scoreboard,Qt::AlignBottom);

    connect(close_scoreboard, &QPushButton::pressed, this, [=]() {
        move_snake_automatically->start(2000/level_); delete display_score;
    });
}
