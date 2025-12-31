

#include "ui_level_1.h"
#include "level_1.h"

/** Default constructor for Level_1
 *  @param parent QWidget parent object of Level_1
 */
Level_1::Level_1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Level_1)
{
    //setup the ui elements from the ui file
    ui->setupUi(this);

    //Connect instructions/controls pushbutton to show_help()
    QObject::connect(ui->help_button, SIGNAL(clicked()), this, SLOT(show_help()));

    //create a new graphics scene to display
    scene = new QGraphicsScene(this);

    //set the graphics view to show this scene
    ui->level1_graphicsView->setScene(scene);

    //set framerate related parameters
    //numbers found through experimentation of what felt like the right play speed
    max_frames_per_second = 50;
    frames_per_second = 33;
    time_change_amount = 7;

    //set scene boundaries to those of the qgraphicsview
    boundary_margin = 5;

    int view_width = ui->level1_graphicsView->geometry().width()-boundary_margin;
    int view_height = ui->level1_graphicsView->geometry().height()-boundary_margin;

    scene->setSceneRect(0,0,view_width,view_height);

    //set our boolean values

    // first game hasn't yet been played
    first_game = true;

    // not yet started
    started=false;

    // set current level and current game to one
    current_level=1;
    current_game=1;

    // haven't just won a level
    won_level=false;

    // haven't yet beat entire game
    won_game=false;

    // time warp is bonus feature (in which ball moves faster when paddle moves right, slower when paddle moves left)
    time_warp=false;

    // create our paddle objects and add them to the scene
    paddle_top = new Paddle(170,25);
    paddle_bottom = new Paddle(170,440);
    //scene->addItem(paddle_top);
    scene->addItem(paddle_bottom);

    // create our ball object and add it to the scene
    ball = new Ball();
    scene->addItem(ball);

    // set losing lines (and draw them)
    set_losing_lines();

    // set boundaries (and draw them) for these scene, thus collisions against the sides will be detected
    set_boundaries();

    // create text item to display when beat entire game
    win_text = new QGraphicsTextItem;
    win_text->setPos(50,200);
    win_text->setPlainText("You won!!!!!!!");
    win_text->setScale(5);
}

/** Virtual destructor to delete ui
 */
Level_1::~Level_1()
{
    delete ui;
}

/** Draw the red lines representing losing.
 */
void Level_1::set_losing_lines()
{

    //Create pens to designate losing line
    QPen losing_line_pen = QPen(Qt::red);

    qreal bottom_y = (scene->sceneRect().bottomLeft().ry()-paddle_top->pos().y())+boundary_margin;
    qreal top_y = (scene->sceneRect().topLeft().ry()+paddle_top->pos().y())-boundary_margin;

    QPointF bottom_left_of_losing_line = QPointF(scene->sceneRect().bottomLeft().rx(),bottom_y);
    QPointF bottom_right_of_losing_line = QPointF(scene->sceneRect().bottomRight().rx(),bottom_y);

    QPointF top_left_of_losing_line = QPointF(scene->sceneRect().topLeft().rx(),top_y);
    QPointF top_right_of_losing_line = QPointF(scene->sceneRect().topRight().rx(),top_y);

    QLineF bottom_losing_line(bottom_left_of_losing_line, bottom_right_of_losing_line);
    QLineF top_losing_line(top_left_of_losing_line, top_right_of_losing_line);

    scene->addLine(bottom_losing_line, losing_line_pen);
    scene->addLine(top_losing_line, losing_line_pen);

}

/** Draws the boundaries of the current scene so collisions against the boundaries can be detected.
 */
void Level_1::set_boundaries()
{
    //Create pens to designate "type" of boundary
    QPen losing_boundary_pen = QPen(Qt::red);
    QPen side_boundary_pen = QPen(Qt::blue);

    //Draw top and bottom boundaries of scene
    QLineF top_line(scene->sceneRect().topLeft(), scene->sceneRect().topRight());
    QLineF bottom_line(scene->sceneRect().bottomLeft(), scene->sceneRect().bottomRight());

    scene->addLine(top_line, losing_boundary_pen);
    scene->addLine(bottom_line, losing_boundary_pen);

    //Draw left and right sides of scene
    QLineF left_line(scene->sceneRect().topLeft(), scene->sceneRect().bottomLeft());
    QLineF right_line(scene->sceneRect().topRight(), scene->sceneRect().bottomRight());

    scene->addLine(left_line, side_boundary_pen);
    scene->addLine(right_line, side_boundary_pen);

}

/** If haven't beaten current game yet, resets the ball, paddles, and sets up new bricks for the next level
 */
void Level_1::next_level()
{

}

/** Resets the current level. Destroys remaining bricks and sets up level just like next_level(). (resets ball, paddles, and sets up new bricks)
 */
void Level_1::reset_level()
{
    //reset 'started' so can start again
    started=false;

    //reset ball
    scene->removeItem(ball);
    delete ball;
    ball = new Ball();
    scene->addItem(ball);

    //reset paddles
    //paddle_top->reset_powerups();
    paddle_bottom->reset_powerups();

    //clear existing bricks

    // get an iterator for iterating through the list
    QList<Brick*>::iterator bricks_iterator = bricks.begin();

    while(bricks_iterator!=bricks.end())
    {
        // scene no longer has ownership, doesn't call delete
        scene->removeItem(*bricks_iterator);

        // erase from QList bricks, return the iterator at the new correct position
        // does call delete
        delete (*bricks_iterator);
        bricks_iterator = bricks.erase(bricks_iterator);
    }

}

