// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.


#include "CPPEmitter.h"
#include <iomanip>

using namespace OCIO;

namespace
{
    using StrList = std::vector<const char*>;

    void AddColorSpace(ConfigRcPtr cfg
        , const char* name
        , ReferenceSpaceType type
        , const StrList& aliases
        , BitDepth bitDepth
        , const StrList& categories
        , const char* encoding
        , const char* eqGroup
        , const char* family
        , bool isData
        , ConstTransformRcPtr trFrom
        , ConstTransformRcPtr trTo
        , const char* desc
    )
    {
        auto out = ColorSpace::Create(type);

        for (auto alias : aliases)
            out->addAlias(alias);

        out->setBitDepth(bitDepth);

        for (auto category : categories)
            out->addCategory(category);

        out->setDescription(desc);
        out->setEncoding(encoding);
        out->setEqualityGroup(eqGroup);
        out->setFamily(family);
        out->setName(name);
        out->setIsData(isData);
        out->setTransform(trFrom, COLORSPACE_DIR_FROM_REFERENCE);
        out->setTransform(trTo, COLORSPACE_DIR_TO_REFERENCE);

        cfg->addColorSpace(out);
    }

    void AddNamedTramsform(ConfigRcPtr cfg
        , const char* name
        , const StrList& aliases
        , const StrList& categories
        , const char* encoding
        , const char* family
        , ConstTransformRcPtr trFwd
        , ConstTransformRcPtr trInv
        , const char* desc)
    {
        auto out = NamedTransform::Create();

        out->setName(name);
        out->setDescription(desc);
        out->setEncoding(encoding);
        out->setFamily(family);
        out->setTransform(trFwd, TRANSFORM_DIR_FORWARD);
        out->setTransform(trInv, TRANSFORM_DIR_INVERSE);

        for (auto alias : aliases)
            out->addAlias(alias);

        for (auto category : categories)
            out->addCategory(category);

        cfg->addNamedTransform(out);
    }

    static const char* sMainCode = R"(
#include<iostream>
#include<sstream>

int main()
{
    auto cfg = CreateConfig();
    if(cfg)
    {
        std::stringstream ssSrc;
        cfg->serialize(ssSrc);
        std::cout << ssSrc.str();
    };
    return 0;
};
)";

    static const char* sPrologCode = R"(
#include <OpenColorIO.h>

using namespace OCIO_NAMESPACE;
namespace {

void AddColorSpace(ConfigRcPtr& cfg
    , const char* name
    , ReferenceSpaceType type
    , const char** aliases
    , BitDepth bitDepth
    , const char** categories
    , const char* encoding
    , const char* eqGroup
    , const char* family
    , bool isData
    , ConstTransformRcPtr trFrom
    , ConstTransformRcPtr trTo
    , const char* desc)
{
    auto out = ColorSpace::Create(type);

    if(aliases)
    {
        for (int i = 0;; ++i)
        {
            const char* alias = aliases[i];
            if (!alias || !*alias)
                break;
            out->addAlias(alias);
        }
    }

    if (categories)
    {
        for (int i = 0;; ++i)
        {
            const char* cat = categories[i];
            if (!cat || !*cat)
                break;
            out->addCategory(cat);
        }
    }

    out->setBitDepth(bitDepth);
    out->setDescription(desc);
    out->setEncoding(encoding);
    out->setEqualityGroup(eqGroup);
    out->setFamily(family);
    out->setName(name);
    out->setIsData(isData);
    if(trFrom)
        out->setTransform(trFrom, COLORSPACE_DIR_FROM_REFERENCE);
    if(trTo)
        out->setTransform(trTo, COLORSPACE_DIR_TO_REFERENCE);

    cfg->addColorSpace(out);
}

void AddNamedTramsform(ConfigRcPtr& cfg
    , const char* name
    , const char** aliases
    , const char** categories
    , const char* encoding
    , const char* family
    , ConstTransformRcPtr trFwd
    , ConstTransformRcPtr trInv
    , const char* desc)
{
    auto out = NamedTransform::Create();

    out->setName(name);
    out->setDescription(desc);
    out->setEncoding(encoding);
    out->setFamily(family);
    if(trFwd)
        out->setTransform(trFwd, TRANSFORM_DIR_FORWARD);
    if(trInv)
        out->setTransform(trInv, TRANSFORM_DIR_INVERSE);

    if(aliases)
    {
        for (int i = 0;; ++i)
        {
            const char* alias = aliases[i];
            if (!alias || !*alias)
                break;
            out->addAlias(alias);
        }
    }

    if (categories)
    {
        for (int i = 0;; ++i)
        {
            const char* cat = categories[i];
            if (!cat || !*cat)
                break;
            out->addCategory(cat);
        }
    }

    cfg->addNamedTransform(out);
}

} // anonymous namespace

)";

} // anonymous namespace


