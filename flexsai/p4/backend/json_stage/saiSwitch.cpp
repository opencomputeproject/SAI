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
#include "saiSwitch.h"
#include "frontends/common/model.h"

namespace SAI {


// These string constants must match those defined in sai_switch.p4
const char* Switch_Model::P4_PIPELINE_PARSER = "parse";
const char* Switch_Model::P4_PIPELINE_DEPARSER = "deparse";
const char* Switch_Model::P4_PIPELINE_INGRESS_L2 = "ingressPort";
const char* Switch_Model::P4_PIPELINE_EGRESS_L2 = "egressPort";
const char* Switch_Model::P4_PIPELINE_INGRESS_L3 = "ingressRif";
const char* Switch_Model::P4_PIPELINE_EGRESS_L3 = "egressRif";
// These string constants must match those in the python transpiler
const char* Switch_Model::JSON_PIPELINE_PARSER = "parse";
const char* Switch_Model::JSON_PIPELINE_DEPARSER = "deparse";
const char* Switch_Model::JSON_PIPELINE_INGRESS_L2 = "control_in_port";
const char* Switch_Model::JSON_PIPELINE_EGRESS_L2 = "control_out_port";
const char* Switch_Model::JSON_PIPELINE_INGRESS_L3 = "control_in_rif";
const char* Switch_Model::JSON_PIPELINE_EGRESS_L3 = "control_out_rif";

SaiSwitch SaiSwitch::instance;

void
SaiSwitch::modelError(const char* format, const IR::Node* node) const {
    ::error(format, node);
    ::error("Are you using an up-to-date saiSwitch.p4?");
}

bool SaiSwitch::find_match_kind(cstring kind_name) {
    bool found = false;
    for (auto m : instance.match_kinds) {
        if (m->toString() == kind_name) {
            found = true;
            break;
        }
    }
    return found;
}

bool SaiSwitch::find_extern(cstring extern_name) {
    bool found = false;
    for (auto m : instance.externs) {
        if (m->type.toString() == extern_name) {
            found = true;
            break;
        }
    }
    return found;
}

void SaiSwitch::setPipelineControls(const IR::ToplevelBlock* toplevel,
                                  std::set<cstring>* controls,
                                  std::map<cstring, cstring>* map) {
    if (errorCount() != 0)
        return;
    auto main = toplevel->getMain();
    if (main == nullptr) {
        ::error("`%1%' module not found for sai switch", IR::P4Program::main);
        return;
    }
    auto ingressPort = main->findParameterValue(sw.ingressPort.name);
    auto egressPort = main->findParameterValue(sw.egressPort.name);
    if (ingressPort == nullptr || egressPort == nullptr ||
        !ingressPort->is<IR::ControlBlock>() || !egressPort->is<IR::ControlBlock>()) {
        modelError("%1%: main package L2 pipeline does not match the expected model", main);
        return;
    }
    auto ingress_name = ingressPort->to<IR::ControlBlock>()->container->name;
    auto egress_name = egressPort->to<IR::ControlBlock>()->container->name;
    controls->emplace(ingress_name);
    controls->emplace(egress_name);
    map->emplace(ingress_name, Switch_Model::JSON_PIPELINE_INGRESS_L2);
    map->emplace(egress_name, Switch_Model::JSON_PIPELINE_EGRESS_L2);

    auto ingressRif = main->findParameterValue(sw.ingressRif.name);
    auto egressRif = main->findParameterValue(sw.egressRif.name);
    if (ingressRif == nullptr || egressRif == nullptr ||
        !ingressRif->is<IR::ControlBlock>() || !egressRif->is<IR::ControlBlock>()) {
        modelError("%1%: main package L3 pipeline does not match the expected model", main);
        return;
    }
    ingress_name = ingressRif->to<IR::ControlBlock>()->container->name;
    egress_name = egressRif->to<IR::ControlBlock>()->container->name;
    controls->emplace(ingress_name);
    controls->emplace(egress_name);
    map->emplace(ingress_name, Switch_Model::JSON_PIPELINE_INGRESS_L3);
    map->emplace(egress_name, Switch_Model::JSON_PIPELINE_EGRESS_L3);
}

const IR::P4Control* SaiSwitch::getIngress(const IR::ToplevelBlock* blk) {
    auto main = blk->getMain();
    auto ctrl = main->findParameterValue(sw.ingressPort.name);
    if (ctrl == nullptr)
        return nullptr;
    if (!ctrl->is<IR::ControlBlock>()) {
        modelError("%1%: main package  match the expected model", main);
        return nullptr;
    }
    return ctrl->to<IR::ControlBlock>()->container;
}

const IR::P4Control* SaiSwitch::getEgress(const IR::ToplevelBlock* blk) {
    auto main = blk->getMain();
    auto ctrl = main->findParameterValue(sw.egressPort.name);
    if (ctrl == nullptr)
        return nullptr;
    if (!ctrl->is<IR::ControlBlock>()) {
        modelError("%1%: main package  match the expected model", main);
        return nullptr;
    }
    return ctrl->to<IR::ControlBlock>()->container;
}

const IR::P4Parser* SaiSwitch::getParser(const IR::ToplevelBlock* blk) {
    auto main = blk->getMain();
    auto ctrl = main->findParameterValue(sw.parser.name);
    if (ctrl == nullptr)
        return nullptr;
    if (!ctrl->is<IR::ParserBlock>()) {
        modelError("%1%: main package  match the expected model", main);
        return nullptr;
    }
    return ctrl->to<IR::ParserBlock>()->container;
}

void
SaiSwitch::setParserControls(const IR::ToplevelBlock* toplevel,
                                     std::set<cstring>* controls) {
    if (errorCount() != 0)
        return;
    auto main = toplevel->getMain();
    auto parser = main->findParameterValue(sw.parser.name);
    if (parser == nullptr || !parser->is<IR::ParserBlock>()) {
        modelError("%1%: main package parser does not match the expected model", main);
        return;
    }
    controls->emplace(parser->to<IR::ParserBlock>()->container->name);
}

void
SaiSwitch::setDeparserControls(const IR::ToplevelBlock* toplevel,
                                  std::set<cstring>* controls) {
    auto main = toplevel->getMain();
    auto deparser = main->findParameterValue(sw.deparser.name);
    if (deparser == nullptr || !deparser->is<IR::ControlBlock>()) {
        modelError("%1%: main package deparser does not match the expected model", main);
        return;
    }
    controls->emplace(deparser->to<IR::ControlBlock>()->container->name);
}


} // namespace SAI

