/**
 * Copyright (c) 2014 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saiserializetest.c
 *
 * @brief   This module defines SAI Serialize Test
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <arpa/inet.h>
#include <sai.h>

#include "saimetadata.h"

#define ASSERT_TRUE(x,fmt,...)                              \
    if (!(x)){                                              \
        fprintf(stderr,                                     \
                "ASSERT TRUE FAILED(%s:%d): %s: " fmt "\n", \
                __func__, __LINE__, #x, ##__VA_ARGS__);     \
        exit(1);}

#define ASSERT_STR_EQ(a,b,r)                                                \
    if (strcmp(a,b) != 0){                                                  \
        fprintf(stderr,                                                     \
                "ASSERT STR_EQ FAILED(%s:%d): is:\n%s\nexpected:\n%s\n",    \
                __func__, __LINE__, a, b);                                  \
        exit(1);}                                                           \
    if ((int)strlen(a) != r){                                               \
        fprintf(stderr,                                                     \
                "ASSERT STR_EQ FAILED(%s:%d): returned length is wrong"     \
                " res (%d) != strlen (%zu)\n",                              \
                __func__, __LINE__, r, strlen(a));                          \
        exit(1);}

#define PRIMITIVE_BUFFER_SIZE 128
#define LONG_BUFFER_SIZE 0x10000

void test_serialize_bool()
{
    char buf[PRIMITIVE_BUFFER_SIZE];

    int res;

    res = sai_serialize_bool(buf, false);
    ASSERT_STR_EQ(buf, "false", res);

    res = sai_serialize_bool(buf, true);
    ASSERT_STR_EQ(buf, "true", res);
}

void test_deserialize_bool()
{
    int res;
    bool b;

    const char *valid_true[] = { "true", "true,", "true\"", "true]", "true}"};
    const char *invalid_true[] = { "truee", "tru1", "true)", "true="};

    const char *valid_false[] = { "false", "false,", "false\"", "false]", "false}"};
    const char *invalid_false[] = { "falsee", "tru1", "false)", "false="};

    size_t n;

    for (n = 0; n < sizeof(valid_true)/sizeof(const char*); n++)
    {
        b = false;
        res = sai_deserialize_bool(valid_true[n], &b);
        ASSERT_TRUE(b, "expected true");
        ASSERT_TRUE(res == 4, "expected true");
    }

    for (n = 0; n < sizeof(invalid_true)/sizeof(const char*); n++)
    {
        b = false;
        res = sai_deserialize_bool(invalid_true[n], &b);
        ASSERT_TRUE(res < 0, "expected negative");
    }

    for (n = 0; n < sizeof(valid_false)/sizeof(const char*); n++)
    {
        b = false;
        res = sai_deserialize_bool(valid_false[n], &b);
        ASSERT_TRUE(!b, "expected false");
        ASSERT_TRUE(res == 5, "expected false");
    }

    for (n = 0; n < sizeof(invalid_false)/sizeof(const char*); n++)
    {
        b = false;
        res = sai_deserialize_bool(invalid_false[n], &b);
        ASSERT_TRUE(res < 0, "expected negative");
    }
}

void test_serialize_chardata()
{
    sai_attribute_value_t val;

    memset(&val, 0, sizeof(val));

    char buf[PRIMITIVE_BUFFER_SIZE];

    int res;

    res = sai_serialize_chardata(buf, val.chardata);
    ASSERT_STR_EQ(buf, "", res);

    strcpy(val.chardata, "foo bar 123");

    res = sai_serialize_chardata(buf, val.chardata);
    ASSERT_STR_EQ(buf, "foo bar 123", res);

    strcpy(val.chardata, "foo \\ bar 123");

    res = sai_serialize_chardata(buf, val.chardata);
    ASSERT_TRUE(res < 0, "expected negative number");

    strcpy(val.chardata, "foo \" bar 123");

    res = sai_serialize_chardata(buf, val.chardata);
    ASSERT_TRUE(res < 0, "expected negative number");

    strcpy(val.chardata, "0123456789012345678912");

    res = sai_serialize_chardata(buf, val.chardata);
    ASSERT_STR_EQ(buf, "0123456789012345678912", res);
}

void test_deserialize_chardata()
{
    sai_attribute_value_t val;

    memset(&val, 0, sizeof(val));

    int res;

    res = sai_deserialize_chardata("", val.chardata);
    ASSERT_STR_EQ(val.chardata, "", res);

    res = sai_deserialize_chardata("foo bar 123", val.chardata);
    ASSERT_STR_EQ(val.chardata, "foo bar 123", res);

    res = sai_deserialize_chardata("foo \\ bar 123", val.chardata);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_chardata("foo \" bar 123", val.chardata);
    ASSERT_STR_EQ(val.chardata, "foo ", res);

    res = sai_deserialize_chardata("foo \x11", val.chardata);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_chardata("01234567890123456789120123456789", val.chardata);
    ASSERT_STR_EQ(val.chardata, "01234567890123456789120123456789", res);

    res = sai_deserialize_chardata("012345678901234567890123456789127", val.chardata);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_chardata("01234567890123456789120123456789\"", val.chardata);
    ASSERT_STR_EQ(val.chardata, "01234567890123456789120123456789", res);
}

void subtest_serialize_object_id(
        _In_ sai_object_id_t id,
        _In_ const char *exp)
{
    char buf[PRIMITIVE_BUFFER_SIZE];

    int res = sai_serialize_object_id(buf, id);
    ASSERT_STR_EQ(buf, exp, res);
}

void test_serialize_object_id()
{
    subtest_serialize_object_id(0, "oid:0x0");
    subtest_serialize_object_id(0x123456789abcdef0, "oid:0x123456789abcdef0");
    subtest_serialize_object_id(0x123459abcdef0, "oid:0x123459abcdef0");
    subtest_serialize_object_id(0xFFFFFFFFFFFFFFFF, "oid:0xffffffffffffffff");
}

void test_deserialize_object_id()
{
    int res;
    size_t n;
    sai_object_id_t oid;

    const char *valid_oids[] = {
        "oid:0x0",
        "oid:0x1",
        "oid:0x0123456789abcdef",
        "oid:0x0123456789abcdef,",
        "oid:0x0123456789abcdef\"",
        "oid:0x0123456789abcdef}",
        "oid:0x0123456789abcdef]",
        };

    const char *invalid_oids[] = {
        "oid:0x",
        "aa",
        "45",
        "oid:0x0123456789abcdefv",
        "oid:0x0123456789abcdef0",
        "oid:0x00123456789abcdef",
        };

    for (n = 0; n < sizeof(valid_oids)/sizeof(const char*); n++)
    {
        res = sai_deserialize_object_id(valid_oids[n], &oid);
        ASSERT_TRUE(res > 0 , "expected true");

        sai_object_id_t ref;
        int len;
        sscanf(valid_oids[n], "oid:0x%"PRIx64"%n", &ref, &len);

        ASSERT_TRUE(res == len, "expected true");
        ASSERT_TRUE(oid == ref, "expected true");
    }

    for (n = 0; n < sizeof(invalid_oids)/sizeof(const char*); n++)
    {
        res = sai_deserialize_object_id(invalid_oids[n], &oid);
        ASSERT_TRUE(res < 0, "expected negative");
    }
}

void test_serialize_mac()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_mac_t mac;

    memcpy(mac, "\x01\x23\x45\x67\x89\xab", 6);

    res = sai_serialize_mac(buf, mac);

    ASSERT_STR_EQ(buf, "01:23:45:67:89:AB", res);
}

void test_deserialize_mac()
{
    int res;
    sai_mac_t mac;

    res = sai_deserialize_mac("11:22:33:44:55:66", mac);
    ASSERT_TRUE(res == 17, "expected 17 length");
    ASSERT_TRUE(memcmp(mac, "\x11\x22\x33\x44\x55\x66", 6) == 0, "expected equal");

    res = sai_deserialize_mac("ff:22:33:44:55:66", mac);
    ASSERT_TRUE(res == 17, "expected 17 length");
    ASSERT_TRUE(memcmp(mac, "\xff\x22\x33\x44\x55\x66", 6) == 0, "expected equal");

    res = sai_deserialize_mac("FF:22:33:44:55:66", mac);
    ASSERT_TRUE(res == 17, "expected 17 length");
    ASSERT_TRUE(memcmp(mac, "\xff\x22\x33\x44\x55\x66", 6) == 0, "expected equal");

    res = sai_deserialize_mac("fF:22:33:44:55:66,", mac);
    ASSERT_TRUE(res == 17, "expected 17 length");
    ASSERT_TRUE(memcmp(mac, "\xff\x22\x33\x44\x55\x66", 6) == 0, "expected equal");

    res = sai_deserialize_mac("1:2:3:4:5:f", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("011:022:33:44:55:66,", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("11:22:33:44:55:z6,", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("11:22:33:44:55:66j", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("111:22:33:44:55:66", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("11:22:33:44:55:]6", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("11:g2:33:44:55:66", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("11:22::33:44:55:66", mac);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_mac("111:2::33:44:55:66", mac);
    ASSERT_TRUE(res < 0, "expected negative");
}

void test_serialize_encrypt_key()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_encrypt_key_t encrypt_key;

    memcpy(encrypt_key, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef", 32);

    res = sai_serialize_encrypt_key(buf, encrypt_key);

    ASSERT_STR_EQ(buf, "01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", res);
}

void test_deserialize_encrypt_key()
{
    int res;
    sai_encrypt_key_t encrypt_key;

    res = sai_deserialize_encrypt_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", encrypt_key);
    ASSERT_TRUE(res == 95, "expected 95 length");
    ASSERT_TRUE(memcmp(encrypt_key, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef", 32) == 0, "expected equal");

    res = sai_deserialize_encrypt_key("1:2:3:4:5:f:a:b:1:2:3:4:5:f:a:b:1:2:3:4:5:f:a:b:1:2:3:4:5:f:a:b", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_encrypt_key("001:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_encrypt_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:zF", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_encrypt_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:Ej", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_encrypt_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_encrypt_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:]F", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_encrypt_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89::CD:EF", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_encrypt_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:01:23:45::67:89:AB:CD:EF", encrypt_key);
    ASSERT_TRUE(res < 0, "expected negative");
}

void test_serialize_auth_key()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_auth_key_t auth_key;

    memcpy(auth_key, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef", 16);

    res = sai_serialize_auth_key(buf, auth_key);

    ASSERT_STR_EQ(buf, "01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", res);
}

void test_deserialize_auth_key()
{
    int res;
    sai_auth_key_t auth_key;

    res = sai_deserialize_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", auth_key);
    ASSERT_TRUE(res == 47, "expected 47 length");
    ASSERT_TRUE(memcmp(auth_key, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef", 16) == 0, "expected equal");

    res = sai_deserialize_auth_key("1:2:3:4:5:f:a:b:1:2:3:4:5:f:a:b", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_auth_key("001:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:zF", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:Ej", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:]F", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89::CD:EF", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB::CD:EF", auth_key);
    ASSERT_TRUE(res < 0, "expected negative");
}

void test_serialize_macsec_auth_key()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_macsec_auth_key_t macsec_auth_key;

    memcpy(macsec_auth_key, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef", 16);

    res = sai_serialize_macsec_auth_key(buf, macsec_auth_key);

    ASSERT_STR_EQ(buf, "01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", res);
}

void test_deserialize_macsec_auth_key()
{
    int res;
    sai_macsec_auth_key_t macsec_auth_key;

    res = sai_deserialize_macsec_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", macsec_auth_key);
    ASSERT_TRUE(res == 47, "expected 47 length");
    ASSERT_TRUE(memcmp(macsec_auth_key, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67\x89\xab\xcd\xef", 16) == 0, "expected equal");

    res = sai_deserialize_macsec_auth_key("1:2:3:4:5:f:a:b:1:2:3:4:5:f:a:b", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_auth_key("001:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:zF", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:Ej", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:EF:", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB:CD:]F", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89::CD:EF", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_auth_key("01:23:45:67:89:AB:CD:EF:01:23:45:67:89:AB::CD:EF", macsec_auth_key);
    ASSERT_TRUE(res < 0, "expected negative");
}

void test_serialize_macsec_salt()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_macsec_salt_t macsec_salt;

    memcpy(macsec_salt, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67", 12);

    res = sai_serialize_macsec_salt(buf, macsec_salt);

    ASSERT_STR_EQ(buf, "01:23:45:67:89:AB:CD:EF:01:23:45:67", res);
}

void test_deserialize_macsec_salt()
{
    int res;
    sai_macsec_salt_t macsec_salt;

    res = sai_deserialize_macsec_salt("01:23:45:67:89:AB:CD:EF:01:23:45:67", macsec_salt);
    ASSERT_TRUE(res == 35, "expected 35 length but got %d", res);
    ASSERT_TRUE(memcmp(macsec_salt, "\x01\x23\x45\x67\x89\xab\xcd\xef\x01\x23\x45\x67", 12) == 0, "expected equal");

    res = sai_deserialize_macsec_salt("1:2:3:4:5:f:a:b:1:2:3:4", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_salt("001:23:45:67:89:AB:CD:EF:01:23:45:67", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_salt("01:23:45:67:89:AB:CD:EF:01:23:45:z7", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_salt("01:23:45:67:89:AB:CD:EF:01:23:45:6j", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_salt("01:23:45:67:89:AB:CD:EF:01:23:45:67:", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_salt("01:23:45:67:89:AB:CD:EF:01:23:45:]7", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_salt("01:23:45:67:89:AB:CD:EF:01:23::67", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_macsec_salt("01:23:45:67:89:AB:CD:EF:01:23::45:67", macsec_salt);
    ASSERT_TRUE(res < 0, "expected negative");
}

void test_serialize_enum()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_object_type_t ot = SAI_OBJECT_TYPE_PORT;

    res = sai_serialize_enum(buf, &sai_metadata_enum_sai_object_type_t, ot);

    ASSERT_STR_EQ(buf, "SAI_OBJECT_TYPE_PORT", res);

    res = sai_serialize_enum(buf, &sai_metadata_enum_sai_object_type_t, -1);

    ASSERT_STR_EQ(buf, "-1", res);

    res = sai_serialize_enum(buf, &sai_metadata_enum_sai_object_type_t, 228);

    ASSERT_STR_EQ(buf, "228", res);

    /* test all enums */

    size_t i = 0;

    for (; i < sai_metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = sai_metadata_all_enums[i];

        size_t j = 0;

        for (; j < emd->valuescount; ++j)
        {
            int value = emd->values[j];

            res = sai_serialize_enum(buf, emd, value);

            ASSERT_STR_EQ(buf, emd->valuesnames[j], res);
        }
    }
}

