package = 'fceux'
version = '0-0'

source = {
   url = ''
}

description = {
  summary = "FECUX_Learning_Environment"
}

build = {
     type = "command",
     build_command = "scons",
     install_command = "scons --prefix=$(LUA_BINDIR)/.. install"
     }
