#include <iostream>
#include <map>
#include <set>

#include <stdlib.h>

extern "C" {
#include <sai.h>
#include "saimetadata.h"
#include "saimetadatautils.h"
}

// node name
#define NN(x) (metadata_enum_sai_object_type_t.valuesshortnames[(x)])

std::set<sai_object_type_t> source;
std::set<sai_object_type_t> target;

void process_object_type_attributes(
        _In_ const sai_attr_metadata_t** meta_attr_list,
        _In_ sai_object_type_t current_object_type)
{
    std::set<sai_object_type_t> otset;

    for (int i = 0; meta_attr_list[i] != NULL; ++i)
    {
        const sai_attr_metadata_t* meta = meta_attr_list[i];

        if (meta->allowedobjecttypeslength == 0)
        {
            // skip attributes that don't contain object id's
            continue;
        }

        if (HAS_FLAG_READ_ONLY(meta->flags))
        {
            // skip attributes that are read only
            continue;
        }

        std::string style;

        switch (meta->attrvaluetype)
        {
            case SAI_ATTR_VALUE_TYPE_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
            case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:

                // we can miss some objects if same object can be set
                // as list in one attribute and as single object in
                // another attribute
                style = "style=bold";

                break;

            default:
                break;
        }

        // this attribute supports objects

        if (meta->allowedobjecttypeslength > 1)
        {
            // point arrows to the same point origin when this is single attribute
            style += " samehead=" + std::string(meta->attridname);
        }

        for (uint32_t j = 0; j < meta->allowedobjecttypeslength; j++)
        {
            sai_object_type_t ot = meta->allowedobjecttypes[j];

            if (otset.find(ot) != otset.end())
            {
                // node was already defined
                continue;
            }

            const char* current = NN(current_object_type);
            const char* dep = NN(ot);

            std::cout << dep << " -> " << current << " [ " << style << " color=\"0.650 0.700 0.700\"];\n";

            otset.insert(ot);

            source.insert(ot);
            target.insert(current_object_type);
        }
    }
}

void process_object_types()
{
    for (int i = 0; metadata_attr_by_object_type[i] != NULL; ++i)
    {
        const sai_attr_metadata_t** meta = metadata_attr_by_object_type[i];

        process_object_type_attributes(meta, (sai_object_type_t)i);
    }
}

void process_colors()
{
    for (int i = 0; metadata_attr_by_object_type[i] != NULL; ++i)
    {
        sai_object_type_t ot = (sai_object_type_t)i;

        bool is_source = source.find(ot) != source.end();
        bool is_target = target.find(ot) != target.end();


        if (is_source && is_target)
        {
            // node is target and source, so it's in the middle
            std::cout << NN(ot) << " [color=\"0.650 0.500 1.000\"];\n";
        }
        else if (is_target)
        {
            // this node is a leaf
            std::cout << NN(ot) << " [color=\"0.355 0.563 1.000\", shape = rect];\n";

        }
        else if (is_source)
        {
            std::cout << NN(ot) << " [color=\"0.650 0.200 1.000\"];\n";
        }
        else
        {
            if (ot == SAI_OBJECT_TYPE_NULL || ot == SAI_OBJECT_TYPE_MAX)
            {
                continue;
            }
            std::cout << NN(ot) << " [color=plum, shape = rect];\n";
        }
    }
}

#define PRINT_NN(x,y,c)\
    std::cout << NN(SAI_OBJECT_TYPE_ ## x) << " -> " << NN(SAI_OBJECT_TYPE_ ## y) << c;

void process_manual_connections()
{
    const char* c = " [color=\"0.650 0.700 0.700\", style = dashed, penwidth=2];\n";

    PRINT_NN(VIRTUAL_ROUTER, ROUTE_ENTRY, c);
    PRINT_NN(ROUTER_INTERFACE, NEIGHBOR_ENTRY, c);
    PRINT_NN(VLAN, FDB_ENTRY, c);

    PRINT_NN(PORT, SWITCH, "[dir=\"none\", color=\"red\", peripheries = 2, penwidth=2.0 , style  = dashed ];\n");
}

int main()
{
    std::cout << "digraph \"SAI Object Dependency Graph\" {\n";
    std::cout << "size=\"18,10\"; ratio = fill;\n";
    std::cout << "node [style=filled];\n";

    process_object_types();

    process_manual_connections();

    process_colors();

    std::cout << NN(SAI_OBJECT_TYPE_SWITCH) << " [color=orange, shape = parallelogram, peripheries = 2];\n";
    std::cout << NN(SAI_OBJECT_TYPE_PORT) << " [color=gold, shape = diamond, peripheries=2];\n";

    std::cout << "}\n";
    return 0;
}
