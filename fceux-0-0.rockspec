package = 'fceux'
version = '0-0'

source = {
   url = ''
}

description = {
  summary = "FECUX"
}

build = {
     type = "command",
     build_command = "scons",
     install_command = "scons --prefix=$(LUA_BINDIR)/.. install"
     }
