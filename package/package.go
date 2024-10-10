package cgfx

import (
	cbase "github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
	cd3d12 "github.com/jurgen-kluft/cd3d12/package"
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
	d3d12pkg := cd3d12.GetPackage()

	// The main package
	mainpkg := denv.NewPackage("cgfx")
	mainpkg.AddPackage(unittestpkg)
	mainpkg.AddPackage(basepkg)
	mainpkg.AddPackage(macospkg)
	mainpkg.AddPackage(gfxcommonpkg)

	// 'cgfx' library
	mainlib := denv.SetupDefaultCppLibProject("cgfx", "github.com\\jurgen-kluft\\cgfx")
	mainlib.Dependencies = append(mainlib.Dependencies, basepkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, gfxcommonpkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, unittestpkg.GetMainLib())

	if denv.IsMacOS() {
		mainlib.Dependencies = append(mainlib.Dependencies, macospkg.GetMainLib())
	} else if denv.IsWindows() {
		mainlib.Dependencies = append(mainlib.Dependencies, d3d12pkg.GetMainLib())
	}

	// 'cgfx' unittest project
	maintest := denv.SetupDefaultCppTestProject("cgfx"+"_test", "github.com\\jurgen-kluft\\cgfx")
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	if denv.IsMacOS() {
		maintest.Dependencies = append(maintest.Dependencies, macospkg.GetMainLib())
	} else if denv.IsWindows() {
		maintest.Dependencies = append(maintest.Dependencies, d3d12pkg.GetMainLib())
	}

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