//____________________________________________________________
int CPPEmitter::Strcasecmp(const char* str1, const char* str2)
{
#ifdef _WIN32
    return ::_stricmp(str1, str2);
#else
    return ::strcasecmp(str1, str2);
#endif
}

//____________________________________________________________
std::string CPPEmitter::_s(const char* s)
{
    if (!s || !*s)
        return { "\"\"" };

    std::string str(s);
    std::string sTerm(")\"");
    std::string dseq{};

    while (str.find(sTerm, 0) != -1)
    {
        dseq += "-";
        sTerm = ")" + dseq + "\"";
    }

    std::string out{ "u8R\"" };
    out += dseq;
    out += "(";
    out += s;
    out += ")";
    out += dseq;
    out += "\"";

    return out;
}

 //____________________________________________________________
 std::string CPPEmitter::_s(BitDepth BD)
{
    switch (BD)
    {
    case BIT_DEPTH_UINT8:
        return "BIT_DEPTH_UINT8";
    case BIT_DEPTH_UINT10:
        return "BIT_DEPTH_UINT10";
    case BIT_DEPTH_UINT12:
        return "BIT_DEPTH_UINT12";
    case BIT_DEPTH_UINT16:
        return "BIT_DEPTH_UINT16";
    case BIT_DEPTH_F16:
        return "BIT_DEPTH_F16";
    case BIT_DEPTH_F32:
        return "BIT_DEPTH_F32";
    case BIT_DEPTH_UINT14:
        return "BIT_DEPTH_UINT14";
    case BIT_DEPTH_UINT32:
        return "BIT_DEPTH_UINT32";
    case BIT_DEPTH_UNKNOWN:
        return "BIT_DEPTH_UNKNOWN";
    default:
        break;
    }

    return "BIT_DEPTH_UNKNOWN";
}

 //____________________________________________________________
 std::string CPPEmitter::_s(ReferenceSpaceType rs)
{
    return (rs == REFERENCE_SPACE_DISPLAY) ? "REFERENCE_SPACE_DISPLAY" : "REFERENCE_SPACE_SCENE";
}

 //____________________________________________________________
 std::string CPPEmitter::_s(TransformDirection dir)
{
    return (dir == TRANSFORM_DIR_FORWARD) ? "TRANSFORM_DIR_FORWARD" : "TRANSFORM_DIR_INVERSE";
}

 //____________________________________________________________
 std::string CPPEmitter::_s(ViewTransformDirection dir)
{
    return dir == (VIEWTRANSFORM_DIR_TO_REFERENCE) ? "VIEWTRANSFORM_DIR_TO_REFERENCE" : "VIEWTRANSFORM_DIR_FROM_REFERENCE";
}

 //____________________________________________________________
 std::string CPPEmitter::_s(NegativeStyle neg)
{
    switch (neg)
    {
    case NEGATIVE_CLAMP:        return "NEGATIVE_CLAMP";
    case NEGATIVE_MIRROR:       return "NEGATIVE_MIRROR";
    case NEGATIVE_PASS_THRU:    return "NEGATIVE_PASS_THRU";
    case NEGATIVE_LINEAR:       return "NEGATIVE_LINEAR";
    default:
        break;
    }

    return "";
}

 //____________________________________________________________
 void CPPEmitter::Process()
{
    if (src->getMajorVersion() < 2)
    {
        throw Exception("Only v2+ configs are supported.");
    }

    auto name = src->getName();

    ss << std::setprecision(std::numeric_limits<double>::digits10);

    // Copyright
    ss << "// SPDX-License-Identifier: BSD-3-Clause\n";
    ss << "// Copyright Contributors to the OpenColorIO Project.\n\n";

    // Explanation
    ss << "//\n// This code is auto-generated to replicate the source config \"" << name << "\"\n//\n\n";
    
    // Prolog code
    ss << sPrologCode;
    
    // Function body
    ss << "// Creates config \"" << name << "\" from scratch using OCIO C++ API\n";
    ss << "ConfigRcPtr CreateConfig()\n{\n";
    IndentIn();

    // Create in-memory config to build while emitting
    cfg = Config::Create();
    ss << tab << "auto cfg = Config::Create();\n";

    // Environment variables:  <<< Skipped >>>
    // Search Path:  <<< Skipped >>>
    ProcessBasics();
    ProcessRoles();
    ProcessFileRules();
    ProcessViewingRules();
    ProcessSharedViews();
    ProcessDisplays();
    // Virtual Displays: <<< Skipped >>>
    ProcessActivesInactives();
    ProcessLooks();
    ProcessViewTransforms();
    ProcessColorSpaces(REFERENCE_SPACE_SCENE);
    ProcessColorSpaces(REFERENCE_SPACE_DISPLAY);
    ProcessNamedTransforms();

    // EPILOG
    ss << tab << "return cfg;\n";
    IndentOut();
    ss << tab << "};\n";

    // main
    if (addMain)
    {
        ss << sMainCode;
    }

    // Serialize the original config
    std::stringstream ssSrc;
    src->serialize(ssSrc);
    std::string srcText = ssSrc.str();

    // serialize the config built from scratch in memory.
    std::stringstream ssCfg;
    cfg->serialize(ssCfg);
    std::string cfgText = ssCfg.str();

    std::string outText = ss.str(); // TODO: this is for easier debugging. remove
}

 //____________________________________________________________
 void CPPEmitter::ProcessBasics()
 {
     // Version
     {
         auto vmaj = src->getMajorVersion();
         auto vmin = src->getMinorVersion();

         cfg->setVersion(vmaj, vmin);
         ss << tab << "cfg->setVersion(" << vmaj << ", " << vmin << "); \n";
     }

     // Strict parsing
     {
         bool isStrict = src->isStrictParsingEnabled();

         cfg->setStrictParsingEnabled(isStrict);
         ss << tab << "cfg->setStrictParsingEnabled(" << isStrict << ");\n";
     }

     // Family Separator
     {
         char sep = src->getFamilySeparator();

         cfg->setFamilySeparator(sep);
         ss << tab << "cfg->setFamilySeparator('" << sep << "');\n";
     }

     // Luma Coefs.
     {
         double luma[3];
         src->getDefaultLumaCoefs(luma);

         cfg->setDefaultLumaCoefs(luma);
         ss << tab << "static double luma[] = {" << luma[0] << ", " << luma[1] << ", " << luma[2] << "};\n";
         ss << tab << "cfg->setDefaultLumaCoefs(luma);\n";
     }

     // Name 
     {
         auto name = src->getName();

         cfg->setName(name);
         ss << tab << "cfg->setName(" << _s(name) << ");\n";
     }

     // Description
     {
         auto desc = src->getDescription();

         cfg->setDescription(desc);
         ss << tab << "cfg->setDescription(" << _s(desc) << ");\n";
     }
 }

 //____________________________________________________________
 void CPPEmitter::ProcessRoles()
 {
     if (src->getNumRoles() <= 0)
         return;

     ss << "\n" << tab << "// Roles\n" << tab << "{\n";
     IndentIn();

     for (int i = 0; i < src->getNumRoles(); ++i)
     {
         const char* role = src->getRoleName(i);
         if (role && *role)
         {
             ConstColorSpaceRcPtr colorspace = src->getColorSpace(role);
             if (colorspace)
             {
                 auto csname = src->getColorSpace(role)->getName();

                 cfg->setRole(role, csname);
                 ss << tab << "cfg->setRole(" << _s(role) << ", " << _s(csname) << ");\n";
             }
             else
             {
                 std::ostringstream os;
                 os << "Colorspace associated to the role '" << role << "', does not exist.";
                 throw Exception(os.str().c_str());
             }
         }
     }
     IndentOut();
     ss << tab << "}\n";
 }
 
 //____________________________________________________________
 void CPPEmitter::ProcessFileRules()
 {
     ss << "\n" << tab << "// File Rules\n" << tab << "{\n";
     IndentIn();

     auto in = src->getFileRules();
     auto rules = FileRules::Create();
     ss << tab << "auto rules = FileRules::Create();\n";

     for (size_t i = 0; i < in->getNumEntries(); ++i)
     {
         auto name = in->getName(i);
         auto cs = in->getColorSpace(i);
         auto regex = in->getRegex(i);
         auto pat = in->getPattern(i);
         auto ext = in->getExtension(i);

         if (0 == Strcasecmp(name, FileRules::DefaultRuleName))
         {
             rules->setDefaultRuleColorSpace(cs);
             ss << tab << "rules->setDefaultRuleColorSpace(" << _s(cs) << ");\n";
         }
         else if (0 == Strcasecmp(name, FileRules::FilePathSearchRuleName))
         {
             rules->insertPathSearchRule(i);
             ss << tab << "rules->insertPathSearchRule(" << i << ");\n";
         }
         else if (regex && *regex)
         {
             rules->insertRule(i, name, cs, regex);
             ss << tab << "rules->insertRule(" << i << ", " << _s(name) << ", " << _s(cs) << ", " << _s(regex) << ");\n";
         }
         else
         {
             rules->insertRule(i, name, cs, pat, ext);
             ss << tab << "rules->insertRule(" << i << ", " << _s(name) << ", " << _s(cs) << ", " << _s(pat) << ", " << _s(ext) << ");\n";
         }

         for (size_t j = 0; j < rules->getNumCustomKeys(i); ++j)
         {
             auto key = in->getCustomKeyName(i, j);
             auto val = in->getCustomKeyValue(i, j);
             rules->setCustomKey(i, key, val); // <<<<
             ss << tab << "rules->setCustomKey(" << i << ", " << _s(key) << ", " << _s(val) << ");\n";
         }
     }

     cfg->setFileRules(rules);
     ss << tab << "cfg->setFileRules(rules);\n";

     IndentOut();
     ss << tab << "}\n";
 }

