/*
####################################################################
# TIE-02201 Ohjelmointi 2: Perusteet, K2019                        #
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project4: Snake: The Revengeance                                 #
# Program description: "While revenge is served cold and vengeance #
#                       is served hot revengeance is often seen    #
#                       served with Cajun with a side of onion     #
#                       rings and often dumplings in the Orient."  #
#                       - urbandictionary                          #
#                                                                  #
# File: main_window.cpp                                            #
# Description: Defines a class implementing a UI for the game.     #
#                                                                  #
# Author: Niklas Nurminen, 283756, niklas.nurminen@tuni.fi         #
####################################################################
*/

#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QLabel>
#include <QDebug>

/*
    FUNCTIONS ARE DESCRIBED IN HEADER FILE!
*/


MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent) {

    ui_.setupUi(this);
    ui_.graphicsView->setScene(&scene_);

    connect(&timer_, &QTimer::timeout, this, &MainWindow::moveSnake);
    connect(&scoreTimer_, &QTimer::timeout, this, &MainWindow::clock); //Additional clock, to show used time

    //I connected everything manually, because default slot names were bad programming language.
    connect(ui_.restartButton, &QPushButton::clicked, this, &MainWindow::onRestartButtonClicked);
    connect(ui_.easyButton, &QPushButton::clicked, this, &MainWindow::onEasyButtonClicked);
    connect(ui_.mediumButton, &QPushButton::clicked, this, &MainWindow::onMediumButtonClicked);
    connect(ui_.hardButton, &QPushButton::clicked, this, &MainWindow::onHardButtonClicked);
    connect(ui_.pauseResumeButton, &QPushButton::clicked, this, &MainWindow::onPauseResumeButtonClicked);
    connect(ui_.playButton, &QPushButton::clicked, this, &MainWindow::onPlayButtonClicked);

    //Snake has default direction to left
    snakeDir_ = "LEFT";

}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    if(event->key() == Qt::Key_W && snakeDir_ != "DOWN" && gameOn_ == true){
        snakeDir_ = "UP";
    }else if(event->key() == Qt::Key_S && snakeDir_ != "UP" && gameOn_ == true){
        snakeDir_ = "DOWN";
    }else if(event->key() == Qt::Key_A && snakeDir_ != "RIGHT" && gameOn_ == true){
        snakeDir_ = "LEFT";
    }else if(event->key() == Qt::Key_D && snakeDir_ != "LEFT" && gameOn_ == true){
        snakeDir_ = "RIGHT";
    }else if(event->key() == Qt::Key_Escape){
        this->close();
    }else if(event->key() == Qt::Key_Space && gameOn_ == true){
        onPauseResumeButtonClicked();

    }
}

void MainWindow::onPlayButtonClicked() {

    //Set buttons to disabled, because otherwise game would crash.
    ui_.playButton->setDisabled(true);
    ui_.easyButton->setDisabled(true);
    ui_.mediumButton->setDisabled(true);
    ui_.hardButton->setDisabled(true);

    //If user hasn't chose game difficulty, then automatically is easy level.
    if(gameMode_ == "NONE" || gameMode_.isEmpty()){
        onEasyButtonClicked();
    }
    gameOn_ = true;

    createHead();
    insertFood();

    //Medium and hard levels have obstacles
    if(isObstacles_ == true){
        createObstacles();
    }

    //Maxsnake = how big snake can grow.
    if(gameMode_ == "EASY"){
        maxSnakeSize_ = boardSize_ * boardSize_;
    }else if(gameMode_ == "MEDIUM"){
        //Minus two, because of the obstacles.
        maxSnakeSize_ = (boardSize_ * boardSize_) - 2;
    }else{
        maxSnakeSize_ = (boardSize_ * boardSize_) - 4;
    }


    adjustSceneArea();
    timer_.start(timerSpeed_);
    scoreTimer_.start(1000);
}

