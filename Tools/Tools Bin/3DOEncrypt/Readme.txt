* 3DO Homebrew Signing Toolkit version 0.6a by Charles Doty

This is the first public release of the 3DO Homebrew Signing Toolkit.

The main purpose of releasing the tool is enabling the release of homebrew
software for the 3DO.

Usage:
------
To use the tool, type:
'3DOEncrypt isofile.iso' or 
'3DOEncrypt genromtags isofile.iso'

The 'genromtags' option regenerates the rom_tags part of the CD. This is needed
when using an iso created from OperaFS[De]Compiler; since it does not correctly
export the rom_tags part of the CD. The signing tool will fix up critical parts
of rom_tags.