//____________________________________________________________
void CPPEmitter::ProcessViewingRules()
{
    ss << "\n" << tab << "// Viewing Rules\n" << tab << "{\n";
    IndentIn();

    auto in = src->getViewingRules();
    auto rules = ViewingRules::Create();
    ss << tab << "auto rules = ViewingRules::Create();\n";

    for (size_t i = 0; i < in->getNumEntries(); ++i)
    {
        auto name = in->getName(i);

        rules->insertRule(i, name);
        ss << tab << "rules->insertRule(" << i << ", " << _s(name) << ");\n";

        for (size_t j = 0; j < rules->getNumColorSpaces(i); ++j)
        {
            auto cs = in->getColorSpace(i, j);

            rules->addColorSpace(i, cs);
            ss << tab << "rules->addColorSpace(" << i << ", " << _s(cs) << ");\n";
        }

        for (size_t j = 0; j < rules->getNumEncodings(i); ++j)
        {
            auto enc = in->getEncoding(i, j);
            rules->addEncoding(i, enc);
            ss << tab << "rules->addEncoding(" << i << ", " << _s(enc) << ");\n";
        }

        for (size_t j = 0; j < rules->getNumCustomKeys(i); ++j)
        {
            auto key = in->getCustomKeyName(i, j);
            auto val = in->getCustomKeyValue(i, j);
            rules->setCustomKey(i, key, val); // <<<<
            ss << tab << "rules->setCustomKey(" << i << ", " << _s(key) << ", " << _s(val) << ");\n";
        }
    }

    cfg->setViewingRules(rules);
    ss << tab << "cfg->setViewingRules(rules);\n";

    IndentOut();
    ss << tab << "}\n";
}

