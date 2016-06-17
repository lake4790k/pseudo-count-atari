package = "pseudo-count-atari"
version = "scm-1"

source = {
   url = "git@github.com:lake4790k/pseudo-count-atari.git"
}

description = {
   summary = "Pseudo count for Atari"
}

dependencies = {
   "lua >= 5.1",
}

build = {
   type = "cmake",
   variables = {
      CMAKE_BUILD_TYPE="Release",
      CMAKE_PREFIX_PATH="$(LUA_BINDIR)/..",
      LUA_PATH="$(LUADIR)",
      LUA_CPATH="$(LIBDIR)"
   }
}