void test_deserialize_enum()
{
    int res;
    int value;

    res = sai_deserialize_enum("SAI_OBJECT_TYPE_PORT", &sai_metadata_enum_sai_object_type_t, &value);

    ASSERT_TRUE(res == strlen("SAI_OBJECT_TYPE_PORT"), "expected true");
    ASSERT_TRUE(value == SAI_OBJECT_TYPE_PORT, "expected true");

    res = sai_deserialize_enum("SAI_OBJECT_TYPE_PORT\"", &sai_metadata_enum_sai_object_type_t, &value);
    ASSERT_TRUE(res == strlen("SAI_OBJECT_TYPE_PORT"), "expected true");
    ASSERT_TRUE(value == SAI_OBJECT_TYPE_PORT, "expected true");

    res = sai_deserialize_enum("SAI_OBJECT_TYPE_PORTS", &sai_metadata_enum_sai_object_type_t, &value);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_enum("-1", &sai_metadata_enum_sai_object_type_t, &value);
    ASSERT_TRUE(res == strlen("-1"), "expected true");
    ASSERT_TRUE(value == -1, "expected true, value = %d", value);

    res = sai_deserialize_enum("100", &sai_metadata_enum_sai_object_type_t, &value);
    ASSERT_TRUE(res == strlen("100"), "expected true");
    ASSERT_TRUE(value == 100, "expected true");

    /* test all enums */

    size_t i = 0;

    for (; i < sai_metadata_all_enums_count; ++i)
    {
        const sai_enum_metadata_t* emd = sai_metadata_all_enums[i];

        size_t j = 0;

        for (; j < emd->valuescount; ++j)
        {
            res = sai_deserialize_enum(emd->valuesnames[j], emd, &value);

            ASSERT_TRUE(res == (int)strlen(emd->valuesnames[j]), "expected true");
            ASSERT_TRUE(value == emd->values[j], "expected true");
        }
    }
}

