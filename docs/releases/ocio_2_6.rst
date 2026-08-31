..
  SPDX-License-Identifier: CC-BY-4.0
  Copyright Contributors to the OpenColorIO Project.


OCIO 2.6 Release
================

Timeline
********

OpenColorIO 2.6 was delivered in September 2026 and is in the VFX Reference Platform for
calendar year 2027.

New Feature Guide
=================

New Fixed Function Transforms
*****************************

For Config Authors
++++++++++++++++++

The following new styles are available for use with FixedFunctionTransforms in config
files with ``ocio_profile_version`` set to 2.6 or higher. They implement a conversion from
a linear RGB space with customizable primaries to the JMh (lightness, colorfulness, hue)
color appearance space used in the ACES 2.0 Output Transforms. The ordering is hue,
colorfulness, lightness, rather than JMh since that is the order already established by
the built-in HSV and HSY Fixed Functions and it conforms to the order expected by the
GradingHueCurveTransform. The scaling is {h/360, M/200, J/100} to allow the resulting
images to be easier to work with in DCCs. It takes the following eight parameters to
describe the primaries and white point of the RGB space:
[ red_x, red_y, green_x, green_y, blue_x, blue_y, white_x, white_y ].

* ``FIXED_FUNCTION_ACES_RGB_TO_HMJ_20``


New Built-in Transforms
***********************

For Config Authors
++++++++++++++++++

In config files with ``ocio_profile_version`` set to 2.6 or higher, config authors may take
advantage of the following new BuiltinTransform style that provides a new look transform
intended for use with the ACES 2.0 Output Transforms (SDR only) that provides a brighter,
higher contrast look that is a more finished/graded look. The brightness is adjusted to be
more appropriate for using ACES 2 Output Transforms outside of cinema or TV where the image
has a brighter surround (such as on a web page or video game).

* ``ACES-LMT - ACES 2.0 DCC Look 1``


Release Notes
=============

For additional details, please see the GitHub release page:

`OCIO 2.6.0 <https://github.com/AcademySoftwareFoundation/OpenColorIO/releases/tag/v2.6.0>`_
