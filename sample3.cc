#include <cassert>
#include <string>
#include <iostream>

#include "wood.hpp"

int main( int argc, const char **argv )
{
    {
        wood::tree<std::string> the_scene("/");

        the_scene.
            push( "box" ).
                node( "1a" ).
                node( "2a" ).
                node( "2b" ).
                node( "player1" ).
                back().
            node( "player2" );

        assert( the_scene.str() == "/(box(1a, 2a, 2b, player1), player2)" );

        the_scene.move("player1", "/");
        assert( the_scene.str() == "/(box(1a, 2a, 2b), player2, player1)" );

        the_scene.move("player1", "/");
        assert( the_scene.str() == "/(box(1a, 2a, 2b), player2, player1)" );
    }

    {
        wood::tree<int> scene(0);

        scene.
            push( 1 ).
                node( 2 ).
                    node( 3 ).
                    back().
                back().
            node( 4 );

        // parse all nodes adding them to global sumatory

        int sum = 0;
        scene.apply( [&sum](int &x){ sum += x; } );

        assert( sum == 10 );
    }

    {
        wood::tree<std::string> scene("/");

        scene.
            push("fruits").
                node("lemon").
                node("orange").
            back().
            push("numbers").
                node("0").
                node("1").
                node("2").
            back();

        assert(  scene.str() == "/(fruits(lemon, orange), numbers(0, 1, 2))" );

        assert(  scene.find("0") );
        assert(  scene.find("1") );
        assert(  scene.find("2") );
        assert( !scene.find("3") );
    }

    std::cout << "All ok." << std::endl;
    return 0;
}
