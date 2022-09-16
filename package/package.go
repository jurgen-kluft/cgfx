package cgfx

import (
	"github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
	"github.com/jurgen-kluft/cunittest/package"
	"github.com/jurgen-kluft/cvulkan/package"
)

// GetPackage returns the package object of 'cgfx'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := cunittest.GetPackage()
	vulkanpkg := cvulkan.GetPackage()
	basepkg := cbase.GetPackage()

	// The main package
	mainpkg := denv.NewPackage("cgfx")
	mainpkg.AddPackage(unittestpkg)
	mainpkg.AddPackage(vulkanpkg)
	mainpkg.AddPackage(basepkg)

	// 'cgfx' library
	mainlib := denv.SetupDefaultCppLibProject("cgfx", "github.com\\jurgen-kluft\\cgfx")
	mainlib.Dependencies = append(mainlib.Dependencies, basepkg.GetMainLib(), unittestpkg.GetMainLib())

	// 'cgfx' unittest project
	maintest := denv.SetupDefaultCppTestProject("cgfx_test", "github.com\\jurgen-kluft\\cgfx")
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, vulkanpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, basepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
