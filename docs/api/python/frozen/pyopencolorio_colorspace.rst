..
  SPDX-License-Identifier: CC-BY-4.0
  Copyright Contributors to the OpenColorIO Project.
  Do not edit! This file was automatically generated by share/docs/frozendoc.py.

.. py:class:: ColorSpace
   :module: PyOpenColorIO
   :canonical: PyOpenColorIO.ColorSpace

   The *:ref:`ColorSpace`* is the state of an image with respect to colorimetry and color encoding. Transforming images between different *ColorSpaces* is the primary motivation for this library.

   While a complete discussion of color spaces is beyond the scope of header documentation, traditional uses would be to have *ColorSpaces* corresponding to: physical capture devices (known cameras, scanners), and internal 'convenience' spaces (such as scene linear, logarithmic).


   .. py:method:: ColorSpace.__init__(*args, **kwargs)
      :module: PyOpenColorIO

      Overloaded function.

      1. __init__(self: PyOpenColorIO.ColorSpace) -> None

      2. __init__(self: PyOpenColorIO.ColorSpace, referenceSpace: PyOpenColorIO.ReferenceSpaceType) -> None

      3. __init__(self: PyOpenColorIO.ColorSpace, referenceSpace: PyOpenColorIO.ReferenceSpaceType = <ReferenceSpaceType.REFERENCE_SPACE_SCENE: 0>, name: str = '', aliases: list[str] = [], family: str = '', encoding: str = '', equalityGroup: str = '', description: str = '', bitDepth: PyOpenColorIO.BitDepth = <BitDepth.BIT_DEPTH_UNKNOWN: 0>, isData: bool = False, allocation: PyOpenColorIO.Allocation = <Allocation.ALLOCATION_UNIFORM: 1>, allocationVars: list[float] = [], toReference: PyOpenColorIO.Transform = None, fromReference: PyOpenColorIO.Transform = None, categories: list[str] = []) -> None


   .. py:method:: ColorSpace.addAlias(self: PyOpenColorIO.ColorSpace, alias: str) -> None
      :module: PyOpenColorIO

      Add an alias for the color space name (the aliases may be used as a synonym for the name). Nothing will be added if the alias is already the color space name, one of its aliases, or the argument is null. The aliases must not conflict with existing roles, color space names, named transform names, or other aliases. This is verified when adding the color space to the config.


   .. py:method:: ColorSpace.addCategory(self: PyOpenColorIO.ColorSpace, category: str) -> None
      :module: PyOpenColorIO

      Add a single category.

      .. note::
         Will do nothing if the category already exists.


   .. py:method:: ColorSpace.clearAliases(self: PyOpenColorIO.ColorSpace) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.clearCategories(self: PyOpenColorIO.ColorSpace) -> None
      :module: PyOpenColorIO

      Clear all the categories.


   .. py:method:: ColorSpace.getAliases(self: PyOpenColorIO.ColorSpace) -> PyOpenColorIO.ColorSpace.ColorSpaceAliasIterator
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.getAllocation(self: PyOpenColorIO.ColorSpace) -> PyOpenColorIO.Allocation
      :module: PyOpenColorIO

      *Allocation*

      If this colorspace needs to be transferred to a limited dynamic range coding space (such as during display with a GPU path), use this allocation to maximize bit efficiency.


   .. py:method:: ColorSpace.getAllocationVars(self: PyOpenColorIO.ColorSpace) -> list[float]
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.getBitDepth(self: PyOpenColorIO.ColorSpace) -> PyOpenColorIO.BitDepth
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.getCategories(self: PyOpenColorIO.ColorSpace) -> PyOpenColorIO.ColorSpace.ColorSpaceCategoryIterator
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.getDescription(self: PyOpenColorIO.ColorSpace) -> str
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.getEncoding(self: PyOpenColorIO.ColorSpace) -> str
      :module: PyOpenColorIO

      *Encodings*

      It is sometimes useful for applications to group color spaces based on how the color values are digitally encoded. For example, images in scene-linear, logarithmic, video, and data color spaces could have different default views. Unlike the Family and EqualityGroup attributes of a color space, the list of Encodings is predefined in the OCIO documentation (rather than being config-specific) to make it easier for applications to utilize.

      Here is an example config entry that could appear under a :ref:`ColorSpace`:

      .. code-block:: cpp

          encoding: scene-linear

      Encoding strings are not case-sensitive. Although users may add their own encodings, the strings will typically come from a fixed set listed in the documentation (similar to roles).


   .. py:method:: ColorSpace.getEqualityGroup(self: PyOpenColorIO.ColorSpace) -> str
      :module: PyOpenColorIO

      Get the :ref:`ColorSpace` group name (used for equality comparisons) This allows no-op transforms between different colorspaces. If an equalityGroup is not defined (an empty string), it will be considered unique (i.e., it will not compare as equal to other ColorSpaces with an empty equality group).


   .. py:method:: ColorSpace.getFamily(self: PyOpenColorIO.ColorSpace) -> str
      :module: PyOpenColorIO

      Get the family, for use in user interfaces (optional) The family string could use a '/' separator to indicate levels to be used by hierarchical menus.


   .. py:method:: ColorSpace.getName(self: PyOpenColorIO.ColorSpace) -> str
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.getReferenceSpaceType(self: PyOpenColorIO.ColorSpace) -> PyOpenColorIO.ReferenceSpaceType
      :module: PyOpenColorIO

      A display color space will use the display-referred reference space.


   .. py:method:: ColorSpace.getTransform(self: PyOpenColorIO.ColorSpace, direction: PyOpenColorIO.ColorSpaceDirection) -> PyOpenColorIO.Transform
      :module: PyOpenColorIO

      *:ref:`Transform`*

      If a transform in the specified direction has been specified, return it. Otherwise return a null ConstTransformRcPtr


   .. py:method:: ColorSpace.hasAlias(self: PyOpenColorIO.ColorSpace, alias: str) -> bool
      :module: PyOpenColorIO

      Return true if alias exists.


   .. py:method:: ColorSpace.hasCategory(self: PyOpenColorIO.ColorSpace, category: str) -> bool
      :module: PyOpenColorIO

      Return true if the category is present.

      A category is used to allow applications to filter the list of color spaces they display in menus based on what that color space is used for.

      Here is an example config entry that could appear under a :ref:`ColorSpace`:

      .. code-block:: cpp

          categories: [ file-io, working-space, basic-3d ]

      The example contains three categories: 'file-io', 'working-space' and 'basic-3d'.

      .. note::
         Category strings are not case-sensitive and the order is not significant.

      There is no limit imposed on length or number. Although users may add their own categories, the strings will typically come from a fixed set listed in the documentation (similar to roles).


   .. py:method:: ColorSpace.isData(self: PyOpenColorIO.ColorSpace) -> bool
      :module: PyOpenColorIO

      *Data*

      ColorSpaces that are data are treated a bit special. Basically, any colorspace transforms you try to apply to them are ignored. (Think of applying a gamut mapping transform to an ID pass). However, the setDataBypass method on :ref:`ColorSpaceTransform` and :ref:`DisplayViewTransform` allow applications to process data when necessary. (Think of sending mattes to an HDR monitor.)

      This is traditionally used for pixel data that represents non-color pixel data, such as normals, point positions, ID information, etc.


   .. py:method:: ColorSpace.removeAlias(self: PyOpenColorIO.ColorSpace, alias: str) -> None
      :module: PyOpenColorIO

      Does nothing if alias is not present.


   .. py:method:: ColorSpace.removeCategory(self: PyOpenColorIO.ColorSpace, category: str) -> None
      :module: PyOpenColorIO

      Remove a category.

      .. note::
         Will do nothing if the category is missing.


   .. py:method:: ColorSpace.setAllocation(self: PyOpenColorIO.ColorSpace, allocation: PyOpenColorIO.Allocation) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.setAllocationVars(self: PyOpenColorIO.ColorSpace, vars: list[float]) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.setBitDepth(self: PyOpenColorIO.ColorSpace, bitDepth: PyOpenColorIO.BitDepth) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.setDescription(self: PyOpenColorIO.ColorSpace, description: str) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.setEncoding(self: PyOpenColorIO.ColorSpace, encoding: str) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.setEqualityGroup(self: PyOpenColorIO.ColorSpace, equalityGroup: str) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.setFamily(self: PyOpenColorIO.ColorSpace, family: str) -> None
      :module: PyOpenColorIO

      Set the family, for use in user interfaces (optional)


   .. py:method:: ColorSpace.setIsData(self: PyOpenColorIO.ColorSpace, isData: bool) -> None
      :module: PyOpenColorIO


   .. py:method:: ColorSpace.setName(self: PyOpenColorIO.ColorSpace, name: str) -> None
      :module: PyOpenColorIO

      If the name is already an alias, that alias is removed.


   .. py:method:: ColorSpace.setTransform(self: PyOpenColorIO.ColorSpace, transform: PyOpenColorIO.Transform, direction: PyOpenColorIO.ColorSpaceDirection) -> None
      :module: PyOpenColorIO

      Specify the transform for the appropriate direction. Setting the transform to null will clear it.


