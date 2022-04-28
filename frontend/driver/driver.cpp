#include "driver.hpp"

#include <fstream>
#include <iostream>

#include "InputHandler.hpp"

int yyFlexLexer::yywrap () { return 1; }

int main (int argc, char **argv) try
{
    InputHandler input;

    try {
        input.setParams (argc, argv);
    }
    catch (const std::runtime_error &err) {
        std::cout << "Input error: " << err.what () << std::endl;
        return 0;
    }

    std::streambuf *cinbuf = std::cin.rdbuf ();
    std::cin.rdbuf (input.code_.rdbuf ());

    yy::FrontendDriver driver (argv[1]);

    try {
        driver.parse ();
    }
    catch (std::runtime_error &err) {
        std::cout << err.what () << std::endl;
        return 0;
    }

    driver.callDump (std::cout);

    if (driver.isNoErrors ()) {
        driver.semantic ();
        driver.printWarnings ();
    }


    driver.printError ();

    std::cin.rdbuf (cinbuf);

    // return 0;

    if (driver.isNoErrors ()) 
        driver.interpret ();
}
catch (std::runtime_error &err) {
    std::cout << err.what() << std::endl;
}
