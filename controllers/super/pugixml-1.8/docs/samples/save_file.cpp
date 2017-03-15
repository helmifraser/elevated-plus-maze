#include "pugixml.hpp"

#include <iostream>

int main()
{
    // get a test document
    pugi::xml_document doc;
    doc.load_string("<foo bar='CHENADKJSALJD'>heyn");
    doc.load_string("<hello bar1='baz'>helmi</hello></foo>");

    // tag::code[]
    // save document to file
    std::cout << "Saving result: " << doc.save_file("new.xml") << std::endl;
    // end::code[]
}

// vim:et