//____________________________________________________________
void CPPEmitter::ProcessSharedViews()
{
    // Shared Views
    if (src->getNumViews(VIEW_SHARED, nullptr) <= 0)
        return;

    ss << "\n" << tab << "// Shared Views\n" << tab << "{\n";
    IndentIn();

    for (int v = 0; v < src->getNumViews(VIEW_SHARED, nullptr); ++v)
    {
        auto name = src->getView(VIEW_SHARED, nullptr, v);
        auto tr = src->getDisplayViewTransformName(nullptr, name);
        auto cs = src->getDisplayViewColorSpaceName(nullptr, name);
        auto lk = src->getDisplayViewLooks(nullptr, name);
        auto rl = src->getDisplayViewRule(nullptr, name);
        auto ds = src->getDisplayViewDescription(nullptr, name);

        cfg->addSharedView(name, tr, cs, lk, rl, ds);
        ss << tab << "cfg->addSharedView(" << _s(name) << ", " << _s(tr) << ", " << _s(cs) << ", " << _s(lk) << ", " << _s(rl) << ", " << _s(ds) << ");\n";
    };

    IndentOut();
    ss << tab << "}\n";
}

//____________________________________________________________
 void CPPEmitter::ProcessDisplays()
{
    ss << "\n" << tab << "// Displays\n" << tab << "{\n";
    IndentIn();

    for (int i = 0; i < src->getNumDisplaysAll(); ++i)
    {
        if (src->isDisplayTemporary(i))
            break;

        const char* display = src->getDisplayAll(i);

        for (int v = 0; v < src->getNumViews(VIEW_DISPLAY_DEFINED, display); ++v)
        {
            auto name = src->getView(VIEW_DISPLAY_DEFINED, display, v);
            auto tr = src->getDisplayViewTransformName(display, name);
            auto cs = src->getDisplayViewColorSpaceName(display, name);
            auto lk = src->getDisplayViewLooks(display, name);
            auto rl = src->getDisplayViewRule(display, name);
            auto ds = src->getDisplayViewDescription(display, name);

            cfg->addDisplayView(display, name, tr, cs, lk, rl, ds);
            ss << tab << "cfg->addDisplayView(" << _s(display) << ", " << _s(name) << ", " << _s(tr) << ", " << _s(cs) << ", " << _s(lk) << ", " << _s(rl) << ", " << _s(ds) << ");\n";
        }

        for (int v = 0; v < src->getNumViews(VIEW_SHARED, display); ++v)
        {
            auto name = src->getView(VIEW_SHARED, display, v);

            cfg->addDisplaySharedView(display, name);
            ss << tab << "cfg->addDisplaySharedView(" << _s(display) << ", " << _s(name) << ");\n";
        }

        ss << "\n";
    }

    IndentOut();
    ss << tab << "}\n";
}