/** Notifies the user of beating the game and starts a new game.
 */
void Level_1::beat_game()
{   

}

/** Starts the game over from level 1 with a faster timer.
 */
void Level_1::new_game()
{

}

/** Connects functions for handling animations, collisions, and checking for win/loss to a timer. Starts that timer.
 */
void Level_1::start_level()
{
    paused=false;

    //Connect timer to handle animations (done by the advance function in QGraphicsScene)
    QObject::connect(&timer, SIGNAL(timeout()), scene, SLOT(advance()));

    //Connect timer to handle collisions
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(handle_collisions()));

    //Connect timer to check for loss
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(check_for_loss()));

    //Connect timer to check for win
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(check_for_win()));

    //start the timer, updating at frames_per_second times per second
    timer.start(1000 / frames_per_second);
}

/** Stops the timer game functions are connected to
 */
void Level_1::pause_level()
{
    timer.stop();
    paused=true;
}

/** Starts the timer game functions are connected to
 */
void Level_1::unpause_level()
{
    //don't allow pausing/unpausing before starting
    if(!started)
        return;

    //don't allow pausing/unpausing if beat game
    if(won_game)
        return;

    timer.start();
    paused=false;
}

/** Decreases speed of timer game functions are connected to.
 */
void Level_1::slow_time()
{
    //decrease the speed of the clock
    frames_per_second -= time_change_amount;
    timer.start(1000/frames_per_second);

    //stop the clock
    timer.stop();
    paused=true;
}

/** Increases speed of timer game functions are connected to up to a predetermined maximum.
 */
void Level_1::speed_up_time()
{
    //don't go above the maximum framerate
    if(frames_per_second >= max_frames_per_second)
        return;

    //increase the speed of the timer
    frames_per_second += time_change_amount;
    timer.start(1000/frames_per_second);

    //stop the clock
    timer.stop();
    paused=true;
}


/** Checks all the currently existing bricks to see if they've been hit. If they have, it removes them from 'bricks', the scene, and deletes them.
 */
void Level_1::handle_collisions()
{

}

/** If ball is above top paddle or below bottom paddle, calls user_lost() to handle a loss.
 */
void Level_1::check_for_loss()
{
    if( ball->pos().y() > paddle_bottom->pos().y())// ||  ball->pos().y() < paddle_top->pos().y())
        user_lost();
}


/** If there are no bricks left, calls user_won() to handle a win.
 */
void Level_1::check_for_win()
{

}


/** Handle a loss. Pause level, notify user of loss, wait for user acknowledgement, and reset the level.
 */
void Level_1::user_lost()
{
    //stop the game while we handle the loss
    pause_level();

    //Prompt user to take action on loss
    QMessageBox::information(this, tr("Double Brick Breaker"),tr("Loss :( \n Press 'Okay' to try again or exit the program to quit."));

    //If the user pressed okay, set up the same level again
    reset_level();
}

/** Handle a win. Pause level, notify user of win, wait for user acknowledgement, and setup the next level.
 */
void Level_1::user_won()
{

}

/** Outputs to QDebug for debugging purposes. Can be filled with any QDebug output calls.
 */
void Level_1::print_debug()
{
    //DEBUG
    qDebug()<<"Ball position is: "<<ball->pos().y();
    //qDebug()<<"Top paddle position is: "<<paddle_top->pos().y();
    qDebug()<<"Bottom paddle position is: "<<paddle_bottom->pos().y();
}

/** Slot connected to keyPressEvents. Catches QKeyEvent's and processes them. Essentially responsible for handling keyboard input.
    @param key_event QKeyEvent of recent keyboard input by the player
 */
void Level_1::keyPressEvent(QKeyEvent* key_event)
{

    //use key 'a' for moving both paddles left
    if((key_event->key() == Qt::Key_A) && !paused)
    {
        //don't allow movement before starting
        if(!started)
            return;

        //move both paddles left
        //paddle_top->move_left();
        paddle_bottom->move_left();

        //if time warp mode is enabled, the ball moves slower the further left the paddle gets
        if(time_warp)
            ball->slower();
    }

    //use key 'd' for moving both paddles right
    if((key_event->key() == Qt::Key_D) && !paused)
    {
        //don't allow movement before starting
        if(!started)
            return;

        //move both paddles right
        //paddle_top->move_right();
        paddle_bottom->move_right();

        //if time warp mode is enabled, the ball moves faster the further right the paddle gets
        if(time_warp)
            ball->faster();

    }

    //toggle paused with the 'P' key
    if(key_event->key() == Qt::Key_P)
    {
        if(paused)
            unpause_level();
        else
            pause_level();
    }

    //start the game with the 'Space' key
    if(key_event->key() == Qt::Key_Space)
    {
        //start the game
        if(!started)
        {
            //set started to true because just started the game
            started=true;

            //won_level false because starting a new level which we have not yet won
            won_level=false;

            //if it's the first game, make all the connections
            if(first_game)
            {
                this->start_level();
                first_game=false;
            }
            //otherwise just unpause the level
            else
                unpause_level();

            //launch the ball to start the game
            ball->launch_ball();
        }
    }
}
