..
  SPDX-License-Identifier: CC-BY-4.0
  Copyright Contributors to the OpenColorIO Project.
  Do not edit! This file was automatically generated by share/docs/frozendoc.py.

.. py:class:: FixedFunctionStyle
   :module: PyOpenColorIO
   :canonical: PyOpenColorIO.FixedFunctionStyle

   Enumeration of the :cpp:class:`:ref:`FixedFunctionTransform`` transform algorithms.

   Members:

     FIXED_FUNCTION_ACES_RED_MOD_03 : Red modifier (ACES 0.3/0.7)

     FIXED_FUNCTION_ACES_RED_MOD_10 : Red modifier (ACES 1.0)

     FIXED_FUNCTION_ACES_GLOW_03 : Glow function (ACES 0.3/0.7)

     FIXED_FUNCTION_ACES_GLOW_10 : Glow function (ACES 1.0)

     FIXED_FUNCTION_ACES_DARK_TO_DIM_10 : Dark to dim surround correction (ACES 1.0)

     FIXED_FUNCTION_REC2100_SURROUND : Rec.2100 surround correction (takes one double for the gamma param)

     FIXED_FUNCTION_RGB_TO_HSV : Classic RGB to HSV function.

     FIXED_FUNCTION_XYZ_TO_xyY : CIE XYZ to 1931 xy chromaticity coordinates.

     FIXED_FUNCTION_XYZ_TO_uvY : CIE XYZ to 1976 u'v' chromaticity coordinates.

     FIXED_FUNCTION_XYZ_TO_LUV : CIE XYZ to 1976 CIELUV colour space (D65 white)

     FIXED_FUNCTION_ACES_GAMUTMAP_02 : ACES 0.2 Gamut clamping algorithm -- NOT IMPLEMENTED YET.

     FIXED_FUNCTION_ACES_GAMUTMAP_07 : ACES 0.7 Gamut clamping algorithm -- NOT IMPLEMENTED YET.

     FIXED_FUNCTION_ACES_GAMUT_COMP_13 : ACES 1.3 Parametric Gamut Compression (expects ACEScg values)

     FIXED_FUNCTION_LIN_TO_PQ : SMPTE ST-2084 OETF, scaled with 100 nits at 1.0 (neg vals mirrored)

     FIXED_FUNCTION_LIN_TO_GAMMA_LOG : Parametrized gamma and log segments with mirroring.

     FIXED_FUNCTION_LIN_TO_DOUBLE_LOG : Two parameterized LogAffineTransforms with a middle linear segment.

     FIXED_FUNCTION_ACES_OUTPUT_TRANSFORM_20 : ACES 2.0 Display Rendering -- EXPERIMENTAL.

     FIXED_FUNCTION_ACES_RGB_TO_JMH_20 : ACES 2.0 RGB to JMh -- EXPERIMENTAL.

     FIXED_FUNCTION_ACES_TONESCALE_COMPRESS_20 : ACES 2.0 Tonescale and chroma compression -- EXPERIMENTAL.

     FIXED_FUNCTION_ACES_GAMUT_COMPRESS_20 : ACES 2.0 Gamut compression -- EXPERIMENTAL.


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_DARK_TO_DIM_10
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_DARK_TO_DIM_10: 4>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUTMAP_02
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUTMAP_02: 10>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUTMAP_07
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUTMAP_07: 11>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUT_COMPRESS_20
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUT_COMPRESS_20: 19>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUT_COMP_13
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_GAMUT_COMP_13: 12>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_GLOW_03
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_GLOW_03: 2>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_GLOW_10
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_GLOW_10: 3>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_OUTPUT_TRANSFORM_20
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_OUTPUT_TRANSFORM_20: 16>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_RED_MOD_03
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_RED_MOD_03: 0>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_RED_MOD_10
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_RED_MOD_10: 1>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_RGB_TO_JMH_20
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_RGB_TO_JMH_20: 17>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_ACES_TONESCALE_COMPRESS_20
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_ACES_TONESCALE_COMPRESS_20: 18>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_LIN_TO_DOUBLE_LOG
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_LIN_TO_DOUBLE_LOG: 15>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_LIN_TO_GAMMA_LOG
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_LIN_TO_GAMMA_LOG: 14>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_LIN_TO_PQ
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_LIN_TO_PQ: 13>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_REC2100_SURROUND
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_REC2100_SURROUND: 5>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_RGB_TO_HSV
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_RGB_TO_HSV: 6>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_XYZ_TO_LUV
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_XYZ_TO_LUV: 9>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_XYZ_TO_uvY
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_XYZ_TO_uvY: 8>


   .. py:attribute:: FixedFunctionStyle.FIXED_FUNCTION_XYZ_TO_xyY
      :module: PyOpenColorIO
      :value: <FixedFunctionStyle.FIXED_FUNCTION_XYZ_TO_xyY: 7>


   .. py:property:: FixedFunctionStyle.value
      :module: PyOpenColorIO

