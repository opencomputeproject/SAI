#ifndef __SAI_SERIALIZE__
#define __SAI_SERIALIZE__

#include "sai.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <iomanip>
#include <map>
#include <tuple>
#include <string.h>

#define TO_STR(x) #x

#define SERIALIZE_LOG(level, fmt, arg ...) {\
    fprintf(stderr, "%d: ", level); \
    fprintf(stderr, fmt, ##arg); \
    fprintf(stderr, "\n"); }

#define SERIALIZE_LOG_ENTER()   SERIALIZE_LOG(SAI_LOG_DEBUG, "%s: >", __FUNCTION__)
#define SERIALIZE_LOG_EXIT()    SERIALIZE_LOG(SAI_LOG_DEBUG, "%s: <", __FUNCTION__)
#define SERIALIZE_LOG_DBG(fmt, arg ...) SERIALIZE_LOG(SAI_LOG_DEBUG, fmt, ##arg)
#define SERIALIZE_LOG_INF(fmt, arg ...) SERIALIZE_LOG(SAI_LOG_INFO, fmt, ##arg)
#define SERIALIZE_LOG_WRN(fmt, arg ...) SERIALIZE_LOG(SAI_LOG_WARN, fmt, ##arg)
#define SERIALIZE_LOG_ERR(fmt, arg ...) SERIALIZE_LOG(SAI_LOG_ERROR, fmt, ##arg)
#define SERIALIZE_LOG_NTC(fmt, arg ...) SERIALIZE_LOG(SAI_LOG_NOTICE, fmt, ##arg)

typedef enum _sai_attr_serialization_type_t 
{
    SAI_SERIALIZATION_TYPE_BOOL,
    SAI_SERIALIZATION_TYPE_CHARDATA,
    SAI_SERIALIZATION_TYPE_UINT8,
    SAI_SERIALIZATION_TYPE_INT8,
    SAI_SERIALIZATION_TYPE_UINT16,
    SAI_SERIALIZATION_TYPE_INT16,
    SAI_SERIALIZATION_TYPE_UINT32,
    SAI_SERIALIZATION_TYPE_INT32,
    SAI_SERIALIZATION_TYPE_UINT64,
    SAI_SERIALIZATION_TYPE_INT64,
    SAI_SERIALIZATION_TYPE_MAC,
    SAI_SERIALIZATION_TYPE_IP4,
    SAI_SERIALIZATION_TYPE_IP6,
    SAI_SERIALIZATION_TYPE_IP_ADDRESS,
    SAI_SERIALIZATION_TYPE_OBJECT_ID,
    SAI_SERIALIZATION_TYPE_OBJECT_LIST,
    SAI_SERIALIZATION_TYPE_UINT8_LIST,
    SAI_SERIALIZATION_TYPE_INT8_LIST,
    SAI_SERIALIZATION_TYPE_UINT16_LIST,
    SAI_SERIALIZATION_TYPE_INT16_LIST,
    SAI_SERIALIZATION_TYPE_UINT32_LIST,
    SAI_SERIALIZATION_TYPE_INT32_LIST,
    SAI_SERIALIZATION_TYPE_UINT32_RANGE,
    SAI_SERIALIZATION_TYPE_INT32_RANGE,
    SAI_SERIALIZATION_TYPE_VLAN_LIST,
    SAI_SERIALIZATION_TYPE_VLAN_PORT_LIST,

    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT8,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT16,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT16,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_INT32,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT32,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_MAC,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP4,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_IP6,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_ID,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_OBJECT_LIST,
    SAI_SERIALIZATION_TYPE_ACL_FIELD_DATA_UINT8_LIST,

    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT8,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT8,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT16,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT16,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_UINT32,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_INT32,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_MAC,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV4,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_IPV6,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_ID,
    SAI_SERIALIZATION_TYPE_ACL_ACTION_DATA_OBJECT_LIST,

    SAI_SERIALIZATION_TYPE_PORT_BREAKOUT,
    SAI_SERIALIZATION_TYPE_QOS_MAP_LIST

} sai_attr_serialization_type_t;

typedef std::map<sai_object_type_t, std::string> sai_object_type_to_string_map_t;

typedef std::map<sai_object_type_t, std::map<sai_attr_id_t, sai_attr_serialization_type_t>> sai_serialization_map_t;

sai_serialization_map_t sai_get_serialization_map();
sai_object_type_to_string_map_t sai_get_object_type_map();

sai_status_t sai_get_object_type_string(sai_object_type_t object_type, std::string &str_object_type);

extern sai_serialization_map_t g_serialization_map;
extern sai_object_type_to_string_map_t g_object_type_map;

template<typename T>
void sai_dealloc_list(
        _In_ T &element)
{
    delete[] element.list;
}

template<typename T>
void sai_dealloc_array(
        _In_ T &element)
{
    delete[] element;
}

void sai_deserialize_buffer(
        _In_ const std::string &s,
        _In_ int index,
        _In_ size_t buffer_size, 
        _In_ void *buffer);

void sai_free_buffer(void * buffer);

