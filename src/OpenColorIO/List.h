#pragma once
#include <OpenColorIO/OpenColorIO.h>

namespace OCIO_NAMESPACE
{
    class List::Impl
    {
    public:
        std::vector<std::string> m_list;
    };
}