void test_serialize_ip4()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_ip4_t ip = htonl(0x0a000015);

    res = sai_serialize_ip4(buf, ip);
    ASSERT_STR_EQ(buf, "10.0.0.21", res);

    ip = 0xFFFFFFFF;
    res = sai_serialize_ip4(buf, ip);
    ASSERT_STR_EQ(buf, "255.255.255.255", res);
}

void test_deserialize_ip4()
{
    int res;

    sai_ip4_t ip;

    res = sai_deserialize_ip4("10.0.0.21", &ip);
    ASSERT_TRUE(res > 0 && strlen("10.0.0.21") == res, "expected true, res: %d", res);
    ASSERT_TRUE(ip == htonl(0x0a000015), "expected true");
    ASSERT_TRUE(memcmp(&ip, "\x0a\x00\x00\x15", 4) == 0, "expected true");

    res = sai_deserialize_ip4("10.0.0.21\"", &ip);
    ASSERT_TRUE(res > 0 && strlen("10.0.0.21") == res, "expected true, res: %d", res);

    res = sai_deserialize_ip4("10.0.0.21/", &ip);
    ASSERT_TRUE(res > 0 && strlen("10.0.0.21") == res, "expected true, res: %d", res);

    res = sai_deserialize_ip4("1::ff", &ip);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_ip4("1.1.256.1", &ip);
    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_serialize_ip6()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_ip6_t ip;

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    memcpy(ip, ip6, 16);
    res = sai_serialize_ip6(buf, ip);
    ASSERT_STR_EQ(buf, "1111:2222:3333:4444:5555:6666:aaaa:bbbb", res);

    uint16_t ip6a[] = { 0x0100, 0, 0, 0, 0, 0, 0, 0xff00 };

    memcpy(ip, ip6a, 16);
    res = sai_serialize_ip6(buf, ip);
    ASSERT_STR_EQ(buf, "1::ff", res);

    uint16_t ip6b[] = { 0, 0, 0, 0, 0, 0, 0, 0x100 };
    memcpy(ip, ip6b, 16);

    res = sai_serialize_ip6(buf, ip);
    ASSERT_STR_EQ(buf, "::1", res);
}

