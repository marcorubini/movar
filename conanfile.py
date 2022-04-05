from conans import ConanFile


class Package(ConanFile):
    name = "ml-movar"
    version = "0.1"
    # No settings/options are necessary, this is header only
    exports_sources = "include/*"

    def package_info(self):
        self.cpp_info.includedirs = ["include"]

    def package(self):
        self.copy("*.hpp", dst="include", src="include")
