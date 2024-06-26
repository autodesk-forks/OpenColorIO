// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.


#pragma once

#include <OpenColorIO/OpenColorIO.h>
#include <sstream>

namespace OCIO = OCIO_NAMESPACE;

class CPPEmitter
{
public:
    CPPEmitter(OCIO::ConstConfigRcPtr srcConfig)
        : src(srcConfig)
    {}

    void SetAddMain(bool b)
    {
        addMain = b;
    }

    bool GetAddMain() const 
    {
        return addMain;
    }

    std::string Emit()
    {
        if(ss.tellp() == 0)
        {
            Process();
        }

        return ss.str();
    };

private:
    using StrList = std::vector<const char*>;

    OCIO::ConstConfigRcPtr src;   // source config to read
    OCIO::ConfigRcPtr cfg;        // config that's constructed via C++ API while processing. Should be identical to the source at the end.
    int indentLevel = 0;
    std::string tab;
    std::ostringstream ss;
    bool addMain = false;

    void IndentIn()
    {
        tab = std::string(4 * (++indentLevel), ' ');
    };

    void IndentOut()
    {
        tab = std::string(4 * (--indentLevel), ' ');
    };

    // Utility Functions
    static int Strcasecmp(const char* str1, const char* str2);
    static std::string _s(const char* s);
    static std::string _s(OCIO::BitDepth BD);
    static std::string _s(OCIO::ReferenceSpaceType rs);
    static std::string _s(OCIO::TransformDirection rs);
    static std::string _s(OCIO::ViewTransformDirection rs);
    static std::string _s(OCIO::NegativeStyle neg);

    // Processing Functions
    void Process();
    void ProcessViewTransforms();
    void ProcessLooks();
    void ProcessActivesInactives();
    void ProcessDisplays();
    void ProcessSharedViews();
    void ProcessViewingRules();
    void ProcessFileRules();
    void ProcessRoles();
    void ProcessBasics();
    void ProcessColorSpaces(OCIO::ReferenceSpaceType type);
    void ProcessNamedTransforms();
    OCIO::TransformRcPtr ProcessTransform(OCIO::ConstTransformRcPtr in, const char* varName);

    template<typename T> std::string ProcessAliases(T in, StrList& al);
    template<typename T> std::string ProcessCategories(T in, StrList& cl);
};

