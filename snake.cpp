#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <forward_list>

#define Radius 30.f
#define PointCount 4
#define Border_y_upper 0
#define Border_y_bottom 510
#define Border_x_right 870
#define Border_x_left 30

class Snake{
    sf::Sprite sprite;
    short duration, old_duration;

public:    
    Snake(const sf::Texture& texture) : sprite(texture), duration(1){
        sprite.setPosition({300.f, 300.f});
    }
    Snake(const short& duration, std::vector<Snake>& snake, const sf::Texture& texture) 
    : sprite(texture), duration(duration)
    {
        const float x_old = snake[snake.size()-1].sprite.getPosition().x, y_old = snake[snake.size()-1].sprite.getPosition().y;
        
        switch(duration){
            case 1:
                sprite.setPosition({x_old-Radius, y_old});
                break;
            case 2:
                sprite.setPosition({x_old+Radius, y_old});
                break;
            case 3:
                sprite.setPosition({x_old, y_old+Radius});
                break;
            case 4:
                sprite.setPosition({x_old, y_old-Radius});
                break;
        }
    }
    const sf::Sprite& get_sprite() const noexcept{
        return this->sprite;
    }
    short get_duration() const noexcept{
        return this->duration;
    }
    void set_duration(const short& duration) noexcept{
            this->duration = duration;
    }
    short get_old_duration()const noexcept{
        return old_duration;
    }
    void moving_sprite(short durationn) noexcept{
    switch(durationn){
        case 1:
            if(this->duration == 2) durationn = 2;
            break;
        case 2:
            if(this->duration == 1) durationn = 1;
            break;
        case 3:
            if(this->duration == 4) durationn = 4;
            break;
        case 4:
            if(this->duration == 3) durationn = 3;
            break;
    }
    old_duration = durationn;   
    this->duration = durationn;
    switch(this->duration){
        case 1:
        //  right
            if(Border_x_right > sprite.getPosition().x)
            {
                sprite.move({Radius, 0.f});
            }else{
                goto exiting;
            }
            break;
        case 2:
        // left
            if(Border_x_left < sprite.getPosition().x)
            {
                sprite.move({-Radius, 0.f});
            }else{
                goto exiting;
            }
            break;
        case 3:
        // up
            if(Border_y_upper < sprite.getPosition().y)
            {
                sprite.move({0.f, -Radius});
            }else{
                goto exiting;
            }
            break;
        case 4:
        //down
            if(Border_y_bottom > sprite.getPosition().y)
            {
                sprite.move({0.f, Radius});
            }else{
                goto exiting;
            }
            break;
            exiting: // debugging

            exit(1);
            //std::cerr <<"x = " <<sprite.getPosition().x << "\ty = "<< sprite.getPosition().y << "\n";
            
    }
} 
};
static int sz_list = 1;
class Board_Game{

    sf::RenderWindow window; 
    sf::CircleShape apple;
    short number = 1, number_milliseconds = 800;
    bool is_apple=true;
    std::vector<Snake> snake;
    uint pos_head_snake_x;
    uint pos_head_snake_y;
    uint pos_apple_x;
    uint pos_apple_y;
    sf::Texture texture, polyana;
    int round_pos(int& a) noexcept{
        while(a % 30 != 0) ++a;
        return a;
    }

public:
    Board_Game() 
    : window(sf::VideoMode({900, 600}), "Snake"), apple(Radius-15.f), texture("png/body.png")
      //polyana("png/1672015093_31-7fon-club-p-fon-dlya-igri-tanchiki-vid-sverkhu-41.jpg")
      {

        snake.emplace_back(texture);
        apple.setPosition({1000.f, 1000.f});

        window.setFramerateLimit(8);
        window.setMouseCursorVisible(false);
        //window.setTexture("png/baseHead.png");
        
    }
    void spawn_apple()noexcept{
        is_apple = false;
        
        while(true){
            bool is_pos = true;
            int ran_x=rand()%870, ran_y = rand()%510;
            ran_x = round_pos(ran_x); ran_y = round_pos(ran_y);
            for(int i = 0; i< sz_list; ++i){
                if(ran_x == snake[i].get_sprite().getPosition().x && ran_y == snake[i].get_sprite().getPosition().y){
                    is_pos = false;
                    break;
                }
            }
            if(is_pos){
                //std::cout << "apple spawn x2 == " << "\t x = " << ran_x << " " << "ran_y = " << ran_y << '\n';
                apple.setPosition({static_cast<float>(ran_x),static_cast<float>(ran_y)});
                
                apple.setFillColor(sf::Color(100, 250, 50));
                return;
            }
        }
    }
    void drawing_game()noexcept{
        sf::Sprite sprite(polyana);
        window.clear();
        window.draw(sprite);
        for(int i = 0; i < snake.size(); ++i)
        window.draw(snake[i].get_sprite());
        window.draw(apple);
        window.display();
    }
    void snake_moving_method()noexcept{
        snake[0].moving_sprite(number);
        
        for(int i = 1; i < snake.size(); ++i){
            snake[i].moving_sprite(snake[i].get_duration());
            snake[i].set_duration(snake[i-1].get_old_duration());
            // сходить. взять новое зн старого объекта. сходить
        }
    }
    void check_snake_on_a_apple()noexcept{
        pos_head_snake_x = snake[0].get_sprite().getPosition().x;
        pos_head_snake_y = snake[0].get_sprite().getPosition().y;
        pos_apple_x = apple.getPosition().x;
        pos_apple_y = apple.getPosition().y;
        // check snake on a apple
        if(
            (pos_head_snake_x == pos_apple_x || pos_head_snake_x == pos_apple_x + 30)
            && 
            (pos_apple_y == pos_head_snake_y || pos_apple_y + 30 == pos_head_snake_y)
            
            )
           {
                //std::cout << "apple eatting\n";
                is_apple = true;
                window.clear();
                Snake body(snake[snake.size()-1].get_duration(), snake, texture);
                snake.emplace_back(body);
           }
        
           // spawn a apple
        if(is_apple){
            spawn_apple();
            ++sz_list;
        }
    }
    void eventing_game()noexcept{
        while (const std::optional event = window.pollEvent())
        {    
            if (event->is<sf::Event::Closed>()){
                window.close();

            }else if(const auto* key_Pressed = event->getIf<sf::Event::KeyPressed>()){
                
                if(key_Pressed->scancode == sf::Keyboard::Scancode::Right){
                    number = 1;
                }else if(key_Pressed->scancode == sf::Keyboard::Scancode::Left){
                    number = 2;
                }else if(key_Pressed->scancode == sf::Keyboard::Scancode::Up){
                    number = 3;
                }else if(key_Pressed->scancode == sf::Keyboard::Scancode::Down){
                    number = 4;
                }                
            }
        }
    }
    void controll_game(){
        while (window.isOpen()) {
        
        eventing_game(); // события
            
        drawing_game(); // отрисовка
        
        snake_moving_method(); // перемещение

        check_snake_on_a_apple(); // еда

        //sf::sleep(sf::milliseconds(200));   
        //std::cout << snake[0].get_sprite().getPosition().x << '\t' << snake[0].get_sprite().getPosition().y << '\n';
    }
    }
    
};



int main() {


    Board_Game board;
    board.controll_game();

    
    return 0; 
}