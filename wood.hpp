/*
 * XML DOM parser/XPath queries/tree builder class

 * Copyright (c) 2010 Mario 'rlyeh' Rodriguez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

 * To do:
 * - SAX parser
 * - subtree insertion, removal and reallocation (doable thru XPath syntax?)

 * - rlyeh ~~ listening to The Cure / A forest (live 1984)
 */

#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "kptree/tree.hh"
#include "kptree/tree_util.hh"
#include "pugixml/pugixml.hpp"

namespace wood
{
    template< class T >
    class tree : public kptree::tree<T>
    {
        public:

        typedef typename kptree::tree<T>::iterator iterator;
        //typedef typename kptree::tree<T>::const_iterator const_iterator;

        typedef typename kptree::tree<T>::sibling_iterator sibling_iterator;
        //typedef typename kptree::tree<T>::const_sibling_iterator const_sibling_iterator;

        protected:

        iterator root, cursor;
        std::vector< iterator > depth;

        public:

        tree( const T &root_item = T() )
        {
            root = this->insert( this->begin(), root_item );
            depth.push_back( root );

            cursor = root;
        }

        bool find( const T &name )
        {
            return ( cursor = std::find( this->begin(), this->end(), name ) ) != this->end();
        }

        typename tree<T>::iterator found() const
        {
            return cursor;
        }

        typename tree<T>::iterator at( const T &name )
        {
            return std::find( this->begin(), this->end(), name );
        }

        tree<T>& push( const T &name )
        {
            depth.push_back( this->append_child( depth.back(), name ) );
            return *this;
        }
        tree<T>& node( const T &name )
        {
            this->append_child( depth.back(), name );
            return *this;
        }
        tree<T>& back()
        {
            if( depth.size() > 1 ) depth.pop_back();
            return *this;
        }

        void move( const T &from_id, const T &to_id )
        {
            iterator from = std::find( this->begin(), this->end(), from_id );
            iterator to   = std::find( this->begin(), this->end(), to_id );

            if( parent(from) == to )
                return; // already there

            iterator temp = this->append_child( to );

            this->move_ontop( temp, from );
        }

        std::string str() const
        {
            std::ostringstream oss;
            kptree::print_tree_bracketed( *this, oss );
            return oss.str();
        }

        // apply function to all nodes, including children
        // example:
        // tree<node> scene;
        // scene.apply( [](node &n){ render(n); } );

        template< typename FUNC >
        void apply( const FUNC &f )
        {
            if( !this->empty() )
                for( sibling_iterator it = this->begin(), end = this->end(); it != end; ++it )
                    apply_subtree( it, f );
        }

        protected:

        template< typename FUNC >
        void apply_subtree( const typename tree<T>::sibling_iterator &iRoot, const FUNC &f )
        {
            f( (*iRoot) );

            if( this->number_of_children(iRoot) != 0 )
                for( sibling_iterator it = this->begin(iRoot), end = this->end(iRoot); it != end; ++it )
                    apply_subtree( it, f );
        }
    };

    class string : public std::string
    {
        public:

        // basic constructors

        string() : std::string()
        {}

        string( const std::string &s ) : std::string( s )
        {}

        string( const char &c ) : std::string( 1, c )
        {}

        string( const char *cstr ) : std::string( cstr ? cstr : "" )
        {}

        template<size_t N>
        string( const char (&cstr)[N] ) : std::string( cstr )
        {}

        string( const bool &val ) : std::string( val ? "true" : "false" )
        {}

        // constructor sugars

        template< typename T >
        string( const T &t ) : std::string() {
            std::stringstream ss;
            if( ss << /* std::boolalpha << */ t )
                this->assign( ss.str() );
        }

        string( const float &t ) : std::string() {
            std::stringstream ss;
            if( ss << (long double)(t) )
                this->assign( ss.str() );
        }

        string( const double &t ) : std::string() {
            std::stringstream ss;
            if( ss << (long double)(t) )
                this->assign( ss.str() );
        }

        string( const long double &t ) : std::string() {
            std::stringstream ss;
            if( ss << (long double)(t) )
                this->assign( ss.str() );
        }

        // conversion

        template< typename T >
        inline T as() const {
            T t;
            if( std::istringstream(*this) >> t )
                return t;
            bool is_true = this->size() && (*this != "0") && (*this != "false");
            return (T)(is_true);
        }

        template<>
        inline char as() const {
            return this->size() == 1 ? (char)(this->operator[](0)) : (char)(as<int>());
        }
        template<>
        inline signed char as() const {
            return this->size() == 1 ? (signed char)(this->operator[](0)) : (signed char)(as<int>());
        }
        template<>
        inline unsigned char as() const {
            return this->size() == 1 ? (unsigned char)(this->operator[](0)) : (unsigned char)(as<int>());
        }

        template<>
        inline const char *as() const {
            return this->c_str();
        }
        template<>
        inline std::string as() const {
            return *this;
        }

        template< typename T >
        operator T() const {
            return as<T>();
        }

        // assignment sugars

        template< typename T >
        string &operator=( const T &t ) {
            this->assign( string(t) );
            return *this;
        }
    };

    class xml
    {
        public:

        enum { verbose = false };

        xml();
        xml( const xml &t );
        xml( const std::string &t );
        xml &operator=( const xml &t );

        // declarative construction

        // push a level of depth, then set node
        xml &push( const wood::string &key, const wood::string &text = wood::string(), const wood::string &comment = wood::string() );
        xml &push( const pugi::xml_node &node );

        // set node at current depth
        xml &set( const wood::string &key, const wood::string &text = wood::string(), const wood::string &comment = wood::string() );
        xml &set( const pugi::xml_node &node );

