#include <iostream>
#include <vector>
#include <sstream>

#include "wood.hpp"

#include "pugixml/pugixml.hpp"
#include "pugixml/pugixml.cpp"

namespace wood
{

xml::xml()
{}

xml::xml( const xml &t )
{
    str( t.str() );
}

xml::xml( const std::string &t )
{
    str( t );
}

xml &xml::operator=( const xml &t )
{
    doc.reset();
    parent = std::vector< pugi::xml_node >();
    latest = pugi::xml_node();

    str( t.str() );
    return *this;
}

// declarative construction

// push a level of depth, then insert node
xml &xml::push( const wood::string &key, const wood::string &text, const wood::string &comment )
{
    if( latest.empty() || (!parent.size()) )
    {
        if( verbose )
            std::cout << "push new" << std::endl;

        latest = doc.append_child( pugi::node_element );
    }
    else
    {
        if( verbose )
            std::cout << "push" << std::endl;

        latest = parent.back().append_child( pugi::node_element );
    }

    update_node( latest, key, text, comment );

    parent.push_back( latest );

    return (*this);
}

// push a level of depth, then insert node
xml &xml::push( const pugi::xml_node &node )
{
    if( latest.empty() || (!parent.size()) )
    {
        if( verbose )
            std::cout << "push new" << std::endl;

        latest = doc.append_child( pugi::node_element );
    }
    else
    {
        if( verbose )
            std::cout << "push" << std::endl;

        latest = parent.back().append_child( pugi::node_element );
    }

    update_node( latest, node );

    parent.push_back( latest );

    return (*this);
}

// set node at current depth
xml &xml::set( const wood::string &key, const wood::string &text, const wood::string &comment )
{
    if( latest.empty() || (!parent.size()) )
    {
        if( verbose )
            std::cout << "node new" << std::endl;

        latest = doc.append_child( pugi::node_element );
    }
    else
    {
        if( verbose )
            std::cout << "node" << std::endl;

        latest = parent.back().append_child( pugi::node_element );
    }

    update_node( latest, key, text, comment );

    return (*this);
}

// set node at current depth
xml &xml::set( const pugi::xml_node &node )
{
    if( latest.empty() || (!parent.size()) )
    {
        if( verbose )
            std::cout << "node new" << std::endl;

        latest = doc.append_child( pugi::node_element );
    }
    else
    {
        if( verbose )
            std::cout << "node" << std::endl;

        latest = parent.back().append_child( pugi::node_element );
    }

    update_node( latest, node );

    return (*this);
}

// set attribute for current node
xml &xml::attrib( const wood::string &key, const wood::string &value )
{
    if( verbose )
        std::cout << "set attribute " << key << " = " << value << std::endl;

    latest.append_attribute( key.c_str() ).set_value( value.c_str() );

    return (*this);
}

// set attribute for current node
xml &xml::attrib( /*const*/ attribute &_attribute )
{
    if( verbose )
        std::cout << "set attribute " << _attribute.name() << " = " << _attribute.value() << std::endl;

    latest.append_attribute( _attribute.name() ).set_value( _attribute.value() );

    return (*this);
}

// pop a level of depth back
xml &xml::back()
{
    if( verbose )
        std::cout << "back" << std::endl;

    if( parent.size() )
        parent.pop_back();

    if( parent.size() )
        latest = parent.back();
    else
        latest = pugi::xml_node();

    return (*this);
}

// serialization

std::string xml::str() const
{
    std::stringstream data;

    doc.save( data );

    if( verbose )
        std::cout << data.str() << std::endl;

    return data.str();
}

void xml::str( const std::string &data )
{
    pugi::xml_parse_result result = doc.load_buffer( data.c_str(), data.size() );
}

void xml::update_node( pugi::xml_node &node, const std::string &key, const std::string &text, const std::string &comment)
{
    node.set_name( key.c_str() );

    if( comment != "" )
        //node.prepend_child( ... )
        node.parent().insert_child_before(pugi::node_comment, node).set_value( comment.c_str() );

    if( text != "" )
        node.append_child(pugi::node_pcdata).set_value( text.c_str() );
}

void xml::update_node( pugi::xml_node &node, const pugi::xml_node &content )
{
    node.set_name( content.name() );

    if( content.value() )
        node.append_child(pugi::node_pcdata).set_value( content.first_child().value() );

    // and comments? content.value() ?
}

xml::iterator xml::begin() const
{
    return doc.root().begin();
}

xml::iterator xml::end() const
{
    return doc.root().end();
}

xml::attribute_iterator xml::attribute_begin() const
{
    return doc.root().attributes_begin();
}

xml::attribute_iterator xml::attribute_end() const
{
    return doc.root().attributes_end();
}


xml::nodes get_children( const xml::node &node )
{
    xml::nodes nodes;
    for( auto it = node.first_child(); it; it = it.next_sibling() )
        nodes.push_back( it );
    return nodes;
}

xml::nodes get_siblings( const xml::node &node )
{
    xml::nodes nodes;
    for( auto it = node.previous_sibling(); it; it = it.previous_sibling() )
        nodes.push_back( it );
    for( auto it = node.next_sibling(); it; it = it.next_sibling() )
        nodes.push_back( it );
    return nodes;
}

size_t has_children( const xml::node &node )
{
#if 0
    return get_children(node).size();
    return std::distance( node.begin(), node.end() );
#else
    size_t num = 0;
    for( auto it = node.first_child(); it; it = it.next_sibling() )
        num++;
    return num;
#endif
}

size_t has_siblings( const xml::node &node )
{
#if 0
    return get_siblings(node).size();
#else
    size_t num = size_t(-2);
    for( auto it = node; it; it = it.previous_sibling() )
        num++;
    for( auto it = node; it; it = it.next_sibling() )
        num++;
    return num;
#endif
}

bool has_parent( const xml::node &node )
{
    return node.parent() != node.root();
}

bool is_null( const xml::node &node )
{
    return node.type() == pugi::node_null;
}

bool is_empty( const xml::node &node )
{
    return node.empty();
}

} // namespace wood
