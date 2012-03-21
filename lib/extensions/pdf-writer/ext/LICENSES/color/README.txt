= Color
Color is a Ruby library to provide basic RGB, CMYK, HSL, and other colourspace
manipulation support to applications that require it. It also provides 152
named RGB colours (184 with spelling variations) that are commonly supported
in HTML, SVG, and X11 applications. A technique for generating monochromatic
contrasting palettes is also included.

The capabilities of the Color library are limited to pure mathematical
manipulation of the colours based on colour theory without reference to colour
profiles (such as sRGB or Adobe RGB). For most purposes, when working with the
RGB and HSL colours, this won't matter. However, some colour models (like CIE
L*a*b*) are not supported because Color does not yet support colour profiles,
giving no meaningful way to convert colours in absolute colour spaces (like
L*a*b*, XYZ) to non-absolute colour spaces (like RGB).

Color version 1.4 is the result of a project merge between color.rb 0.1.0 by
Matt Lyon and color-tools 1.3 by Austin Ziegler. Please see History.txt for
details on the changes this merge brings. Color version 1.4.1 changes
development to GitHub and fixes some small problems.

Copyright:: Copyright (c) 2005 - 2010 by Austin Ziegler and Matt Lyon
Version::   1.4.1
Homepage::  http://rubyforge.org/projects/color/
Licence::   MIT-Style; see Licence.txt

Color::Palette was developed based on techniques described by Andy "Malarkey"
Clarke[1], implemented in JavaScript by Steve G. Chipman at SlayerOffice[2] and
by Patrick Fitzgerald of BarelyFitz[3] in PHP.

[1] http://www.stuffandnonsense.co.uk/archives/creating_colour_palettes.html 
[2] http://slayeroffice.com/tools/color_palette/
[3] http://www.barelyfitz.com/projects/csscolor/