void test_deserialize_ip6()
{
    int res;

    sai_ip6_t ip;

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    const char *buf = "1111:2222:3333:4444:5555:6666:aaaa:bbbb";

    res = sai_deserialize_ip6(buf, ip);
    ASSERT_TRUE(memcmp(ip, ip6, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");

    buf = "1111:2222:3333:4444:5555:6666:aaaa:bbbb/";

    res = sai_deserialize_ip6(buf, ip);
    ASSERT_TRUE(memcmp(ip, ip6, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf) - 1, "expected true");

    buf = "1111:2222:3333:4444:5555:6666:aaaa:bbbb\"";

    res = sai_deserialize_ip6(buf, ip);
    ASSERT_TRUE(memcmp(ip, ip6, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf) - 1, "expected true");

    uint16_t ip6a[] = { 0x0100, 0, 0, 0, 0, 0, 0, 0xff00 };

    buf = "1::ff";
    res = sai_deserialize_ip6(buf, ip);
    ASSERT_TRUE(memcmp(ip, ip6a, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");

    uint16_t ip6b[] = { 0, 0, 0, 0, 0, 0, 0, 0x100 };

    buf = "::1";
    res = sai_deserialize_ip6(buf, ip);
    ASSERT_TRUE(memcmp(ip, ip6b, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");

    buf = "255.255.255.255";
    res = sai_deserialize_ip6(buf, ip);
    ASSERT_TRUE(res < 0, "expected negative number");

    buf = "1::456::3";
    res = sai_deserialize_ip6(buf, ip);
    ASSERT_TRUE(res < 0, "expected negative number");
}

void subtest_serialize_ip_addres_v4(
        _In_ uint32_t ip,
        _In_ const char *exp)
{
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_ip_address_t ipaddr;

    ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    ipaddr.addr.ip4 = htonl(ip);

    int res = sai_serialize_ip_address(buf, &ipaddr);

    ASSERT_STR_EQ(buf, exp, res);
}

void test_serialize_ip_address()
{
    subtest_serialize_ip_addres_v4(0x0a000015, "10.0.0.21");
    subtest_serialize_ip_addres_v4(0x01020304, "1.2.3.4");
    subtest_serialize_ip_addres_v4(0, "0.0.0.0");
    subtest_serialize_ip_addres_v4((uint32_t)-1, "255.255.255.255");

    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_ip_address_t ipaddr;

    ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    res = sai_serialize_ip_address(buf, &ipaddr);
    ASSERT_TRUE(res > 0, "expected positive number");

    ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
    res = sai_serialize_ip_address(buf, &ipaddr);
    ASSERT_TRUE(res > 0, "expected positive number");

    /* invalid address family */

    ipaddr.addr_family = 2;
    res = sai_serialize_ip_address(buf, &ipaddr);
    ASSERT_TRUE(res < 0, "expected negative number");

    /* test ip v6 */

    ipaddr.addr_family = SAI_IP_ADDR_FAMILY_IPV6;

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    memcpy(ipaddr.addr.ip6, ip6, 16);

    res = sai_serialize_ip_address(buf, &ipaddr);

    ASSERT_STR_EQ(buf, "1111:2222:3333:4444:5555:6666:aaaa:bbbb", res);

    uint16_t ip6a[] = { 0x0100, 0, 0, 0, 0, 0, 0, 0xff00 };

    memcpy(ipaddr.addr.ip6, ip6a, 16);

    res = sai_serialize_ip_address(buf, &ipaddr);

    ASSERT_STR_EQ(buf, "1::ff", res);

    uint16_t ip6b[] = { 0, 0, 0, 0, 0, 0, 0, 0x100 };

    memcpy(ipaddr.addr.ip6, ip6b, 16);

    res = sai_serialize_ip_address(buf, &ipaddr);

    ASSERT_STR_EQ(buf, "::1", res);
}


void test_deserialize_ip_address()
{
    int res;

    /* ipv4 */

    sai_ip_address_t ip;

    res = sai_deserialize_ip_address("10.0.0.21", &ip);
    ASSERT_TRUE(res > 0 && strlen("10.0.0.21") == res, "expected true, res: %d", res);
    ASSERT_TRUE(ip.addr.ip4 == htonl(0x0a000015), "expected true");
    ASSERT_TRUE(memcmp(&ip.addr.ip4, "\x0a\x00\x00\x15", 4) == 0, "expected true");
    ASSERT_TRUE(ip.addr_family == SAI_IP_ADDR_FAMILY_IPV4, "expected true");

    res = sai_deserialize_ip_address("10.0.0.21\"", &ip);
    ASSERT_TRUE(res > 0 && strlen("10.0.0.21") == res, "expected true, res: %d", res);

    res = sai_deserialize_ip_address("10.0.0.21/", &ip);
    ASSERT_TRUE(res > 0 && strlen("10.0.0.21") == res, "expected true, res: %d", res);

    res = sai_deserialize_ip_address("255.255.255.255", &ip);
    ASSERT_TRUE(res > 0 && strlen("255.255.255.255") == res, "expected true, res: %d", res);

    res = sai_deserialize_ip_address("1.1.256.1", &ip);
    ASSERT_TRUE(res < 0, "expected negative number");

    /* ipv6 */

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    const char *buf = "1111:2222:3333:4444:5555:6666:aaaa:bbbb";

    res = sai_deserialize_ip_address(buf, &ip);
    ASSERT_TRUE(memcmp(ip.addr.ip6, ip6, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");
    ASSERT_TRUE(ip.addr_family == SAI_IP_ADDR_FAMILY_IPV6, "expected true");

    buf = "1111:2222:3333:4444:5555:6666:aaaa:bbbb/";

    res = sai_deserialize_ip_address(buf, &ip);
    ASSERT_TRUE(memcmp(ip.addr.ip6, ip6, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf) - 1, "expected true");

    buf = "1111:2222:3333:4444:5555:6666:aaaa:bbbb\"";

    res = sai_deserialize_ip_address(buf, &ip);
    ASSERT_TRUE(memcmp(ip.addr.ip6, ip6, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf) - 1, "expected true");

    uint16_t ip6a[] = { 0x0100, 0, 0, 0, 0, 0, 0, 0xff00 };

    buf = "1::ff";
    res = sai_deserialize_ip_address(buf, &ip);
    ASSERT_TRUE(memcmp(ip.addr.ip6, ip6a, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");

    uint16_t ip6b[] = { 0, 0, 0, 0, 0, 0, 0, 0x100 };

    buf = "::1";
    res = sai_deserialize_ip_address(buf, &ip);
    ASSERT_TRUE(memcmp(ip.addr.ip6, ip6b, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");

    buf = "1::456::3";
    res = sai_deserialize_ip_address(buf, &ip);
    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_serialize_ip_prefix()
{
    sai_ip_prefix_t prefix;

    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    prefix.addr_family = 2;
    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_TRUE(res < 0, "expected negative number");

    /* ipv4 */

    prefix.addr_family = SAI_IP_ADDR_FAMILY_IPV4;

    prefix.addr.ip4 = htonl(0x0a000015);
    prefix.mask.ip4 = htonl(0xffffffff);

    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_STR_EQ(buf, "10.0.0.21/32", res);

    prefix.addr.ip4 = htonl(0x0a000015);
    prefix.mask.ip4 = htonl(0xffff0000);

    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_STR_EQ(buf, "10.0.0.21/16", res);

    prefix.addr.ip4 = htonl(0x0a000015);
    prefix.mask.ip4 = htonl(0);

    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_STR_EQ(buf, "10.0.0.21/0", res);

    /* ipv6 */

    prefix.addr_family = SAI_IP_ADDR_FAMILY_IPV6;

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };
    memcpy(prefix.addr.ip6, ip6, 16);
    memset(prefix.mask.ip6, 0xff, 16);

    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_STR_EQ(buf, "1111:2222:3333:4444:5555:6666:aaaa:bbbb/128", res);

    uint16_t ip6a[] = { 0x0100, 0, 0, 0, 0, 0, 0, 0xff00 };
    memcpy(prefix.addr.ip6, ip6a, 16);
    memset(prefix.mask.ip6, 0, 16);
    memset(prefix.mask.ip6, 0xff, 8);
    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_STR_EQ(buf, "1::ff/64", res);

    uint16_t ip6b[] = { 0x0100, 0, 0, 0, 0, 0, 0, 0xff00 };
    memcpy(prefix.addr.ip6, ip6b, 16);
    memset(prefix.mask.ip6, 0, 16);
    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_STR_EQ(buf, "1::ff/0", res);

    uint16_t ip6c[] = { 0, 0, 0, 0, 0, 0, 0, 0x100 };
    memcpy(prefix.addr.ip6, ip6c, 16);
    memset(prefix.mask.ip6, 0xff, 16);
    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_STR_EQ(buf, "::1/128", res);

    uint16_t ip6d[] = { 0, 0, 0, 0, 0, 0, 0, 0x100 };
    memcpy(prefix.addr.ip6, ip6d, 16);
    memset(prefix.mask.ip6, 0xff, 16);
    prefix.mask.ip6[5] = 0; /* hole */
    res = sai_serialize_ip_prefix(buf, &prefix);
    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_deserialize_ip_prefix()
{
    sai_ip_prefix_t prefix;

    int res;

    res = sai_deserialize_ip_prefix("foo", &prefix);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_ip_prefix("256.1.1.1/30", &prefix);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_ip_prefix("fffff::1/128", &prefix);
    ASSERT_TRUE(res < 0, "expected negative number");

    /* ipv4 */

    res = sai_deserialize_ip_prefix("10.0.0.21/32", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV4, "expected true");
    ASSERT_TRUE(prefix.addr.ip4 == htonl(0x0a000015), "expected true: 0x%x", prefix.addr.ip4);
    ASSERT_TRUE(prefix.mask.ip4 == htonl(0xffffffff), "expected true");

    res = sai_deserialize_ip_prefix("10.0.0.21/17", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV4, "expected true");
    ASSERT_TRUE(prefix.addr.ip4 == htonl(0x0a000015), "expected true");
    ASSERT_TRUE(prefix.mask.ip4 == htonl(0xffff8000), "expected true");

    res = sai_deserialize_ip_prefix("10.0.0.21/17\"", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV4, "expected true");
    ASSERT_TRUE(prefix.addr.ip4 == htonl(0x0a000015), "expected true");
    ASSERT_TRUE(prefix.mask.ip4 == htonl(0xffff8000), "expected true");
    ASSERT_TRUE(res == (int)strlen("10.0.0.21/17"), "expected true");

    res = sai_deserialize_ip_prefix("10.0.0.21/0", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV4, "expected true");
    ASSERT_TRUE(prefix.addr.ip4 == htonl(0x0a000015), "expected true");
    ASSERT_TRUE(prefix.mask.ip4 == htonl(0), "expected true");

    /* ipv6 */

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3303, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };
    uint16_t mask[16];

    memset(mask, 0xff, sizeof(mask));
    res = sai_deserialize_ip_prefix("1111:2222:333:4444:5555:6666:aaaa:bbbb/128", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV6, "expected true");
    ASSERT_TRUE(memcmp(prefix.addr.ip6, ip6, 16) == 0, "expected true");
    ASSERT_TRUE(memcmp(prefix.mask.ip6, mask, 16) == 0, "expected true");
    ASSERT_TRUE(res == (int)strlen("1111:2222:333:4444:5555:6666:aaaa:bbbb/128"), "expected true: %d", res);

    memset(mask, 0, sizeof(mask));
    memset(mask, 0xff, 8);
    ((uint8_t*)mask)[8] = 0x80;

    uint16_t ip6a[] = { 0x0100, 0, 0, 0, 0, 0, 0, 0xff00 };

    res = sai_deserialize_ip_prefix("1::ff/65\"", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV6, "expected true");
    ASSERT_TRUE(res == (int)strlen("1::ff/65"), "expected true");

    ASSERT_TRUE(memcmp(prefix.addr.ip6, ip6a, 16) == 0, "expected true");
    ASSERT_TRUE(memcmp(prefix.mask.ip6, mask, 16) == 0, "expected true");

    memset(mask, 0, sizeof(mask));
    res = sai_deserialize_ip_prefix("1::ff/0", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV6, "expected true");
    ASSERT_TRUE(res == (int)strlen("1::ff/0"), "expected true");

    ASSERT_TRUE(memcmp(prefix.addr.ip6, ip6a, 16) == 0, "expected true");
    ASSERT_TRUE(memcmp(prefix.mask.ip6, mask, 16) == 0, "expected true");

    uint16_t ip6b[] = { 0, 0, 0, 0, 0, 0, 0, 0x100 };

    memset(mask, 0xff, sizeof(mask));
    res = sai_deserialize_ip_prefix("::1/128", &prefix);
    ASSERT_TRUE(prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV6, "expected true");
    ASSERT_TRUE(res == (int)strlen("::1/128"), "expected true");
    ASSERT_TRUE(memcmp(prefix.addr.ip6, ip6b, 16) == 0, "expected true");
    ASSERT_TRUE(memcmp(prefix.mask.ip6, mask, 16) == 0, "expected true");
}

void test_serialize_ip4_mask()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_ip4_t mask = 0;

    res = sai_serialize_ip4_mask(buf, mask);

    ASSERT_STR_EQ(buf, "0", res);

    mask = 0xffffffff;

    res = sai_serialize_ip4_mask(buf, mask);

    ASSERT_STR_EQ(buf, "32", res);

    mask = 0xffffffff;

    int i;
    char exp[PRIMITIVE_BUFFER_SIZE];

    for (i = 32; i >= 0; i--)
    {
        res = sai_serialize_ip4_mask(buf, htonl(mask));

        sprintf(exp, "%d", i);

        ASSERT_STR_EQ(buf, exp, res);

        mask = mask << 1;
    }

    mask = htonl(0xff001); /* holes */

    res = sai_serialize_ip4_mask(buf, mask);

    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_deserialize_ip4_mask()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_ip4_t mask = 1;

    mask = 0xffffffff;

    int i;

    for (i = 32; i >= 0; i--)
    {
        sai_ip4_t mask2;

        sprintf(buf, "%d", i);

        res = sai_deserialize_ip4_mask(buf, &mask2);

        ASSERT_TRUE(res == (int)strlen(buf), "expected true: %d: 0x%x", i, mask2);
        ASSERT_TRUE(htonl(mask) == mask2, "expected true 0x%x vs 0x%x", mask, mask2);

        mask = mask << 1;
    }

    res = sai_deserialize_ip4_mask("-1", &mask);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_ip4_mask("33", &mask);
    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_serialize_ip6_mask()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];

    sai_ip6_t mask = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    mask[1] = 0xff; /* mask with holes */

    res = sai_serialize_ip6_mask(buf, mask);

    ASSERT_TRUE(res < 0, "expected negative number");

    uint8_t *m = (uint8_t*)mask;

    int n = 0;
    char bufn[PRIMITIVE_BUFFER_SIZE];
    char ipv6[PRIMITIVE_BUFFER_SIZE];

    for (; n <= 128; n++)
    {
        memset(m, 0, 16);

        int k;
        for (k = 0; k < n; k++)
        {
            uint8_t u = (uint8_t)(0xff << (7 - k%8));
            m[k/8] |= u;
        }

        sprintf(bufn, "%d", n);

        sai_serialize_ip6(ipv6, mask);
        res = sai_serialize_ip6_mask(buf, mask);

        ASSERT_STR_EQ(buf, bufn, res);
    }
}

void test_deserialize_ip6_mask()
{
    sai_ip6_t mask  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    sai_ip6_t mask2 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    uint8_t *m = (uint8_t*)mask;

    int res;

    char bufn[PRIMITIVE_BUFFER_SIZE];

    int n = 0;

    for (; n <= 128; n++)
    {
        memset(m, 0, 16);

        int k;
        for (k = 0; k < n; k++)
        {
            uint8_t u = (uint8_t)(0xff << (7 - k%8));
            m[k/8] |= u;
        }

        sprintf(bufn, "%d", n);

        res = sai_deserialize_ip6_mask(bufn, mask2);
        ASSERT_TRUE(res == (int)strlen(bufn), "expected true, res: %d, n: %d", res, n);
        sai_serialize_ip6(bufn, mask2);
        ASSERT_TRUE(memcmp(mask, mask2, 16) == 0, "expected true: %d = %s", n, bufn);
    }

    res = sai_deserialize_ip6_mask("-1", mask);
    ASSERT_TRUE(res < 0, "expected negative number");

    res = sai_deserialize_ip6_mask("129", mask);
    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_serialize_route_entry()
{
    sai_route_entry_t re;

    char buf[PRIMITIVE_BUFFER_SIZE];
    int res;

    re.switch_id = 0x123;
    re.vr_id = 0xfab;
    re.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV4;

    re.destination.addr.ip4 = htonl(0x01020304);
    re.destination.mask.ip4 = htonl(0xffffffff);

    res = sai_serialize_route_entry(buf, &re);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"vr_id\":\"oid:0xfab\",\"destination\":\"1.2.3.4/32\"}", res);

    re.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV6;

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    memcpy(re.destination.addr.ip6, ip6, 16);
    memset(re.destination.mask.ip6, 0xff, 16);

    res = sai_serialize_route_entry(buf, &re);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"vr_id\":\"oid:0xfab\",\"destination\":\"1111:2222:3333:4444:5555:6666:aaaa:bbbb/128\"}", res);

    re.destination.addr_family = 2;

    res = sai_serialize_route_entry(buf, &re);

    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_deserialize_route_entry()
{
    sai_route_entry_t re;

    char buf[PRIMITIVE_BUFFER_SIZE];
    char buf2[PRIMITIVE_BUFFER_SIZE];
    int res;

    re.switch_id = 0x123;
    re.vr_id = 0xfab;
    re.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV4;

    re.destination.addr.ip4 = htonl(0x01020304);
    re.destination.mask.ip4 = htonl(0xffffffff);

    res = sai_serialize_route_entry(buf, &re);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"vr_id\":\"oid:0xfab\",\"destination\":\"1.2.3.4/32\"}", res);

    sai_route_entry_t dere;

    memset(&dere, 0, sizeof(dere));
    res = sai_deserialize_route_entry(buf, &dere);
    res = sai_serialize_route_entry(buf2, &dere);

    ASSERT_TRUE(res == (int)strlen(buf), "result length is not expected: %d", res);
    ASSERT_TRUE(strcmp(buf, buf2) == 0, "deserialized value is not the same as serialized");

    re.destination.addr_family = SAI_IP_ADDR_FAMILY_IPV6;

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    memcpy(re.destination.addr.ip6, ip6, 16);
    memset(re.destination.mask.ip6, 0xff, 16);

    res = sai_serialize_route_entry(buf, &re);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"vr_id\":\"oid:0xfab\",\"destination\":\"1111:2222:3333:4444:5555:6666:aaaa:bbbb/128\"}", res);

    memset(&dere, 0, sizeof(dere));
    res = sai_deserialize_route_entry(buf, &dere);
    res = sai_serialize_route_entry(buf2, &dere);

    ASSERT_TRUE(res == (int)strlen(buf), "result length is not expected: %d", res);
    ASSERT_TRUE(strcmp(buf, buf2) == 0, "deserialized value is not the same as serialized");

    re.destination.addr_family = 2;

    res = sai_serialize_route_entry(buf, &re);

    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_serialize_neighbor_entry()
{
    char buf[PRIMITIVE_BUFFER_SIZE];
    int res;

    sai_neighbor_entry_t ne;

    ne.switch_id = 0x123;
    ne.rif_id = 0xfab;

    ne.ip_address.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    ne.ip_address.addr.ip4 = htonl(0x01020304);

    res = sai_serialize_neighbor_entry(buf, &ne);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"rif_id\":\"oid:0xfab\",\"ip_address\":\"1.2.3.4\"}", res);

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    memcpy(ne.ip_address.addr.ip6, ip6, 16);

    ne.ip_address.addr_family = SAI_IP_ADDR_FAMILY_IPV6;

    res = sai_serialize_neighbor_entry(buf, &ne);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"rif_id\":\"oid:0xfab\",\"ip_address\":\"1111:2222:3333:4444:5555:6666:aaaa:bbbb\"}", res);

    ne.ip_address.addr_family = 2;

    res = sai_serialize_neighbor_entry(buf, &ne);

    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_deserialize_neighbor_entry()
{
    char buf[PRIMITIVE_BUFFER_SIZE];
    char buf2[PRIMITIVE_BUFFER_SIZE];
    int res;

    sai_neighbor_entry_t ne;

    ne.switch_id = 0x123;
    ne.rif_id = 0xfab;

    ne.ip_address.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    ne.ip_address.addr.ip4 = htonl(0x01020304);

    res = sai_serialize_neighbor_entry(buf, &ne);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"rif_id\":\"oid:0xfab\",\"ip_address\":\"1.2.3.4\"}", res);

    sai_neighbor_entry_t dene;

    memset(&dene, 0, sizeof(dene));
    res = sai_deserialize_neighbor_entry(buf, &dene);
    res = sai_serialize_neighbor_entry(buf2, &dene);

    ASSERT_TRUE(res == (int)strlen(buf), "result length is not expected: %d", res);
    ASSERT_TRUE(strcmp(buf, buf2) == 0, "deserialized value is not the same as serialized");

    uint16_t ip6[] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0xaaaa, 0xbbbb };

    memcpy(ne.ip_address.addr.ip6, ip6, 16);

    ne.ip_address.addr_family = SAI_IP_ADDR_FAMILY_IPV6;

    res = sai_serialize_neighbor_entry(buf, &ne);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"rif_id\":\"oid:0xfab\",\"ip_address\":\"1111:2222:3333:4444:5555:6666:aaaa:bbbb\"}", res);

    memset(&dene, 0, sizeof(dene));
    res = sai_deserialize_neighbor_entry(buf, &dene);
    res = sai_serialize_neighbor_entry(buf2, &dene);

    ASSERT_TRUE(res == (int)strlen(buf), "result length is not expected: %d", res);
    ASSERT_TRUE(strcmp(buf, buf2) == 0, "deserialized value is not the same as serialized");

    ne.ip_address.addr_family = 2;

    res = sai_serialize_neighbor_entry(buf, &ne);

    ASSERT_TRUE(res < 0, "expected negative number");
}

void test_serialize_fdb_entry()
{
    char buf[PRIMITIVE_BUFFER_SIZE];
    int res;

    sai_fdb_entry_t fe;

    fe.switch_id = 0x123;
    fe.bv_id = 0xfab;

    memcpy(fe.mac_address, "\x01\x23\x45\x67\x89\xab", 6);

    res = sai_serialize_fdb_entry(buf, &fe);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"}", res);
}

void test_deserialize_fdb_entry()
{
    char buf[PRIMITIVE_BUFFER_SIZE];
    char buf2[PRIMITIVE_BUFFER_SIZE];
    int res;

    sai_fdb_entry_t fe;

    fe.switch_id = 0x123;
    fe.bv_id = 0xfab;

    memcpy(fe.mac_address, "\x01\x23\x45\x67\x89\xab", 6);

    res = sai_serialize_fdb_entry(buf, &fe);

    ASSERT_STR_EQ(buf, "{\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"}", res);

    sai_fdb_entry_t defe;

    memset(&defe, 0, sizeof(defe));

    res = sai_deserialize_fdb_entry(buf, &defe);
    res = sai_serialize_fdb_entry(buf2, &defe);

    ASSERT_TRUE(res == (int)strlen(buf), "result length is not expected: %d", res);
    ASSERT_TRUE(strcmp(buf, buf2) == 0, "deserialized value is not the same as serialized");

    /* negative cases */

    const char* ncases[] = {
        "dfd",
        "[\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"}",
        "{\"switch_it\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"}",
        "{\"switch_id\":\"oid:0xg23\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"}",
        "{\"switch_id\":\"oid:0x123\",\"mac1address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"}",
        "{\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:h3:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"}",
        "{\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv1id\":\"oid:0xfab\"}",
        "{\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xtab\"}",
        "{\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\"]",
        "{\"switch_id\":\"oid:0x123\",\"mac_address\":\"01:23:45:67:89:AB\",\"bv_id\":\"oid:0xfab\'}",
    };

    size_t i = 0;
    for (; i < sizeof(ncases)/sizeof(const char*); ++i)
    {
        res = sai_deserialize_fdb_entry(ncases[i], &defe);
        ASSERT_TRUE(res < 0, "expected negative result: %d", res);
    }
}

