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

#include <stdio.h>
#include <string>
#include <iostream>
#include <Python.h>

#include "ir/ir.h"
#include "control-plane/p4RuntimeSerializer.h"
#include "frontends/common/applyOptionsPragmas.h"
#include "frontends/common/parseInput.h"
#include "frontends/p4/frontend.h"
#include "lib/error.h"
#include "lib/exceptions.h"
#include "lib/gc.h"
#include "lib/log.h"
#include "lib/nullstream.h"
#include "backend.h"
#include "midend.h"
#include "options.h"
#include "JsonObjects.h"

int main_wrapper(int argc, char *const argv[]) {
    setup_gc_logging();


    AutoCompileContext autoSaiContext(new SAI::SaiContext);
    auto& options = SAI::SaiContext::get().options();
    options.langVersion = SAI::SaiOptions::FrontendVersion::P4_16;
    options.compilerVersion = "0.0.3";

    if (options.process(argc, argv) != nullptr)
        options.setInputFile();
    if (::errorCount() > 0)
        return 1;

    auto hook = options.getDebugHook();

    // SAI is not required to be compatibility with the previous p4-14 compiler.
    options.preprocessor_options += " -D__TARGET_SAI__";
    auto program = P4::parseP4File(options);
    if (program == nullptr || ::errorCount() > 0)
        return 1;
    try {
        P4::P4COptionPragmaParser optionsPragmaParser;
        program->apply(P4::ApplyOptionsPragmas(optionsPragmaParser));

        P4::FrontEnd frontend;
        frontend.addDebugHook(hook);
        program = frontend.run(options, program);
    } catch (const Util::P4CExceptionBase &bug) {
        std::cerr << bug.what() << std::endl;
        return 1;
    }
    if (program == nullptr || ::errorCount() > 0)
        return 1;

    const IR::ToplevelBlock* toplevel = nullptr;
    SAI::MidEnd midEnd(options);
    midEnd.addDebugHook(hook);
    try {
        toplevel = midEnd.process(program);
        if (::errorCount() > 1 || toplevel == nullptr ||
            toplevel->getMain() == nullptr)
            return 1;
        if (options.dumpJsonFile)
            JSONGenerator(*openFile(options.dumpJsonFile, true)) << program << std::endl;
    } catch (const Util::P4CExceptionBase &bug) {
        std::cerr << bug.what() << std::endl;
        return 1;
    }
    if (::errorCount() > 0)
        return 1;

    // backend depends on the modified refMap and typeMap from midEnd.
    SAI::Backend backend(options.isv1(), &midEnd.refMap,
            &midEnd.typeMap, &midEnd.enumMap);
    try {
        backend.addDebugHook(hook);
        backend.process(toplevel, options);
        backend.convert(options);
    } catch (const Util::P4CExceptionBase &bug) {
        std::cerr << bug.what() << std::endl;
        return 1;
    }
    if (::errorCount() > 0)
        return 1;

    if (!options.outputFile.isNullOrEmpty()) {
        std::ostream* out = openFile(options.outputFile, false);
        if (out != nullptr) {
            backend.serialize(*out);
            out->flush();
            std::cout << "Wrote output to " << options.outputFile << std::endl;
        }
    }

    P4::serializeP4RuntimeIfRequired(program, options);
    
    // for now, Python backend requires json but controller needs proto text
    std::ostream* outPy = openFile(options.p4RuntimeJsonFile, false);
    if (outPy != nullptr) {
        auto p4Runtime = P4::generateP4Runtime(program);
        p4Runtime.serializeP4InfoTo(outPy, P4::P4RuntimeFormat::JSON);
//        serializeP4Runtime(outPy, program, toplevel, &midEnd.refMap,
//                           &midEnd.typeMap, P4::P4RuntimeFormat::JSON);
        std::cout << "Wrote temporary runtime file to " << options.p4RuntimeJsonFile << std::endl;
    }

#include "p4c_python.cpp"

    static int py_argc = 8; //TODO find a better way to use this
    wchar_t * py_argv[py_argc];
    py_argv[0] = Py_DecodeLocale("P4_compiler.py", NULL);
    py_argv[1] = Py_DecodeLocale("-b", NULL);
    py_argv[2] = Py_DecodeLocale(compiler_path.c_str(), NULL); 
    // py_argv[3] = (char*) "-o"; 
    // py_argv[4] = (char*) "output"; //TODO: add possibility to change output path(?)
    py_argv[3] = Py_DecodeLocale("-p", NULL);
    py_argv[4] = Py_DecodeLocale(options.p4RuntimeJsonFile.c_str(), NULL);
    py_argv[5] = Py_DecodeLocale("--api", NULL);
    py_argv[6] = Py_DecodeLocale("SAI", NULL);
    py_argv[7] = Py_DecodeLocale(options.outputFile.c_str(), NULL);
    
    Py_SetProgramName(Py_DecodeLocale("sai_json_compiler", NULL));
    Py_Initialize();
    PySys_SetArgv(py_argc, py_argv);
    std::string compiler = compiler_path + "/output_stage/P4_compiler.py";

    FILE *fd = fopen(compiler.c_str(), "r"); // SAISRCDIR will be replaced in cmake
    std::cout << "Running " << compiler << std::endl;
    PyRun_SimpleFile(fd,"P4_compiler.py");
    Py_Finalize();
    fclose(fd);
    
    std::cout << "Done" << std::endl;

    return ::errorCount() > 0;
}

// The wrapping of main is just for debugging purposes with LVM
// LLDB MI/eclipse. Hard coding the args and wrap it.
int main(int argc, char *const argv[]) {
    std::cout << "Sai backend" << std::endl;
    return main_wrapper(argc,argv);

}

