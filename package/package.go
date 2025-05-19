package cgfx

import (
	callocator "github.com/jurgen-kluft/callocator/package"
	cbase "github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
	cd3d12 "github.com/jurgen-kluft/cd3d12/package"
	cmacos "github.com/jurgen-kluft/cmacos/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'cgfx'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := cunittest.GetPackage()
	basepkg := cbase.GetPackage()
	allocatorpkg := callocator.GetPackage()
	macospkg := cmacos.GetPackage()
	d3d12pkg := cd3d12.GetPackage()

	// The main package
	mainpkg := denv.NewPackage("cgfx")
	mainpkg.AddPackage(unittestpkg)
	mainpkg.AddPackage(basepkg)
	mainpkg.AddPackage(macospkg)
	mainpkg.AddPackage(allocatorpkg)

	// 'cgfx' library
	mainlib := denv.SetupCppLibProject("cgfx", "github.com\\jurgen-kluft\\cgfx")
	mainlib.AddDependencies(basepkg.GetMainLib()...)
	mainlib.AddDependencies(allocatorpkg.GetMainLib()...)

	if denv.IsMacOS() {
		mainlib.AddDependencies(macospkg.GetMainLib()...)
	} else if denv.IsWindows() {
		mainlib.AddDependencies(d3d12pkg.GetMainLib()...)
	}

	// 'cgfx' unittest project
	maintest := denv.SetupDefaultCppTestProject("cgfx"+"_test", "github.com\\jurgen-kluft\\cgfx")
	maintest.AddDependencies(unittestpkg.GetMainLib()...)
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	if denv.IsMacOS() {
		maintest.AddDependencies(macospkg.GetMainLib()...)
	} else if denv.IsWindows() {
		maintest.AddDependencies(d3d12pkg.GetMainLib()...)
	}

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
