#+
#  Niri <- CC, DC
#
#  A Niri object configures NIRI.
#
#  Options:
#           none in addition to those inherited from DC and CC
#
#  Methods:
#           apply controller    Applies the configuration to the specified
#                               controller (detector or components) or both
#                               controllers.
#
#           init                Sets the object to its initial state.
#
#           save chan           Writes the object definition to the specified
#                               I/O channel.
#
#  D Terrett 24 July 2001
#
#  Copyright CCLRC
#+

itcl::class Niri::Niri {
   inherit Niri::CC Niri::DC
   public {
      constructor {args} {eval configure $args}
      method apply {{controller ""}}
      method init {}
      method save {chan}
   }
}

itcl::body Niri::Niri::apply {{controller ""}} {
   switch $controller {
      detector {
         Niri::DC::apply
      }
      components {
         Niri::CC::apply
      }
      default {
         Niri::CC::apply
         Niri::DC::apply
      }
   }
}

itcl::body Niri::Niri::init {} {
   Niri::CC::init
   Niri::DC::init
}

itcl::body Niri::Niri::save {chan} {
   Niri::CC::save $chan
   Niri::DC::save $chan
}
