#include <iostream>

#include "wood.hpp"

//xlm2json
//json2xml

int main( int argc, const char **argv )
{
    {
        wood::xml xml;

        xml.
            push("root").
                set("integer", 13).
                set("float", 3.14159f).
                set("string", "hello world").
            back();

        std::cout << xml.str() << std::endl;
    }

    {
        wood::xml xml;

        xml.
            push("bookstore").
                push("book").
                    set("title", "Harry Potter").
                        attrib("lang", "eng").
                    set("author", "J K. Rowling").
                    set("year", 2005).
                    set("price", 29.99).
                back().
                push("book").
                    set("title", "Robopocalypse").
                        attrib("lang", "eng").
                        attrib("lang", "spa").
                    set("author", "Daniel H. Wilson").
                    set("year", 2011).
                    set("price", 19.99).
                back().
            back();

        std::cout << xml.str() << std::endl;

        do {
            std::cout << "Enter XPath query: ";

            std::string s;
            std::getline(std::cin, s);

            std::cout << "XPath query() result:" << std::endl;
            std::cout << xml.get<std::string>(s) << std::endl;
        }
        while(1);
    }

    {
        wood::xml networks;

        networks.
            set( 1, "Grupo" ).
            push( 2, "Europa" ).
                set( 3, "UK" ).
                push( 4, "ES" ).
                    set( 5, "AND" ).
                    set(6, "GAL" ).
                    set( 7, "MAD" ).
                    back().
                set( 8, "IT" ).
                set( 9, "FR" ).
                back().
            push( 10, "America" ).
                set( 11, "AR" ).
                set( 12, "BR" ).
                set( 13, "CL" ).
                set( 14, "CO" ).
                set( 15, "MX" ).
                back().
            push( 16, "Asia" ).
                set( 17, "CH" ).
                back().
            set( 18, "Resto" );

        std::cout << networks.str() << std::endl;
    }

    return 0;
}

#if 0

[ref] http://www.w3schools.com/xpath

<bookstore>
    <book>
      <title>Harry Potter</title>
      <author>J K. Rowling</author>
      <year>2005</year>
      <price>29.99</price>
    </book>
</bookstore>

// parent: _book_ is parent of { title, author, year, price }
// children: { title, author, year, price } are children of _book_
// siblings: { title, author, year, price } are siblings
// ancestors: { book, bookstore } are ancestors of _title_
// descendants: { book, title, author, year, price } are descendants of _bookstore_

                            [SELECTING NODES]
nodename                            Selects all nodes with the name "nodename"
/                                   Selects from the root node
//                                  Selects nodes in the document from the current node that match the selection no matter where they are
.                                   Selects the current node
..                                  Selects the parent of the current node
@                                   Selects attributes

bookstore                           Selects all nodes with the name "bookstore"
/bookstore                          Selects the root element bookstore
bookstore/book                      Selects all book elements that are children of bookstore
//book                              Selects all book elements no matter where they are in the document
bookstore//book                     Selects all book elements that are descendant of the bookstore element, no matter where they are under the bookstore element
//@lang                             Selects all attributes that are named lang

                              [PREDICATES]
/bookstore/book[1]                  Selects the first book element that is the child of the bookstore element.
/bookstore/book[last()]             Selects the last book element that is the child of the bookstore element
/bookstore/book[last()-1]           Selects the last but one book element that is the child of the bookstore element
/bookstore/book[position()<3]       Selects the first two book elements that are children of the bookstore element
//title[@lang]                      Selects all the title elements that have an attribute named lang
//title[@lang='eng']                Selects all the title elements that have an attribute named lang with a value of 'eng'
/bookstore/book[price>35.00]        Selects all the book elements of the bookstore element that have a price element with a value greater than 35.00
/bookstore/book[price>35.00]/title  Selects all the title elements of the book elements of the bookstore element that have a price element with a value greater than 35.00

                        [SELECTING UNKNOWN NODES]
*                                   Matches any element node
@*                                  Matches any attribute node
node()                              Matches any node of any kind

/bookstore/*                        Selects all the child nodes of the bookstore element
//*                                 Selects all elements in the document
//title[@*]                         Selects all title elements which have any attribute

                        [SELECTING SEVERAL PATHS]
//book/title | //book/price         Selects all the title AND price elements of all book elements
//title | //price                   Selects all the title AND price elements in the document
/bookstore/book/title | //price     Selects all the title elements of the book element of the bookstore element AND all the price elements in the document

                                [AXES]
ancestor                            Selects all ancestors (parent, grandparent, etc.) of the current node
ancestor-or-self                    Selects all ancestors (parent, grandparent, etc.) of the current node and the current node itself
attribute                           Selects all attributes of the current node
child                               Selects all children of the current node
descendant                          Selects all descendants (children, grandchildren, etc.) of the current node
descendant-or-self                  Selects all descendants (children, grandchildren, etc.) of the current node and the current node itself
following                           Selects everything in the document after the closing tag of the current node
following-sibling                   Selects all siblings after the current node
namespace                           Selects all namespace nodes of the current node
parent                              Selects the parent of the current node
preceding                           Selects all nodes that appear before the current node in the document, except ancestors, attribute nodes and namespace nodes
preceding-sibling                   Selects all siblings before the current node
self                                Selects the current node

child::book                         Selects all book nodes that are children of the current node
attribute::lang                     Selects the lang attribute of the current node
child::*                            Selects all element children of the current node
attribute::*                        Selects all attributes of the current node
child::text()                       Selects all text node children of the current node
child::node()                       Selects all children of the current node
descendant::book                    Selects all book descendants of the current node
ancestor::book                      Selects all book ancestors of the current node
ancestor-or-self::book              Selects all book ancestors of the current node - and the current as well if it is a book node
child::*/child::price               Selects all price grandchildren of the current node

#endif