void MainWindow::moveSnake() {

    //Update snakebody to new position
    for(int i = snakeSize_ - 1; i > 0; --i){
        snakeparts_.at(i)->setPos(snakeparts_.at(i-1)->scenePos());
    }

    //Update snakehead to new position
    if(snakeDir_ == "UP"){
        //If head is over gameboard size, it comes from otherside.
        if(snakeparts_.at(0)->pos().y() < 0){
            snakeparts_.at(0)->setPos(QPoint(snakeparts_.at(0)->pos().x(), boardSize_-1));
        //If head in gameboard sizes, just simple give new position
        }else{
            snakeparts_.at(0)->setPos(snakeparts_.at(0)->scenePos() + QPoint(0,-1));
        }

    }else if(snakeDir_ == "DOWN"){
        if(snakeparts_.at(0)->pos().y() > boardSize_-1){
            snakeparts_.at(0)->setPos(QPoint(snakeparts_.at(0)->pos().x(), 0));
        }else{
            snakeparts_.at(0)->setPos(snakeparts_.at(0)->scenePos() + QPoint(0,1));
        }

    }else if(snakeDir_ == "LEFT"){
        if(snakeparts_.at(0)->pos().x() < 0){
            snakeparts_.at(0)->setPos(QPoint(boardSize_-1, snakeparts_.at(0)->pos().y()));
        }else{
            snakeparts_.at(0)->setPos(snakeparts_.at(0)->scenePos() + QPoint(-1,0));
        }

    }else if(snakeDir_ == "RIGHT"){
        if(snakeparts_.at(0)->pos().x() > boardSize_ - 1){
            snakeparts_.at(0)->setPos(QPoint(0, snakeparts_.at(0)->pos().y()));
        }else{
            snakeparts_.at(0)->setPos(snakeparts_.at(0)->scenePos() + QPoint(1,0));
        }
    }

    checkCollision();

}

void MainWindow::adjustSceneArea() {
    //Create area size
    const QRectF area(0, 0, boardSize_, boardSize_);
    scene_.setSceneRect(area);
    ui_.graphicsView->fitInView(area);
}

void MainWindow::checkHit()
{
    if(snakeparts_.at(0)->scenePos() == food_->scenePos()){
        addPart(); //Add new part for snake
        locateFood(); //Change food location so where else

        //If gamemode medium or hard, snake movement gets faster and faster
        if(gameMode_ != "EASY"){
            if(timerSpeed_ > 50){
                timerSpeed_ -= 20;
            }
        }
    }
}

void MainWindow::addPart()
{
    ++snakeSize_;
    const QRectF part_rect(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::red);

    //New GraphicsEllipseitem is done, which is also pushed to list
    QGraphicsEllipseItem* part = new QGraphicsEllipseItem();
    part = scene_.addEllipse(part_rect, pen, brush); //Adds to scene
    part->setPos(food_->scenePos()); //snakes new part position is old food pos.
    snakeparts_.push_back(part);

    colorGradient();

    checkWin();
}

void MainWindow::insertFood()
{
    const QRectF food_rect(0.1, 0.1, 0.7, 0.7);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::blue);
    food_ = scene_.addRect(food_rect, pen, brush);
    locateFood();
}

void MainWindow::locateFood()
{
    //distribution gives limits where it can be placed
    std::uniform_int_distribution<int> dist_xy(0, boardSize_ - 1);

    while(true){
        food_->setPos(dist_xy(rng_), dist_xy(rng_));
        if(!isInFreeSquare()){ // Check no snakepart under
            if(isObstacles_ == true){
                if(!isInFreeSquareObs()){ //Check no obstacle under
                    return;
                }
            }else{
                return;
            }
        }
    }
}

void MainWindow::checkCollision()
{
    bool collided = false;
    for(int i = 1; i < snakeSize_; ++i){
        if(snakeparts_.at(i)->scenePos() == snakeparts_.at(0)->scenePos()){
            collided = true;
        }
    }
    if(isObstacles_ == true){
        for(int i = 0; i < obstacles_.size(); ++i){
            if(snakeparts_.at(0)->scenePos() == obstacles_.at(i)->scenePos()){
                collided = true;
            }
        }
    }

    //If snakehead has been collided
    if(collided == true){
        printHiscores();
        onRestartButtonClicked();
    }else{
        checkHit();
    }

    //Comment out, if want to end game when head hits wall.
    /*if(snakeparts_.at(0)->pos().x() > boardSize_ || snakeparts_.at(0)->pos().x() < 0 ||
            snakeparts_.at(0)->pos().y() > boardSize_ || snakeparts_.at(0)->pos().y() < 0){
        timer_.stop();
    }*/
}

void MainWindow::checkWin()
{
    if(maxSnakeSize_ == snakeSize_){
        printHiscores();
        onRestartButtonClicked();
    }
}

bool MainWindow::isInFreeSquare()
{
    for(int i = 0; i < snakeparts_.size(); ++i){
        if(snakeparts_.at(i)->scenePos() == food_->scenePos()){
            return true;
        }
    }
    return false;
}


bool MainWindow::isInFreeSquareObs()
{
    for(int i = 0; i < obstacles_.size(); ++i){
        if(food_->scenePos() == obstacles_.at(i)->scenePos()){
            return true;
        }
    }
    return false;
}