//____________________________________________________________
 void CPPEmitter::ProcessActivesInactives()
{
    // Active Displays
    {
        auto ads = src->getActiveDisplays();

        cfg->setActiveDisplays(ads);
        ss << tab << "cfg->setActiveDisplays(" << _s(ads) << ");\n";
    }

    // Active Views
    {
        auto avs = src->getActiveViews();

        cfg->setActiveViews(avs);
        ss << tab << "cfg->setActiveViews(" << _s(avs) << ");\n";
    }

    // Inactive Color Spaces
    {
        auto ics = src->getInactiveColorSpaces();

        cfg->setInactiveColorSpaces(ics);
        ss << tab << "cfg->setInactiveColorSpaces(" << _s(ics) << ");\n";
    }
}
 
//____________________________________________________________
void CPPEmitter::ProcessLooks()
{
    if (src->getNumLooks() <= 0)
        return;

    ss << "\n" << tab << "// Looks\n" << tab << "{\n";
    IndentIn();

    for (int i = 0; i < src->getNumLooks(); ++i)
    {
        auto name = src->getLookNameByIndex(i);

        ss << "\n" << tab << "//" << name << "\n";
        ss << tab << "{\n";
        IndentIn();

        auto in = src->getLook(name);
        auto ds = in->getDescription();
        auto ps = in->getProcessSpace();
        auto inFwd = in->getTransform();
        auto inInv = in->getInverseTransform();

        auto trFwd = ProcessTransform(inFwd, "trFwd");
        auto trInv = ProcessTransform(inInv, "trInv");

        auto look = Look::Create();
        look->setName(name);
        look->setDescription(ds);
        look->setProcessSpace(ps);
        if (trFwd)
            look->setTransform(trFwd);
        if (trInv)
            look->setInverseTransform(trInv);
        cfg->addLook(look);

        ss << tab << "auto look = Look::Create();\n";
        ss << tab << "look->setName(" << _s(name) << ");\n";
        ss << tab << "look->setDescription(" << _s(ds) << ");\n";
        ss << tab << "look->setProcessSpace(" << _s(ps) << ");\n";
        if (trFwd)
            ss << tab << "look->setTransform(trFwd);\n";
        if (trInv)
            ss << tab << "look->setInverseTransform(trInv);\n";
        ss << tab << "cfg->addLook(look);\n";

        IndentOut();
        ss << tab << "}\n";
    }

    IndentOut();
    ss << tab << "}\n";
}

//____________________________________________________________
 void CPPEmitter::ProcessViewTransforms()
{
    ss << "\n" << tab << "// View Transforms\n" << tab << "{\n";
    IndentIn();

    // Default view transform name
    auto def = src->getDefaultViewTransformName();

    cfg->setDefaultViewTransformName(def);
    ss << tab << "cfg->setDefaultViewTransformName(" << _s(def) << ");\n";

    for (int i = 0; i < src->getNumViewTransforms(); ++i)
    {

        auto name = src->getViewTransformNameByIndex(i);

        ss << "\n" << tab << "// " << name << "\n";
        ss << tab << "{\n";
        IndentIn();

        auto in = src->getViewTransform(name);
        auto ds = in->getDescription();
        auto fa = in->getFamily();
        auto rs = in->getReferenceSpaceType();
        //auto ca = in->getCategory();

        auto inFrom = in->getTransform(VIEWTRANSFORM_DIR_FROM_REFERENCE);
        auto inTo = in->getTransform(VIEWTRANSFORM_DIR_TO_REFERENCE);
        auto trFrom = ProcessTransform(inFrom, "trFrom");
        auto trTo = ProcessTransform(inTo, "trTo");

        auto vt = ViewTransform::Create(rs);
        vt->setName(name);
        vt->setDescription(ds);
        vt->setFamily(fa);
        vt->setTransform(trFrom, VIEWTRANSFORM_DIR_FROM_REFERENCE);
        vt->setTransform(trTo, VIEWTRANSFORM_DIR_TO_REFERENCE);
        cfg->addViewTransform(vt);

        // TODO: Categories

        ss << tab << "auto vt = ViewTransform::Create(" << _s(rs) << ");\n";
        ss << tab << "vt->setName(" << _s(name) << ");\n";
        ss << tab << "vt->setDescription(" << _s(ds) << ");\n";
        ss << tab << "vt->setFamily(" << _s(fa) << ");\n";
        if (inFrom)
            ss << tab << "vt->setTransform(trFrom, VIEWTRANSFORM_DIR_FROM_REFERENCE);\n";
        if (inTo)
            ss << tab << "vt->setTransform(trTo, VIEWTRANSFORM_DIR_TO_REFERENCE);\n";
        ss << tab << "cfg->addViewTransform(vt);\n";

        IndentOut();
        ss << tab << "}\n";
    }

    IndentOut();
    ss << tab << "}\n";
}

