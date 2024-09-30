package cgfx

import (
	cbase "github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
	cgfxcommon "github.com/jurgen-kluft/cgfxcommon/package"
	cmacos "github.com/jurgen-kluft/cmacos/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'cgfx'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := cunittest.GetPackage()
	basepkg := cbase.GetPackage()
	gfxcommonpkg := cgfxcommon.GetPackage()
	macospkg := cmacos.GetPackage()

	// The main package
	mainpkg := denv.NewPackage("cgfx")
	mainpkg.AddPackage(unittestpkg)
	mainpkg.AddPackage(basepkg)
	mainpkg.AddPackage(macospkg)
	mainpkg.AddPackage(gfxcommonpkg)

	// 'cgfx' library
	mainlib := denv.SetupDefaultCppLibProject("cgfx", "github.com\\jurgen-kluft\\cgfx")
	mainlib.Dependencies = append(mainlib.Dependencies, basepkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, macospkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, gfxcommonpkg.GetMainLib())

	// 'cgfx' unittest project
	maintest := denv.SetupDefaultCppTestProject("cgfx"+"_test", "github.com\\jurgen-kluft\\cgfx")
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, basepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, macospkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, gfxcommonpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