void test_serialize_notifications()
{
    char buf[0x100 * PRIMITIVE_BUFFER_SIZE];
    int res;
    const char* ret;

    sai_object_id_t switch_id = 0x123abc;

    sai_fdb_event_notification_data_t data;
    memset(&data, 0, sizeof(data));

    res = sai_serialize_fdb_event_notification(buf, 1, &data);
    ret = "{\"count\":1,\"data\":[{\"event_type\":\"SAI_FDB_EVENT_LEARNED\",\"fdb_entry\":{\"switch_id\":\"oid:0x0\",\"mac_address\":\"00:00:00:00:00:00\",\"bv_id\":\"oid:0x0\"},\"attr_count\":0,\"attr\":null}]}";

    ASSERT_STR_EQ(buf, ret, res);

    char buffer[7] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };

    sai_attribute_t attrs[1];

    /*
     * TODO we set to zero, since attr serialize is not supported yet, after
     * support we need to fix this.
     */

    res = sai_serialize_packet_event_notification(buf, switch_id, 7, buffer, 0, attrs);
    ret = "{\"switch_id\":\"oid:0x123abc\",\"buffer_size\":7,\"buffer\":[17,34,51,68,85,102,119],\"attr_count\":0,\"attr_list\":null}";
    ASSERT_STR_EQ(buf, ret, res);

    sai_port_oper_status_notification_t data1;
    memset(&data1, 0, sizeof(data1));

    res = sai_serialize_port_state_change_notification(buf, 1, &data1);
    ret = "{\"count\":1,\"data\":[{\"port_id\":\"oid:0x0\",\"port_state\":\"SAI_PORT_OPER_STATUS_UNKNOWN\",\"port_error_status\":\"SAI_PORT_ERROR_STATUS_CLEAR\"}]}";
    ASSERT_STR_EQ(buf, ret , res);

    sai_queue_deadlock_notification_data_t data2;
    memset(&data2, 0, sizeof(data2));

    res = sai_serialize_queue_pfc_deadlock_notification(buf, 1, &data2);
    ret = "{\"count\":1,\"data\":[{\"queue_id\":\"oid:0x0\",\"event\":\"SAI_QUEUE_PFC_DEADLOCK_EVENT_TYPE_DETECTED\",\"app_managed_recovery\":false}]}";
    ASSERT_STR_EQ(buf, ret, res);

    res = sai_serialize_switch_shutdown_request_notification(buf, switch_id);
    ret = "{\"switch_id\":\"oid:0x123abc\"}";
    ASSERT_STR_EQ(buf, ret, res);

    res = sai_serialize_switch_state_change_notification(buf, switch_id, SAI_SWITCH_OPER_STATUS_UP);
    ret = "{\"switch_id\":\"oid:0x123abc\",\"switch_oper_status\":\"SAI_SWITCH_OPER_STATUS_UP\"}";
    ASSERT_STR_EQ(buf, ret, res);

    char buffer1[7] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };

    sai_attribute_t attrs1[1];

    res = sai_serialize_tam_event_notification(buf, 1, 7, buffer1, 0, attrs1);
    ret = "{\"tam_event_id\":\"oid:0x1\",\"buffer_size\":7,\"buffer\":[17,34,51,68,85,102,119],\"attr_count\":0,\"attr_list\":null}";
    ASSERT_STR_EQ(buf, ret, res);
}

