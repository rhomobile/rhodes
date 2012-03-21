#--
# Color
# Colour management with Ruby
# http://rubyforge.org/projects/color
#   Version 1.4.1
#
# Licensed under a MIT-style licence. See Licence.txt in the main
# distribution for full licensing information.
#
# Copyright (c) 2005 - 2010 Austin Ziegler and Matt Lyon
#++

class Color::RGB
  AliceBlue             = Color::RGB.new(0xf0, 0xf8, 0xff)
  AntiqueWhite          = Color::RGB.new(0xfa, 0xeb, 0xd7)
  Aqua                  = Color::RGB.new(0x00, 0xff, 0xff)
  Aquamarine            = Color::RGB.new(0x7f, 0xff, 0xd4)
  Azure                 = Color::RGB.new(0xf0, 0xff, 0xff)
  Beige                 = Color::RGB.new(0xf5, 0xf5, 0xdc)
  Bisque                = Color::RGB.new(0xff, 0xe4, 0xc4)
  Black                 = Color::RGB.new(0, 0, 0)
  BlanchedAlmond        = Color::RGB.new(0xff, 0xeb, 0xcd)
  Blue                  = Color::RGB.new(0x00, 0x00, 0xff)
  BlueViolet            = Color::RGB.new(0x8a, 0x2b, 0xe2)
  Brown                 = Color::RGB.new(0xa5, 0x2a, 0x2a)
  BurlyWood             = Color::RGB.new(0xde, 0xb8, 0x87)
  Burlywood             = BurlyWood
  CadetBlue             = Color::RGB.new(0x5f, 0x9e, 0xa0)
  Carnation             = Color::RGB.new(0xff, 0x5e, 0xd0)
  Cayenne               = Color::RGB.new(0x8d, 0x00, 0x00)
  Chartreuse            = Color::RGB.new(0x7f, 0xff, 0x00)
  Chocolate             = Color::RGB.new(0xd2, 0x69, 0x1e)
  Coral                 = Color::RGB.new(0xff, 0x7f, 0x50)
  CornflowerBlue        = Color::RGB.new(0x64, 0x95, 0xed)
  Cornsilk              = Color::RGB.new(0xff, 0xf8, 0xdc)
  Crimson               = Color::RGB.new(0xdc, 0x14, 0x3c)
  Cyan                  = Color::RGB.new(0x00, 0xff, 0xff)
  DarkBlue              = Color::RGB.new(0x00, 0x00, 0x8b)
  DarkCyan              = Color::RGB.new(0x00, 0x8b, 0x8b)
  DarkGoldenRod         = Color::RGB.new(0xb8, 0x86, 0x0b)
  DarkGoldenrod         = DarkGoldenRod
  DarkGray              = Color::RGB.new(0xa9, 0xa9, 0xa9)
  DarkGreen             = Color::RGB.new(0x00, 0x64, 0x00)
  DarkGrey              = DarkGray
  DarkKhaki             = Color::RGB.new(0xbd, 0xb7, 0x6b)
  DarkMagenta           = Color::RGB.new(0x8b, 0x00, 0x8b)
  DarkOliveGreen        = Color::RGB.new(0x55, 0x6b, 0x2f)
  DarkOrange            = Color::RGB.new(0xff, 0x8c, 0x00)
  DarkOrchid            = Color::RGB.new(0x99, 0x32, 0xcc)
  DarkRed               = Color::RGB.new(0x8b, 0x00, 0x00)
  DarkSalmon            = Color::RGB.new(0xe9, 0x96, 0x7a)
  DarkSeaGreen          = Color::RGB.new(0x8f, 0xbc, 0x8f)
  DarkSlateBlue         = Color::RGB.new(0x48, 0x3d, 0x8b)
  DarkSlateGray         = Color::RGB.new(0x2f, 0x4f, 0x4f)
  DarkSlateGrey         = DarkSlateGray
  DarkTurquoise         = Color::RGB.new(0x00, 0xce, 0xd1)
  DarkViolet            = Color::RGB.new(0x94, 0x00, 0xd3)
  DarkoliveGreen        = DarkOliveGreen
  Darkorange            = Color::RGB.new(0xff, 0x8c, 0x00)
  Darksalmon            = DarkSalmon
  DeepPink              = Color::RGB.new(0xff, 0x14, 0x93)
  DeepSkyBlue           = Color::RGB.new(0x00, 0xbf, 0xbf)
  DimGray               = Color::RGB.new(0x69, 0x69, 0x69)
  DimGrey               = DimGray
  DodgerBlue            = Color::RGB.new(0x1e, 0x90, 0xff)
  Feldspar              = Color::RGB.new(0xd1, 0x92, 0x75)
  FireBrick             = Color::RGB.new(0xb2, 0x22, 0x22)
  Firebrick             = FireBrick
  FloralWhite           = Color::RGB.new(0xff, 0xfa, 0xf0)
  ForestGreen           = Color::RGB.new(0x22, 0x8b, 0x22)
  Fuchsia               = Color::RGB.new(0xff, 0x00, 0xff)
  Gainsboro             = Color::RGB.new(0xdc, 0xdc, 0xdc)
  GhostWhite            = Color::RGB.new(0xf8, 0xf8, 0xff)
  Gold                  = Color::RGB.new(0xff, 0xd7, 0x00)
  GoldenRod             = Color::RGB.new(0xda, 0xa5, 0x20)
  Goldenrod             = GoldenRod
  Gray                  = Color::RGB.new(0x80, 0x80, 0x80)
  Gray10                = Color::RGB.from_percentage(10, 10, 10)
  Gray20                = Color::RGB.from_percentage(20, 20, 20)
  Gray30                = Color::RGB.from_percentage(30, 30, 30)
  Gray40                = Color::RGB.from_percentage(40, 40, 40)
  Gray50                = Color::RGB.from_percentage(50, 50, 50)
  Gray60                = Color::RGB.from_percentage(60, 60, 60)
  Gray70                = Color::RGB.from_percentage(70, 70, 70)
  Gray80                = Color::RGB.from_percentage(80, 80, 80)
  Gray90                = Color::RGB.from_percentage(90, 90, 90)
  Green                 = Color::RGB.new(0x00, 0x80, 0x00)
  GreenYellow           = Color::RGB.new(0xad, 0xff, 0x2f)
  Grey                  = Gray
  Grey10                = Gray10
  Grey20                = Gray20
  Grey30                = Gray30
  Grey40                = Gray40
  Grey50                = Gray50
  Grey60                = Gray60
  Grey70                = Gray70
  Grey80                = Gray80
  Grey90                = Gray90
  HoneyDew              = Color::RGB.new(0xf0, 0xff, 0xf0)
  Honeydew              = HoneyDew
  HotPink               = Color::RGB.new(0xff, 0x69, 0xb4)
  IndianRed             = Color::RGB.new(0xcd, 0x5c, 0x5c)
  Indigo                = Color::RGB.new(0x4b, 0x00, 0x82)
  Ivory                 = Color::RGB.new(0xff, 0xff, 0xf0)
  Khaki                 = Color::RGB.new(0xf0, 0xe6, 0x8c)
  Lavender              = Color::RGB.new(0xe6, 0xe6, 0xfa)
  LavenderBlush         = Color::RGB.new(0xff, 0xf0, 0xf5)
  LawnGreen             = Color::RGB.new(0x7c, 0xfc, 0x00)
  LemonChiffon          = Color::RGB.new(0xff, 0xfa, 0xcd)
  LightBlue             = Color::RGB.new(0xad, 0xd8, 0xe6)
  LightCoral            = Color::RGB.new(0xf0, 0x80, 0x80)
  LightCyan             = Color::RGB.new(0xe0, 0xff, 0xff)
  LightGoldenRodYellow  = Color::RGB.new(0xfa, 0xfa, 0xd2)
  LightGoldenrodYellow  = LightGoldenRodYellow
  LightGray             = Color::RGB.new(0xd3, 0xd3, 0xd3)
  LightGreen            = Color::RGB.new(0x90, 0xee, 0x90)
  LightGrey             = LightGray
  LightPink             = Color::RGB.new(0xff, 0xb6, 0xc1)
  LightSalmon           = Color::RGB.new(0xff, 0xa0, 0x7a)
  LightSeaGreen         = Color::RGB.new(0x20, 0xb2, 0xaa)
  LightSkyBlue          = Color::RGB.new(0x87, 0xce, 0xfa)
  LightSlateBlue        = Color::RGB.new(0x84, 0x70, 0xff)
  LightSlateGray        = Color::RGB.new(0x77, 0x88, 0x99)
  LightSlateGrey        = LightSlateGray
  LightSteelBlue        = Color::RGB.new(0xb0, 0xc4, 0xde)
  LightYellow           = Color::RGB.new(0xff, 0xff, 0xe0)
  Lightsalmon           = LightSalmon
  LightsteelBlue        = LightSteelBlue
  Lime                  = Color::RGB.new(0x00, 0xff, 0x00)
  LimeGreen             = Color::RGB.new(0x32, 0xcd, 0x32)
  Linen                 = Color::RGB.new(0xfa, 0xf0, 0xe6)
  Magenta               = Color::RGB.new(0xff, 0x00, 0xff)
  Maroon                = Color::RGB.new(0x80, 0x00, 0x00)
  MediumAquaMarine      = Color::RGB.new(0x66, 0xcd, 0xaa)
  MediumAquamarine      = MediumAquaMarine
  MediumBlue            = Color::RGB.new(0x00, 0x00, 0xcd)
  MediumOrchid          = Color::RGB.new(0xba, 0x55, 0xd3)
  MediumPurple          = Color::RGB.new(0x93, 0x70, 0xdb)
  MediumSeaGreen        = Color::RGB.new(0x3c, 0xb3, 0x71)
  MediumSlateBlue       = Color::RGB.new(0x7b, 0x68, 0xee)
  MediumSpringGreen     = Color::RGB.new(0x00, 0xfa, 0x9a)
  MediumTurquoise       = Color::RGB.new(0x48, 0xd1, 0xcc)
  MediumVioletRed       = Color::RGB.new(0xc7, 0x15, 0x85)
  MidnightBlue          = Color::RGB.new(0x19, 0x19, 0x70)
  MintCream             = Color::RGB.new(0xf5, 0xff, 0xfa)
  MistyRose             = Color::RGB.new(0xff, 0xe4, 0xe1)
  Moccasin              = Color::RGB.new(0xff, 0xe4, 0xb5)
  NavajoWhite           = Color::RGB.new(0xff, 0xde, 0xad)
  Navy                  = Color::RGB.new(0x00, 0x00, 0x80)
  OldLace               = Color::RGB.new(0xfd, 0xf5, 0xe6)
  Olive                 = Color::RGB.new(0x80, 0x80, 0x00)
  OliveDrab             = Color::RGB.new(0x6b, 0x8e, 0x23)
  Olivedrab             = OliveDrab
  Orange                = Color::RGB.new(0xff, 0xa5, 0x00)
  OrangeRed             = Color::RGB.new(0xff, 0x45, 0x00)
  Orchid                = Color::RGB.new(0xda, 0x70, 0xd6)
  PaleGoldenRod         = Color::RGB.new(0xee, 0xe8, 0xaa)
  PaleGoldenrod         = PaleGoldenRod
  PaleGreen             = Color::RGB.new(0x98, 0xfb, 0x98)
  PaleTurquoise         = Color::RGB.new(0xaf, 0xee, 0xee)
  PaleVioletRed         = Color::RGB.new(0xdb, 0x70, 0x93)
  PapayaWhip            = Color::RGB.new(0xff, 0xef, 0xd5)
  PeachPuff             = Color::RGB.new(0xff, 0xda, 0xb9)
  Peachpuff             = PeachPuff
  Peru                  = Color::RGB.new(0xcd, 0x85, 0x3f)
  Pink                  = Color::RGB.new(0xff, 0xc0, 0xcb)
  Plum                  = Color::RGB.new(0xdd, 0xa0, 0xdd)
  PowderBlue            = Color::RGB.new(0xb0, 0xe0, 0xe6)
  Purple                = Color::RGB.new(0x80, 0x00, 0x80)
  Red                   = Color::RGB.new(0xff, 0x00, 0x00)
  RosyBrown             = Color::RGB.new(0xbc, 0x8f, 0x8f)
  RoyalBlue             = Color::RGB.new(0x41, 0x69, 0xe1)
  SaddleBrown           = Color::RGB.new(0x8b, 0x45, 0x13)
  Salmon                = Color::RGB.new(0xfa, 0x80, 0x72)
  SandyBrown            = Color::RGB.new(0xf4, 0xa4, 0x60)
  SeaGreen              = Color::RGB.new(0x2e, 0x8b, 0x57)
  SeaShell              = Color::RGB.new(0xff, 0xf5, 0xee)
  Seashell              = SeaShell
  Sienna                = Color::RGB.new(0xa0, 0x52, 0x2d)
  Silver                = Color::RGB.new(0xc0, 0xc0, 0xc0)
  SkyBlue               = Color::RGB.new(0x87, 0xce, 0xeb)
  SlateBlue             = Color::RGB.new(0x6a, 0x5a, 0xcd)
  SlateGray             = Color::RGB.new(0x70, 0x80, 0x90)
  SlateGrey             = SlateGray
  Snow                  = Color::RGB.new(0xff, 0xfa, 0xfa)
  SpringGreen           = Color::RGB.new(0x00, 0xff, 0x7f)
  SteelBlue             = Color::RGB.new(0x46, 0x82, 0xb4)
  Tan                   = Color::RGB.new(0xd2, 0xb4, 0x8c)
  Teal                  = Color::RGB.new(0x00, 0x80, 0x80)
  Thistle               = Color::RGB.new(0xd8, 0xbf, 0xd8)
  Tomato                = Color::RGB.new(0xff, 0x63, 0x47)
  Turquoise             = Color::RGB.new(0x40, 0xe0, 0xd0)
  Violet                = Color::RGB.new(0xee, 0x82, 0xee)
  VioletRed             = Color::RGB.new(0xd0, 0x20, 0x90)
  Wheat                 = Color::RGB.new(0xf5, 0xde, 0xb3)
  White                 = Color::RGB.new(0xff, 0xff, 0xff)
  WhiteSmoke            = Color::RGB.new(0xf5, 0xf5, 0xf5)
  Yellow                = Color::RGB.new(0xff, 0xff, 0x00)
  YellowGreen           = Color::RGB.new(0x9a, 0xcd, 0x32)

  AliceBlue.freeze
  AntiqueWhite.freeze
  Aqua.freeze
  Aquamarine.freeze
  Azure.freeze
  Beige.freeze
  Bisque.freeze
  Black.freeze
  BlanchedAlmond.freeze
  Blue.freeze
  BlueViolet.freeze
  Brown.freeze
  Burlywood.freeze
  CadetBlue.freeze
  Cayenne.freeze
  Carnation.freeze
  Chartreuse.freeze
  Chocolate.freeze
  Coral.freeze
  CornflowerBlue.freeze
  Cornsilk.freeze
  Crimson.freeze
  Cyan.freeze
  DarkBlue.freeze
  DarkCyan.freeze
  DarkGoldenrod.freeze
  DarkGray.freeze
  DarkGreen.freeze
  DarkKhaki.freeze
  DarkMagenta.freeze
  DarkoliveGreen.freeze
  Darkorange.freeze
  DarkOrchid.freeze
  DarkRed.freeze
  Darksalmon.freeze
  DarkSeaGreen.freeze
  DarkSlateBlue.freeze
  DarkSlateGray.freeze
  DarkTurquoise.freeze
  DarkViolet.freeze
  DeepPink.freeze
  DeepSkyBlue.freeze
  DimGray.freeze
  DodgerBlue.freeze
  Feldspar.freeze
  Firebrick.freeze
  FloralWhite.freeze
  ForestGreen.freeze
  Fuchsia.freeze
  Gainsboro.freeze
  GhostWhite.freeze
  Gold.freeze
  Goldenrod.freeze
  Gray.freeze
  Green.freeze
  GreenYellow.freeze
  Honeydew.freeze
  HotPink.freeze
  IndianRed.freeze
  Indigo.freeze
  Ivory.freeze
  Khaki.freeze
  Lavender.freeze
  LavenderBlush.freeze
  LawnGreen.freeze
  LemonChiffon.freeze
  LightBlue.freeze
  LightCoral.freeze
  LightCyan.freeze
  LightGoldenrodYellow.freeze
  LightGray.freeze
  LightGreen.freeze
  LightPink.freeze
  Lightsalmon.freeze
  LightSeaGreen.freeze
  LightSkyBlue.freeze
  LightSlateBlue.freeze
  LightSlateGray.freeze
  LightsteelBlue.freeze
  LightYellow.freeze
  Lime.freeze
  LimeGreen.freeze
  Linen.freeze
  Magenta.freeze
  Maroon.freeze
  MediumAquamarine.freeze
  MediumBlue.freeze
  MediumOrchid.freeze
  MediumPurple.freeze
  MediumSeaGreen.freeze
  MediumSlateBlue.freeze
  MediumSpringGreen.freeze
  MediumTurquoise.freeze
  MediumVioletRed.freeze
  MidnightBlue.freeze
  MintCream.freeze
  MistyRose.freeze
  Moccasin.freeze
  NavajoWhite.freeze
  Navy.freeze
  OldLace.freeze
  Olive.freeze
  Olivedrab.freeze
  Orange.freeze
  OrangeRed.freeze
  Orchid.freeze
  PaleGoldenrod.freeze
  PaleGreen.freeze
  PaleTurquoise.freeze
  PaleVioletRed.freeze
  PapayaWhip.freeze
  Peachpuff.freeze
  Peru.freeze
  Pink.freeze
  Plum.freeze
  PowderBlue.freeze
  Purple.freeze
  Red.freeze
  RosyBrown.freeze
  RoyalBlue.freeze
  SaddleBrown.freeze
  Salmon.freeze
  SandyBrown.freeze
  SeaGreen.freeze
  Seashell.freeze
  Sienna.freeze
  Silver.freeze
  SkyBlue.freeze
  SlateBlue.freeze
  SlateGray.freeze
  Snow.freeze
  SpringGreen.freeze
  SteelBlue.freeze
  Tan.freeze
  Teal.freeze
  Thistle.freeze
  Tomato.freeze
  Turquoise.freeze
  Violet.freeze
  VioletRed.freeze
  Wheat.freeze
  White.freeze
  WhiteSmoke.freeze
  Yellow.freeze
  YellowGreen.freeze
  Gray10.freeze
  Gray20.freeze
  Gray30.freeze
  Gray40.freeze
  Gray50.freeze
  Gray60.freeze
  Gray70.freeze
  Gray80.freeze
  Gray90.freeze
end
