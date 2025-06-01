package cgfx

import (
	callocator "github.com/jurgen-kluft/callocator/package"
	cbase "github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
	cd3d12 "github.com/jurgen-kluft/cd3d12/package"
	cmacos "github.com/jurgen-kluft/cmacos/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

const (
	repo_path = "github.com\\jurgen-kluft"
	repo_name = "cgfx"
)

func GetPackage() *denv.Package {
	name := repo_name

	// dependencies
	cunittestpkg := cunittest.GetPackage()
	cbasepkg := cbase.GetPackage()
	callocpkg := callocator.GetPackage()
	cd3d12pkg := cd3d12.GetPackage()
	cmacospkg := cmacos.GetPackage()

	// main package
	mainpkg := denv.NewPackage(repo_path, repo_name)
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(cbasepkg)
	mainpkg.AddPackage(callocpkg)
	mainpkg.AddPackage(cd3d12pkg)
	mainpkg.AddPackage(cmacospkg)

	// main library
	mainlib := denv.SetupCppLibProject(mainpkg, name)
	mainlib.AddDependencies(cbasepkg.GetMainLib()...)
	mainlib.AddDependencies(callocpkg.GetMainLib()...)
	mainlib.AddDependencies(cd3d12pkg.GetMainLib()...)
	mainlib.AddDependencies(cmacospkg.GetMainLib()...)

	if denv.IsMacOS() {
		mainlib.AddDependencies(cmacospkg.GetMainLib()...)
	} else if denv.IsWindows() {
		mainlib.AddDependencies(cd3d12pkg.GetMainLib()...)
	}

	// test library
	testlib := denv.SetupCppTestLibProject(mainpkg, name)
	testlib.AddDependencies(cbasepkg.GetTestLib()...)
	testlib.AddDependencies(callocpkg.GetTestLib()...)
	testlib.AddDependencies(cd3d12pkg.GetTestLib()...)
	testlib.AddDependencies(cmacospkg.GetTestLib()...)
	testlib.AddDependencies(cunittestpkg.GetTestLib()...)

	if denv.IsMacOS() {
		testlib.AddDependencies(cmacospkg.GetTestLib()...)
	} else if denv.IsWindows() {
		testlib.AddDependencies(cd3d12pkg.GetTestLib()...)
	}

	// unittest project
	maintest := denv.SetupCppTestProject(mainpkg, name)
	maintest.AddDependencies(cunittestpkg.GetMainLib()...)
	maintest.AddDependency(testlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddTestLib(testlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