        // set attribute for current node
        xml &attrib( const wood::string &key, const wood::string &value );
        xml &attrib( /*const*/ pugi::xml_attribute &attribute );

        // pop a level of depth back
        xml &back();

        // XPath query api

        template <typename T>
        bool get( T &t, const std::string &XPath ) const
        {
#ifdef PUGIXML_NO_EXCEPTIONS
            wood::string result = pugi::xpath_query( XPath.c_str() ).evaluate_string( doc );
            t = result.as<T>();
            return true;
#else
            try
            {
                std::string result = pugi::xpath_query( XPath.c_str() ).evaluate_string( doc );
                t = result.as<T>();
                return true;
            }
            catch( const pugi::xpath_exception &e )
            {
                std::cerr << "<wood/xml.hpp> says: error! Select failed: " << e.what() << std::endl;
            }
            catch( ... )
            {
                std::cerr << "<wood/xml.hpp> says: error! Exception raised!" << std::endl;
            }
            t = T();
            return false;
#endif
        }

        template <typename T>
        T get( const std::string &XPath ) const {
            T t;
            return get(t, XPath) ? t : T();
        }

        // XPath select api

        typedef pugi::xpath_node_set selection;

        selection select( const std::string &xpath ) const {
            return doc.select_nodes( xpath.c_str() );
        }

        // serialization

        std::string str() const;
               void str( const std::string &data );

        // iteration
        // iterators

        typedef pugi::xml_node               node;
        typedef std::vector<node>            nodes;
        typedef pugi::xml_node_iterator      iterator; //iterators
        typedef pugi::xml_attribute          attribute; //attributes
        typedef pugi::xml_attribute_iterator attribute_iterator; //attribute_iterators

                  iterator begin() const;
                  iterator end() const;
        attribute_iterator attribute_begin() const;
        attribute_iterator attribute_end() const;

#if 0

        template< typename FUNC >
        void apply( const FUNC &f ) const
        {
            auto root = doc.root();
            for( auto it = root.first_child(); it; it = it.next_sibling() )
                apply_subtree( it, f );
        }

        template< typename FUNC >
        void apply( const FUNC &f )
        {
            auto root = doc.root();
            for( auto it = root.first_child(); it; it = it.next_sibling() )
                apply_subtree( it, f );
        }

        protected:

        template< typename FUNC >
        void apply_subtree( node &it, const FUNC &f ) const
        {
            f( it );

            // auto node = it;
            // int number_of_children = std::distance(node.begin(), node.end()));

            for( auto jt = it.first_child(); jt; jt = jt.next_sibling() )
                apply_subtree( jt, f );
        }

        template< typename FUNC >
        void apply_subtree( node &it, const FUNC &f )
        {
            f( it );

            // auto node = it;
            // int number_of_children = std::distance(node.begin(), node.end()));

            for( auto jt = it.first_child(); jt; jt = jt.next_sibling() )
                apply_subtree( jt, f );
        }

#endif

#if 1

    public:

        // xpath examples:
        // "//*" whole tree
        // "/Profile/Tools/Tool[@AllowRemote='true' and @DeriveCaptionFrom='lastparam']"
        // "//Tool[contains(Description, 'build system')]"
        // "//book[position()<=3]"  Selects the first three book elements
        // "//book[last()]" Selects the last book element

        /*
        <root>
            <Dev>
                <Emp>1</Emp>
                <Floor>1</Floor>
                <Salary>1200.4</Salary>
            </Dev>
            <Dev>
                <Emp>2</Emp>
                <Salary>3100.8</Salary>
            </Dev>
            <Dev>
                <Emp>3</Emp>
                <Floor>1</Floor>
            </Dev>
        </root>
        */
        // "sum(/*/Dev/Salary[number(.) = number(.)])" sums all /dev/salary that are numeric

        // Apply a lambda top-down to a region selected by xpath query

        template< typename FUNC >
        void forward( const std::string &xpath, const FUNC &fn ) const
        {
            auto selected = select( xpath );
            for( auto &it : selected )
                fn( it.node() );
        }

        template< typename FUNC >
        void forward( const std::string &xpath, const FUNC &fn )
        {
            auto selected = select( xpath );
            for( auto &it : selected )
                fn( it.node() );
        }

        // Apply a lambda bottom-up to a region selected by xpath query

        template< typename FUNC >
        void backward( const std::string &xpath, const FUNC &fn ) const
        {
            auto selected = select( xpath );
            for( size_t i = selected.size(); i-- > 0; )
                fn( selected[i].node() );
        }

        template< typename FUNC >
        void backward( const std::string &xpath, const FUNC &fn )
        {
            auto selected = select( xpath );
            for( size_t i = selected.size(); i-- > 0; )
                fn( selected[i].node() );
        }

#endif

        protected:

        //details:

public:
        pugi::xml_document doc;
protected:
        pugi::xml_node latest;
        std::vector< pugi::xml_node > parent;

        void update_node( pugi::xml_node &node, const std::string &key, const std::string &text, const std::string &comment);
        void update_node( pugi::xml_node &node, const pugi::xml_node &content );
    };

      bool is_empty( const xml::node &node );
      bool is_null( const xml::node &node );

      bool has_parent( const xml::node &node );
    size_t has_children( const xml::node &node );
    size_t has_siblings( const xml::node &node );

    xml::nodes get_children( const xml::node &node );
    xml::nodes get_siblings( const xml::node &node );

} //namespace wood

namespace
{
    template <typename T>
    inline std::ostream &operator<<( std::ostream &oss, const wood::tree<T> &scene ) {
        return oss << scene.str(), oss;
    }
}
