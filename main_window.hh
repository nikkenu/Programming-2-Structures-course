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
# File: main_window.hh                                             #
# Description: Declares a class implementing a UI for the game.    #
#                                                                  #
# Author: Niklas Nurminen, 283756, niklas.nurminen@tuni.fi         #
####################################################################
*/

#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include <random>


/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */
    void keyPressEvent(QKeyEvent* event) override;


private slots:

    /* \brief Start the game.
     */
    void onPlayButtonClicked();

    /* \brief Move the Snake by a square and check for collisions.
     *
     * The game ends if a wall or the Snake itself gets in the way.
     * When a food gets eaten a point is gained and the Snake grows.
     */
    void moveSnake();

    /* \brief onEasyButtonClicked
     * Changes game settings to be "easy"
     * Should be called at start of new game
     */
    void onEasyButtonClicked();

    /* \brief onMediumButtonClicked
     * Changes game settings to be "medium"
     * Should be called at start of new game
     */
    void onMediumButtonClicked();

    /* \brief onHardButtonClicked
     * Changes game settings to be "hard"
     * Should be called at start of new game
     */
    void onHardButtonClicked();

    /* \brief onPauseResumeButtonClicked
     * Can pause the game
     * Should be clicked when want to pause the game and take rest
     */
    void onPauseResumeButtonClicked();

    /* \brief onRestartButtonClicked
     * Initializes the game and start immediately new game
     * Should be called when want to restart or game ends
     */
    void onRestartButtonClicked();

    /* \brief clock
     * Clock is used to check how long user has been playing that round
     * Call always when new game starts
     */
    void clock();

private:

    /* \brief Make the play field visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */
    void adjustSceneArea();

    /* \brief checkHit
     * Checks if snake head has same position than food has.
     * Should be called everytime snakemoves
     */
    void checkHit();

    /* \brief addPart
     * Makes new part for snake
     * Should be called every time snake's head eats food
     */
    void addPart();

    /* \brief insertFood
     * Adds QGraphicsRectItem to scene
     * Call when start the game
     */
    void insertFood();

    /* \brief locateFood
     * Change foods spot if snake's head eats it
     * Call everytime when eated
     */
    void locateFood();

    /* \brief checkCollision
     * Check if snakehead hits own body or obstacle
     * Call everytime that snakemoves
     */
    void checkCollision();

    /* \brief checkWin
     * Compare snakesize to max snakesize
     * Call everytime that snake grows
     */
    void checkWin();

    /* \brief releaseGameMode
     *  Changes easy, medium and hard buttons to default
     *  Call everytime game ends
     */
    void releaseGameMode();

    /* \brief createHead
     * Makes QGraphicsEllipseItem
     * Call when new game starts
     */
    void createHead();

    /* \brief printHiscores
     * Appends game's score to textBrowser
     * Should be called every time the game ends.
     */
    void printHiscores();

    /* \brief colorGradient
     * Makes snakebody darker to end
     * Should be called every time the snake grows
     */
    void colorGradient();

    /* \brief createObstacles
     * Makes obstacles to board to make it harder
     * Should be called at the start if game mode is right
     */
    void createObstacles();

    /* \brief isInFreeSquare
     * Checks that food doesnt have the same position that snakebody
     * Should be called every time when move food or make obstacle
     */
    bool isInFreeSquare();

    /* \brief isInFreeSquareObs
     *  Checks that food doesnt have the same position that obstacle
     *  Should be called every time when move food
     */
    bool isInFreeSquareObs();

    int snakeSize_;                             /**< Snakes size. */
    int timerSpeed_;                            /**< Update time */
    int boardSize_;                             /**< Max boardsize. */
    int maxSnakeSize_;                          /**< max size for snake */
    int seconds_ = 0;                           /**< Seconds that user has spend. */
    int minutes_ = 0;                           /**< Minutes that user has spend */

    bool isObstacles_ = false;                  /**< Obstacles or not */
    bool gameOn_ = false;                       /**< Is game on?. */
    QString gameMode_;                          /**< Easy, medium or hard difficulty level */

    const int tailColor_ = 255;                 /**< To help change tail colours depending snakes lenght. */
    const int defaultBoardSize_ = 10;           /**< Default board size width, height */

    QString snakeDir_;                          /**< Snakes direction at the moment. */
    QList<QGraphicsEllipseItem*> snakeparts_;   /**< List that includes snakeparts */
    QList<QGraphicsRectItem*> obstacles_;       /**< List that includes obstacles. */

    QGraphicsTextItem* win_ = nullptr;
    Ui::MainWindow ui_;                         /**< Accesses the UI widgets. */
    QGraphicsRectItem* food_ = nullptr;         /**< The food item in the scene. */
    QGraphicsScene scene_;                      /**< Manages drawable objects. */
    QTimer timer_;                              /**< Triggers the Snake to move. */
    QTimer scoreTimer_;                         /**< Triggers clock to update used time */
    std::default_random_engine rng_;            /**< Randomizes food locations. */

};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
