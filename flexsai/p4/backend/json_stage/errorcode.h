/*
Copyright 2013-present Barefoot Networks, Inc.

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

#ifndef _BACKENDS_SAI_ERRORCODE_H_
#define _BACKENDS_SAI_ERRORCODE_H_

#include "ir/ir.h"
#include "lib/json.h"
#include "frontends/p4/typeMap.h"
#include "frontends/common/resolveReferences/referenceMap.h"
#include "helpers.h"

namespace SAI {

class ErrorCodesVisitor : public Inspector {
    ErrorCodesMap*    errorCodesMap;
 public:
    // we map error codes to numerical values for bmv2
    bool preorder(const IR::Type_Error* errors) override;
    explicit ErrorCodesVisitor(ErrorCodesMap* errorCodesMap) :
        errorCodesMap(errorCodesMap)
    { CHECK_NULL(errorCodesMap); setName("ErrorCodeVisitor"); }
};

}  // namespace SAI

#endif  /* _BACKENDS_SAI_ERRORCODE_H_ */
