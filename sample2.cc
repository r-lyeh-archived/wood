#include <iostream>
#include <sstream>

#include "wood.hpp"

enum { verbose = false };

std::string print( const wood::xml &xml )
{
    std::stringstream ss;

    xml.forward( "//*", [&ss]( wood::xml::node &node ){
        node.print( ss );
        ss << "\tsiblings: " << wood::has_siblings(node) << std::endl;
        ss << "\tchildren: " << wood::has_children(node) << std::endl;
        ss << "\tparent: "   << wood::has_parent(node) << std::endl;
    } );

    return ss.str();
}

int main( int argc, const char **argv )
{
    {
        wood::xml networks;

        int oid;

        oid = 18;
        networks.
        push( "networks" ).attrib( "oid", oid-- ).
            set( "Grupo" ).attrib( "oid", oid-- ).
            push( "Europa" ).attrib( "oid", oid-- ).
                set( "UK" ).attrib( "oid", oid-- ).
                push( "ES" ).attrib( "oid", oid-- ).
                    set( "AND" ).attrib( "oid", oid-- ).
                    set( "GAL" ).attrib( "oid", oid-- ).
                    set( "MAD" ).attrib( "oid", oid-- ).
                    back().
                set( "IT" ).attrib( "oid", oid-- ).
                set( "FR" ).attrib( "oid", oid-- ).
                back().
            push( "America" ).attrib( "oid", oid-- ).
                set( "AR" ).attrib( "oid", oid-- ).
                set( "BR" ).attrib( "oid", oid-- ).
                set( "CL" ).attrib( "oid", oid-- ).
                set( "CO" ).attrib( "oid", oid-- ).
                set( "MX" ).attrib( "oid", oid-- ).
                back().
            push( "Asia" ).attrib( "oid", oid-- ).
                set( "CH" ).attrib( "oid", oid-- ).
                back().
            set( "Resto" ).attrib( "oid", oid-- );

        wood::xml aggr;

        oid = 29;
        aggr.
        push( "aggr" ).
            set( "NONE" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "all" ).attrib( "name", "Identificador interno regional" ).
            set( "UP" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Numero de empleados" ).
            set( "FUP" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Empleados discapacitados" ).
            set( "DW" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Factor de emision CH4" ).
            set( "FDW" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Factor de emision KWH" ).
            set( "AVE" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Porcentaje de proyectos satisfactorios" ).
            set( "WAVE" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Edad media" ).
            set( "MAXLEN" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Puntuacion media (1-3)" ).
            set( "STAT" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "Selecciona 1 de 5 opciones" ).
            set( "BOOL" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "num" ).attrib( "name", "¿Existe codigo etico?" ).
            set( "CONCAT" ).attrib( "oid", oid-- ).attrib( "VAL_TYPE", "txt" ).attrib( "name", "Describa las acciones realizadas" ).
            back();

        if( verbose )
        std::cout << aggr.str() << std::endl;

        wood::xml values;

        int val_id = 18;
        int network_oid = 18;

        oid = 20;
        values.
        push( "values" ).
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number",   0 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number",   0 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 814 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number",   0 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 541 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 274 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 485 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number",  84 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 102 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number",   0 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 185 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 346 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number",  84 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 179 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 206 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number",   0 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 460 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            set( "value" ).attrib( "ind_oid", oid ).attrib( "ind_val_id", val_id-- ).attrib( "network_oid", network_oid-- ).attrib( "val_number", 240 ).attrib( "val_str", "" ).attrib("start_dtm", "").attrib("end_dtm", "").
            back();

        if( verbose )
        std::cout << values.str() << std::endl;

        int sum = 0;
        values.forward( "//*", [&sum]( wood::xml::node &node ){
            auto &parent = node.parent();

            std::stringstream ss;
            ss << '$' << node.attribute("val_number").value() << ' ';
            node.print( ss );

            std::cout << ss.str();
        } );

        std::cout << print(values) << std::endl;
    }

    {
        #define $QUOTE(...) #__VA_ARGS__
        std::string xml = $QUOTE(
        <root>
            <Dev>
                <Emp salary='2'>1</Emp>
                <Floor>1</Floor>
                <Salary>1200.4</Salary>
            </Dev>
            <Dev>
                <Emp>2</Emp>
                <Salary>3100.8</Salary>
            </Dev>
            <Dev>
                <Emp salary='4'>3</Emp>
                <Floor>1</Floor>
            </Dev>
        </root>
     );

        wood::xml doc;
        doc.str( xml );

        std::cout << "Salary=" << doc.get<double>("sum(/*/Dev/Emp/@salary[number(.) = number(.)])") << std::endl;
        std::cout << "Salary=" << doc.get<double>("sum(/*/Dev/Salary[number(.) = number(.)])") << std::endl;
    }

    return 0;
}