void sai_serialize_log(
        _In_ sai_log_level_t log_level,
        _In_ const char *file,
        _In_ int line,
        _In_ const char *func,
        _In_ const char *format,
        ...)
    __attribute__ ((format (printf, 5, 6)));

void sai_serialize_log(
        _In_ sai_log_level_t log_level,
        _In_ const char *file,
        _In_ int line,
        _In_ const char *func,
        _In_ const char *format,
        ...)
{
    char buffer[LONG_BUFFER_SIZE];
    char logbuffer[PRIMITIVE_BUFFER_SIZE];

    va_list ap;
    va_start(ap, format);
    vsprintf(buffer, format, ap);
    va_end(ap);

    sai_serialize_log_level(logbuffer, log_level);

    /*
     * Print warnings and lib errors to stdout, only asserts should be printed
     * on stderr.
     */

    printf("%s:%s:%s:%d: %s\n", logbuffer, file, func, line, buffer);
}

void test_serialize_attr_value_pointer()
{
    char buf[0x100 * PRIMITIVE_BUFFER_SIZE];
    int res;
    const char* ret;

    sai_attribute_t attr;

    attr.id = SAI_SWITCH_ATTR_SWITCH_STATE_CHANGE_NOTIFY;
    attr.value.ptr = (sai_pointer_t)0xaabb;

    const sai_attr_metadata_t *meta = sai_metadata_get_attr_metadata(
            SAI_OBJECT_TYPE_SWITCH,
            SAI_SWITCH_ATTR_SWITCH_STATE_CHANGE_NOTIFY);

    res = sai_serialize_attribute(buf, meta, &attr);

    ret = "{\"id\":\"SAI_SWITCH_ATTR_SWITCH_STATE_CHANGE_NOTIFY\",\"value\":{\"ptr\":\"ptr:0xaabb\"}}";

    ASSERT_STR_EQ(buf, ret, res);
}

