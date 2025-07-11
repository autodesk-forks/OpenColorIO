..
  SPDX-License-Identifier: CC-BY-4.0
  Copyright Contributors to the OpenColorIO Project.
  Do not edit! This file was automatically generated by share/docs/frozendoc.py.

.. py:class:: GradingRGBMSW
   :module: PyOpenColorIO
   :canonical: PyOpenColorIO.GradingRGBMSW

   Used by the grading tone transforms to hold the red, green, blue, master, start, and width components of a single parameter. The master component affects all three channels (RGB). The start and width components control the range of tones affected. Although this struct simply uses "start" and "width" for all the range values, the actual user-facing name changes based on the parameter.


   .. py:method:: GradingRGBMSW.__init__(*args, **kwargs)
      :module: PyOpenColorIO

      Overloaded function.

      1. __init__(self: PyOpenColorIO.GradingRGBMSW) -> None

      2. __init__(self: PyOpenColorIO.GradingRGBMSW, red: float, green: float, blue: float, master: float, start: float, width: float) -> None

      3. __init__(self: PyOpenColorIO.GradingRGBMSW, start: float, width: float) -> None


   .. py:property:: GradingRGBMSW.blue
      :module: PyOpenColorIO


   .. py:property:: GradingRGBMSW.green
      :module: PyOpenColorIO


   .. py:property:: GradingRGBMSW.master
      :module: PyOpenColorIO


   .. py:property:: GradingRGBMSW.red
      :module: PyOpenColorIO


   .. py:property:: GradingRGBMSW.start
      :module: PyOpenColorIO


   .. py:property:: GradingRGBMSW.width
      :module: PyOpenColorIO