void sai_serialize_buffer(
        _In_ const void *buffer, 
        _In_ size_t buffer_size, 
        _Out_ std::string &s);

template<typename T>
void sai_serialize_primitive(
        _In_ const T &element,
        _Out_ std::string &s)
{
    unsigned const char* mem = reinterpret_cast<const unsigned char*>(&element);

    sai_serialize_buffer(mem, sizeof(T), s);
}

template<typename T>
void sai_serialize_list(
        _In_ const T &element,
        _Out_ std::string &s,
        _In_ bool countOnly)
{
    sai_serialize_primitive(element.count, s);

    if (countOnly)
        return;

    for (size_t i = 0; i < element.count; i++)
    {
        sai_serialize_primitive(element.list[i], s);
    }
}

template<typename T>
void sai_free_list(
        _In_ T &element)
{
    delete[] element.list;
    element.list = NULL;
}

template<class T, typename U>
T* sai_alloc_n_of_ptr_type(U count, T*)
{
    return new T[count];
}

template<typename T, typename U>
void sai_alloc_list(
        _In_ T count,
        _In_ U &element)
{
    element.count = count;
    element.list = sai_alloc_n_of_ptr_type(count, element.list);
}

int char_to_int(
        _In_ const char c);

template<typename  T>
void sai_deserialize_primitive(
        _In_ const std::string & s,
        _In_ int &index,
        _Out_ T &element)
{
    size_t count = sizeof(T);

    unsigned char *mem = reinterpret_cast<unsigned char*>(&element);

    const char *ptr = s.c_str() + index;

    for (size_t i = 0; i < count; i ++)
    {
        int u = char_to_int(ptr[2 * i]);
        int l = char_to_int(ptr[2 * i + 1]);

        unsigned char c = (u << 4) | l;

        mem[i] = c;
    }

    index += count * 2;
}

template<typename T>
void sai_deserialize_list(
        _In_ const std::string &s,
        _In_ int &index,
        _Out_ T &element,
        _In_ bool countOnly)
{
    sai_deserialize_primitive(s, index, element.count);

    sai_alloc_list(element.count, element);

    if (countOnly)
        return;

    for (size_t i = 0; i < element.count; i++)
    {
        sai_deserialize_primitive(s, index, element.list[i]);
    }
}

template<typename T>
void transfer_primitive(
        _In_ const T &src_element,
        _In_ T &dst_element)
{
    const unsigned char* src_mem = reinterpret_cast<const unsigned char*>(&src_element);
    unsigned char* dst_mem = reinterpret_cast<unsigned char*>(&dst_element);

    memcpy(dst_mem, src_mem, sizeof(T));
}

template<typename T>
void transfer_list(
        _In_ const T &src_element,
        _In_ T &dst_element,
        _In_ bool countOnly)
{
    transfer_primitive(src_element.count, dst_element.count);

    if (countOnly)
        return;

    for (size_t i = 0; i < src_element.count; i++)
    {
        transfer_primitive(src_element.list[i], dst_element.list[i]);
    }
}

sai_status_t sai_deserialize_attr_value(
        _In_ const std::string &s,
        _In_ int &index,
        _In_ const sai_attr_serialization_type_t type,
        _Out_ sai_attribute_t &attr,
        _In_ bool countOnly);

sai_status_t sai_deserialize_free_attribute_value(
        _In_ const sai_attr_serialization_type_t type,
        _In_ sai_attribute_t &attr);

sai_status_t sai_serialize_attr(
        _In_ const sai_attr_serialization_type_t type,
        _In_ const sai_attribute_t &attr,
        _Out_ std::string &s,
        _In_ bool countOnly);

sai_status_t sai_serialize_attr_id(
        _In_ const sai_attribute_t &attr,
        _Out_ std::string &s);

sai_status_t sai_serialize_attr_value(
        _In_ const sai_attr_serialization_type_t type,
        _In_ const sai_attribute_t &attr,
        _Out_ std::string &s,
        _In_ bool countOnly);

sai_status_t sai_get_serialization_type(
        _In_ const sai_object_type_t object_type,
        _In_ const sai_attr_id_t attr_id,
        _Out_ sai_attr_serialization_type_t &serialization_type);


sai_status_t sai_serialize_fdb_event_notification_data(
        _In_ sai_fdb_event_notification_data_t *fdb,
        _Out_ std::string &s);

sai_status_t sai_deserialize_fdb_event_notification_data(
        _In_ const std::string &s,
        _In_ int index,
        _Out_ sai_fdb_event_notification_data_t *fdb);

sai_status_t sai_deserialize_free_fdb_event_notification_data(
        _In_ sai_fdb_event_notification_data_t *fdb);

sai_status_t transfer_attribute(
        _In_ sai_attr_serialization_type_t serialization_type,
        _In_ sai_attribute_t &src_attr,
        _In_ sai_attribute_t &dst_attr,
        _In_ bool countOnly);

void transfer_attributes(
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *src_attr_list,
        _In_ sai_attribute_t *dst_attr_list,
        _In_ bool countOnly);

#endif // __SAI_SERIALIZE__