void test_deserialize_pointer()
{
    char buf[0x100 * PRIMITIVE_BUFFER_SIZE];
    sai_pointer_t ptr = 0;
    int res;

    res = sai_serialize_pointer(buf, ptr);

    ASSERT_STR_EQ(buf, "ptr:(nil)", res);

    res = sai_deserialize_pointer(buf, &ptr);

    ASSERT_TRUE(res > 0, "expected success");
    ASSERT_TRUE(ptr == 0, "expected pointer to be null");

#if INTPTR_MAX == INT32_MAX

    const char *buf1 = "ptr:0x11223344";

    res = sai_deserialize_pointer(buf1, &ptr);

    ASSERT_TRUE(res > 0, "expected success");
    ASSERT_TRUE(ptr == (sai_pointer_t)0x11223344, "not equal pointer");

#else

    const char *buf1 = "ptr:0x1122334455667788";

    res = sai_deserialize_pointer(buf1, &ptr);

    ASSERT_TRUE(res > 0, "expected success");
    ASSERT_TRUE(ptr == (sai_pointer_t)0x1122334455667788, "not equal pointer");

#endif
}

void test_serialize_enum_list()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];
    sai_s32_list_t list;

    sai_object_type_t ot[2] = {SAI_OBJECT_TYPE_PORT, SAI_OBJECT_TYPE_LAG};
    list.count = 2;
    list.list = (int32_t *)&ot[0];

    res = sai_serialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);

    ASSERT_STR_EQ(buf, "{\"count\":2,\"list\":[\"SAI_OBJECT_TYPE_PORT\",\"SAI_OBJECT_TYPE_LAG\"]}", res);

    ot[1] = -1;
    res = sai_serialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);

    ASSERT_STR_EQ(buf, "{\"count\":2,\"list\":[\"SAI_OBJECT_TYPE_PORT\",\"-1\"]}", res);

    ot[1] = 228;

    res = sai_serialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);
    ASSERT_STR_EQ(buf, "{\"count\":2,\"list\":[\"SAI_OBJECT_TYPE_PORT\",\"228\"]}", res);

    ot[1] = SAI_OBJECT_TYPE_LAG;
    res = sai_serialize_enum_list(buf, NULL, &list);
    ASSERT_STR_EQ(buf, "{\"count\":2,\"list\":[1,2]}", res);

    list.count = 0;
    list.list = NULL;
    res = sai_serialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);
    ASSERT_STR_EQ(buf, "{\"count\":0,\"list\":null}", res);
}