.. py:class:: ColorSpaceCategoryIterator
   :module: PyOpenColorIO.ColorSpace
   :canonical: PyOpenColorIO.ColorSpace.ColorSpaceCategoryIterator


   .. py:method:: ColorSpaceCategoryIterator.__getitem__(self: PyOpenColorIO.ColorSpace.ColorSpaceCategoryIterator, arg0: int) -> str
      :module: PyOpenColorIO.ColorSpace


   .. py:method:: ColorSpaceCategoryIterator.__iter__(self: PyOpenColorIO.ColorSpace.ColorSpaceCategoryIterator) -> PyOpenColorIO.ColorSpace.ColorSpaceCategoryIterator
      :module: PyOpenColorIO.ColorSpace


   .. py:method:: ColorSpaceCategoryIterator.__len__(self: PyOpenColorIO.ColorSpace.ColorSpaceCategoryIterator) -> int
      :module: PyOpenColorIO.ColorSpace


   .. py:method:: ColorSpaceCategoryIterator.__next__(self: PyOpenColorIO.ColorSpace.ColorSpaceCategoryIterator) -> str
      :module: PyOpenColorIO.ColorSpace


.. py:class:: ColorSpaceAliasIterator
   :module: PyOpenColorIO.ColorSpace
   :canonical: PyOpenColorIO.ColorSpace.ColorSpaceAliasIterator


   .. py:method:: ColorSpaceAliasIterator.__getitem__(self: PyOpenColorIO.ColorSpace.ColorSpaceAliasIterator, arg0: int) -> str
      :module: PyOpenColorIO.ColorSpace


   .. py:method:: ColorSpaceAliasIterator.__iter__(self: PyOpenColorIO.ColorSpace.ColorSpaceAliasIterator) -> PyOpenColorIO.ColorSpace.ColorSpaceAliasIterator
      :module: PyOpenColorIO.ColorSpace


   .. py:method:: ColorSpaceAliasIterator.__len__(self: PyOpenColorIO.ColorSpace.ColorSpaceAliasIterator) -> int
      :module: PyOpenColorIO.ColorSpace


   .. py:method:: ColorSpaceAliasIterator.__next__(self: PyOpenColorIO.ColorSpace.ColorSpaceAliasIterator) -> str
      :module: PyOpenColorIO.ColorSpace

