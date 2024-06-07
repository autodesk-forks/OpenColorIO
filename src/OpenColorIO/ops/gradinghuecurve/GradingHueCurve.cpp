// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.

#include <algorithm>
#include <sstream>

#include <OpenColorIO/OpenColorIO.h>

#include "ops/gradinghuecurve/GradingHueCurve.h"

namespace OCIO_NAMESPACE
{
namespace
{
static const std::vector<GradingControlPoint> DefaultHueHueCtrl{ {0.0f, 0.0f}, 
                                                                 {1.f/6.f, 1.f/6.f}, 
                                                                 {2.f/6.f, 2.f/6.f}, 
                                                                 {0.5f, 0.5f},
                                                                 {4.f/6.f, 4.f/6.f}, 
                                                                 {5.f/6.f, 5.f/6.f}  };

static const std::vector<GradingControlPoint> DefaultHueSatCtrl{ {0.0f, 1.0f}, 
                                                                 {1.f/6.f, 1.0f}, 
                                                                 {2.f/6.f, 1.0f}, 
                                                                 {0.5f, 1.0f},
                                                                 {4.f/6.f, 1.0f}, 
                                                                 {5.f/6.f, 1.0f} };

static const std::vector<GradingControlPoint> DefaultHueFxCtrl{ {0.0f, 0.0f}, 
                                                                {1.f/6.f, 0.f}, 
                                                                {2.f/6.f, 0.f}, 
                                                                {0.5f, 0.f},
                                                                {4.f/6.f, 0.f}, 
                                                                {5.f/6.f, 0.f} };

static const std::vector<GradingControlPoint> DefaultLumSatCtrl{ {0.0f, 1.0f}, {0.5f, 1.0f}, {1.0f, 1.0f} };
static const std::vector<GradingControlPoint> DefaultLumSatLinCtrl{ {-7.0f, 1.0f}, {0.f, 1.0f}, {7.0f, 1.0f} };

static const std::vector<GradingControlPoint> DefaultSatSatCtrl{ {0.0f, 0.0f}, {0.5f, 0.5f}, {1.0f, 1.0f} };
static const std::vector<GradingControlPoint> DefaultSatLumCtrl{ {0.0f, 1.0f}, {0.5f, 1.0f}, {1.0f, 1.0f} };

static const std::vector<GradingControlPoint> DefaultLumLumCtrl{ { 0.f, 0.f },{ 0.5f, 0.5f },{ 1.0f, 1.0f } };
static const std::vector<GradingControlPoint> DefaultLumLumLinCtrl{ { -7.0f, -7.0f },{ 0.f, 0.f },{ 7.0f, 7.0f } };

}

const GradingBSplineCurveImpl HueCurveImpl::DefaultHueHue(DefaultHueHueCtrl, BSplineCurveType::HUE_HUE_B_SPLINE );
const GradingBSplineCurveImpl HueCurveImpl::DefaultHueSat(DefaultHueSatCtrl, BSplineCurveType::PERIODIC_B_SPLINE );
const GradingBSplineCurveImpl HueCurveImpl::DefaultHueFx(DefaultHueFxCtrl, BSplineCurveType::PERIODIC_B_SPLINE );
const GradingBSplineCurveImpl HueCurveImpl::DefaultLumSat(DefaultLumSatCtrl, BSplineCurveType::HORIZONTAL_B_SPLINE);
const GradingBSplineCurveImpl HueCurveImpl::DefaultLumSatLin(DefaultLumSatLinCtrl, BSplineCurveType::HORIZONTAL_B_SPLINE);
const GradingBSplineCurveImpl HueCurveImpl::DefaultSatSat(DefaultSatSatCtrl, BSplineCurveType::DIAGONAL_B_SPLINE);
const GradingBSplineCurveImpl HueCurveImpl::DefaultSatLum(DefaultSatLumCtrl, BSplineCurveType::HORIZONTAL_B_SPLINE);
const GradingBSplineCurveImpl HueCurveImpl::DefaultLumLum(DefaultLumLumCtrl, BSplineCurveType::DIAGONAL_B_SPLINE);
const GradingBSplineCurveImpl HueCurveImpl::DefaultLumLumLin(DefaultLumLumLinCtrl, BSplineCurveType::DIAGONAL_B_SPLINE);

const std::array<std::reference_wrapper<const GradingBSplineCurveImpl>, static_cast<size_t>(HUE_NUM_CURVES)> 
   HueCurveImpl::DefaultCurvesLin( {  std::ref(DefaultHueHue),
                                      std::ref(DefaultHueSat),
                                      std::ref(DefaultHueSat),
                                      std::ref(DefaultLumSatLin),
                                      std::ref(DefaultSatSat),
                                      std::ref(DefaultLumLumLin),
                                      std::ref(DefaultSatLum),
                                      std::ref(DefaultHueFx) });


const std::array<std::reference_wrapper<const GradingBSplineCurveImpl>, static_cast<size_t>(HUE_NUM_CURVES)> 
   HueCurveImpl::DefaultCurves(  {  std::ref(DefaultHueHue),
                                    std::ref(DefaultHueSat),
                                    std::ref(DefaultHueSat),
                                    std::ref(DefaultLumSat),
                                    std::ref(DefaultSatSat),
                                    std::ref(DefaultLumLum),
                                    std::ref(DefaultSatLum),
                                    std::ref(DefaultHueFx) });

HueCurveImpl::HueCurveImpl() : 
    HueCurveImpl(GRADING_LOG)
{}

HueCurveImpl::HueCurveImpl(GradingStyle style)
{  
   auto & curves = (style == GRADING_LIN) ? HueCurveImpl::DefaultCurvesLin:
                                            HueCurveImpl::DefaultCurves;

   for (int c = 0; c < HUE_NUM_CURVES; ++c)
   {
       m_curves[c] = curves[c].get().createEditableCopy();
   }
}

HueCurveImpl::HueCurveImpl(const GradingHueCurves & curves)
{
    m_curves[HUE_HUE] = curves.hueHue->createEditableCopy();
    m_curves[HUE_SAT] = curves.hueSat->createEditableCopy();
    m_curves[HUE_LUM] = curves.hueLum->createEditableCopy();
    m_curves[LUM_SAT] = curves.lumSat->createEditableCopy();
    m_curves[SAT_SAT] = curves.satSat->createEditableCopy();
    m_curves[LUM_LUM] = curves.lumLum->createEditableCopy();
    m_curves[SAT_LUM] = curves.satLum->createEditableCopy();
    m_curves[HUE_FX] = curves.hueFx->createEditableCopy();
}

HueCurveImpl::HueCurveImpl(const ConstGradingHueCurveRcPtr & rhs)
{
    auto impl = dynamic_cast<const HueCurveImpl *>(rhs.get());
    if (impl)
    {
        for (int c = 0; c < HUE_NUM_CURVES; ++c)
        {
            m_curves[c] = impl->m_curves[c]->createEditableCopy();
        }
    }
}

GradingHueCurveRcPtr HueCurveImpl::createEditableCopy() const
{
    auto newCurve = std::make_shared<HueCurveImpl>();
    for (int c = 0; c < HUE_NUM_CURVES; ++c)
    {
        newCurve->m_curves[c] = m_curves[c]->createEditableCopy();
    }
    
    GradingHueCurveRcPtr res = newCurve;
    return res;
}

namespace
{
const char * CurveType(int c)
{
    const HueCurveType curve = static_cast<HueCurveType>(c);
    switch (curve)
    {
    case HUE_HUE:
        return "hue_hue";
    case HUE_SAT:
        return "hue_sat";
    case HUE_LUM:
        return "hue_lum";
    case LUM_SAT:
        return "lum_sat";
    case SAT_SAT:
        return "sat_sat";
    case LUM_LUM:
        return "lum_lum";
    case SAT_LUM:
        return "sat_lum";
    case HUE_FX:
        return "hue_fx";
    case HUE_NUM_CURVES:
    default:
        break;
    }
    return "invalid";
}
}

void HueCurveImpl::validate() const
{
    for (int c = 0; c < HUE_NUM_CURVES; ++c)
    {
        try
        {
            m_curves[c]->validate();
        }
        catch (Exception & e)
        {
            std::ostringstream oss;
            oss << "GradingHueCurve validation failed for curve: " << CurveType(c) << "' curve "
                << "with: " << e.what();
            throw Exception(oss.str().c_str());
        }
    }
}

bool HueCurveImpl::isIdentity() const
{
    for (int c = 0; c < HUE_NUM_CURVES; ++c)
    {
        if (!IsGradingCurveIdentity(m_curves[c]))
        {
            return false;
        }
    }
    return true;
}

ConstGradingBSplineCurveRcPtr HueCurveImpl::getCurve(HueCurveType c) const
{
    return m_curves[c];
}

GradingBSplineCurveRcPtr HueCurveImpl::getCurve(HueCurveType c)
{
    return m_curves[c];
}

GradingHueCurveRcPtr GradingHueCurve::Create(GradingStyle style)
{
    auto newCurve = std::make_shared<HueCurveImpl>(style);
    GradingHueCurveRcPtr res = newCurve;
    return res;
}

GradingHueCurveRcPtr GradingHueCurve::Create(const ConstGradingHueCurveRcPtr & rhs)
{
    auto newCurve = std::make_shared<HueCurveImpl>(rhs);
    GradingHueCurveRcPtr res = newCurve;
    return res;
}

GradingHueCurveRcPtr GradingHueCurve::Create(const GradingHueCurves & curves)
{
    auto newCurve = std::make_shared<HueCurveImpl>(curves);
    GradingHueCurveRcPtr res = newCurve;
    return res;
}

bool operator==(const GradingHueCurve & lhs, const GradingHueCurve & rhs)
{

    for (int c = 0; c < HUE_NUM_CURVES; ++c)
    {
        if (*(lhs.getCurve(static_cast<HueCurveType>(c))) != *(rhs.getCurve(static_cast<HueCurveType>(c))))
        {
            return false;
        }
    }
    return true;
}

bool operator!=(const GradingHueCurve & lhs, const GradingHueCurve & rhs)
{
    return !(lhs == rhs);
}


} // namespace OCIO_NAMESPACE