void MainWindow::releaseGameMode()
{
    ui_.easyButton->setStyleSheet("background-color: rgb(255,0,0)");
    ui_.mediumButton->setStyleSheet("background-color: rgb(255,0,0)");
    ui_.hardButton->setStyleSheet("background-color: rgb(255,0,0)");
}

void MainWindow::createHead()
{
    const QRectF head_rect(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::red);

    QGraphicsEllipseItem* part = new QGraphicsEllipseItem();
    part = scene_.addEllipse(head_rect, pen, brush);
    part->setPos(5,5);
    part->setZValue(1); //Makes sure that head is always on top of everything
    snakeparts_.push_back(part);
    snakeSize_ = 1;
}

void MainWindow::printHiscores()
{
    scoreTimer_.stop();
    QString score;
    score += QString::number(minutes_);
    score += " min ";
    score += QString::number(seconds_);
    score += " sec , score: ";
    if(maxSnakeSize_ == snakeSize_){
        score += "WIN!";
    }else{
        score += QString::number(snakeSize_);
    }

    score += " level: " + gameMode_;

    ui_.scoreboardBrowser->append(score);
}

void MainWindow::colorGradient()
{
    for(int i = 1; i < snakeSize_; ++i){
        //Makes colour to go darker in the end of the snake
        snakeparts_.at(i)->setBrush(QColor(tailColor_/i,0,0));
    }
}

void MainWindow::createObstacles()
{
    int amountObstacles;
    if(gameMode_ == "MEDIUM"){
        amountObstacles = 2;
    }else{
        amountObstacles = 4;
    }

    const QRectF obs_rect(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(QColor(204,102,0));

    std::uniform_int_distribution<int> dist_xy(0, boardSize_ - 1);

    //Here we make 2 or 4 new QGraphicsRectItems which are added to list
    for(int i = 0; i < amountObstacles; ++i){
        QGraphicsRectItem* obs = new QGraphicsRectItem();
        obs = scene_.addRect(obs_rect, pen, brush);
        while(true){
            obs->setPos(dist_xy(rng_), dist_xy(rng_));
            if(!isInFreeSquare() && obs->scenePos() != food_->scenePos()){
                obstacles_.push_back(obs);
                break;
            }
        }
    }
}

void MainWindow::onEasyButtonClicked()
{
    releaseGameMode();
    ui_.easyButton->setStyleSheet("background-color: rgb(0,255,0)");
    gameMode_ = "EASY";
    timerSpeed_ = 1000;
    boardSize_ = defaultBoardSize_;
}

void MainWindow::onMediumButtonClicked()
{
    releaseGameMode();
    ui_.mediumButton->setStyleSheet("background-color: rgb(0,255,0)");
    gameMode_ = "MEDIUM";
    timerSpeed_ = 500;
    isObstacles_ = true;
    boardSize_ = defaultBoardSize_ * 2;
}

void MainWindow::onHardButtonClicked()
{
    releaseGameMode();
    ui_.hardButton->setStyleSheet("background-color: rgb(0,255,0)");
    gameMode_ = "HARD";
    timerSpeed_ = 200;
    isObstacles_ = true;
    boardSize_ = defaultBoardSize_ * 3;
}

void MainWindow::onPauseResumeButtonClicked()
{
    if(ui_.pauseResumeButton->text() == "Pause"){
        timer_.stop();
        scoreTimer_.stop();
        ui_.pauseResumeButton->setText("Resume");
    }else{
        timer_.start();
        scoreTimer_.start();
        ui_.pauseResumeButton->setText("Pause");
    }
}

void MainWindow::onRestartButtonClicked()
{
    //Works only if game is on
    if(gameOn_ == true){
        timer_.stop();

        //We delete all snakepart QGraphicsEllipseItems from scene.
        qDeleteAll(snakeparts_);
        snakeparts_.clear();

        delete food_;

        //Same as snakeparts, if obstacles are allowed
        if(isObstacles_ == true){
            qDeleteAll(obstacles_);
            obstacles_.clear();
        }

        //Rest of the function just clear everything to basic settings
        isObstacles_ = false;
        snakeDir_ = "LEFT";
        gameOn_ = false;
        snakeSize_ = 0;
        gameMode_ = "NONE";

        seconds_ = 0;
        minutes_ = 0;

        ui_.playButton->setDisabled(false);
        ui_.easyButton->setDisabled(false);
        ui_.mediumButton->setDisabled(false);
        ui_.hardButton->setDisabled(false);

        releaseGameMode();
    }

}

void MainWindow::clock(){
    if(seconds_ == 60){
        ++minutes_;
        seconds_ = 0;
    }else{
        ++seconds_;
    }
}