void test_deserialize_enum_list()
{
    int res;
    const char *buf;
    sai_s32_list_t list = {0};

    buf = "{\"count\":2,\"list\":[\"SAI_OBJECT_TYPE_PORT\",\"SAI_OBJECT_TYPE_LAG\"]}";
    res = sai_deserialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");
    ASSERT_TRUE(list.count == 2, "expected true");
    ASSERT_TRUE(list.list[0] == SAI_OBJECT_TYPE_PORT, "expected true");
    ASSERT_TRUE(list.list[1] == SAI_OBJECT_TYPE_LAG, "expected true");
    free(list.list);
    list.list = NULL;
    list.count = 0;

    buf = "{\"count\":2,\"list\":[\"SAI_OBJECT_TYPE_PORT\",\"-1\"]}";
    res = sai_deserialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");
    ASSERT_TRUE(list.count == 2, "expected true");
    ASSERT_TRUE(list.list[0] == SAI_OBJECT_TYPE_PORT, "expected true");
    ASSERT_TRUE(list.list[1] == -1, "expected true");
    free(list.list);
    list.list = NULL;
    list.count = 0;

    buf = "{\"count\":2,\"list\":[\"SAI_OBJECT_TYPE_PORT\",\"228\"]}";
    res = sai_deserialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");
    ASSERT_TRUE(list.count == 2, "expected true");
    ASSERT_TRUE(list.list[0] == SAI_OBJECT_TYPE_PORT, "expected true");
    ASSERT_TRUE(list.list[1] == 228, "expected true");
    free(list.list);
    list.list = NULL;
    list.count = 0;

    buf = "{\"count\":2,\"list\":[1,2]}";
    res = sai_deserialize_enum_list(buf, NULL, &list);
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");
    ASSERT_TRUE(list.count == 2, "expected true");
    ASSERT_TRUE(list.list[0] == SAI_OBJECT_TYPE_PORT, "expected true");
    ASSERT_TRUE(list.list[1] == SAI_OBJECT_TYPE_LAG, "expected true");
    free(list.list);
    list.list = NULL;
    list.count = 0;

    buf = "{\"count\":0,\"list\":null}";
    res = sai_deserialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");
    ASSERT_TRUE(list.count == 0, "expected true");
    ASSERT_TRUE(list.list == NULL, "expected true");

    buf = "{\"count\":0,\"list\":}";
    res = sai_deserialize_enum_list(buf, &sai_metadata_enum_sai_object_type_t, &list);
    ASSERT_TRUE(res == -1, "expected true");
}

void test_serialize_attr_id()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE];
    sai_attr_id_t attr_id;

    attr_id = SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS;

    res = sai_serialize_attr_id(buf, NULL, attr_id);

    ASSERT_TRUE(res < 0, "expected negative");

    /* test all ids */

    size_t i = 0;

    for (; i < sai_metadata_attr_sorted_by_id_name_count; ++i)
    {
        const sai_attr_metadata_t* amd = sai_metadata_attr_sorted_by_id_name[i];

        res = sai_serialize_attr_id(buf, amd, amd->attrid);

        ASSERT_STR_EQ(buf, amd->attridname, res);
    }
}

void test_deserialize_attr_id()
{
    int res;
    sai_attr_id_t attr_id;

    res = sai_deserialize_attr_id("100", &attr_id);
    ASSERT_TRUE(res < 0, "expected negative");

    res = sai_deserialize_attr_id("-1", &attr_id);
    ASSERT_TRUE(res < 0, "expected negative");

    /* test all enums */

    size_t i = 0;

    for (; i < sai_metadata_attr_sorted_by_id_name_count; ++i)
    {
        const sai_attr_metadata_t* amd = sai_metadata_attr_sorted_by_id_name[i];

        res = sai_deserialize_attr_id(amd->attridname, &attr_id);

        ASSERT_TRUE(res == (int)strlen(amd->attridname), "expected true");

        ASSERT_TRUE(attr_id == amd->attrid, "expected true");
    }
}

void test_serialize_attribute()
{
    int res;
    char buf[PRIMITIVE_BUFFER_SIZE * 2];
    sai_attribute_t attribute = {0};
    const sai_attr_metadata_t* amd;

    amd = sai_metadata_get_attr_metadata(SAI_OBJECT_TYPE_SWITCH, 0);
    attribute.id = SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS;
    attribute.value.u32 = 3;

    res = sai_serialize_attribute(buf, amd, &attribute);

    ASSERT_STR_EQ(buf, "{\"id\":\"SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS\",\"value\":{\"u32\":3}}", res);

    attribute.id = 99999;
    memset(&attribute.value, 0x55, sizeof(attribute.value));

    res = sai_serialize_attribute(buf, NULL, &attribute);

    ASSERT_TRUE(res < 0, "expected negative");
}

void test_deserialize_attribute()
{
    int res;
    const char *buf;
    sai_attribute_t attribute = {0};

    buf = "{\"id\":\"SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS\",\"value\":{\"u32\":3}}";
    res = sai_deserialize_attribute(buf, &attribute);
    ASSERT_TRUE(res == (int)strlen(buf), "expected true");
    ASSERT_TRUE(attribute.id == SAI_SWITCH_ATTR_NUMBER_OF_ACTIVE_PORTS, "expected true");
    ASSERT_TRUE(attribute.value.u32 == 3, "expected true");

    buf = "{\"id\":\"99999\",\"value\":{85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85}}";
    res = sai_deserialize_attribute(buf, &attribute);
    ASSERT_TRUE(res < 0, "expected negative");
}

int main()
{

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
    sai_metadata_log = &sai_serialize_log;
#pragma GCC diagnostic pop

    ASSERT_TRUE(sizeof(sai_size_t) <= sizeof(uint64_t),
            "sai_size_t should be less or equal to 64 bit uint");

    test_serialize_bool();
    test_deserialize_bool();

    test_serialize_chardata();
    test_deserialize_chardata();

    /* TODO test int/uint serialize/deserialize */

    test_serialize_object_id();
    test_deserialize_object_id();

    test_serialize_mac();
    test_deserialize_mac();

    test_serialize_enum();
    test_deserialize_enum();

    test_serialize_ip4();
    test_deserialize_ip4();

    test_serialize_ip6();
    test_deserialize_ip6();

    test_serialize_ip_address();
    test_deserialize_ip_address();

    test_serialize_ip_prefix();
    test_deserialize_ip_prefix();

    test_serialize_ip4_mask();
    test_deserialize_ip4_mask();

    test_serialize_ip6_mask();
    test_deserialize_ip6_mask();

    test_serialize_ip4_mask();
    test_serialize_ip6_mask();

    test_serialize_attr_value_pointer();

    test_deserialize_pointer();

    /* test generated methods */

    test_serialize_route_entry();
    test_serialize_neighbor_entry();
    test_serialize_fdb_entry();

    test_deserialize_route_entry();
    test_deserialize_neighbor_entry();
    test_deserialize_fdb_entry();

    test_serialize_notifications();

    test_serialize_encrypt_key();
    test_deserialize_encrypt_key();
    test_serialize_auth_key();
    test_deserialize_auth_key();
    test_serialize_macsec_auth_key();
    test_deserialize_macsec_auth_key();
    test_serialize_macsec_salt();
    test_deserialize_macsec_salt();

    test_serialize_enum_list();
    test_deserialize_enum_list();
    test_serialize_attr_id();
    test_deserialize_attr_id();
    test_serialize_attribute();
    test_deserialize_attribute();

    return 0;
}
