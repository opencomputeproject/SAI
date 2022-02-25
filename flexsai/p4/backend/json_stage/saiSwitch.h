/*
Copyright 2017 Mellanox.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _BACKENDS_SAI_SAISWITCH_H_
#define _BACKENDS_SAI_SAISWITCH_H_

#include "ir/ir.h"
#include "lower.h"
#include "lib/gmputil.h"
#include "lib/json.h"
#include "frontends/common/resolveReferences/referenceMap.h"
#include "frontends/p4/coreLibrary.h"
#include "frontends/p4/enumInstance.h"
#include "frontends/p4/methodInstance.h"
#include "frontends/p4/typeMap.h"
#include "helpers.h"

namespace SAI {

using ::Model::Elem;
using ::Model::Type_Model;
using ::Model::Param_Model;

// Block has a name and a collection of elements
template<typename T>
struct Block_Model : public Type_Model {
    std::vector<T> elems;
    explicit Block_Model(cstring name) :
        ::Model::Type_Model(name) {}
};

/// Enum_Model : Block_Model<Elem> : Type_Model
struct Enum_Model : public Block_Model<Elem> {
    ::Model::Type_Model type;
    explicit Enum_Model(cstring name) :
        Block_Model(name), type("Enum") {}
};

/// Parser_Model : Block_Model<Param_Model> : Type_Model
struct Parser_Model : public Block_Model<Param_Model> {
    ::Model::Type_Model type;
    explicit Parser_Model(cstring name) :
        Block_Model<Param_Model>(name), type("Parser") {}
};

/// Control_Model : Block_Model<Param_Model> : Type_Model
struct Control_Model : public Block_Model<Param_Model> {
    ::Model::Type_Model type;
    explicit Control_Model(cstring name) :
        Block_Model<Param_Model>(name), type("Control") {}
};

/// Method_Model : Block_Model<Param_Model> : Type_Model
struct Method_Model : public Block_Model<Param_Model> {
    ::Model::Type_Model type;
    explicit Method_Model(cstring name) :
        Block_Model<Param_Model>(name), type("Method") {}
};

/// Extern_Model : Block_Model<Method_Model> : Type_Model
struct Extern_Model : public Block_Model<Method_Model> {
    ::Model::Type_Model type;
    explicit Extern_Model(cstring name) :
        Block_Model<Method_Model>(name), type("Extern") {}
};

// Basic sai pipeline
struct Switch_Model : public ::Model::Elem {

    // These string constants must match those defined in sai_switch.p4
    static const char* P4_PIPELINE_PARSER;
    static const char* P4_PIPELINE_DEPARSER;
    static const char* P4_PIPELINE_INGRESS_L2;
    static const char* P4_PIPELINE_EGRESS_L2;
    static const char* P4_PIPELINE_INGRESS_L3;
    static const char* P4_PIPELINE_EGRESS_L3;
    // These string constants show up in the JSON output
    static const char* JSON_PIPELINE_PARSER;
    static const char* JSON_PIPELINE_DEPARSER;
    static const char* JSON_PIPELINE_INGRESS_L2;
    static const char* JSON_PIPELINE_EGRESS_L2;
    static const char* JSON_PIPELINE_INGRESS_L3;
    static const char* JSON_PIPELINE_EGRESS_L3;

    Switch_Model() : Model::Elem("SaiSwitch"),
                     file("sai_model.p4"),
                     parser(P4_PIPELINE_PARSER), ingressPort(P4_PIPELINE_INGRESS_L2), ingressRif(P4_PIPELINE_INGRESS_L3),
                     egressRif(P4_PIPELINE_EGRESS_L3), egressPort(P4_PIPELINE_EGRESS_L2), deparser(P4_PIPELINE_DEPARSER) {}
    ::Model::Elem       file;
    ::Model::Elem parser;  // names of the package arguments
    ::Model::Elem ingressPort;
    ::Model::Elem ingressRif;
    ::Model::Elem egressRif;
    ::Model::Elem egressPort;
    ::Model::Elem deparser;
};

/// SaiSwitch : Model::Model
class SaiSwitch : public ::Model::Model {
 public:
    std::vector<Parser_Model*>  parsers;
    std::vector<Control_Model*> controls;
    std::vector<Extern_Model*>  externs;
    std::vector<Type_Model*>    match_kinds;
    Switch_Model                sw;
    bool find_match_kind(cstring kind_name);
    bool find_extern(cstring extern_name);
    static SaiSwitch instance;
    SaiSwitch() : ::Model::Model("0.2") {}

    void modelError(const char* format, const IR::Node* place) const;

    void setPipelineControls(const IR::ToplevelBlock* toplevel,
                                      std::set<cstring>* controls,
                                      std::map<cstring, cstring>* map);
    void setParserControls(const IR::ToplevelBlock* blk, std::set<cstring>* controls);
    void setDeparserControls(const IR::ToplevelBlock* blk, std::set<cstring>* controls);

    const IR::P4Control* getIngress(const IR::ToplevelBlock* blk);
    const IR::P4Control* getEgress(const IR::ToplevelBlock* blk);
    const IR::P4Parser*  getParser(const IR::ToplevelBlock* blk);

};

}  // namespace SAI

std::ostream& operator<<(std::ostream &out, Model::Type_Model& m);
std::ostream& operator<<(std::ostream &out, Model::Param_Model& p);
std::ostream& operator<<(std::ostream &out, SAI::SaiSwitch& e);
std::ostream& operator<<(std::ostream &out, SAI::Method_Model& p);
std::ostream& operator<<(std::ostream &out, SAI::Parser_Model* p);
std::ostream& operator<<(std::ostream &out, SAI::Control_Model* p);
std::ostream& operator<<(std::ostream &out, SAI::Extern_Model* p);

// saiSwitch

#endif  /* _BACKENDS_SAI_SAISWITCH_H_ */