std::ostream& operator<<(std::ostream &out, Model::Type_Model& m) {
    out << "Type_Model(" << m.toString() << ")";
    return out;
}

std::ostream& operator<<(std::ostream &out, Model::Param_Model& p) {
    out << "Param_Model(" << p.toString() << ") " << p.type;
    return out;
}

std::ostream& operator<<(std::ostream &out, SAI::SaiSwitch& e) {
    out << "PortableModel " << e.version << std::endl;
    for (auto v : e.parsers)  out << v;
    for (auto v : e.controls) out << v;
    for (auto v : e.externs)  out << v;
    return out;
}

std::ostream& operator<<(std::ostream &out, SAI::Method_Model& p) {
    out << "Method_Model(" << p.toString() << ") " << p.type << std::endl;
    for (auto e : p.elems) {
        out << "    " << e << std::endl;
    }
    return out;
}

std::ostream& operator<<(std::ostream &out, SAI::Parser_Model* p) {
    out << "Parser_Model(" << p->toString() << ") " << p->type << std::endl;
    for (auto e : p->elems) {
        out << "  " << e << std::endl;
    }
    return out;
}

std::ostream& operator<<(std::ostream &out, SAI::Control_Model* p) {
    out << "Control_Model(" << p->toString() << ") " << p->type << std::endl;
    for (auto e : p->elems) {
        out << "  " << e << std::endl;
    }
    return out;
}

std::ostream& operator<<(std::ostream &out, SAI::Extern_Model* p) {
    out << "Extern_Model(" << p->toString() << ") " << p->type << std::endl;
    for (auto e : p->elems) {
        out << "  " << e << std::endl;
    }
    return out;
}

// getSkipControls();

// getPipelineControls();

// getUpdateChecksum();