//____________________________________________________________
 void CPPEmitter::ProcessColorSpaces(ReferenceSpaceType type)
{
    auto searchType = type == REFERENCE_SPACE_SCENE ? SEARCH_REFERENCE_SPACE_SCENE : SEARCH_REFERENCE_SPACE_DISPLAY;
    const char* sCaption = type == REFERENCE_SPACE_SCENE ? "Scene" : "Display";

    ss << "\n" << tab << "// " << sCaption << " Color Spaces\n" << tab << "{\n";
    IndentIn();

    for (int i = 0; i < src->getNumColorSpaces(searchType, COLORSPACE_ALL); ++i)
    {
        auto name = src->getColorSpaceNameByIndex(searchType, COLORSPACE_ALL, i);
        auto in = src->getColorSpace(name);

        ss << "\n" << tab << "// " << name << "\n" << tab << "{\n";
        IndentIn();

        // aliases
        StrList lAlias{};
        auto sAlias = ProcessAliases(in, lAlias);

        // categories
        StrList lCat{};
        auto sCat = ProcessCategories(in, lCat);

        auto bd = in->getBitDepth();
        auto ds = in->getDescription();
        auto en = in->getEncoding();
        auto eq = in->getEqualityGroup();
        auto fa = in->getFamily();
        auto da = in->isData();

        auto inFrom = in->getTransform(COLORSPACE_DIR_FROM_REFERENCE);
        auto inTo = in->getTransform(COLORSPACE_DIR_TO_REFERENCE);
        auto trFrom = ProcessTransform(inFrom, "trFrom");
        auto trTo = ProcessTransform(inTo, "trTo");

        AddColorSpace(cfg, name, type, lAlias, bd, lCat, en, eq, fa, da, trFrom, trTo, ds);

        if (!sAlias.empty())
            ss << tab << "static const char* aliases[] = " << sAlias << ";\n";
        if (!sCat.empty())
            ss << tab << "static const char* cats[] = " << sCat << ";\n";

        ss << tab << "AddColorSpace(cfg, " << _s(name) << "\n";
        IndentIn();
        ss << tab << ", " << _s(type)
            << tab << ", " << (sAlias.empty() ? "nullptr" : "aliases") << "\n"
            << tab << ", " << _s(bd) << "\n"
            << tab << ", " << (sCat.empty() ? "nullptr" : "cats") << "\n"
            << tab << ", " << _s(en) << "\n"
            << tab << ", " << _s(eq) << "\n"
            << tab << ", " << _s(fa) << "\n"
            << tab << ", " << (da ? "true" : "false") << "\n"
            << tab << ", " << (trFrom ? "trFrom" : "nullptr") << "\n"
            << tab << ", " << (trTo ? "trTo" : "nullptr") << "\n"
            << tab << ", " << _s(ds) << "\n"
            << tab << ");\n";
        IndentOut();
        IndentOut();
        ss << tab << "}\n";
    }

    IndentOut();
    ss << tab << "}\n";
}

 //____________________________________________________________
 void CPPEmitter::ProcessNamedTransforms()
{
    ss << "\n" << tab << "// Named Transforms\n" << tab << "{\n";
    IndentIn();

    for (int i = 0; i < src->getNumNamedTransforms(); ++i)
    {
        auto name = src->getNamedTransformNameByIndex(i);

        ss << "\n" << tab << "// " << name << "\n";
        ss << tab << "{\n";
        IndentIn();

        auto in = src->getNamedTransform(name);
        auto ds = in->getDescription();
        auto fa = in->getFamily();
        auto en = in->getEncoding();
        auto inFwd = in->getTransform(TRANSFORM_DIR_FORWARD);
        auto inInv = in->getTransform(TRANSFORM_DIR_INVERSE);

        TransformRcPtr trFwd;
        TransformRcPtr trInv;
        if (inFwd)
            trFwd = ProcessTransform(inFwd, "trFwd");
        if (inInv)
            trInv = ProcessTransform(inInv, "trInv");

        // Aliases
        StrList lAlias{};
        auto sAlias = ProcessAliases(in, lAlias);

        // categories
        StrList lCat{};
        auto sCat = ProcessCategories(in, lCat);

        AddNamedTramsform(cfg, name, lAlias, lCat, en, fa, trFwd, trInv, ds);

        if (!sAlias.empty())
            ss << tab << "static const char* aliases[] = " << sAlias << ";\n";
        if (!sCat.empty())
            ss << tab << "static const char* cats[] = " << sCat << ";\n";

        ss << tab << "AddNamedTramsform(cfg, " << _s(name) << "\n";
        IndentIn();
        ss << tab << ", " << (sAlias.empty() ? "nullptr" : "aliases") << "\n"
            << tab << ", " << (sCat.empty() ? "nullptr" : "cats") << "\n"
            << tab << ", " << _s(en) << "\n"
            << tab << ", " << _s(fa) << "\n"
            << tab << ", " << (trFwd ? "trFwd" : "nullptr") << "\n"
            << tab << ", " << (trInv ? "trInv" : "nullptr") << "\n"
            << tab << ", " << _s(ds) << "\n"
            << tab << ");\n";
        IndentOut();

        IndentOut();
        ss << tab << "}\n";
    }

    IndentOut();
    ss << tab << "}\n";
}

 //____________________________________________________________
 TransformRcPtr CPPEmitter::ProcessTransform(ConstTransformRcPtr inTr, const char* varName)
{
    if (!inTr)
        return nullptr;

    auto dir = inTr->getDirection();

    // BuiltinTransform
    if (ConstBuiltinTransformRcPtr in = DynamicPtrCast<const BuiltinTransform>(inTr))
    {
        auto st = in->getStyle();

        auto t = BuiltinTransform::Create();
        t->setStyle(st);
        t->setDirection(dir);

        ss << tab << "auto " << varName << " = BuiltinTransform::Create(); \n";
        ss << tab << varName << "->setStyle(" << _s(st) << ");\n";
        ss << tab << varName << "->setDirection(" << _s(dir) << ");\n";

        return t;
    }

    // GroupTransform
    if (ConstGroupTransformRcPtr in = DynamicPtrCast<const GroupTransform>(inTr))
    {
        auto num = in->getNumTransforms();

        auto& meta = in->getFormatMetadata();
        auto name = meta.getName();

        auto grp = GroupTransform::Create();
        grp->setDirection(dir);
        if (name && *name)
        {
            grp->getFormatMetadata().setName(name);
        }

        ss << tab << "auto " << varName << " = GroupTransform::Create();\n";
        ss << tab << varName << "->setDirection(" << _s(dir) << " );\n";
        if (name && *name)
            ss << tab << varName << "->getFormatMetadata().setName(" << _s(name) << " );\n";
        ss << tab << "{\n";
        IndentIn();

        for (int i = 0; i < num; ++i)
        {
            std::stringstream childVar;
            childVar << varName << i;

            auto inChild = in->getTransform(i);
            auto child = ProcessTransform(inChild, childVar.str().c_str());
            grp->appendTransform(child);

            ss << tab << varName << "->appendTransform(" << childVar.str() << ");\n";
        }

        IndentOut();
        ss << tab << "}\n";

        return grp;
    }

    // ExponentTransform
    if (ConstExponentTransformRcPtr in = DynamicPtrCast<const ExponentTransform>(inTr))
    {
        double val[4];
        in->getValue(val);
        auto neg = in->getNegativeStyle();

        auto t = ExponentTransform::Create();
        t->setValue(val);
        t->setNegativeStyle(neg);
        t->setDirection(dir);

        ss << tab << "auto " << varName << " = ExponentTransform::Create(); \n";
        ss << tab << varName << "->setNegativeStyle(" << _s(neg) << ");\n";
        ss << tab << varName << "->setDirection(" << _s(dir) << ");\n";
        ss << tab << varName << "->setValue({" << val[0] << ", " << val[1] << ", " << val[2] << ", " << val[3] << "});\n";

        return t;
    }

    // ExponentWithLinearTransform
    if (ConstExponentWithLinearTransformRcPtr in = DynamicPtrCast<const ExponentWithLinearTransform>(inTr))
    {
        double gam[4];
        double off[4];
        in->getGamma(gam);
        in->getOffset(off);
        auto neg = in->getNegativeStyle();

        auto t = ExponentWithLinearTransform::Create();
        t->setGamma(gam);
        t->setOffset(off);
        t->setNegativeStyle(neg);
        t->setDirection(dir);

        ss << tab << "auto " << varName << " = ExponentWithLinearTransform::Create(); \n";
        ss << tab << varName << "->setNegativeStyle(" << _s(neg) << ");\n";
        ss << tab << varName << "->setDirection(" << _s(dir) << ");\n";
        ss << tab << varName << "->setGamma({" << gam[0] << ", " << gam[1] << ", " << gam[2] << ", " << gam[3] << "});\n";
        ss << tab << varName << "->setOffset({" << off[0] << ", " << off[1] << ", " << off[2] << ", " << off[3] << "});\n";

        return t;
    }

    // ExponentWithLinearTransform
    if (ConstMatrixTransformRcPtr in = DynamicPtrCast<const MatrixTransform>(inTr))
    {
        double mat[16];
        double off[4];
        in->getMatrix(mat);
        in->getOffset(off);
        // TODO: in->getFileInputBitDepth() ??
        // TODO: in->getFileOutputBitDepth() ??

        auto t = MatrixTransform::Create();
        t->setMatrix(mat);
        t->setOffset(off);
        t->setDirection(dir);

        ss << tab << "auto " << varName << " = MatrixTransform::Create(); \n";
        ss << tab << varName << "->setDirection(" << _s(dir) << ");\n";
        ss << tab << varName << "->setOffset(std::array<double, 4>({" << off[0] << ", " << off[1] << ", " << off[2] << ", " << off[3] << "}).data());\n";

        ss << tab << "static const double " << varName << "Mat[] = {" << mat[0];
        for (int i = 1; i < 16; ++i)
            ss << ", " << mat[i];
        ss << "};\n";

        ss << tab << varName << "->setMatrix(" << varName << "Mat);\n";

        return t;
    }

    // ExponentWithLinearTransform
    if (ConstLogCameraTransformRcPtr in = DynamicPtrCast<const LogCameraTransform>(inTr))
    {
        double linB[3] = { 0.0, 0.0, 0.0 };
        double logSS[3] = { 1.0, 1.0, 1.0 };
        double linSS[3] = { 1.0, 1.0, 1.0 };
        double linSO[3] = { 0.0, 0.0, 0.0 };
        double logSO[3] = { 0.0, 0.0, 0.0 };
        double linS[3] = { 1.0, 1.0, 1.0 };

        in->getLogSideSlopeValue(logSS);
        in->getLogSideOffsetValue(logSO);
        in->getLinSideSlopeValue(linSS);
        in->getLinSideOffsetValue(linSO);
        in->getLinSideBreakValue(linB);
        bool hasLinS = in->getLinearSlopeValue(linS);
        const double baseVal = in->getBase();
        // TODO: More?

        auto t = LogCameraTransform::Create(linB);
        t->setDirection(dir);
        t->setLogSideSlopeValue(logSS);
        t->setLogSideOffsetValue(logSO);
        t->setLinSideSlopeValue(linSS);
        t->setLinSideOffsetValue(linSO);
        if (hasLinS)
            t->setLinearSlopeValue(linS);
        t->setBase(baseVal);

        ss << tab << "auto " << varName << " = LogCameraTransform::Create({" << linB[0] << ", " << linB[1] << ", " << linB[2] << "}); \n";
        ss << tab << varName << "->setDirection(" << _s(dir) << ");\n";
        ss << tab << varName << "->setLogSideSlopeValue( {" << logSS[0] << ", " << logSS[1] << ", " << logSS[2] << "});\n";
        ss << tab << varName << "->setLogSideOffsetValue({" << logSO[0] << ", " << logSO[1] << ", " << logSO[2] << "});\n";
        ss << tab << varName << "->setLinSideSlopeValue( {" << linSS[0] << ", " << linSS[1] << ", " << linSS[2] << "});\n";
        ss << tab << varName << "->setLinSideOffsetValue({" << linSO[0] << ", " << linSO[1] << ", " << linSO[2] << "});\n";
        if (hasLinS)
            ss << tab << varName << "->setLinearSlopeValue({" << linS[0] << ", " << linS[1] << ", " << linS[2] << "});\n";
        ss << tab << varName << "->setBase(" << baseVal << ");\n";

        return t;
    }


    // TODO: More Transform types

    return nullptr;
}

//____________________________________________________________
template<typename T>
std::string CPPEmitter::ProcessAliases(T in, StrList& al)
{
    auto n = in->getNumAliases();
    if (n <= 0)
        return {};

    std::ostringstream ss;
    ss << "{";
    for (size_t i = 0; i < n; i++)
    {
        auto alias = in->getAlias(i);
        al.push_back(alias);
        ss << (i ? "," : "") << _s(alias);
    }
    ss << ",\"\"}"; // double tap terminator

    return ss.str();
}

//____________________________________________________________
template<typename T>
std::string CPPEmitter::ProcessCategories(T in, StrList& cl)
{
    auto n = in->getNumCategories();
    if (n <= 0)
        return {};

    std::ostringstream ss;
    ss << "{";
    for (int i = 0; i < n; i++) // aliases return size_t, categories int
    {
        auto cat = in->getCategory(i);
        cl.push_back(cat);
        ss << (i ? "," : "") << _s(cat);
    }
    ss << ",\"\"}"; // double tap terminator

    return ss.str();
}
