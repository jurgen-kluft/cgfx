package main

import (
	"github.com/jurgen-kluft/ccode"
	"github.com/jurgen-kluft/cgfx/package"
)

func main() {
	ccode.Init()
	ccode.Generate(cgfx.GetPackage())
}
