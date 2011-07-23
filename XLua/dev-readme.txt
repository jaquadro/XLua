The Dev folder is not part of the standard MMF directory tree.

Currently this folder contains the VC++ library for the XLua extension,
which can be used by developers to create their own external C modules
for XLua.  The xlua.lib file can be used to directly replace any
standard Lua 5.1.4 .lib file for building new or existing C modules.
Any existing C Modules written for Lua must be rebuilt against this
xlua library to successfully load.

In the future additional information and a header file will be provided
to allow extension authors to interface directly with the XLua object.
The included OpenGL C Module is an example of interfacing with the
third party OpenGL extension, as well as the existing LuaGL C Module.

If you do not intend to develop or build C Modules for your XLua
scripts, you may discard this directory.